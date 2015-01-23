#include "cv_headers.h"
extern void destroyCvWindow(char* name);
extern void makeCV(bool &ext, int &defects);
extern void playHu(bool &ext, int &defects1, int &defects);
extern float *vectorMs;
extern float *covarianceMatrix;
extern IplImage* getImage();
extern bool calibration();