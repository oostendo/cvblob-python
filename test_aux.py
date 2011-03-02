#!/usr/bin/python

import cv
import cvblob

pt1 = (1, 1);
pt2 = (2, 2);
pt3 = (3, 2);

print cvblob.DistancePointPoint(pt1, pt2)
print cvblob.DotProductPoints(pt1, pt2, pt3)
print cvblob.CrossProductPoints(pt1, pt2, pt3)
print cvblob.DistanceLinePoint(pt1, pt2, pt3)
print cvblob.DistanceLinePoint(pt1, pt2, pt3, False)

