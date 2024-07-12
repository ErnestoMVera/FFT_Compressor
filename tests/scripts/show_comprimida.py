#!/home/ernestovera/anaconda3/bin/python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
import struct
import cmath
import sys
imgfile = open(sys.argv[1], mode = 'rb')
d = imgfile.read(4)
width = int.from_bytes(d, "little")
d = imgfile.read(4)
height = int.from_bytes(d, "little")
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
        d = imgfile.read(4)
        b = struct.unpack('f', d)[0]
        z = complex(a, b)
        temp.append(abs(z))
    image.append(temp)
image = np.array(image)
plt.imshow(np.log(1 + image), cmap = cm.gray)
plt.colorbar()
plt.title("Imagen comprimida")
plt.show()
