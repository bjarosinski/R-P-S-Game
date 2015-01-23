#include "cv_headers.h"

//funckja zwraca w trzecim parametrze liczb� defekt�w konturu contour przekraczaj�cych g��boko�� thr
void recognizeSymbol ( CvSeq* contour, float thr, int &defects ) {
	
	if ( ( contour == NULL ) || ( contour->total < 1 ) ) {
		defects = -1;
		return;
	}
	//znajduje wypuk�o�ci na zbiorze punkt�w (konturze)
	CvSeq *hull = cvConvexHull2( contour, 0, CV_CLOCKWISE, 0 );
	//wyszczeg�lnia defekty
	CvSeq *defects_seq = cvConvexityDefects( contour, hull );
	
	if (!defects_seq){
		defects=-1;
		return;
	}
	//Reader sekwencji defekt�w
	CvSeqReader reader;
	//Zainicjowanie czytnika
	cvStartReadSeq( defects_seq, &reader, 0 );
	int defectCount = 0;
	int elemTotal = defects_seq->total;
	int elemSize = defects_seq->elem_size;

	for ( int i = 0; i < elemTotal; i++ ) {
		//tworzenie defektu b�d�cego kolejnym defektem wskazywanym przez reader
		CvConvexityDefect* next = ( CvConvexityDefect* )( reader.ptr );
		//przesuni�cie readera
		CV_NEXT_SEQ_ELEM( elemSize, reader );
		//czy defekt przekracza zadany prog
		if ( next->depth >= thr )
			defectCount++;
	}
	defects = defectCount;
}
