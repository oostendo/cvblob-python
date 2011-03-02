Python Wrapper for the cvblob extension to OpenCV

2011 Nathan Oostendorp, Ingenuitas Inc.

This wrapper provides cvBlob's functionality wrappered in a style similar to
the native OpenCV python bindings.  The function calls are as close as 
possible to the C++ functions so that you can use the C++ docs/example code.

Conventions are similar to the OpenCV 2.2 python interface, in brief:
 - functions: cvFUNCTIONNAME is contracted to FUNCTIONAME
 - data structures: CvDATATYPE is contracted to DATATYPE 
 - color triples and coordinates are expressed as tuples
 - vectors/arrays/lists expressed as python lists
 - maps are expressed as python dicts

For examples, look at the test python code.

Brief install instructions (for OpenCV 2.2, cvBlob 0.10.3 under Ubuntu 10.10):

Step 1: install some prerequisite libraries:

  apt-get install build-essential swig gfortran cmake gcc pkg-config libjpeg62-dev libtiff4-dev libpng12-dev libopenexr-dev libavformat-dev libswscale-dev liblapack-dev python-dev python-setuptools python-nose boost-build libboost-all-dev

Step 2: Download most recent versions of NumPy, OpenCV, and cvBlob
 - http://sourceforge.net/projects/numpy/files/NumPy/
 - http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/
 - http://code.google.com/p/cvblob/
  
Step 3: Make NumPy, OpenCV, and cvBlob 

Numpy:

  python setup.py build --fcompiler=gnu95
  python setup.py install
  python -c 'import numpy; numpy.test()'

OpenCV:

  mkdir BUILD
  cd BUILD
  cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_PYTHON_SUPPORT=ON -D WITH_TBB=ON -D CMAKE_INSTALL_PREFIX=/usr ..
  make
  make install
  python -c ‘import cv;’

cvBlob:

  cmake
  make
  make install

cvblob-python (from your cvblob directory):

  cd cvblob-VERSION/contrib
  git git://github.com/oostendo/cvblob-python.git python 
  cd python/cvblob
  ./build.sh (you may need to edit if you have a different python than 2.6)
  cd ..
  python test.py
  cp cvblob /usr/lib/python2.6/site-packages (or wherever)
  
Thanks to Willowgarage for OpenCV, Cristobal Carnero Linan for cvblob,
Dave Abrhams for Boost Python, and et al.
