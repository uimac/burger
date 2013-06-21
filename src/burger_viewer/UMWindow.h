/**
 * @file UMWindow.h
 */
#pragma once

#include <memory>
#include <tchar.h>
#include <windows.h>
#include "UMMacro.h"

namespace burger
{

/**
 * main window
 */
class UMWindow
{
	DISALLOW_COPY_AND_ASSIGN(UMWindow);

public:
	static UMWindow& instance() {
		static UMWindow window;
		return window;
	}
	
	/**
	 * just a wrapper of win main
	 */
	int win_main(HINSTANCE hInst, HANDLE hPrev, TCHAR* cmd, int show);

	/**
	 * just a wrapper of win proc
	 */
	int win_proc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);

private:
	UMWindow() : width_(1024), height_(768) {}

	/// initial client width
	int width_;
	/// initial client height
	int height_;

};

} // burger
