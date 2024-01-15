# "Gravity" simulation

Uh this doesnt work like gravity but it looks cool. First C & Raylib "project" btw.

## "Features" (bugs)

- Has two modes: Interactive and Exportable
  - Interactive: You can interact with the "gravity" simulation
  - Exportable: You can export the simulation at **60 FPS** using the uh *broken "gravity" simulation technology*™️
  - But wait, you can export the simulation with CUSTOM FPS and CUSTOM LENGTH!
- "Gravity" simulation: Its not gravity, its a buggy gravity code that... somehow is interesting.

## Export

Make sure to create a `frames` directory using `mkdir frames`

Run the simulation. It will look weird and will have low fps, but it will create frame images.
Will take more memory bcuz i think memory leaks and im new to c to detect them but yeah...

Command to create video from frames (requires ffmpeg):
`ffmpeg -framerate <your framerate> -i 'frames/%d.png' -c:v libx264 -pix_fmt yuv420p output.mp4`

Make sure to replace <your framerate> with the framerate you configured.
All the configurable parameters are in config.h (root directory of repo)

For example, for 60fps:
`ffmpeg -framerate 60 -i 'frames/%d.png' -c:v libx264 -pix_fmt yuv420p output.mp4`

The simulation takes a relatively long time to export. Drink coffee or smth.
The exported simulation is much better compared to the interactive mode. However, you cant... interact... with the simulation in export mode.

For some reason, it flips the frames in Y axis. I totally will fix it but heres the command to fix it:
`ffmpeg -i output.mp4 -vf "vflip" -c:a copy output_flipped.mp4`
