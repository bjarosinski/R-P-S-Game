#include "cv_headers.h"

//funckja zwraca w trzecim parametrze liczbê defektów konturu contour przekraczaj¹cych g³êbokoœæ thr
void recognizeSymbol ( CvSeq* contour, float thr, int &defects ) {
	
	if ( ( contour == NULL ) || ( contour->total < 1 ) ) {
		defects = -1;
		return;
	}
	//znajduje wypuk³oœci na zbiorze punktów (konturze)
	CvSeq *hull = cvConvexHull2( contour, 0, CV_CLOCKWISE, 0 );
	//wyszczególnia defekty
	CvSeq *defects_seq = cvConvexityDefects( contour, hull );
	
	if (!defects_seq){
		defects=-1;
		return;
	}
	//Reader sekwencji defektów
	CvSeqReader reader;
	//Zainicjowanie czytnika
	cvStartReadSeq( defects_seq, &reader, 0 );
	int defectCount = 0;
	int elemTotal = defects_seq->total;
	int elemSize = defects_seq->elem_size;

	for ( int i = 0; i < elemTotal; i++ ) {
		//tworzenie defektu bêd¹cego kolejnym defektem wskazywanym przez reader
		CvConvexityDefect* next = ( CvConvexityDefect* )( reader.ptr );
		//przesuniêcie readera
		CV_NEXT_SEQ_ELEM( elemSize, reader );
		//czy defekt przekracza zadany prog
		if ( next->depth >= thr )
			defectCount++;
	}
	defects = defectCount;
}
