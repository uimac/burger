/**
 * @file UMWindow.cpp
 */
#include "UMWindow.h"
#include "UMDirectX11Viewer.h"

namespace burger
{

// event callbacks
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return UMWindow::instance().win_proc(hWnd, msg, wParam, lParam);
}

/**
 * just a wrapper of win main
 */
int UMWindow::win_main(HINSTANCE hInst, HANDLE hPrev, TCHAR* cmd, int show)
{
	// register WNDCLASSEX
	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = NULL;
		wcex.lpfnWndProc = WinProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = _T("burger_viewer");
		wcex.hIconSm = NULL;
		if (!RegisterClassEx(&wcex)) { return false; }
	}
	
	// create main window
	HWND hwnd;
	{
		int w = width_
			+ ::GetSystemMetrics(SM_CXEDGE)
			+ ::GetSystemMetrics(SM_CXBORDER)
			+ ::GetSystemMetrics(SM_CXDLGFRAME);

		int h = height_
			+ ::GetSystemMetrics(SM_CYEDGE)
			+ ::GetSystemMetrics(SM_CYBORDER)
			+ ::GetSystemMetrics(SM_CYDLGFRAME)
			+ ::GetSystemMetrics(SM_CYCAPTION);

		int x = (::GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		hwnd = ::CreateWindowEx(
			WS_EX_ACCEPTFILES,
			wcex.lpszClassName,
			wcex.lpszClassName,
			WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			x, y, w, h,
			NULL,
			NULL,
			hInst,
			NULL
		);
		// disable WM_PAINT
		ValidateRect(NULL, NULL);
		if (!hwnd) { return false; }
	}
	
	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);
	
	// create my viewer
	UMDirectX11Viewer viewer;
	viewer.init(hwnd, width_, height_);

	static MSG msg;

	for (;;) 
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
		else
		{
			// refresh window
			if (!viewer.refresh())
			{
				// the end
				::PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
		::Sleep(1);
	}
	::Sleep(1);

	return true;
}

/**
 * just a wrapper of win proc
 */
int UMWindow::win_proc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				break;
			}
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
	}
	if (DefWindowProc(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	return false;
}

} // burger
