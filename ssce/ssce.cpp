// ssce  - Simple source code editor
// Win32 API + Scintilla

#define _WIN32_WINNT 0x0A00

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Scintilla.h"
HWND hwndScintilla = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE: {
      if (hwndScintilla != nullptr) {
        MoveWindow(hwndScintilla, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
      }
      return 0;
    }
  }
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// Entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  const wchar_t CLASS_NAME[] = L"ssce";

  // Register window class.
  WNDCLASSW wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClassW(&wc);

  // Create main window.
  HWND hwnd = CreateWindowExW(0, CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr,
                              nullptr, hInstance, nullptr);

  if (hwnd == nullptr) {
    return 0;
  }

  // Load Scintilla DLL.
  HMODULE hmod = LoadLibraryW(L"Scintilla.dll");

  if (hmod == nullptr) {
    MessageBoxW(hwnd, L"The Scintilla DLL could not be loaded", L"Error",
                MB_OK | MB_ICONERROR);

    return 0;
  }

  // Create Scintilla editor.
  hwndScintilla =
      CreateWindowExW(0, L"Scintilla", L"",
                      WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 0,
                      0, 0, 0, hwnd, nullptr, hInstance, nullptr);

  if (hwndScintilla == nullptr) {
    MessageBoxW(hwnd, L"Could not create Scintilla window", L"Error",
                MB_OK | MB_ICONERROR);

    FreeLibrary(hmod);
    return 0;
  }

  // Use DirectWrite for text rendering.
  SendMessageW(hwndScintilla, SCI_SETTECHNOLOGY,
               SC_TECHNOLOGY_DIRECTWRITERETAIN, 0);

  // Show line numbers.
  SendMessageW(hwndScintilla, SCI_SETMARGINWIDTHN, 0, 40);

  SendMessageW(hwndScintilla, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);

  // Set the editor font.
  SendMessageW(hwndScintilla, SCI_STYLESETFONT, STYLE_DEFAULT,
               (LPARAM) "Cascadia Code");

  SendMessageW(hwndScintilla, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);

  SendMessageW(hwndScintilla, SCI_STYLECLEARALL, 0, 0);

  // Use spaces instead of tabs.
  SendMessageW(hwndScintilla, SCI_SETUSETABS, FALSE, 0);

  // Set the tab width to 2 spaces.
  SendMessageW(hwndScintilla, SCI_SETTABWIDTH, 2, 0);

  // Show the main window.
  ShowWindow(hwnd, nCmdShow);

  // Give keyboard focus to the editor.
  SetFocus(hwndScintilla);

  // Message loop.
  MSG msg = {};

  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  FreeLibrary(hmod);

  return 0;
}