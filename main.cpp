#include <stdio.h>
#include "api_window.h"
#include <iostream>
#include <cstdio>
#include "resource.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow)
{
	apiCreateWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}