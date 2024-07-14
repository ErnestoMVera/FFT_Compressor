#!/home/ernestovera/anaconda3/bin/python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
import struct
import sys
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
        d = imgfile.read(4)
        a = struct.unpack('f', d)[0]
        temp.append(a)
    image.append(temp)
imgfile.close()
plt.imshow(image, cmap = cm.gray)
plt.colorbar()
plt.title("Imagen descomprimida")
plt.show()

