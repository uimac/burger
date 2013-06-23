/**
 * @file UMMain.cpp
 */

#include <Windows.h>
#include <crtdbg.h>

#include "UMWindow.h"

// win main
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR cmd, int show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return burger::UMWindow::instance().win_main(hInst, hPrev, cmd, show);
}