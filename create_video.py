import os
import sys

if len(sys.argv) < 3:
    try: FPS = int(sys.argv[1])
    except: print('Please provide proper fps');exit(1)
else:
    try: FPS = int(sys.argv[2])
    except: print('Please provide proper fps');exit(1)


os.system(f"ffmpeg -framerate {FPS} -i \"frames/%d.png\" -c:v libx264 -pix_fmt yuv420p output.mp4")
os.system("ffmpeg -i output.mp4 -vf \"vflip\" -c:a copy output_flipped.mp4")