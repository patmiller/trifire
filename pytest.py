try:
    import cv2
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    fps = 1
    width = 640
    height = 480
    out = cv2.VideoWriter("movie.mp4", fourcc, 1, (width, height))
    import numpy as np
except ModuleNotFoundError:
    out = None

import json
import os
import sys
import trifire

COMMANDS = sys.argv[1] if len(sys.argv) > 1 else "rrrsll"

state = trifire.init()
print(json.dumps(state,indent=2))

for command in COMMANDS:
    print('run',command)
    state = trifire.play(state,command)
    print(state)
    if out is not None:
        rgb = [((x & 0xFF0000)>>16, (x & 0xFF00)>>8, (x & 0xFF))
               for x in trifire.render(state)]
        frame_rgb = np.array(rgb,dtype=np.uint8).reshape(480,640,3)
        frame_bgr = frame_rgb[:, :, ::-1]
        out.write(frame_bgr)
print(json.dumps(state,indent=2))

bmp = trifire.bmp(state)
open('pytest.bmp','wb').write(bmp)
os.system('open pytest.bmp')

if out is not None:
    out.release()
    os.system('open movie.mp4')
