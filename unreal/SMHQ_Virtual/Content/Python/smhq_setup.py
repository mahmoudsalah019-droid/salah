"""One-time project setup for SMHQ_Virtual (idempotent: safe to run again).

Creates:
  /Game/SMHQ/Blueprints/BP_*         Blueprint children of the C++ classes (tuning without code)
  /Game/SMHQ/Materials/M_SMHQ_Screen  media screen material  (texture param "Video", scalar "Brightness")
  /Game/SMHQ/Materials/M_SMHQ_Emissive fixture glow master  (vector "Color", scalars "EmissiveStrength", "Intensity")
  /Game/Movies/MS_<name>              FileMediaSource for every .mp4 found in Content/Movies

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


make_blueprints()
make_screen_material()
make_emissive_material()
make_media_sources()
log("done")
