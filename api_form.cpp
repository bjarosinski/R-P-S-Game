#include "api_headers.h"
#include "cv_window.h"
#include <string>
#include  <cstdio>
#include "game_logic.h"

using namespace std;
#define button1 501
#define combo1 502
#define IDT_TIMER1 503
#define IDT_TIMER2 505
#define button2 504

int defects=-1;
int defects1=-1;
BITMAP bckgInfo;
HBITMAP bckgHbm[5];
int as;
HWND combo;
HWND static_name, static_name2;
HWND button, buttonT;
HWND static_score;
HWND static_timer;
HWND hStatic2, hStatic;
bool ext=false;
unsigned int seconds=0;
int playOption=0;
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
bool sets=false;


char* setTime(unsigned int secs){

	char* tmp = new char[6];
	int min = secs/60;
	int sec = secs%60;
	if(sec<10)
		sprintf(tmp,"%d : 0%d",min,sec);
	else
		sprintf(tmp,"%d : %d",min,sec);
	return tmp;
}

HBITMAP loadImg(char* path){
	
	HBITMAP bmp;

	bmp=( HBITMAP ) LoadImage( NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	

	if(bmp==0)
	{
		MessageBox(0,"Nie mo¿na otworzyæ pliku bitmap.bmp z folderu z aplikacj¹","Brak pliku",MB_ICONERROR);	
	}

	return bmp;

}
int apiCreateWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow){

	
	
	WNDCLASSEX wc;
	MSG Komunikat;

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "KPN";
	wc.hIconSm = NULL;
	
	if( !RegisterClassEx( & wc ) )
	{
		MessageBox( NULL, "Okna nie uda³o siê zarejestrowaæ", "Dalej",
		MB_ICONEXCLAMATION | MB_OK );
		return 1;
	}

	HWND hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "KPN", "Papier, kamieñ, no¿yce", WS_OVERLAPPEDWINDOW,
	50, 50, 620, 540, NULL, NULL, hInstance, NULL );

	if( hwnd == NULL )
	{
		MessageBox( NULL, "B³¹d w czasie tworzenia okna", "Dalej", MB_ICONEXCLAMATION );
		return 1;
	}	

	button=CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Start Game",WS_CHILD | WS_VISIBLE,400,400,70,30,hwnd,(HMENU) button1,hInstance,NULL);
	buttonT=CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Calibration",WS_CHILD | WS_VISIBLE,300,400,70,30,hwnd,(HMENU) button2,hInstance,NULL);
	HFONT hNormalFont =( HFONT ) GetStockObject( DEFAULT_GUI_FONT );
	HFONT myFont = CreateFont(25, 0, 0, 0, FW_MEDIUM, 1, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SCRIPT,"Arial");
	HFONT myFont2 = CreateFont(30, 0, 0, 0, FW_BOLD, 1, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,"Arial");

	bckgHbm[0] =loadImg("bckg2.bmp");
	bckgHbm[1] = loadImg("kamien.bmp");
	bckgHbm[2] = loadImg("papier.bmp");
	bckgHbm[3] = loadImg("nozyce.bmp");
	bckgHbm[4] = loadImg("mysl.bmp");

	hStatic = CreateWindowEx(0, "STATIC", NULL , WS_CHILD | WS_VISIBLE | 
									SS_BITMAP, 70, 220, 150, 150, hwnd, NULL, 
									hInstance, NULL);	

	hStatic2 = CreateWindowEx(0, "STATIC", NULL , WS_CHILD | WS_VISIBLE | 
									SS_BITMAP, 370, 220, 150, 150, hwnd, NULL, 
									hInstance, NULL);

	HWND hStatic3 = CreateWindowEx(0,"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
							250,200,100,30,hwnd,NULL,NULL,NULL);

	static_score = CreateWindowEx(0,"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
							250,250,100,30,hwnd,NULL,NULL,NULL);

	combo = CreateWindowEx( WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
						  70, 70, 150, 200, hwnd, (HMENU) combo1, NULL, NULL);

	static_name = CreateWindowEx( NULL, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
								70, 150, 150, 20, hwnd, NULL, NULL, NULL);
			
	static_name2 = CreateWindowEx( NULL, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
								370, 150, 150, 20, hwnd, NULL, NULL, NULL);

	static_timer = CreateWindowEx( NULL, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_CENTER,
								270, 350, 60, 30, hwnd, NULL, NULL, NULL);

	SendMessage( button, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
	SendMessage( buttonT, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
	SendMessage( combo, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
	SendMessage( static_name, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
	SendMessage( static_name2, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
	SendMessage( static_timer, WM_SETFONT,( WPARAM ) myFont, 0 );
	SendMessage( static_score, WM_SETFONT,( WPARAM ) myFont2, 0 );
	SendMessage( hStatic3, WM_SETFONT,( WPARAM ) myFont2, 0 );
	SendMessage( combo, CB_ADDSTRING, 0,( LPARAM ) "Gra z komputerem" );
	SendMessage( combo, CB_ADDSTRING, 0,( LPARAM ) "Gra z cz³owiekiem" );
	SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
	SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
	SendMessage( combo,CB_SETCURSEL,(WPARAM)0,1); 
	SetWindowText( static_name, "Komputer" );
	SetWindowText( static_name2, "Gracz" );
	SetWindowText( hStatic3, "SCORE" );
	SetWindowText( static_score, "0:0" );
	//SetWindowText( static_timer, "0:0" );
	GetObject( bckgHbm[0], sizeof( bckgInfo ), & bckgInfo );
	
	ShowWindow( hwnd, nCmdShow );
	UpdateWindow( hwnd );

	while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
    
	return Komunikat.wParam;

}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	HDC hdco, hdcbitmapy;
	PAINTSTRUCT ps;

    switch( msg )
    {
	case WM_CREATE:
		loadCallSettings();
	break;
	case WM_COMMAND:
		
		switch(wParam)
		{

		case button1:
			seconds=0;
			SetTimer(hwnd,IDT_TIMER1,1000,(TIMERPROC)NULL);
			SetTimer(hwnd,IDT_TIMER2,3000,(TIMERPROC)NULL);
			Button_Enable(button,false);
			Button_Enable(combo,false);
			startGame();
			if(playOption==1)
				playHu(ext,defects1,defects);
			else
				makeCV(ext,defects);
			return 0;
		break;

		case button2:

			calibration();
			saveCallSettings();
		break;

		}
	
		if (lParam == (LPARAM)combo && HIWORD (wParam) == CBN_SELENDOK)
		{
                 //jesli wybor sie zmienil to sprawdzamy jaka aktualnie
                 //pozycja jest wybrana
				int iIndex = SendMessage(combo, CB_GETCURSEL, 0, 0);
                 //i jesli to pozycja 3 wyswietlamy komunikat
                if (0 == iIndex)
                {
					playOption=0;
					SetWindowText( static_name, "Komputer" );
					SetWindowText( static_name2, "Gracz" );
                }

				else
				{
					playOption=1;
					SetWindowText( static_name, "Gracz 1" );
					SetWindowText( static_name2, "Gracz 2" );

				}
		}
	break;
	
	case WM_PAINT:

		hdco=BeginPaint(hwnd,&ps); //kontekst okna uzyskujemy w momencie odrysowywania
		hdcbitmapy=CreateCompatibleDC(hdco); //teraz musimy utworzyæ kontekst pamiêciowy dla bitmapy
		bckgHbm[0]=(HBITMAP)SelectObject(hdcbitmapy,bckgHbm[0]); 
		BitBlt(hdco,0,0,bckgInfo.bmWidth,bckgInfo.bmHeight,hdcbitmapy,0,0,SRCCOPY);
		bckgHbm[0]=(HBITMAP)SelectObject(hdcbitmapy,bckgHbm[0]); //z powrotem zamieniamy bitmapy w kontekœcie
		DeleteDC(hdcbitmapy); //usuwamy kontekst pamiêciowy bitmapy
		EndPaint(hwnd,&ps);
	break;

	case WM_TIMER: 
 
		if(wParam==IDT_TIMER1) {
			SetWindowText( static_timer, setTime(++seconds ));
			//sprintf(buf,"%d",defects);
			//SetWindowText( static_score, buf);
			
			if(! sets){

				SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
				SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
			}
			sets = false;
			
		}

		if(wParam==IDT_TIMER2) {
			
			if(playOption == 0 || playOption == 1){
				if( defects>=0 && defects <=5 && !( playOption == 1 &&  ( ( defects1<0 ) || ( defects1>5 ) ) )  ){

					if(playOption == 0)
						battle( setPlayerChoice( 1, aiChoice() ), setPlayerChoice(2,defects) );
					else
						battle( setPlayerChoice( 1, setPlayerChoice(2,defects1) ), setPlayerChoice(2,defects) );
					SetWindowText( static_score, getScore());

					switch( getPlayerChoice( 1 ) ) {
					case unknown:
						SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
						break;
					case stone:
						SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[1]);
						break;
					case paper:
						SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[2]);
						break;
					case scissors:
						SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP, ( LPARAM )bckgHbm[3]);
						break;
					}

					switch( getPlayerChoice( 2 ) ) {
					case unknown:
						SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
						break;
					case stone:
						SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[1]);
						break;
					case paper:
						SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[2]);
						break;
					case scissors:
						SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP, ( LPARAM )bckgHbm[3]);
						break;
					
					}
					sets = true;
				}
				else {
					SendMessage(hStatic2, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
					SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP,( LPARAM )bckgHbm[4]);
				}
			}
		//MessageBox(0,"Nie mo¿na otworzyæ pliku bitmap.bmp z folderu z aplikacj¹","Brak pliku",MB_ICONERROR);	
		}
		  break;
		

	case WM_CLOSE:

		ext=true;
        DestroyWindow( hwnd );
    break;
        
    case WM_DESTROY:

        PostQuitMessage( 0 );
    break;
        
    default:

		
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
	return 0;
}

