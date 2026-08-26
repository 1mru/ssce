// ssce  - Simple source code editor

#define _WIN32_WINNT 0x0A00 // DPI awareness
#define WIN32_LEAN_AND_MEAN // Trim fat from API
#include <windows.h>
#include "Scintilla.h"

HWND hwndMainWindow = nullptr;
HMODULE hmodScintilla = LoadLibraryW(L"Scintilla.dll");
HWND hwndEditor = nullptr;

LRESULT CALLBACK WindowProc(
  HWND hwnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  switch (uMsg) {
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE: {
      if (hwndEditor != nullptr) {
        MoveWindow(
          hwndEditor,
          0, 0,
          LOWORD(lParam), HIWORD(lParam),
          TRUE
        );
      }
      return 0;
    }
  }
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

namespace ssce {
  void setDpiAwareness() {
    SetProcessDpiAwarenessContext(
      DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    );
  }

  void createMainWindow(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"ssce";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);
    
    hwndMainWindow = CreateWindowExW(
      0,
      CLASS_NAME,
      CLASS_NAME,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr,
      nullptr,
      hInstance,
      nullptr
    );
  }

  void createEditor(HINSTANCE hInstance) {
    hwndEditor = CreateWindowExW(
      0,
      L"Scintilla",
      L"",
      WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
      0, 0,
      0, 0,
      hwndMainWindow,
      nullptr,
      hInstance,
      nullptr
    );
  }

  void configureEditor() {
    SendMessageW(hwndEditor, SCI_SETMARGINWIDTHN, 0, 50);
    SendMessageW(hwndEditor, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  
    // Note A rather than W
    SendMessageA(
      hwndEditor,
      SCI_STYLESETFONT,
      STYLE_DEFAULT,
      (LPARAM) "Cascadia Code" // Note lack of L before font name
    );
  
    SendMessageW(hwndEditor, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
    SendMessageW(hwndEditor, SCI_SETTABWIDTH, 2, 0);
    SendMessageW(hwndEditor, SCI_SETUSETABS, FALSE, 0);
  }

  void destroyEditor() {
    DestroyWindow(hwndEditor);
    FreeLibrary(hmodScintilla);
  }
  
}

int WINAPI wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE,
  _In_ PWSTR,
  _In_ int nCmdShow
) {
  ssce::setDpiAwareness();
  ssce::createMainWindow(hInstance);
  ssce::createEditor(hInstance);
  ssce::configureEditor();

  ShowWindow(hwndMainWindow, nCmdShow);
  SetFocus(hwndEditor);

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  ssce::destroyEditor();
  return 0;
}
