// ssce  - Simple source code editor
// １．とにかくスピード重視
// ２．Keep it Simple, Stupid
// ３．必要十分のコメントをかく

#define _WIN32_WINNT 0x0A00  // DPI awareness
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Scintilla.h"

HWND hwndEditor = nullptr;

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE: {
      if (hwndEditor == nullptr) return 0;
      MoveWindow(hwndEditor, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;
    }
  }
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}  // wndProc

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  const wchar_t CLASS_NAME[] = L"ssce";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  if (!RegisterClassW(&wc)) return -1;

  HWND hwndMain = CreateWindowExW(
    0,
    CLASS_NAME,
    CLASS_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr,
    nullptr,
    hInst,
    nullptr
  ); if (hwndMain == nullptr) return -1;

  HMODULE hmodScintilla = LoadLibraryW(L"Scintilla.dll");
  if (hmodScintilla == nullptr) return -1;

  hwndEditor = CreateWindowExW(
    0,
    L"Scintilla",
    L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
    0, 0,
    0, 0,
    hwndMain,
    nullptr,
    hInst,
    nullptr
  );
  
  if (hwndEditor == nullptr) return -1;

  SendMessageW(hwndEditor, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hwndEditor, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hwndEditor, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hwndEditor, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hwndEditor, SCI_SETUSETABS, FALSE, 0);
  // WではなくAを使うのはScintillaの仕様
  SendMessageA(hwndEditor, SCI_STYLESETFONT, STYLE_DEFAULT,
               (LPARAM) "Consolas"  // Lをつけないことに注意
  );

  ShowWindow(hwndMain, nCmdShow);
  SetFocus(hwndEditor);

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  FreeLibrary(hmodScintilla);
  return 0;
}  // wWinMain
