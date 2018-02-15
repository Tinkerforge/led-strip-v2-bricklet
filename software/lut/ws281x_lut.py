#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Use inverted pattern (mosfet inverts the signal in Bricklet)
def pattern(bit):
    if bit:
        return 0b0001
    else:
        return 0b0111

values = []
for i in range(256):
    value = 0
    for j in reversed(range(8)):
        value |= pattern((i >> j) & 1) << 4*(7-j)
    values.append(value)

print('const uint32_t ws281x_lut[256] = {')
for value in values:
    print('\t{0}, '.format(bin(value)))
print('};')
