# RetroSurroundPlayer — prototype 0.1

A classic-style Windows video player prototype with selectable audio processing modes.

## Current prototype
- Opens and plays files supported by the installed Qt Multimedia backend.
- Classic Windows-like controls: Open, Play/Pause, Stop, timeline, volume.
- Audio mode selector: Direct, Stereo Widening (experimental), Matrix Surround (experimental).
- Includes a standalone DSP class for offline/stream integration experiments.

## Important audio limitation
The included `SurroundProcessor` is a starting point, not a faithful or certified Dolby Pro Logic II decoder. Real Pro Logic II decoding requires a more complete matrix decoder with steering logic, phase-aware filtering and carefully validated channel extraction. The current UI selection is a scaffold: Qt Multimedia's high-level player output is not yet routed through the DSP class.

## Build (Windows 10/11)
1. Install Visual Studio 2022 with **Desktop development with C++**.
2. Install Qt 6 with the matching MSVC kit and the **Multimedia** module.
3. Install CMake (or use the one bundled with Visual Studio).
4. Open a Developer PowerShell with Qt's MSVC environment configured.
5. From this directory:
   ```powershell
   cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2022_64"
   cmake --build build --config Release
   ```
   Replace the Qt path with the actual installed version and kit.
6. Run `build\Release\RetroSurroundPlayer.exe` (path may vary by generator).

## Next development milestones
1. Route decoded audio through a custom FFmpeg audio pipeline.
2. Add a tested matrix decoder for legacy Dolby Surround-encoded stereo.
3. Add optional binaural HRTF rendering for headphones.
4. Add audio/video clock synchronization, track selection, subtitles and playlist.
5. Package Qt/FFmpeg runtime dependencies and review all licenses.

## Naming
“Dolby” and “Pro Logic II” are trademarks of their respective owners. This independent prototype is not affiliated with or certified by Dolby.
