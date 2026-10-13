"""Per-session rebuild for SMHQ_Virtual (no DirectLink).

Order: wipe last session -> import Datasmith -> materials + Nanite -> doors -> lights + rooms -> switches
       -> maquette info points -> cinema screens -> teleport points -> QA report -> save.

Inputs (project folder):
  SMHQ_Data/export/HQ_GF.udatasmith   exported from SketchUp this session (override with env SMHQ_UDS)
  SMHQ_Data/*.csv                     schedules (see README.md)
Output:
  Saved/SMHQ_QA.csv                   one row per problem + counts; feeds Checks 1 and 12

Naming (approved 2026-10-13):
  DR_<Floor>_<E##>_<D##>          single-leaf door          DR_GF_E28_D43
  DR_<Floor>_<E##>_<D##>_<L|R>    double-door leaf          DR_GF_E28_D43_L
  LT_<Floor>_<E##>_<KIND>_<NN>    light fixture             LT_GF_E01_DOWN_03
  SW_<Floor>_<E##>_<NN>           wall switch               SW_GF_E01_01
  MQ-<NN>                         maquette                  MQ-03
  SCR_<Floor>_<E##>_<NN>          cinema screen             SCR_GF_E05_01   (PENDING approval)

Run with the level that should receive the content open (pilot: one level; generated actors are tagged and
replaced every run, hand-placed actors such as SunSky / PostProcess / PlayerStart are never touched).
API names marked (verify) must be confirmed against the UE 5.8 Python API on day 1.
"""
import csv
import os
import re

import unreal

PROJECT = unreal.Paths.project_dir()
DATA = os.path.join(PROJECT, "SMHQ_Data")
UDS_FILE = os.environ.get("SMHQ_UDS", os.path.join(DATA, "export", "HQ_GF.udatasmith"))
IMPORT_TO = "/Game/SMHQ/Import/GF"
BP_DIR = "/Game/SMHQ/Blueprints"
GEN_TAG = "SMHQ_GEN"        # everything this script spawns
IMP_TAG = "SMHQ_IMPORT"     # everything Datasmith imported

DOOR_RE = re.compile(r"^DR_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_(?P<door>D\d+)(?:_(?P<leaf>[LR]))?$")
LIGHT_RE = re.compile(r"^LT_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_(?P<kind>[A-Z]+)_(?P<num>\d+)$")
SWITCH_RE = re.compile(r"^SW_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_(?P<num>\d+)$")
SCREEN_RE = re.compile(r"^SCR_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_(?P<num>\d+)$")
MAQUETTE_RE = re.compile(r"(?P<mq>MQ-\d+)")

LIGHT_CLASS = {
    "DOWN": unreal.SpotLight, "SPOT": unreal.SpotLight,
    "PEND": unreal.PointLight, "WALL": unreal.PointLight,
    "LINE": unreal.RectLight, "COVE": unreal.RectLight,
}

actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
eal = unreal.EditorAssetLibrary
qa = []   # (severity, kind, name, message)


# ------------------------------------------------------------------ helpers
def issue(severity, kind, name, message):
    qa.append((severity, kind, name, message))


def read_csv(file_name, *key_cols):
    path = os.path.join(DATA, file_name)
    if not os.path.exists(path):
        issue("WARN", "data", file_name, "CSV missing - defaults used")
        return {}
    with open(path, newline="", encoding="utf-8-sig") as f:
        return {tuple(row[k].strip() for k in key_cols): row for row in csv.DictReader(f)}


def yes(value):
    return str(value or "").strip().lower() in ("yes", "y", "true", "1")


def num(value, default):
    try:
        return float(value)
    except (TypeError, ValueError):
        return default


def door_label(actor):
    # Datasmith may append _<n> to duplicate names ("DR_GF_E28_D43_L_2"); door codes never end in _<digits>
    return re.sub(r"_\d+$", "", actor.get_actor_label())


def mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent)


def add_tags(actor, *tags):
    actor.tags = list(actor.tags) + [unreal.Name(t) for t in tags]


def spawn(cls, location, rotation=None, label=None, *tags):
    actor = actors.spawn_actor_from_class(cls, location, rotation or unreal.Rotator())
    add_tags(actor, GEN_TAG, *tags)
    if label:
        actor.set_actor_label(label)
    return actor


def hide_source(actor):
    """Keep the imported mesh in the level (it is wiped next run) but hide it and remove its collision."""
    actor.set_actor_hidden_in_game(True)
    actor.set_is_temporarily_hidden_in_editor(True)
    actor.set_actor_enable_collision(False)


def blueprint_or(name, fallback):
    path = f"{BP_DIR}/{name}"
    if eal.does_asset_exist(path):
        cls = eal.load_blueprint_class(path)
        if cls:
            return cls
    issue("WARN", "setup", name, "Blueprint missing - using C++ class (run smhq_setup.py)")
    return fallback


def load(path):
    path = (path or "").strip()
    return eal.load_asset(path) if path else None


sm_subsystem = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)


def ensure_simple_collision(mesh, name):
    try:
        if sm_subsystem.get_simple_collision_count(mesh) == 0:                       # (verify)
            sm_subsystem.add_simple_collisions(mesh, unreal.ScriptingCollisionShapeType.BOX)  # (verify)
    except Exception as exc:  # keep going; QA shows it
        issue("WARN", "collision", name, f"could not add box collision: {exc}")


# ------------------------------------------------------------------ 1. wipe previous session
def wipe():
    old = [a for a in actors.get_all_level_actors()
           if unreal.Name(GEN_TAG) in a.tags or unreal.Name(IMP_TAG) in a.tags]
    if old:
        actors.destroy_actors(old)
    if eal.does_directory_exist(IMPORT_TO):
        eal.delete_directory(IMPORT_TO)


# ------------------------------------------------------------------ 2. import
def import_datasmith():
    if not os.path.exists(UDS_FILE):
        raise RuntimeError(f"Datasmith file not found: {UDS_FILE}")
    before = {a.get_path_name() for a in actors.get_all_level_actors()}
    scene = unreal.DatasmithSceneElement.construct_datasmith_scene_from_file(UDS_FILE)
    if scene is None:
        raise RuntimeError("Datasmith could not read the file")
    if hasattr(scene, "import_scene"):          # (verify) 5.8 name
        scene.import_scene(IMPORT_TO)
    else:
        scene.import_scenes(IMPORT_TO)
    imported = [a for a in actors.get_all_level_actors() if a.get_path_name() not in before]
    for a in imported:
        add_tags(a, IMP_TAG)
    return imported


# ------------------------------------------------------------------ 3. materials + Nanite
def apply_materials_and_nanite(imported):
    mapping = {k[0].lower().replace(" ", "_"): row["UnrealMaterial"]
               for k, row in read_csv("Materials.csv", "SketchUpMaterial").items()}
    done_meshes = set()
    for a in imported:
        comp = mesh_component(a)
        if not comp or not comp.static_mesh:
            continue
        is_glass = False
        for i in range(comp.get_num_materials()):
            mat = comp.get_material(i)
            mat_name = mat.get_name().lower() if mat else ""
            is_glass = is_glass or "glass" in mat_name
            for su_name, ue_path in mapping.items():
                if su_name and su_name in mat_name:
                    new_mat = load(ue_path)
                    if new_mat:
                        comp.set_material(i, new_mat)
                    else:
                        issue("WARN", "material", su_name, f"Unreal material not found: {ue_path}")
                    break
        mesh = comp.static_mesh
        key = mesh.get_path_name()
        if key in done_meshes or is_glass:
            continue
        settings = mesh.get_editor_property("nanite_settings")      # (verify)
        if not settings.enabled:
            settings.enabled = True
            mesh.set_editor_property("nanite_settings", settings)
        done_meshes.add(key)


# ------------------------------------------------------------------ 4. doors
def build_doors(imported):
    door_cls = blueprint_or("BP_Door", unreal.SMHQDoor)
    rows = read_csv("Doors.csv", "Floor", "Door")
    groups = {}
    for a in imported:
        label = door_label(a)
        if not label.startswith("DR_"):
            continue
        m = DOOR_RE.match(label)
        comp = mesh_component(a)
        if not m:
            issue("ERROR", "door", label, "name does not match DR_<Floor>_<E##>_<D##>[_L|_R]")
            continue
        if not comp or not comp.static_mesh:
            issue("ERROR", "door", label, "no static mesh")
            continue
        floor, door, leaf = m["floor"], m["door"], m["leaf"]
        row = rows.get((floor, door))
        if row is None:
            issue("WARN", "door", label, "no row in Doors.csv - defaults used (swing 90, by key)")
            row = {}
        angle = num(row.get("OpenAngle"), 90.0)
        if leaf == "R":
            angle = num(row.get("OpenAngle_R"), -angle)
        ensure_simple_collision(comp.static_mesh, label)
        bp = spawn(door_cls, a.get_actor_location(), a.get_actor_rotation(), f"Door_{label}", f"{floor}_{m['space']}")
        bp.set_actor_scale3d(a.get_actor_scale3d())
        bp.configure_door(comp.static_mesh, angle, (row.get("Type") or "").strip().lower() == "sliding",
                          yes(row.get("AutoOpen")), yes(row.get("Locked")))
        hide_source(a)
        groups.setdefault((floor, door), []).append((leaf, bp))

    for (floor, door), leaves in groups.items():
        kinds = sorted(leaf or "-" for leaf, _ in leaves)
        if kinds == ["L", "R"]:
            (_, first), (_, second) = leaves
            first.set_paired_leaf(second)
            second.set_paired_leaf(first)
        elif kinds != ["-"]:
            issue("ERROR", "door", f"{floor}_{door}", f"leaves {kinds}: expected one leaf without suffix, or _L + _R")


# ------------------------------------------------------------------ 5. lights + room managers
def build_lights(imported):
    types = read_csv("LightTypes.csv", "Kind")
    overrides = read_csv("Lights.csv", "Name")
    rooms = {}
    for a in imported:
        label = a.get_actor_label()
        if not label.startswith("LT_"):
            continue
        m = LIGHT_RE.match(label)
        if not m:
            issue("ERROR", "light", label, "name does not match LT_<Floor>_<E##>_<KIND>_<NN>")
            continue
        kind = m["kind"]
        if kind not in LIGHT_CLASS:
            issue("ERROR", "light", label, f"unknown fixture kind {kind}")
            continue
        base, over = types.get((kind,), {}), overrides.get((label,), {})
        if not base:
            issue("WARN", "light", label, f"kind {kind} missing in LightTypes.csv - engine defaults used")
        value = {k: (over.get(k) or base.get(k) or "").strip() for k in ("Lumens", "Kelvin", "IES")}

        cls = LIGHT_CLASS[kind]
        origin, extent = a.get_actor_bounds(False)
        location = unreal.Vector(origin.x, origin.y, origin.z - extent.z - 2.0)
        rotation = unreal.Rotator(0.0, -90.0, 0.0) if cls is not unreal.PointLight else unreal.Rotator()  # (roll, pitch, yaw)
        room = f"{m['floor']}_{m['space']}"
        light = spawn(cls, location, rotation, f"L_{label}", room)
        lc = light.light_component
        lc.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
        lc.set_editor_property("intensity_units", unreal.LightUnits.LUMENS)
        lc.set_editor_property("intensity", num(value["Lumens"], 800.0))
        lc.set_editor_property("use_temperature", True)
        lc.set_editor_property("temperature", num(value["Kelvin"], 3000.0))
        if value["IES"]:
            ies = load(value["IES"])
            if ies:
                lc.set_editor_property("ies_texture", ies)
                lc.set_editor_property("use_ies_brightness", True)
            else:
                issue("WARN", "light", label, f"IES asset not found: {value['IES']}")
        light.attach_to_actor(a, "", unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD,
                              unreal.AttachmentRule.KEEP_WORLD, False)
        rooms.setdefault(room, origin)

    room_cls = blueprint_or("BP_RoomLights", unreal.SMHQRoomLights)
    managers = {}
    for room, origin in rooms.items():
        manager = spawn(room_cls, origin, None, f"Room_{room}")
        manager.configure_room(unreal.Name(room))
        managers[room] = manager
    return managers


# ------------------------------------------------------------------ 5b. wall switches
def build_switches(imported, managers):
    switch_cls = blueprint_or("BP_LightSwitch", unreal.SMHQLightSwitch)
    for a in imported:
        label = a.get_actor_label()
        if not label.startswith("SW_"):
            continue
        m = SWITCH_RE.match(label)
        if not m:
            issue("ERROR", "switch", label, "name does not match SW_<Floor>_<E##>_<NN>")
            continue
        room = f"{m['floor']}_{m['space']}"
        comp = mesh_component(a)
        mesh = comp.static_mesh if comp else None
        if mesh:
            ensure_simple_collision(mesh, label)
        switch = spawn(switch_cls, a.get_actor_location(), a.get_actor_rotation(), f"Switch_{label}", room)
        switch.set_actor_scale3d(a.get_actor_scale3d())
        switch.configure_switch(mesh, managers.get(room))
        if mesh:
            hide_source(a)
        if room not in managers:
            issue("WARN", "switch", label, f"no lights found in {room} - switch does nothing")


# ------------------------------------------------------------------ 6. maquette info points
def build_info_points(imported):
    info_cls = blueprint_or("BP_InfoPoint", unreal.SMHQInfoPoint)
    rows = read_csv("Maquettes.csv", "Maquette")
    seen = set()
    for a in imported:
        m = MAQUETTE_RE.search(a.get_actor_label())
        if not m or m["mq"] in seen:
            continue
        code = m["mq"]
        seen.add(code)
        row = rows.get((code,))
        if row is None:
            issue("WARN", "maquette", code, "no row in Maquettes.csv - info point has no content")
            row = {}
        images = []
        for path in (row.get("Images") or "").split(";"):
            if path.strip():
                tex = load(path)
                if tex:
                    images.append(tex)
                else:
                    issue("WARN", "maquette", code, f"image not found: {path.strip()}")
        video = load(row.get("Video"))
        origin, extent = a.get_actor_bounds(False)
        point = spawn(info_cls, unreal.Vector(origin.x, origin.y, origin.z + extent.z + 30.0), None, f"Info_{code}")
        point.configure_info(unreal.Name(code), (row.get("Project") or "").strip(),
                             unreal.Text(row.get("Title") or ""), unreal.Text(row.get("Description") or ""),
                             images, video)


# ------------------------------------------------------------------ 7. cinema screens
def build_screens(imported, managers):
    screen_cls = blueprint_or("BP_CinemaScreen", unreal.SMHQCinemaScreen)
    rows = read_csv("Screens.csv", "Screen")
    for a in imported:
        label = a.get_actor_label()
        m = SCREEN_RE.match(label)
        if not m:
            continue
        row = rows.get((label,))
        if row is None:
            issue("WARN", "screen", label, "no row in Screens.csv - screen stays black")
            row = {}
        comp = mesh_component(a)
        source = load(row.get("Media"))
        room = f"{m['floor']}_{m['space']}"
        extent = unreal.Vector(num(row.get("HallSizeX"), 20.0) * 50.0, num(row.get("HallSizeY"), 20.0) * 50.0,
                               num(row.get("HallSizeZ"), 6.0) * 50.0)        # size in m -> half extent in cm
        offset = unreal.Vector(num(row.get("HallOffsetX"), 0.0) * 100.0, num(row.get("HallOffsetY"), 0.0) * 100.0,
                               num(row.get("HallOffsetZ"), 0.0) * 100.0)
        screen = spawn(screen_cls, a.get_actor_location(), a.get_actor_rotation(), f"Screen_{label}", room)
        screen.set_actor_scale3d(a.get_actor_scale3d())
        screen.configure_screen(comp.static_mesh if comp else None, source,
                                yes(row.get("AutoPlay", "Yes")), yes(row.get("Loop", "Yes")),
                                managers.get(room) if yes(row.get("DimRoom", "Yes")) else None, extent, offset)
        if comp:
            hide_source(a)


# ------------------------------------------------------------------ 8. teleport points
def build_teleports():
    tp_cls = blueprint_or("BP_TeleportPoint", unreal.SMHQTeleportPoint)
    for order, ((floor, space), row) in enumerate(read_csv("Spaces.csv", "Floor", "Space").items()):
        if not yes(row.get("Teleport")):
            continue
        location = unreal.Vector(num(row.get("X"), 0.0) * 100.0, num(row.get("Y"), 0.0) * 100.0,
                                 num(row.get("Z"), 0.0) * 100.0)                # m -> cm, floor level
        tp = spawn(tp_cls, location, unreal.Rotator(0.0, 0.0, num(row.get("Yaw"), 0.0)), f"TP_{floor}_{space}")
        tp.configure_teleport(unreal.Name(f"{floor}_{space}"), unreal.Text(row.get("NameAR") or ""),
                              unreal.Text(row.get("NameEN") or ""), order)


# ------------------------------------------------------------------ 9. QA + save
def write_report():
    prefixes = ("Door_", "L_", "Room_", "Switch_", "Info_", "Screen_", "TP_")
    counts = {p.rstrip("_"): 0 for p in prefixes}
    for a in actors.get_all_level_actors():
        label = a.get_actor_label()
        for p in prefixes:
            if label.startswith(p):
                counts[p.rstrip("_")] += 1
    os.makedirs(os.path.join(PROJECT, "Saved"), exist_ok=True)
    with open(os.path.join(PROJECT, "Saved", "SMHQ_QA.csv"), "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["severity", "kind", "name", "message"])
        writer.writerows(qa)
        writer.writerow(["INFO", "counts", "", str(counts)])
    errors = sum(1 for row in qa if row[0] == "ERROR")
    unreal.log(f"[smhq_rebuild] {counts} | {errors} errors, {len(qa) - errors} warnings -> Saved/SMHQ_QA.csv")
    unreal.log(f"SMHQ_QA_ERRORS={errors}")


def main():
    wipe()
    imported = import_datasmith()
    apply_materials_and_nanite(imported)
    build_doors(imported)
    managers = build_lights(imported)
    build_switches(imported, managers)
    build_info_points(imported)
    build_screens(imported, managers)
    build_teleports()
    write_report()
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)


main()
