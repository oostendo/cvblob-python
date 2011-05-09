###########################################################################
#
#  cvblob/__init__.py
#
#  2011 Nathan Oostendorp, Ingenuitas Inc
#
#  this is analogous to the cvblob.h file -- giving constants, declaring
#  the non-native types, and adding a couple helper functions
#  the real "guts" of this project is in _cvblob.so
#

import cv
from _cvblob import *

#chaincode constants
CV_CHAINCODE_UP = 0 # Up.
CV_CHAINCODE_UP_RIGHT = 1 # Up and right.
CV_CHAINCODE_RIGHT = 2 # Right.
CV_CHAINCODE_DOWN_RIGHT = 3 # Down and right.
CV_CHAINCODE_DOWN = 4 # Down.
CV_CHAINCODE_DOWN_LEFT = 5 # Down and left.
CV_CHAINCODE_LEFT = 6 # Left.
CV_CHAINCODE_UP_LEFT = 7 # Up and left.

#blob rendering options
CV_BLOB_RENDER_COLOR          = 0x0001 # Render each blog with a different color. \see cvRenderBlobs
CV_BLOB_RENDER_CENTROID       = 0x0002 # Render centroid. \see cvRenderBlobs
CV_BLOB_RENDER_BOUNDING_BOX   = 0x0004 # Render bounding box. \see cvRenderBlobs
CV_BLOB_RENDER_ANGLE          = 0x0008 # Render angle. \see cvRenderBlobs
CV_BLOB_RENDER_TO_LOG         = 0x0010 # Print blob data to log out. \see cvRenderBlobs
CV_BLOB_RENDER_TO_STD         = 0x0020 # Print blob data to std out. \see cvRenderBlobs

#cv track rendering
CV_TRACK_RENDER_ID           = 0x0001 # Print the ID of each track in the image. \see cvRenderTracks
CV_TRACK_RENDER_BOUNDING_BOX = 0x0002 # Draw bounding box of each track in the image. \see cvRenderTracks
CV_TRACK_RENDER_TO_LOG       = 0x0010 # Print track info to log out. \see cvRenderTracks
CV_TRACK_RENDER_TO_STD       = 0x0020 # Print track info to log out. \see cvRenderTracks

#yeah, i hate this -- not sure how to get templated #defines out
#so they have been wrappered in functions
IPL_DEPTH_LABEL = getIPL_DEPTH_LABEL();
CV_BLOB_MAX_LABEL = getCV_BLOB_MAX_LABEL();

cvChainCodeMoves= [ [ 0, -1],
  [ 1, -1],
  [ 1,  0],
  [ 1,  1],
  [ 0,  1],
  [-1,  1],
  [-1,  0],
  [-1, -1]
  ];

import types
#note that for the simple typedefs we can use casting, so these are pretty easy
ChainCode = types.IntType
Label = types.IntType
ID = types.IntType

#for each of these, we're going to be using converter functions to/from their
#python equivalents
Blobs = types.DictType
LabelBlob = types.TupleType
Tracks = types.DictType
IDTrack = types.TupleType
ChainCodes = types.ListType
ContoursChainCode = types.ListType
ContourPolygon = types.ListType

#this is some weird creepy magic in boost::python, but i had no ability
#to export a function named "Label" (expect it collided with some keyword)
def Label(img, imgOut, blobs):
  return _cvblob.LabelBlobs(img, imgOut, blobs)

#these were just a little too painful to wrapper for what they actually do
#compared to just implementing it in python.  Might bite you if you have
#1000s of blobs, and allows for sloppy memory mgmt 
def FilterByArea(blobs, minArea, maxArea):
  for i in blobs.keys():
    if (blobs[i].area < minArea or blobs[i].area > maxArea):
      del blobs[i];

def FilterByLabel(blobs, label):
  for i in blobs.keys():
    if (blobs[i].label != label):
      del blobs[i];

def SetImageROItoBlob(img, blob):
  cv.SetImageROI(img, (blob.minx, blob.miny, blob.maxx - blob.minx, blob.maxy - blob.miny))
