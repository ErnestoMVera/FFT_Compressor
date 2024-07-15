#!/home/ernestovera/anaconda3/bin/python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
import struct
import sys
if len(sys.argv) == 1:
    print("Image file is needed")
    exit(0)
if len(sys.argv) == 2:
    title = "Original Image"
    sub = ""
elif len(sys.argv) == 3:
    title = sys.argv[2]
    sub = ""
else:
    title = "Original Image"
    sub = ""
imgfile = open(sys.argv[1], mode = 'rb')
d = imgfile.read(4)
width = int.from_bytes(d, "little")
d = imgfile.read(4)
height = int.from_bytes(d, "little")
print("Width x Height =", width, "x", height)
image = []
for i in range(height):
    temp = []
    for j in range(width):
        d = imgfile.read(1)
        a = int.from_bytes(d, "little")
        temp.append(a)
    image.append(temp)
imgfile.close()
plt.imshow(image, cmap = cm.gray)
plt.colorbar()
plt.title(title)
plt.show()

