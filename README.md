# DoomVst
Doom, but it runs in a VST plugin inside your DAW program and uses MIDI input for controls.
<img src="https://github.com/michael-truscott/DoomVst/blob/master/docs/DoomVst.png">

Adapted from doomgeneric (https://github.com/ozkl/doomgeneric) and hastily hacked together into a JUCE plugin. Game audio not supported, sorry :(

## How to run
- Copy DoomVst.vst3 into your DAW's VST folder.
- Copy a Doom IWAD file (e.g. doom.wad or doom2.wad) into your DAW's working directory (usually something like C:\Program Files\[ProgramName]).
- Add an instance of the Doom Vst plugin to a track in your DAW.
- Kill some demons.

Tested with REAPER (x64), may not work with other DAWs.

**WARNING:** May possibly crash your DAW and cause you to lose work, so probably don't use this on anything important.

## Controls
The plugin is controlled by MIDI note on/off events from the host DAW.
- C3 - Turn Left
- D3 - Move Forward
- E3 - Turn Right
- F3 - Move Backward  
- C4 - Enter
- D4 - Shoot
- E4 - Use
- F4 - Run

## Compiling from source
### Requires
- Visual Studio 2022
- JUCE 7 (https://juce.com/)

Open DoomVst.jucer in Projucer and then export the project to Visual Studio 2022, then build and run it.
