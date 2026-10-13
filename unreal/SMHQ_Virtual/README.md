# SMHQ_Virtual — Unreal Engine 5.8 project (code only)

> **DO NOT RUN ANYTHING until م. صلاح says "ابدأ".** This folder was written and pushed in advance; nothing here has been compiled or executed yet.

Plan (Arabic, with English appendix): [`../../virtual-office-unreal-plan.md`](../../virtual-office-unreal-plan.md) · Approved checks: [`../../PROJECT.md`](../../PROJECT.md)

## What is here

| Path | Purpose |
|---|---|
| `SMHQ_Virtual.uproject` | Project file (engine 5.8, C++ module, plugins: Datasmith, Python, Editor Scripting, SunPosition, Electra/WMF media; PixelStreaming2 off) |
| `Config/` | Lumen-only rendering, DX12/SM6, Enhanced Input classes, default GameMode, packaging (Movies staged) |
| `Source/SMHQ_Virtual/` | C++ gameplay: visitor (`SMHQCharacter`), UI (`SMHQPlayerController`, `SMHQHUDWidget`), `SMHQDoor`, `SMHQRoomLights`, `SMHQLightSwitch`, `SMHQDayNight`, `SMHQInfoPoint`, `SMHQCinemaScreen`, `SMHQTeleportPoint` |
| `Content/Python/smhq_setup.py` | One-time: Blueprint children `BP_*`, materials `M_SMHQ_Screen` / `M_SMHQ_Emissive`, media sources for `Content/Movies/*.mp4` |
| `Content/Python/smhq_rebuild.py` | Every session: wipe → import Datasmith → materials/Nanite → doors, lights, rooms, switches, info points, screens, teleports → `Saved/SMHQ_QA.csv` |
| `SMHQ_Data/*.csv` | Schedules (headers only; fill per pilot scope). `export/` receives the session's `.udatasmith` (git-ignored) |

## Order of work on the office PC (after "ابدأ")

0. **Toolchain check** — plan section 6.0 (VS 2026 first; fallback ladder to VS 2022 on D:).
1. Build the editor target:
   ```
   "<UE_5.8>\Engine\Build\BatchFiles\Build.bat" SMHQ_VirtualEditor Win64 Development -Project="<abs>\SMHQ_Virtual.uproject" -WaitMutex -NoHotReload
   ```
   Save the full log. Toolchain line to look for: `Using Visual Studio 20xx 14.xx.xxxxx toolchain`.
2. Open the project once in the editor (first shader compile can take hours on the Arc 140T). Create the pilot level with a SunSky, a Post Process Volume (unbound, manual exposure) and a Player Start; save it.
3. Run `smhq_setup.py` once (Tools → Execute Python Script).
4. `smhq_setup.py` also builds `/Game/SMHQ/UI/F_SMHQ_Arabic` from `SMHQ_Data/fonts/NotoNaskhArabic-{Regular,Bold}.ttf` (office identity font, SIL OFL 1.1 — keep `OFL.txt` with it). If the scripted font step fails, follow the manual steps in plan section 2.7. Verify Arabic shaping and right-to-left order on screen.
5. Each session: export `HQ_GF.udatasmith` from SketchUp into `SMHQ_Data/export/`, open the pilot level, run `smhq_rebuild.py`, read `Saved/SMHQ_QA.csv`.
6. Play (PIE) and evaluate the 13 locked checks in `PROJECT.md`. Report: table pass / fail / needs verification + screenshots + the QA CSV.

## CSV schedules

| File | Columns | Notes |
|---|---|---|
| `Doors.csv` | `Floor,Door,Type,OpenAngle,OpenAngle_R,AutoOpen,Locked` | Key = Floor + Door (`GF,D43`). Type `Swing`/`Sliding`. Booleans `Yes`/`No`. `OpenAngle_R` empty → `-OpenAngle` |
| `LightTypes.csv` | `Kind,Lumens,Kelvin,IES` | Defaults per fixture kind (placeholder values — replace from the lighting schedule) |
| `Lights.csv` | `Name,Lumens,Kelvin,IES` | Optional per-fixture overrides, e.g. `LT_GF_E01_DOWN_03` |
| `Spaces.csv` | `Floor,Space,NameAR,NameEN,Teleport,X,Y,Z,Yaw` | Teleport anchors, metres from the model origin, Z = floor level |
| `Maquettes.csv` | `Maquette,Project,Title,Description,Images,Video` | `Images` = `;`-separated texture asset paths; `Video` = media source asset path |
| `Materials.csv` | `SketchUpMaterial,UnrealMaterial` | Substring match on the imported material name |
| `Screens.csv` | `Screen,Media,AutoPlay,Loop,DimRoom,HallSizeX,HallSizeY,HallSizeZ,HallOffsetX,HallOffsetY,HallOffsetZ` | Approved names: `SCR_GF_E27_01` (auditorium, dims E27) and `SCR_GF_E09_01` (project gallery, no dimming). Fill `Media` and hall sizes (m) before the pilot |

## Things to verify on day 1 (expected first-compile risks)

- `UInputMappingContext::MapKey` still returns `FEnhancedActionKeyMapping&` in 5.8.
- SunSky Blueprint still exposes `SolarTime` and `UpdateSun`.
- Python names: `DatasmithSceneElement.import_scene`, `StaticMesh.nanite_settings`, `StaticMeshEditorSubsystem.add_simple_collisions`, `MediaTextureFactoryNew`, `FileMediaSourceFactoryNew`.
- Media in UI: a `UMediaTexture` drawn directly in a `UImage` brush.
- Arabic text shaping and right-to-left order in UMG with Noto Naskh Arabic; Python access to `FontData.font_face_asset`.

Any compile error: send the full UBT log back; do not edit around it silently.
