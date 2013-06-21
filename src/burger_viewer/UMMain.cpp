/**
 * @file UMMain.cpp
 */

#include <Windows.h>
#include "UMWindow.h"

// win main
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR cmd, int show)
{
	return burger::UMWindow::instance().win_main(hInst, hPrev, cmd, show);
}