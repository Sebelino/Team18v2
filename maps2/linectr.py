#!/bin/python2

import os,glob

width = 0
height = 0
fatfile = ""
tallfile = ""
for file in glob.glob('*.in'):
    h = 0
    with open(file) as f:
        for line in f:
            w = len(line)
            if w > width:
                fatfile = file
                width = w
            h += 1
    if h > height:
        tallfile = file
        height = h
print "max width=%s"% width
print "max height=%s"% height
print "tallest file=%s"% tallfile
print "fattest file=%s"% fatfile
