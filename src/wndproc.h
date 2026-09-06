#ifndef WNDPROC_H
#define WNDPROC_H

#include <windows.h>  // Win32 API

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif  // WNDPROC_H
