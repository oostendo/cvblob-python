#!/usr/bin/python

import cv
import cvblob

cc = cvblob.ContourChainCode()
cc.startingPoint = (1,1);
cc.chainCode = [
  cvblob.CV_CHAINCODE_UP,
  cvblob.CV_CHAINCODE_UP,
  cvblob.CV_CHAINCODE_RIGHT,
  cvblob.CV_CHAINCODE_DOWN,
  cvblob.CV_CHAINCODE_DOWN,
  cvblob.CV_CHAINCODE_LEFT];

print cvblob.ContourChainCodePerimeter(cc)


cp = cvblob.ContourPolygon()
cp.extend([(1,1), (1,3), (2,3), (2,1)]);

print cvblob.ContourPolygonPerimeter(cp)


