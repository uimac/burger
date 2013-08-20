/**
 * @file UMWindow.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <tchar.h>
#include <windows.h>
#include "UMMacro.h"
#include "UMDirectX11Viewer.h"

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
	UMWindow() : width_(1280), height_(720) {
		viewer = UMDirectX11Viewer::create();
	}

	/// initial client width
	int width_;
	/// initial client height
	int height_;
	
	UMDirectX11ViewerPtr viewer;

};

} // burger
