// ssce  - Simple source code editor

#define _WIN32_WINNT 0x0A00 // DPI awareness
#define WIN32_LEAN_AND_MEAN // Trim fat from API
#include <windows.h>
#include <stdexcept>
#include "Scintilla.h"

HWND hwndMainWindow = nullptr;
HWND hwndEditor = nullptr;
HMODULE hmodScintilla = nullptr;

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

/* 関数はすべてvoid型で、例外をスローする */
namespace ssce {
  void setDpiAwareness() {
    if (!SetProcessDpiAwarenessContext(
      DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    )) {
      throw std::runtime_error("Failed to set DPI awareness");
    }
  }

  void createMainWindow(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"ssce";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassW(&wc)) {
      throw std::runtime_error("Failed to register window class");
    }

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

    if (hwndMainWindow == nullptr) {
      throw std::runtime_error("Failed to create main window");
    }
  }

  void createEditor(HINSTANCE hInstance) {
    hmodScintilla = LoadLibraryW(L"Scintilla.dll");
    if (hmodScintilla == nullptr) {
      throw std::runtime_error("Failed to load Scintilla.dll");
    }

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

    if (hwndEditor == nullptr) {
      throw std::runtime_error("Failed to create Scintilla editor");
    }
  }

  void configureEditor() {
    if (hwndEditor == nullptr) {
      throw std::runtime_error("Editor window is not created");
    }

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
    if (hwndEditor != nullptr) {
      DestroyWindow(hwndEditor);
      hwndEditor = nullptr;
    }

    if (hmodScintilla != nullptr) {
      FreeLibrary(hmodScintilla);
      hmodScintilla = nullptr;
    }
  }
  
}

int WINAPI wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE,
  _In_ PWSTR,
  _In_ int nCmdShow
) {
    try {
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
  } catch (const std::exception& e) {
    MessageBoxA(
      nullptr,
      e.what(),
      "Error",
      MB_ICONERROR | MB_OK
    );
    return -1;
  }
}
