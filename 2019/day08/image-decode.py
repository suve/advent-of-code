#!/usr/bin/python3
# -*- coding: utf-8 -*-

import PIL.Image
import os
import sys
import tempfile


lines = sys.stdin.read().split("\n")

size = lines[0].split(":")
width = int(size[0])
height = int(size[1])
size = width * height

layer = []
data = lines[1]
while len(data) > 0:
    layer.append(data[:size])
    data = data[size:]

layers = len(layer)
pixels = list(layer[layers-1])
for l in range(layers-2, -1, -1):
    for i in range(size):
        pixel = layer[l][i]
        if pixel in ['0', '1']:
            pixels[i] = pixel

for p in range(len(pixels)):
    if pixels[p] == '0':
        pixels[p] = 0
    elif pixels[p] == '1':
        pixels[p] = 255
    else:
        pixels[p] = 127


# PIL needs to be able to seek() in the file it's writing to,
# so unfortunately we can't use stdout directly
handle, tempname = tempfile.mkstemp()

img = PIL.Image.frombytes('L', (width, height), bytes(pixels))
img.save(tempname, "PNG")

data = open(tempname, 'rb').read()
os.unlink(tempname)
open("/dev/stdout", 'wb').write(data)
