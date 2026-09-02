#include "mainwin.h"
#include <windows.h>

HWND makeMainWin(HINSTANCE hInst) {
  return CreateWindowExW(
    0,
    L"ssce",
    L"ssce",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr,
    nullptr,
    hInst,
    nullptr
  );
}