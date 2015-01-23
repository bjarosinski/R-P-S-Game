#include "cv_headers.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;
float vectorMs[] = { 144.8209f,   86.6673f };
//135.2135, 89.8604
float covarianceMatrix[] = {
					157.3959f, -60.4160f, -60.4160f, 278.6709f};

//1 157.3959, -60.4160, =60.4160, 278,6709
// 90.2443, -43.6965, -43.6965, 318.8425

IplImage *eventImg = NULL;
bool zaznaczanie = false;
CvPoint point1 = cvPoint(0, 0);
CvPoint point2 = cvPoint(0, 0);
IplImage *img;
IplImage *imgHSV;
uchar *values[2];
CvArr **valuesHS;
bool shot=true;
	int pixels;
	bool load = false;

void on_mouse2( int evt, int x, int y, int flags, void* )
{
	if ( evt == CV_EVENT_LBUTTONDOWN ) {
		shot = false;
	} 
}

IplImage* getImage(){

	CvCapture *camera = cvCreateCameraCapture (0);
	IplImage* frame;
	IplImage* result;
	cvNamedWindow( "Click to take a picture" );
	cvSetMouseCallback( "Click to take a picture", on_mouse2 );
	frame = cvQueryFrame( camera );
	result = cvCloneImage( frame );
	while( shot ) {

		frame = cvQueryFrame(camera);
		cvFlip( frame, result, 1 );
		cvShowImage( "Click to take a picture", result );
		cvWaitKey( 30 );

	}
	cvDestroyWindow( "Click to take a picture" );
	return result;

}
void on_mouse( int evt, int x, int y, int flags, void* )
{

    if ( !eventImg ) {
        return;
	}
	IplImage *imgCopy = cvCreateImage( cvSize( eventImg->width, 
				eventImg->height ), eventImg->depth, eventImg->nChannels );
	cvCopyImage( eventImg, imgCopy );
	if ( evt == CV_EVENT_LBUTTONDOWN ) {
		point1 = cvPoint( x, y );
		zaznaczanie = true;
	} else if ( zaznaczanie && ( evt == CV_EVENT_MOUSEMOVE ) && ( flags & CV_EVENT_FLAG_LBUTTON ) ) {
		cvDrawRect( imgCopy, point1, cvPoint( x, y ), CV_RGB( 0, 128, 0 ), 1 );
		cvShowImage( "Choose a hand", imgCopy );
		cvReleaseImage( &imgCopy );
    } else if ( zaznaczanie && ( ( evt == CV_EVENT_LBUTTONUP) || !( flags & CV_EVENT_FLAG_LBUTTON ) ) ) {
		point2 = cvPoint(x, y);
		cvDrawRect( imgCopy, point1, point2, CV_RGB( 0, 128, 0 ), 1 );
		cvShowImage( "Choose a hand", imgCopy );
		cvReleaseImage( &imgCopy );
		zaznaczanie = false;
	}
}

bool calibration() {

	img = getImage();

	if ( !img )
		return false;
	if ( img->nChannels != 3 ) {
		cvReleaseImage( &img );
		return false;
	}
	//zamiana na HSV
	imgHSV = cvCreateImage( cvSize( img->width, img->height ), 8, 3 );
	cvCvtColor( img, imgHSV, CV_RGB2HSV );

	eventImg = img;
	cvNamedWindow( "Choose a hand", 1 );
	cvShowImage( "Choose a hand", img );
	cvSetMouseCallback( "Choose a hand" , on_mouse, 0 );

	while(true) {
		int c = cvWaitKey( 500 ); 

		if (c == 27)
			break;
		else if (c == '\r') {
			load = true;
			break;
		}

		if ( !cvGetWindowHandle( "Choose a hand" ) )
			break; // zamknieto okno bez wybierania
	}

	if (load) {

		//obliczanie wspó³rzênych krañcowych
		int xmin = min( point1.x, point2.x );
		int xmax = max( point1.x, point2.x );
		int ymin = min( point1.y, point2.y );
		int ymax = max( point1.y, point2.y );

		if ((xmin != xmax) && (ymin != ymax)) {
			//oblicza iloœæ pikseli na zaznaczonym obrazie
			int pixelsCount = ( xmax - xmin ) * ( ymax - ymin );
			//values[0]-kana³ H, values[1]-kana³ S
			values[0] = (uchar *)realloc(values[0], (pixelsCount) * sizeof(uchar));
			values[1] = (uchar *)realloc(values[1], (pixelsCount) * sizeof(uchar));

			int step = imgHSV->widthStep;
			uchar* data = (uchar *)imgHSV->imageData;
			//wype³nianie macierzy values
			for (int i = ymin; i < ymax; ++i)
				for (int j = xmin; j < xmax; ++j) {
					values[0][pixels] = data[i*step+j*3+0];
					values[1][pixels] = data[i*step+j*3+1];
					++pixels;
				}
		}
	}

	if ( cvGetWindowHandle( "Choose a hand" ) ) {
		cvDestroyWindow( "Choose a hand" );
	}
	if ( pixels == 0 )
		return false;
	cvReleaseImage( &img );
	cvReleaseImage( &imgHSV );
	
	for( int i = 0; i<4; i++ ){

		if(i<2)
			vectorMs[i]=0.0;
		covarianceMatrix[i]=0.0;
	}
	
	void **data = ( void ** )malloc( pixels * sizeof( void * ) );
	float *dataFloat = ( float * )malloc( pixels * 2 * sizeof( float ) );

	/*for ( int i = 0; i < pixels; ++i ) {
		dataFloat[i*2] = ( float )values[0][i];
		dataFloat[i*2+1] = ( float )values[1][i];
		CvMat wart = cvMat( 1, 2, CV_32FC1, &dataFloat[i*2] );
		CvMat *nwart = new CvMat( wart );
		data[i] = nwart;
	}*/
	int i=0;
	while( i < pixels ){

		dataFloat[i*2] = ( float )values[0][i];
		dataFloat[i*2+1] = ( float )values[1][i];
		CvMat wart = cvMat( 1, 2, CV_32FC1, &dataFloat[i*2] );
		CvMat *nwart = new CvMat( wart );
		data[i] = nwart;
		i++;
	}
	//macierz kowariancji przechowuj¹ca wartoœci w covarianceMatrix
	CvMat cvMatKowariancja = cvMat( 2, 2, CV_32FC1, covarianceMatrix );
	CvMat cvMs = cvMat( 1, 2, CV_32FC1, vectorMs );
	//obliczenie wartoœci macierzy kowariancji i wartoœci œrednich
	cvCalcCovarMatrix( ( const void ** )&data[0], pixels, &cvMatKowariancja, &cvMs, CV_COVAR_SCALE | CV_COVAR_NORMAL );

	for ( int i = 0; i < pixels; ++i )
		delete ( CvMat * )data[i];
	free( data );
	free( dataFloat );

	return true;
}
