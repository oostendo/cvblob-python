#!/usr/bin/python

import cv
import cvblob

#load the blob image from the test folder
img = cv.LoadImage("../../test/test.png", 1)

#convert to a greyscale image, and set intensity thresholds
grey = cv.CreateImage(cv.GetSize(img), cv.IPL_DEPTH_8U, 1)
cv.CvtColor(img, grey, cv.CV_BGR2GRAY)
cv.Threshold(grey, grey, 100, 255, cv.CV_THRESH_BINARY)

#build the label image
labelImg = cv.CreateImage(cv.GetSize(img), cvblob.IPL_DEPTH_LABEL, 1)

#initialize a blobs class, and extract blobs from the greyscale image
blobs = cvblob.Blobs()
#import inspect
#print inspect.getmembers(grey)
#print grey.__repr__()
result = cvblob.Label(grey, labelImg, blobs)
numblobs = len(blobs.keys())

print str(numblobs) + " blobs found covering " + str(result) + "px"

avgsize = int(result / numblobs)

import copy
aboveAvgBlobs = copy.copy(blobs)
cvblob.FilterByArea(aboveAvgBlobs, avgsize, result)
print "blobs " + str(aboveAvgBlobs.keys()) + " are greater than the avg size of " + str(avgsize) + "px"

numblobs = len(blobs.keys())
print str(numblobs) + " blobs found covering " + str(result) + "px"
filtered = cv.CreateImage(cv.GetSize(img), cv.IPL_DEPTH_8U, 1)
cvblob.FilterLabels(labelImg, filtered, blobs)

bigblob = cvblob.GreaterBlob(blobs)
print "largest blob is " + str(bigblob) + " which is " + str(blobs[bigblob].area) + " px"

bigblobs = copy.copy(blobs)
cvblob.FilterByLabel(bigblobs, bigblob)
print str(len(bigblobs.keys())) + " blobs with label " + str(bigblob)

centroid = cvblob.Centroid(bigblobs[bigblob])
print "centroid of blob " + str(bigblob) + " is " + str(centroid)

print "angle of blob " + str(bigblob) + " is " + str(cvblob.Angle(blobs[bigblob]))

cvblob.SaveImageBlob("blob_" + str(bigblob) + ".png", img, blobs[bigblob])



print "point x: 250 y: 100 is blob " + str(cvblob.GetLabel(labelImg, 250, 100))

#cvblob.RenderBlob(labelImg, blobs[bigblob], img, rendered, cvblob.CV_BLOB_RENDER_COLOR|cvblob.CV_BLOB_RENDER_CENTROID|cvblob.CV_BLOB_RENDER_BOUNDING_BOX|cvblob.CV_BLOB_RENDER_ANGLE, cv.RGB(0,0,255), 0.9)
#cvblob.RenderBlob(labelImg, blobs[1], img, rendered, cvblob.CV_BLOB_RENDER_COLOR|cvblob.CV_BLOB_RENDER_CENTROID|cvblob.CV_BLOB_RENDER_BOUNDING_BOX|cvblob.CV_BLOB_RENDER_ANGLE, cv.RGB(255,0,255))
#cvblob.RenderBlob(labelImg, blobs[bigblob], img, rendered, cvblob.CV_BLOB_RENDER_COLOR|cvblob.CV_BLOB_RENDER_CENTROID|cvblob.CV_BLOB_RENDER_BOUNDING_BOX|cvblob.CV_BLOB_RENDER_ANGLE)
#cvblob.RenderBlob(labelImg, blobs[bigblob], img, rendered)
imgOut = cv.CreateImage(cv.GetSize(img), cv.IPL_DEPTH_8U, 3)
cv.Zero(imgOut)
cvblob.RenderBlobs(labelImg, blobs, img, imgOut, cvblob.CV_BLOB_RENDER_COLOR|cvblob.CV_BLOB_RENDER_CENTROID|cvblob.CV_BLOB_RENDER_BOUNDING_BOX|cvblob.CV_BLOB_RENDER_ANGLE, 1.0)

#cvblob.SetImageROItoBlob(imgOut, blobs[bigblob]);

print "mean color for blob " + str(bigblob) + " is " + str(cvblob.BlobMeanColor(blobs[bigblob], labelImg, img))

cv.SaveImage("test_filtered.png", filtered)
cv.SaveImage("test_rendered.png", imgOut)
#draw the rendered blobs on an outline
#cvblob.RenderBlobs(labelImg, blobs, img, imgOut)

