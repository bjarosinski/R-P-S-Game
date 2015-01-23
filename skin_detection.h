#ifndef SKIN_DETECTION_Hs
#define SKIN_DETECTION_Hs

#include "cv_headers.h"

extern IplImage* skinDetection( IplImage* img,double thrLam, unsigned int canalVthr, float* vectorMs, float* covarianceMatrix );
extern CvSeq* drawContours( IplImage *img, IplImage *mask );

extern float vectorMs[];
extern float covarianceMatrix[];
#endif