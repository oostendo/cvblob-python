#!/usr/bin/env python
#
# Test script to isolate why cvblob-python wrapper of cvUpdateTracks currently not working
# Modelled after test_tracking.cpp found in cvblob/test/
# Uses 	EnterExitCrossingPaths2front_blobs.mpeg also in cvblob/test/
# 
# To test how cvb.Label is performing:
# mkdir tmp/
# uncomment last line in this script 
# People start walking near frame 185
#
import cv
import cvblob as cvb
import subprocess


def TrackFile(anim):
	tracks=cvb.Tracks()
	capture=cv.CreateFileCapture(anim)
	cv.GrabFrame(capture)
	img=cv.RetrieveFrame(capture)
	frame = cv.CreateImage(cv.GetSize(img), img.depth, img.nChannels)
	cnt=1
	while cv.GrabFrame(capture):
		# Capture Frames
		img = cv.RetrieveFrame(capture)
		cv.ResetImageROI(frame)
		cv.ConvertScale(img, frame, 1, 0)
		cv.Threshold(frame, frame, 100, 200, cv.CV_THRESH_BINARY)
		#rct=cv.Rectangle(0, 25, 383, 287)
		cv.SetImageROI(frame, (0, 25, 383, 287))
		chB=cv.CreateImage(cv.GetSize(frame),8,1)
		cv.Split(frame,chB,None,None,None)
		labelImg = cv.CreateImage(cv.GetSize(frame), cvb.IPL_DEPTH_LABEL, 1)
		
		# Get Blobs and try Update Tracks
		blobs=cvb.Blobs()
		result=cvb.Label(chB, labelImg, blobs)
		cvb.FilterByArea(blobs, 500, 1000)
		
		# Trys are implemented here just to ensure crashes don't happen when blobs are not present
		try:
			print type(blobs.items()[0][1])
		except:
			pass	
		cvb.UpdateTracks(blobs, tracks, 5., 10, 0)
		try:
			print type(blobs.items()[0][1])
		except:
			pass
		try:
			print type(tracks.items()[0][1])
		except:
			pass
		imgOut = cv.CreateImage(cv.GetSize(frame), cv.IPL_DEPTH_8U, 3)
		cv.Zero(imgOut)		
		cvb.RenderBlobs(labelImg, blobs, frame, imgOut, cvb.CV_BLOB_RENDER_COLOR|cvb.CV_BLOB_RENDER_CENTROID|cvb.CV_BLOB_RENDER_BOUNDING_BOX)
		
		# Save images to see what's blobs are getting out.
		cnt=cnt+1;print cnt    #
		cv.SaveImage('blobs'+str(cnt)+'.png', imgOut)
	
	return tracks, blobs
	
	
if __name__=="__main__":
	anim='../../test/EnterExitCrossingPaths2front_blobs.mpeg'
	trks, blbs = TrackFile(anim)
	#subprocess.call('animate -delay 5 `ls -v tmp/blobs*.png`',shell=True)
