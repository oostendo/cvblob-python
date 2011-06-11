#include <boost/python.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <cvblob.h> 

using namespace boost::python;

/***************************************************************


C++ side of Python interface for the cvblob library

2011 Nathan Oostendorp, Ingenuitas Inc

This shared object is loaded by __init__.py and provides most of the
direct wrapper calls.  

****************************************************************/

//First some macros, which will help us get from boost::python types
//to our cvblob-specific data formats.

//in OpenCV2.2 the convention for the cvPoint type is to treat it as a 
//tuple, we wrapper that here
#define tuple2CvPoint(a) cvPoint(extract<int>(a[0]), extract<int>(a[1]))
#define tuple2CvPoint2D64f(a) cvPoint2D64f(extract<double>(a[0]), extract<double>(a[1]))
#define tuple2CvScalar(a) cvScalar(extract<double>(a[0]), extract<double>(a[1]), extract<double>(a[2]))

//simple casting to our wrapped structs
#define object2CvBlob(a) (cvb::CvBlob*)extract<Blob*>(a)
#define object2CvTrack(a) (cvb::CvTrack*)extract<Track*>(a)

//lifted from OpenCV's python interface (modules/python/cv.cpp)
//this struct is how the descended IplImage is structured in the native
//OpenCV python bindings
struct iplimage_t {
  PyObject_HEAD
  IplImage *a;
  PyObject *data;
  size_t offset;
};

//helper function to convert python's cv.iplimage objects to native 
//cpp IplImage pointers
IplImage* object2IplImage(boost::python::object o) {
  iplimage_t *cva = (iplimage_t*)o.ptr();
  return (IplImage*)(cva->a);
}

//we're going to use Blob as a descended struct from cvBlob
struct Blob : cvb::CvBlob { };

//convert a python dictionary of int -> Blob to the std::map CvBlobs
cvb::CvBlobs* dict2CvBlobs(boost::python::dict d) {
  cvb::CvBlobs *b = new cvb::CvBlobs;
  boost::python::list keys = d.keys();
  boost::python::list vals = d.values();
  for (int i = 0; i < len(keys); i++) {
    b->insert(cvb::CvLabelBlob(extract<cvb::CvLabel>(keys[i]), object2CvBlob(vals[i])));
  }
  
  return b;
}

//similarly with Track, our wrappered python is descended
struct Track : cvb::CvTrack { };

//if I wuz a smarter C++ programmer, there would probably be a way to 
//template this with dict2CvBlobs, since both are fairly simple std::maps
cvb::CvTracks* dict2CvTracks (boost::python::dict d) {
  cvb::CvTracks *b = new cvb::CvTracks;
  boost::python::list keys = d.keys();
  boost::python::list vals = d.values();
  for (int i = 0; i < len(keys); i++) {
    b->insert(cvb::CvIDTrack(extract<cvb::CvID>(keys[i]), object2CvTrack(vals[i])));
  }
  
  return b;
}

//cv_aux.cpp wrappers, these are fairly basic math functions
double DistancePointPoint(tuple a, tuple b) {
  return cvb::cvDistancePointPoint(tuple2CvPoint(a), tuple2CvPoint(b)); 
} 

double DotProductPoints(tuple a, tuple b, tuple c) {
  return cvb::cvDotProductPoints(tuple2CvPoint(a), tuple2CvPoint(b), tuple2CvPoint(c)); 
}

double CrossProductPoints(tuple a, tuple b, tuple c) {
  return cvb::cvCrossProductPoints(tuple2CvPoint(a), tuple2CvPoint(b), tuple2CvPoint(c)); 
}

//DistanceLinePoint the is isSegment parameter is optional 
//so we need a wrapper for each case -- parameter provided, or not
double DistanceLinePoint(tuple a, tuple b, tuple c, bool isSegment){
  return cvb::cvDistanceLinePoint(tuple2CvPoint(a), tuple2CvPoint(b), tuple2CvPoint(c), isSegment); 
}

//same function, no segment parameter provided
double DistanceLinePoint1(tuple a, tuple b, tuple c){
  return cvb::cvDistanceLinePoint(tuple2CvPoint(a), tuple2CvPoint(b), tuple2CvPoint(c)); }

//I am having a hard time finding how exactly you are supposed to export 
//computed #define constants, so this works, but ick. 
int getIPL_DEPTH_LABEL(){ 
    return IPL_DEPTH_LABEL;
}
namespace cvb {
  int getCV_BLOB_MAX_LABEL(){ 
    return CV_BLOB_MAX_LABEL;
  }
}

cvb::CvChainCodes* list2CvChainCodes(boost::python::list l) {
  cvb::CvChainCodes *cc = new cvb::CvChainCodes();
  for (int i = 0; i < len(l); i++) {
    cc->push_back((cvb::CvChainCode) extract<int>(l[i]));
  }
  return cc; 
}


//class wrapper for the ContourChainCode
//uses Python-based types 
class ContourChainCode {
  public: 
  tuple startingPoint;
  boost::python::list chainCode;  

  cvb::CvContourChainCode* toCvContourChainCode();
};

//function to convert to a cvBlob type cvContourChainCode
cvb::CvContourChainCode* ContourChainCode::toCvContourChainCode(){
  cvb::CvContourChainCode *ccc = new cvb::CvContourChainCode;
  ccc->startingPoint = tuple2CvPoint(startingPoint);
  cvb::CvChainCodes *cc = list2CvChainCodes(chainCode);
  ccc->chainCode = *cc;
  
  return ccc; 
}

//function to convert a python list of ContourChainCode objects
//to a CvContoursChainCode object 
cvb::CvContoursChainCode* list2CvContoursChainCode(boost::python::list l) {
  cvb::CvContoursChainCode *ccc = new cvb::CvContoursChainCode();
  for (int i = 0; i < len(l); i++) {
    ContourChainCode c = extract<ContourChainCode>(l[i]);
    ccc->push_back(c.toCvContourChainCode());
  }
  
  return ccc;
}

//convert a list of tuples (points) to a ContourPolygon
cvb::CvContourPolygon* list2CvContourPolygon(boost::python::list l) {
  cvb::CvContourPolygon *cp = new cvb::CvContourPolygon();
  for (int i = 0; i < len(l); i++) {
    CvPoint p = tuple2CvPoint(l[i]);
    cp->push_back(p);
  }
  return cp;
}

//cvblob.cpp wrappers
double ContourChainCodePerimeter(ContourChainCode c){
  return cvb::cvContourChainCodePerimeter(c.toCvContourChainCode());
}

double ContourPolygonPerimeter(boost::python::list l){
  return cvb::cvContourPolygonPerimeter(list2CvContourPolygon(l));
}


//cvlabel.cpp wrappers

//this wrapper for the cvb::cvLabel function should be called label by my
//conventions, but unfortunately that was causing inexplicable runtime errors
//so we're calling it LabelBlobs and using __init__.py to wrapper
unsigned int LabelBlobs(boost::python::object img, boost::python::object imgOut, boost::python::dict &b) {
  cvb::CvBlobs blobs;
  unsigned int result = 0;
  result = cvb::cvLabel(object2IplImage(img), object2IplImage(imgOut), blobs);

  //load the result blobset into the passed dict
  for (cvb::CvBlobs::const_iterator i = blobs.begin(); i != blobs.end(); i++) {
    b[i->first] = (Blob*)i->second;
  }

  return result; 
}

//this function is a little bit deceptive since the other filter functions
//actually change the blobs dict -- this one creates a binary image with 
//blob forms in imgOut
void FilterLabels(boost::python::object imgIn, boost::python::object imgOut, boost::python::dict blobs) {
  cvb::cvFilterLabels(object2IplImage(imgIn), object2IplImage(imgOut), *dict2CvBlobs(blobs));
}

//do a lookup on the label image -- give the label of a blob at a specific
// X and Y coordinate
cvb::CvLabel GetLabel(boost::python::object img, unsigned int x, unsigned int y) {
  return cvb::cvGetLabel(object2IplImage(img), x, y);
}

//find the blob with the greatest area in blobs 
cvb::CvLabel GreaterBlob(boost::python::dict blobs) {
  return cvb::cvGreaterBlob(*dict2CvBlobs(blobs));
}

//compute the angle of the blob (in radians)
double Angle(Blob blob) {
  return cvb::cvAngle((cvb::CvBlob*)&blob);
}

//compute the centroid of blob, return as a tuple
tuple Centroid(Blob blob) {
  CvPoint2D64f c = cvb::cvCentroid((cvb::CvBlob*)&blob);
  return make_tuple(c.x, c.y);
}

//save the image segment of img defined by the blobs bounding area
//to filename
void SaveImageBlob(char *filename, boost::python::object img, Blob blob) {
  cvb::cvSaveImageBlob(filename, object2IplImage(img), (cvb::CvBlob*)&blob);
}

//render a blob on an image 
//three optional parameters mean a total of 4 RenderBlob functions.  
//First: all params provided
void RenderBlob(boost::python::object imgLabel, Blob blob, boost::python::object imgSource, boost::python::object imgDest, int mode, boost::python::tuple color, double alpha) {
  cvb::cvRenderBlob(object2IplImage(imgLabel), (cvb::CvBlob*)&blob, object2IplImage(imgSource), object2IplImage(imgDest), (short)mode, tuple2CvScalar(color), alpha);
}

//RenderBlob1 is with only required params
void RenderBlob1(boost::python::object imgLabel, Blob blob, boost::python::object imgSource, boost::python::object imgDest) {
  cvb::cvRenderBlob(object2IplImage(imgLabel), (cvb::CvBlob*)&blob, object2IplImage(imgSource), object2IplImage(imgDest));
}

//RenderBlob2 with only mode provided
void RenderBlob2(boost::python::object imgLabel, Blob blob, boost::python::object imgSource, boost::python::object imgDest, int mode) {
  cvb::cvRenderBlob(object2IplImage(imgLabel), (cvb::CvBlob*)&blob, object2IplImage(imgSource), object2IplImage(imgDest), (short)mode);
}

//RenderBlob3 with mode and color provided
void RenderBlob3(boost::python::object imgLabel, Blob blob, boost::python::object imgSource, boost::python::object imgDest, int mode, boost::python::tuple color) {
  cvb::cvRenderBlob(object2IplImage(imgLabel), (cvb::CvBlob*)&blob, object2IplImage(imgSource), object2IplImage(imgDest), (unsigned short)mode, tuple2CvScalar(color));
}

//Render a dict of blobs to the imgDest image
//Like RenderBlob, RenderBlobs has optional parameters which we'll break
//out in a similar way -- first, all params
void RenderBlobs(boost::python::object imgLabel, boost::python::dict blobs, boost::python::object imgSource, boost::python::object imgDest, int mode, double alpha){
  cvRenderBlobs(object2IplImage(imgLabel), *dict2CvBlobs(blobs), object2IplImage(imgSource), object2IplImage(imgDest), (unsigned short)mode, alpha);
} 

//only required parameters (no mode or alpha)
void RenderBlobs1(boost::python::object imgLabel, boost::python::dict blobs, boost::python::object imgSource, boost::python::object imgDest){
  cvRenderBlobs(object2IplImage(imgLabel), *dict2CvBlobs(blobs), object2IplImage(imgSource), object2IplImage(imgDest));
} 

//only mode optional parameter (no alpha)
void RenderBlobs2(boost::python::object imgLabel, boost::python::dict blobs, boost::python::object imgSource, boost::python::object imgDest, int mode){
  cvRenderBlobs(object2IplImage(imgLabel), *dict2CvBlobs(blobs), object2IplImage(imgSource), object2IplImage(imgDest), (unsigned short)mode);
} 

//find the mean color value of a blob.  return as a color tuple
boost::python::tuple BlobMeanColor(boost::python::object blob, boost::python::object imgLabel, boost::python::object img){
  CvScalar clr = cvb::cvBlobMeanColor(object2CvBlob(blob), object2IplImage(imgLabel), object2IplImage(img));
  return make_tuple(clr.val[0], clr.val[1], clr.val[2]);
}

//cvtrack.cpp wrappers

//given a dict of blobs b, update dict t with the tracks for each blob
void UpdateTracks(boost::python::dict b, boost::python::dict t, double thDistance, unsigned int thInactive, unsigned int thActive) {
  cvb::CvTracks* tracks = dict2CvTracks(t);
  cvb::cvUpdateTracks(*dict2CvBlobs(b), *tracks, thDistance, thInactive, thActive);

  //we need to make sure everything in *tracks is represented in dict
  for (cvb::CvTracks::const_iterator i = tracks->begin(); i != tracks->end(); i++) {
    t[i->first] = (Track*)i->second; 
  }
}

//boost block -- here's where we reveal everything to Python
BOOST_PYTHON_MODULE(_cvblob) {
  class_<ContourChainCode> ("ContourChainCode")
    .def_readwrite("startingPoint", &ContourChainCode::startingPoint) 
    .def_readwrite("chainCode", &ContourChainCode::chainCode);

  class_<Blob> ("Blob")
    .def_readwrite("label", &Blob::label)
    .def_readwrite("minx", &Blob::minx)
    .def_readwrite("maxx", &Blob::maxx)
    .def_readwrite("miny", &Blob::miny)
    .def_readwrite("maxy", &Blob::maxy)
    .def_readwrite("centroid", &Blob::centroid)
    .def_readwrite("m00", &Blob::m00)
    .def_readwrite("area", &Blob::area)
    .def_readwrite("m10", &Blob::m10)
    .def_readwrite("m01", &Blob::m01)
    .def_readwrite("m11", &Blob::m11)
    .def_readwrite("m20", &Blob::m20)
    .def_readwrite("m02", &Blob::m02)
    .def_readwrite("u11", &Blob::u11)
    .def_readwrite("u20", &Blob::u20)
    .def_readwrite("u02", &Blob::u02)
    .def_readwrite("n11", &Blob::n11)
    .def_readwrite("n20", &Blob::n20)
    .def_readwrite("n02", &Blob::n02)
    .def_readwrite("p1", &Blob::p1)
    .def_readwrite("p2", &Blob::p2)
    .def_readwrite("contour", &Blob::contour)
    .def_readwrite("internalContours", &Blob::internalContours);
 
  class_<Track> ("Track")
    .def_readwrite("id", &Track::id)
    .def_readwrite("label", &Track::label)
    .def_readwrite("minx", &Track::minx)
    .def_readwrite("maxx", &Track::maxx)
    .def_readwrite("miny", &Track::miny)
    .def_readwrite("maxy", &Track::maxy)
    .def_readwrite("centroid", &Track::centroid)
    .def_readwrite("lifetime", &Track::lifetime)
    .def_readwrite("active", &Track::active)
    .def_readwrite("inactive", &Track::inactive);

  //cvaux.cpp
  def("DistancePointPoint", DistancePointPoint);
  def("DistanceLinePoint", DistanceLinePoint);
  def("DistanceLinePoint", DistanceLinePoint1); //without optional param
  def("DotProductPoints", DotProductPoints);
  def("CrossProductPoints", CrossProductPoints);

  //cvblob.cpp
  def("GreaterBlob", &GreaterBlob);
  //filterByArea and filterByLabel implemented in __init__.py
  def("Centroid", &Centroid);
  def("Angle", &Angle);
  def("SaveImageBlob", &SaveImageBlob);

  def("RenderBlob", &RenderBlob);  //all params provided
  def("RenderBlob", &RenderBlob1); //no optional params
  def("RenderBlob", &RenderBlob2); //only render mode 
  def("RenderBlob", &RenderBlob3); //only render mode & color (no alpha)

  def("RenderBlobs", &RenderBlobs); //all params provided
  def("RenderBlobs", &RenderBlobs1); //only required params (no mode or alpha)
  def("RenderBlobs", &RenderBlobs2); //optional mode parameter provided

  def("GetLabel", &GetLabel);
  def("ContourChainCodePerimeter", ContourChainCodePerimeter);
  def("ContourPolygonPerimeter", ContourPolygonPerimeter);

  //cvlabel.cpp
  def("LabelBlobs", &LabelBlobs);
  def("FilterLabels", &FilterLabels);

  //cvColor.cpp
  def("BlobMeanColor", BlobMeanColor);

  //cvtrack.cpp
  def("UpdateTracks", UpdateTracks);



  //build-specific constants that we don't want to hardcode in __init__.py 
  def("getIPL_DEPTH_LABEL", getIPL_DEPTH_LABEL); 
  def("getCV_BLOB_MAX_LABEL", cvb::getCV_BLOB_MAX_LABEL); 
  
}



