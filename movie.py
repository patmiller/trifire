try:
    import cv2
except ModuleNotFoundError as e:
    print("You need to run % pip3 install opencv-python")
    raise 
import glob
import os
import sys
import re

def framekey(s):
    return int(re.search(r'\d+',s).group())

# Grab the frames and sort numerically
frames = sys.argv[1:] or glob.glob("frame-*.bmp")
frames = sorted(frames,key=framekey) 
assert frames

# Get the frame size info (assume all the same)
frame0 = cv2.imread(frames[0])
height, width, _ = frame0.shape

# Create a video writer
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
fps = 1
video = cv2.VideoWriter('movie.mp4', fourcc, fps, (width, height))

# Fill in the frames we want (in numerical order)
for filename in frames:
    frame = cv2.imread(filename)
    print(frame.shape)
    print(frame)
    xx
    video.write(frame)

video.release()
