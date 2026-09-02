// ssce  - Simple source code editor
// １．とにかくスピード重視
// ２．Keep it Simple, Stupid
// ３．必要十分のコメントを書く

#define _WIN32_WINNT 0x0A00  // DPI awareness
#define WIN32_LEAN_AND_MEAN

#include <fstream>
#include <windows.h>
#include <commdlg.h>
#include "Scintilla.h"
#include "edit.h"
#include "mainwin.h"

#define IDM_OPEN    1
#define IDM_SAVE    2
#define IDM_QUIT    4
#define IDM_SAVEAS  3

HWND hEdit = nullptr;

wchar_t path[MAX_PATH] = {};

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }
  
  if (uMsg == WM_SIZE) {
    resizeEdit(hEdit, LOWORD(lParam), HIWORD(lParam));
    return 0;
  }
  
  if (uMsg == WM_NOTIFY) {
    NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lParam);
    if (nmhdr->hwndFrom != hEdit) return 0;
    
    if (nmhdr->code == SCN_UPDATEUI) {
      resizeMarginEdit(hEdit);
      return 0;
    }
    
    if (nmhdr->code == SCN_MODIFIED) {      
      wchar_t title[MAX_PATH + 16];
      wsprintfW(title, L"%s* - ssce", path);
      SetWindowTextW(hwnd, title);
    }
    return 0;
  } // WM_NOTIFY
  
  if (uMsg == WM_COMMAND) {
    if (LOWORD(wParam) == IDM_OPEN) {      
      OPENFILENAMEW ofn = {};
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = hwnd;
      ofn.lpstrFile = path;
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
      ofn.Flags = OFN_FILEMUSTEXIST;
      if (!GetOpenFileNameW(&ofn)) return 0;
      
      std::ifstream file(path, std::ios::binary);
      if (!file) return 0;
      
      std::string text((std::istreambuf_iterator<char>(file)), {});
      SendMessageA(hEdit, SCI_SETTEXT, 0, (LPARAM)text.c_str());
      SendMessageA(hEdit, SCI_SETSAVEPOINT, 0, 0);
      wchar_t title[MAX_PATH + 16];
      wsprintfW(title, L"%s - ssce", path);
      SetWindowTextW(hwnd, title);
      return 0;
    } // IDM_OPEN
    
    if (LOWORD(wParam) == IDM_SAVE) {
      if (!path[0]) {
        SendMessageW(hwnd, WM_COMMAND, IDM_SAVEAS, 0);
        return 0;
      }
      
      int length = SendMessageA(hEdit, SCI_GETTEXTLENGTH, 0, 0);
      std::string text(length + 1, '\0');
      SendMessageA(hEdit, SCI_GETTEXT, length + 1, (LPARAM)text.data());
      
      std::ofstream file(path, std::ios::binary);
      if (!file) return 0;
      
      file.write(text.data(), length);
      
      SendMessageA(hEdit, SCI_SETSAVEPOINT, 0, 0);
      wchar_t title[MAX_PATH + 16];
      wsprintfW(title, L"%s - ssce", path);
      SetWindowTextW(hwnd, title);
      return 0;
    } // IDM_SAVE
    
    if (LOWORD(wParam) == IDM_SAVEAS) {    
      OPENFILENAMEW ofn = {};
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = hwnd;
      ofn.lpstrFile = path;
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
      ofn.Flags = OFN_OVERWRITEPROMPT;
    
      if (!GetSaveFileNameW(&ofn)) return 0;
    
      int length = SendMessageA(hEdit, SCI_GETTEXTLENGTH, 0, 0);
      std::string text(length + 1, '\0');
      SendMessageA(hEdit, SCI_GETTEXT, length + 1, (LPARAM)text.data());
    
      std::ofstream file(path, std::ios::binary);
      if (!file) return 0;
    
      file.write(text.data(), length);
    
      SendMessageA(hEdit, SCI_SETSAVEPOINT, 0, 0);
      wchar_t title[MAX_PATH + 16];
      wsprintfW(title, L"%s - ssce", path);
      SetWindowTextW(hwnd, title);
      return 0;
    } // IDM_SAVEAS
    
    if (LOWORD(wParam) == IDM_QUIT) {
      DestroyWindow(hwnd);
      return 0;
    }
  }
  
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}  // wndProc

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = L"ssce";
  if (!RegisterClassW(&wc)) return -1;

  HWND hMain = makeMainWin(hInst);
  if (hMain == nullptr) return -1;

  HMODULE hScintilla = LoadLibraryW(L"Scintilla.dll");
  if (hScintilla == nullptr) return -1;

  hEdit = makeEdit(hMain, hInst);
  if (hEdit == nullptr) return -1;

  optEdit(hEdit);
  
  ShowWindow(hMain, nCmdShow);
  SetFocus(hEdit);
  
  ACCEL accel[] = {
    { FVIRTKEY | FCONTROL, 'O', IDM_OPEN },
    { FVIRTKEY | FCONTROL, 'S', IDM_SAVE },
    { FVIRTKEY | FCONTROL | FSHIFT, 'S', IDM_SAVEAS },
    { FVIRTKEY | FCONTROL, 'Q', IDM_QUIT }
  };
  HACCEL hAccel = CreateAcceleratorTableW(accel, ARRAYSIZE(accel));

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    if (!TranslateAcceleratorW(hMain, hAccel, &msg)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  FreeLibrary(hScintilla);
  return 0;
}  // wWinMain
