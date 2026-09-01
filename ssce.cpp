// ssce  - Simple source code editor
// １．とにかくスピード重視
// ２．Keep it Simple, Stupid
// ３．必要十分のコメントをかく

#define _WIN32_WINNT 0x0A00  // DPI awareness
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Scintilla.h"

HWND hEdit = nullptr;

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }
  
  else if (uMsg == WM_SIZE) {
    if (hEdit == nullptr) return 0;
    MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
    return 0;
  }
  
  else if (uMsg == WM_NOTIFY) {
    NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lParam);
    if (nmhdr->hwndFrom != hEdit) return 0;
    
    if (nmhdr->code == SCN_UPDATEUI) { // Try SCN_MODIFIED as well
      int lines = SendMessageW(hEdit, SCI_GETLINECOUNT, 0, 0);
      int buf = SendMessageW(hEdit, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"_");
      int digit = SendMessageW(hEdit, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"9");
      int width = 0;
      
      //if (lines < 10) width = buf + digit;
      //else ifif (lines < 100) width = buf + 2 * digit;
      if(lines < 1000) width = buf + 3 * digit;
      else if (lines < 10000) width = buf + 4 * digit;
      else if (lines < 100000) width = buf + 5 * digit;
      else if (lines < 1000000) width = buf + 6 * digit;
      else width = buf + 7 * digit;
      
      SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, width);
    }
    return 0;
  }
  
  else return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}  // wndProc

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  const wchar_t CLASS_NAME[] = L"ssce";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  if (!RegisterClassW(&wc)) return -1;

  HWND hMain = CreateWindowExW(
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
  ); if (hMain == nullptr) return -1;

  HMODULE hScintilla = LoadLibraryW(L"Scintilla.dll");
  if (hScintilla == nullptr) return -1;

  hEdit = CreateWindowExW(
    0,
    L"Scintilla",
    L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
    0, 0,
    0, 0,
    hMain,
    nullptr,
    hInst,
    nullptr
  );
  
  if (hEdit == nullptr) return -1;

  SendMessageW(hEdit, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hEdit, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hEdit, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETUSETABS, FALSE, 0);
  // WではなくAを使うのはScintillaの仕様
  SendMessageA(hEdit, SCI_STYLESETFONT, STYLE_DEFAULT,
               (LPARAM) "Consolas"  // Lをつけないことに注意
  );

  ShowWindow(hMain, nCmdShow);
  SetFocus(hEdit);

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  FreeLibrary(hScintilla);
  return 0;
}  // wWinMain
