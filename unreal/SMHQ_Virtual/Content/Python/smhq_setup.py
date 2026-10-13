"""One-time project setup for SMHQ_Virtual (idempotent: safe to run again).

Creates:
  /Game/SMHQ/Blueprints/BP_*         Blueprint children of the C++ classes (tuning without code)
  /Game/SMHQ/Materials/M_SMHQ_Screen  media screen material  (texture param "Video", scalar "Brightness")
  /Game/SMHQ/Materials/M_SMHQ_Emissive fixture glow master  (vector "Color", scalars "EmissiveStrength", "Intensity")
  /Game/Movies/MS_<name>              FileMediaSource for every .mp4 found in Content/Movies
  /Game/SMHQ/UI/F_SMHQ_Arabic         runtime font (Regular + Bold) from SMHQ_Data/fonts/NotoNaskhArabic-*.ttf
                                      Noto Naskh Arabic = office identity font, SIL Open Font License 1.1 (SMHQ_Data/fonts/OFL.txt)

Run inside the editor: Tools > Execute Python Script, or
  UnrealEditor.exe SMHQ_Virtual.uproject -ExecutePythonScript="<abs>/Content/Python/smhq_setup.py"
API names marked (verify) must be confirmed against the UE 5.8 Python API on day 1.
"""
import os
import unreal

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
eal = unreal.EditorAssetLibrary
mel = unreal.MaterialEditingLibrary

BP_DIR = "/Game/SMHQ/Blueprints"
MAT_DIR = "/Game/SMHQ/Materials"
MOVIE_DIR = "/Game/Movies"
UI_DIR = "/Game/SMHQ/UI"
FONT_DIR = os.path.join(unreal.Paths.project_dir(), "SMHQ_Data", "fonts")
FONT_FILES = {"Regular": "NotoNaskhArabic-Regular.ttf", "Bold": "NotoNaskhArabic-Bold.ttf"}

BLUEPRINTS = {
    "BP_Door": unreal.SMHQDoor,
    "BP_RoomLights": unreal.SMHQRoomLights,
    "BP_LightSwitch": unreal.SMHQLightSwitch,
    "BP_InfoPoint": unreal.SMHQInfoPoint,
    "BP_CinemaScreen": unreal.SMHQCinemaScreen,
    "BP_TeleportPoint": unreal.SMHQTeleportPoint,
    "BP_DayNight": unreal.SMHQDayNight,
}


def log(msg):
    unreal.log(f"[smhq_setup] {msg}")


def make_blueprints():
    for name, parent in BLUEPRINTS.items():
        path = f"{BP_DIR}/{name}"
        if eal.does_asset_exist(path):
            log(f"exists: {path}")
            continue
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", parent)
        bp = asset_tools.create_asset(name, BP_DIR, unreal.Blueprint, factory)
        eal.save_loaded_asset(bp)
        log(f"created: {path}")


def new_material(name):
    path = f"{MAT_DIR}/{name}"
    if eal.does_asset_exist(path):
        log(f"exists: {path}")
        return None
    return asset_tools.create_asset(name, MAT_DIR, unreal.Material, unreal.MaterialFactoryNew())


def make_screen_material():
    material = new_material("M_SMHQ_Screen")
    if material is None:
        return
    # A MediaTexture default makes the sampler "External", which media playback needs (verify).
    default_tex = asset_tools.create_asset("T_SMHQ_ScreenDefault", MAT_DIR, unreal.MediaTexture,
                                           unreal.MediaTextureFactoryNew())  # (verify factory name)
    video = mel.create_material_expression(material, unreal.MaterialExpressionTextureSampleParameter2D, -600, 0)
    video.set_editor_property("parameter_name", "Video")
    video.set_editor_property("texture", default_tex)
    video.set_editor_property("sampler_type", unreal.MaterialSamplerType.SAMPLERTYPE_EXTERNAL)
    brightness = mel.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -600, 300)
    brightness.set_editor_property("parameter_name", "Brightness")
    brightness.set_editor_property("default_value", 2.0)
    multiply = mel.create_material_expression(material, unreal.MaterialExpressionMultiply, -250, 100)
    mel.connect_material_expressions(video, "RGB", multiply, "A")
    mel.connect_material_expressions(brightness, "", multiply, "B")
    mel.connect_material_property(multiply, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    rough = mel.create_material_expression(material, unreal.MaterialExpressionConstant, -250, 300)
    rough.set_editor_property("r", 0.3)
    mel.connect_material_property(rough, "", unreal.MaterialProperty.MP_ROUGHNESS)
    mel.recompile_material(material)
    eal.save_loaded_asset(default_tex)
    eal.save_loaded_asset(material)
    log("created: M_SMHQ_Screen")


def make_emissive_material():
    material = new_material("M_SMHQ_Emissive")
    if material is None:
        return
    color = mel.create_material_expression(material, unreal.MaterialExpressionVectorParameter, -800, 0)
    color.set_editor_property("parameter_name", "Color")
    color.set_editor_property("default_value", unreal.LinearColor(1.0, 0.9, 0.75, 1.0))
    strength = mel.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -800, 250)
    strength.set_editor_property("parameter_name", "EmissiveStrength")   # driven by SMHQRoomLights (0 = off)
    strength.set_editor_property("default_value", 1.0)
    intensity = mel.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -800, 400)
    intensity.set_editor_property("parameter_name", "Intensity")
    intensity.set_editor_property("default_value", 10.0)
    mul_a = mel.create_material_expression(material, unreal.MaterialExpressionMultiply, -500, 100)
    mel.connect_material_expressions(color, "", mul_a, "A")
    mel.connect_material_expressions(strength, "", mul_a, "B")
    mul_b = mel.create_material_expression(material, unreal.MaterialExpressionMultiply, -250, 150)
    mel.connect_material_expressions(mul_a, "", mul_b, "A")
    mel.connect_material_expressions(intensity, "", mul_b, "B")
    mel.connect_material_property(mul_b, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    mel.connect_material_property(color, "", unreal.MaterialProperty.MP_BASE_COLOR)
    mel.recompile_material(material)
    eal.save_loaded_asset(material)
    log("created: M_SMHQ_Emissive")


def make_media_sources():
    movies = os.path.join(unreal.Paths.project_content_dir(), "Movies")
    if not os.path.isdir(movies):
        log("no Content/Movies folder yet - skipping media sources")
        return
    for file_name in sorted(os.listdir(movies)):
        if not file_name.lower().endswith(".mp4"):
            continue
        stem = os.path.splitext(file_name)[0].replace(" ", "_")
        path = f"{MOVIE_DIR}/MS_{stem}"
        if eal.does_asset_exist(path):
            continue
        source = asset_tools.create_asset(f"MS_{stem}", MOVIE_DIR, unreal.FileMediaSource,
                                          unreal.FileMediaSourceFactoryNew())  # (verify factory name)
        source.set_file_path(f"./Movies/{file_name}")
        eal.save_loaded_asset(source)
        log(f"created: {path}")


def import_font_face(typeface, file_name):
    path = os.path.join(FONT_DIR, file_name)
    if not os.path.exists(path):
        log(f"font file missing: {path}")
        return None
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", path)
    task.set_editor_property("destination_path", UI_DIR)
    task.set_editor_property("destination_name", f"FF_SMHQ_Arabic_{typeface}")
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)
    asset_tools.import_asset_tasks([task])
    for object_path in task.get_editor_property("imported_object_paths"):
        asset = eal.load_asset(object_path)
        if isinstance(asset, unreal.FontFace):
            return asset
    log(f"no FontFace imported from {file_name}")
    return None


def make_arabic_font():
    """F_SMHQ_Arabic: runtime composite font, default typeface entries 'Regular' and 'Bold'.
    If any step fails, do it by hand (plan section 2.7): import the TTFs, right-click the Regular
    Font Face > Create Font, rename to F_SMHQ_Arabic, Font Cache Type = Runtime, add the Bold face."""
    path = f"{UI_DIR}/F_SMHQ_Arabic"
    if eal.does_asset_exist(path):
        log(f"exists: {path}")
        return
    faces = {name: import_font_face(name, file) for name, file in FONT_FILES.items()}
    faces = {name: face for name, face in faces.items() if face}
    if "Regular" not in faces:
        log("Arabic font NOT created - Regular face missing; UI falls back to the engine font")
        return
    try:
        font = asset_tools.create_asset("F_SMHQ_Arabic", UI_DIR, unreal.Font, unreal.FontFactory())  # (verify)
        font.set_editor_property("font_cache_type", unreal.FontCacheType.RUNTIME)
        composite = font.get_editor_property("composite_font")
        typeface = composite.get_editor_property("default_typeface")
        entries = []
        for name in ("Regular", "Bold"):
            if name in faces:
                data = unreal.FontData()
                data.set_editor_property("font_face_asset", faces[name])    # (verify) exposed to Python?
                entry = unreal.TypefaceEntry()
                entry.set_editor_property("name", name)
                entry.set_editor_property("font", data)
                entries.append(entry)
        typeface.set_editor_property("fonts", entries)
        composite.set_editor_property("default_typeface", typeface)
        font.set_editor_property("composite_font", composite)
        eal.save_loaded_asset(font)
        log(f"created: {path} ({', '.join(faces)})")
    except Exception as exc:
        log(f"Arabic font needs the manual steps (plan 2.7): {exc}")


make_blueprints()
make_arabic_font()
make_screen_material()
make_emissive_material()
make_media_sources()
log("done")
