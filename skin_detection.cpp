#include "cv_headers.h"

IplImage* skinDetection( IplImage* img,double thrLam, unsigned int canalVthr, float *vectorMs, float*covarianceMatrix ) {

	IplImage* imgHSV = cvCreateImage( cvSize( img->width, img->height), 8, 3 );
	cvCvtColor(img, imgHSV, CV_RGB2HSV);
	int height = imgHSV->height;
	int width = imgHSV->width;
	int step = imgHSV->widthStep;
	//HSV - 3 kana³y 
	int channels = 3;
	//cvCvtColor(img, imgHSV, CV_RGB2HSV);

	IplImage* mask = cvCreateImage( cvSize( width, height ), 8, 1 );
	
	CvMat* cm;
	CvMat* cmInv;
	CvMat* ms;

	//Tworzenie macierzy 2x2 przechowuj¹cej wartoœci macierzy kowariancji 
		//Cv_32FC1 - 32 bitowy float o depth = 1 channel;
	cm = cvCreateMat( 2, 2, CV_32FC1 );
	//Wype³nianie macierzy cm wartoœciami macierzy kowariancji
	cvInitMatHeader( cm, 2, 2, CV_32FC1, covarianceMatrix );
	//Tworzenie macierzy odwrotnej 2x2 do macierzy CS
	cmInv = cvCreateMat( 2, 2, CV_32FC1 );
	cvInvert( cm, cmInv );
	//Tworzenie macierzy 1x2 przechowuj¹cej wartoœci œrednie
	ms = cvCreateMat( 1, 2, CV_32FC1 );
	//Wype³nianie macierzy ms wartoœciami œrednimi
	cvInitMatHeader( ms, 1, 2, CV_32FC1, vectorMs );
	
	//Tworzenie macierzy pomocniczych
	CvMat* h = cvCreateMat( 1, 2, CV_32FC1 );
	CvMat* hMs = cvCreateMat( 1, 2, CV_32FC1 );
	CvMat* hMsT = cvCreateMat( 2, 1, CV_32FC1 );
	CvMat* hMsCmInv = cvCreateMat( 1, 2, CV_32FC1 );
	CvMat* hMsCmInvhMsT = cvCreateMat( 1, 1, CV_32FC1 );
	//data przchowywaæ bêdzie wskaŸnik do danych
	uchar* data = ( uchar * )imgHSV->imageData;
	//stepMadk - rozmiar pojedynczego wiersza maski w bajtach
	int stepMask = mask->widthStep;
	//wskaŸnik do danych maski
	uchar* dataMask = ( uchar * )mask->imageData;
	for ( int i = 0; i < height; i++ )
		for ( int j = 0; j < width; j++ ) {
			//X[0][0] - wartoœæ kana³u H
			cvmSet( h, 0, 0, ( double )data[i*step+j*3+0] );
			//X[0][1] - wartoœæ kana³u S
			cvmSet( h, 0, 1, ( double )data[i*step+j*3+1]) ;
			//Xms - ró¿nica miêdzy wartoœciami HS obrazu, a wartoœciami œrednimi pobranymi w czasie kalibracji
			cvSub( h, ms, hMs );
			//Transpozycja macierzy Xms
			cvTranspose( hMs, hMsT );
			//Mno¿enie macierzy Xms a macierzy odwrotnej do macierzy kowariancji
			cvMatMul( hMs, cmInv, hMsCmInv );
			//Mno¿enie macierzy XmsCsInv i  XmsT
			cvMatMul( hMsCmInv, hMsT, hMsCmInvhMsT );
			//lambda - rozumiana jako odchylenie punktu od wartoœci œredniej 
			double lam = cvmGet( hMsCmInvhMsT, 0, 0 );

			//prog canalu V - natezenia oswietlenia oraz odchylenia
			if ( ( data[i*step+j*3+2] >= canalVthr ) && ( lam < thrLam ) ) {

				dataMask[i*stepMask+j]=255;
			}

			else
				dataMask[i*stepMask+j]=0;
		}
	
	cvReleaseMat( &ms );
	cvReleaseMat( &cmInv );
	cvReleaseMat( &cm );
	cvReleaseMat( &hMsCmInvhMsT );
	cvReleaseMat( &hMsCmInv );
	cvReleaseMat( &hMsT );
	cvReleaseMat( &hMs );
	cvReleaseMat( &h );

	//cvSmooth( mask, mask, CV_MEDIAN, 3 );
	cvDilate( mask, mask, NULL, 4 );
	cvErode( mask, mask, NULL, 3 );
	return mask;
}

CvSeq* drawContours( IplImage *img, IplImage *mask ){

	CvSeq *kontur;
	//"przechowalnia" sekwencji punktów tworz¹cych kontury
	CvMemStorage* storage = cvCreateMemStorage( 0 );
	CvSeq* contours;
	//squares - sekwencja punktów
	CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );
	CvSeq* result;
	//wyszukiwanie konturów, RETR_TREE - pobiera wszystkie kontury i rekonstruuje pe³n¹ hierarchiê zagnie¿d¿onych konturów
		//CV_CHAIN_APPROX_SIMPLE - kompresuje poziome, pionowe i ukoœne segmenty i pozostawia tylko ich punkty koñcowe
		//contours - wskaŸnik na pierwszy kontur
	cvFindContours( mask, storage, &contours, sizeof( CvContour ), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	while( contours )
	{
		//przybli¿a krzywe z okreœlon¹ precyzj¹ 
		result = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.001, 0 );

		//znalezeienie d³oni, najwiêkszego konturu
		if ( fabs( cvContourArea( result,CV_WHOLE_SEQ ) ) > 10000 )
		{
			squares = result;
			break;
		}

		// bierze nastêpny kontur
		contours = contours->h_next;
	}

	// wynik
	kontur = squares;


	if ( kontur == NULL )
		return NULL;
	//rysuje kontur
		//red line, thickness 2 , levels - 15
	cvDrawContours( img, kontur, CV_RGB( 255,0,0 ), CV_RGB( 255,0,0 ), 15, 2, CV_AA, cvPoint( 0,0 ) );
	return kontur;
}