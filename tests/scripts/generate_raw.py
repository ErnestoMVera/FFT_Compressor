#!/home/ernestovera/anaconda3/bin/python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm 
import cv2 as cv
import pandas as pd
import os
import sys
nw = 0
jpgfile = sys.argv[1]
img = cv.imread(jpgfile, cv.IMREAD_GRAYSCALE)
to_write = open(sys.argv[2], mode = 'wb')
print(img.shape[1].to_bytes(4, 'little'))
print(img.shape[0].to_bytes(4, 'little'))
to_write.write(img.shape[1].to_bytes(4, 'little'))
to_write.write(img.shape[0].to_bytes(4, 'little'))
for h in range(img.shape[0]):
    for w in range(img.shape[1]):
        temp = int(img[h][w])
        to_write.write(temp.to_bytes(1, 'little'))
        nw = nw + 1
print("Expected:", img.shape[0]*img.shape[1])
print("real:", nw)
to_write.close()

