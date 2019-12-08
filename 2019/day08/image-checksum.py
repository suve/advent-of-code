#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

lines = sys.stdin.read().split("\n")

size = lines[0].split(":")
width = int(size[0])
height = int(size[1])
size = width * height

layers = []
data = lines[1]
while len(data) > 0:
    layers.append(data[:size])
    data = data[size:]

layer_digits = []
for l in range(len(layers)):
    digits = 10 * [0]
    for i in range(len(layers[l])):
        digit = int(layers[l][i])
        digits[digit] += 1
    layer_digits.append(digits)

best = 0
for l in range(1, len(layers)):
    if layer_digits[l][0] < layer_digits[best][0]:
        best = l

print("Layer {} has {} zeros".format(best, layer_digits[best][0]))
print("Checksum: {}".format(layer_digits[best][1] * layer_digits[best][2]))
