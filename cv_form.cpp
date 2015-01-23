#include "cv_headers.h"
#include "skin_detection.h"
#include "recognizeSymbol.h"


CvSeq* cnt, *cnt2;

void destroyCvWindow(char* name){

	cvDestroyWindow(name);
	return;
}

void playHu(bool &ext, int &defects1, int &defects){

	int width, height;

	CvCapture *camera = cvCreateCameraCapture ( 0 );

	IplImage *frame, *firstPl, *secondPl, *result, *mask1, *mask2, *small;

	cvNamedWindow("Camera",0);
	cvMoveWindow("Camera",670,50);

	frame=cvQueryFrame(camera);

	width=frame->width;
	height=frame->height;

	

	cvResizeWindow("Camera",width/2,height/2);
	//second=cvCreateImage(cvSize(width/2,height/2),klatka->depth,klatka->nChannels);
	//tworzenie pomocnczych obrazów
	small=cvCreateImage(cvSize(width/2,height/2),frame->depth,frame->nChannels);
	result=cvCreateImage(cvSize(width/2,height/2),frame->depth,frame->nChannels);
	firstPl=cvCreateImage(cvSize(small->width/2,small->height),small->depth,small->nChannels);
	secondPl=cvCreateImage(cvSize(small->width/2,small->height),small->depth,small->nChannels);
	while(!ext){

		cvZero(result);
		frame = cvQueryFrame(camera);
		cvWaitKey(10);
		//zmiana rozmiaru zdjêcia w celu wzrostu p³ynnoœci
		cvResize(frame,small);
		//odbicie lustrzane wzglêdem osi y
		cvFlip(small,small,1);
		//podzia³ obrazu na dwie czêœci - dwóch graczy
		cvSetImageROI(small,cvRect(0,0,width/4,height/2));	
		cvCopy(small,firstPl,NULL);
		cvResetImageROI(small);
		cvSetImageROI(small,cvRect(width/4,0,width/4,height/2));		
		cvCopy(small,secondPl,NULL);
		cvResetImageROI(small);
		//rozpoznanie symbolu dla obu graczy
		mask1=skinDetection(firstPl,7,30,vectorMs,covarianceMatrix);
		cnt=drawContours(firstPl,mask1);
		recognizeSymbol(cnt,30 ,defects1);
		mask2=skinDetection(secondPl,7,30,vectorMs,covarianceMatrix);
		cnt2=drawContours(secondPl,mask2);
		recognizeSymbol(cnt2,30,defects);
		//³¹czenie obrazów z narysowanymi konturami
		cvSetImageROI(result,cvRect(width/4,0,width/4,height/2));
		cvAdd(result,secondPl,result,NULL);
		cvResetImageROI(result);
		cvSetImageROI(result,cvRect(0,0,width/4,height/2));
		cvAdd(result,firstPl,result,NULL);
		cvResetImageROI(result);
		//dodanie linii na œrodku zdjêcia
		cvLine(result,cvPoint(width/4,0),cvPoint(width/4,height/2),cvScalarAll(255.0),2);
		cvShowImage("Camera",result);		
	}
	cvReleaseCapture(&camera);
	cvDestroyWindow("Camera");
	return;
}
void makeCV(bool &ext, int &defects){

	
	int width, height;

	CvCapture *camera = cvCreateCameraCapture (0);
	IplImage *frame,*second,*result,*mask;

	cvNamedWindow("Camera",0);
	cvMoveWindow("Camera",670,50);
	
	frame= cvQueryFrame(camera);//klatki z kamery
	width=frame->width;
	height=frame->height;

	

	cvResizeWindow("Camera",width/2,height/2);
	second=cvCreateImage(cvSize(width/2,height/2),frame->depth,frame->nChannels);
	result=cvCreateImage(cvSize(width/2,height/2),frame->depth,frame->nChannels);
	
	while(!ext){
	
		frame = cvQueryFrame(camera);//klatki z kamery
		//cv::Mat resized(100,100, klatka->type());
		//cv::resize(image, resized, resized.size());
		cvWaitKey(10);
		cvResize(frame,second);
		cvFlip(second,result,1);
		mask=skinDetection(result,4,40,vectorMs,covarianceMatrix);
		cnt=drawContours(result,mask);
		recognizeSymbol(cnt,30,defects);
		//cvLine(last,cvPoint(width/2,1),cvPoint(width/2,height),cvScalarAll(255.0),3);
		cvShowImage("Camera",result);
		

	}
	cvReleaseCapture(&camera);
	cvDestroyWindow("Camera");
	return;

}