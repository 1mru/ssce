// ssce  - Simple source code editor

#define _WIN32_WINNT 0x0A00 // DPI awareness
#define WIN32_LEAN_AND_MEAN // Trim fat from API

#include <stdexcept>
#include <windows.h>
#include "Scintilla.h"
#include "editor.h"

HWND hwndMainWindow = nullptr;
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
      if (mainEditor.get() != nullptr) {
        MoveWindow(
          mainEditor.get(),
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
      0,                              // スタイル
      CLASS_NAME,                     // ウィンドウクラス名
      CLASS_NAME,                     // ウィンドウタイトル
      WS_OVERLAPPEDWINDOW,            // ウィンドウスタイル
      CW_USEDEFAULT, CW_USEDEFAULT,   // 位置
      CW_USEDEFAULT, CW_USEDEFAULT,   // サイズ
      nullptr,                        // 親ウィンドウ
      nullptr,                        // メニューハンドル
      hInstance,                      // インスタンスハンドル
      nullptr                         // 追加パラメータ
    );

    if (hwndMainWindow == nullptr) {
      throw std::runtime_error("Failed to create main window");
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
      
      Editor mainEditor(hInstance, hwndMainWindow);

      ShowWindow(hwndMainWindow, nCmdShow);
      SetFocus(mainEditor.get());

      MSG msg = {};
      while (GetMessageW(&msg, nullptr, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }

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
