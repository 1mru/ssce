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

#define IDM_OPEN      101
#define IDM_SAVE      102
#define IDM_QUIT      104
#define IDM_SAVEAS    103
#define IDM_UNDO      105
#define IDM_CUT       106
#define IDM_COPY      107
#define IDM_PASTE     108
#define IDM_SELECTALL 109

HWND hEdit = nullptr;
wchar_t path[MAX_PATH] = {};

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }
  
  if (uMsg == WM_SIZE) {
    MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
    return 0;
  }
  
  if (uMsg == WM_NOTIFY) {
    NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lParam);
    if (nmhdr->hwndFrom != hEdit) return 0;
    
    if (nmhdr->code == SCN_UPDATEUI) {
      int lines = SendMessageW(hEdit, SCI_GETLINECOUNT, 0, 0);
      int digit = SendMessageA(hEdit, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"9");
      int width = 0;
      if(lines < 1000) width = 4 * digit;
      else if (lines < 10000) width = 5 * digit;
      else if (lines < 100000) width = 6 * digit;
      else if (lines < 1000000) width = 7 * digit;
      else width = 8 * digit;
      SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, width);
    } // SCN_UPDATEUI
    
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
      if (SendMessageW(hEdit, SCI_GETMODIFY, 0, 0)) {
        int result = MessageBoxW(
          hwnd,
          L"保存されていない変更があります。\n終了しますか？",
          L"ssce",
          MB_YESNO | MB_ICONWARNING
        );
        if (result != IDYES) return 0;
      }

      DestroyWindow(hwnd);
      return 0;
    }
    
    if (LOWORD(wParam) == IDM_UNDO) {
      SendMessageW(hEdit, SCI_UNDO, 0, 0);
      return 0;
    }
  
    if (LOWORD(wParam) == IDM_CUT) {
      SendMessageW(hEdit, SCI_CUT, 0, 0);
      return 0;
    }
    
    if (LOWORD(wParam) == IDM_COPY) {
      SendMessageW(hEdit, SCI_COPY, 0, 0);
      return 0;
    }
    
    if (LOWORD(wParam) == IDM_PASTE) {
      SendMessageW(hEdit, SCI_PASTE, 0, 0);
      return 0;
    }
    
    if (LOWORD(wParam) == IDM_SELECTALL) {
      SendMessageW(hEdit, SCI_SELECTALL, 0, 0);
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

  HMENU hFileMenu = CreatePopupMenu();
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_OPEN, L"開く")) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVE, L"保存")) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVEAS, L"名前を付けて保存")) return -1;
  if (!AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr)) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_QUIT, L"終了")) return -1;
  
  HMENU hEditMenu = CreatePopupMenu();
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_UNDO, L"元に戻す")) return -1;
  if (!AppendMenuW(hEditMenu, MF_SEPARATOR, 0, nullptr)) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_CUT, L"切り取り")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_COPY, L"コピー")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_PASTE, L"貼り付け")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_SELECTALL, L"すべて選択")) return -1;
  
  HMENU hMenu = CreateMenu();
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"ファイル")) return -1;
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"編集")) return -1;
  
  HWND hMain = CreateWindowExW(
    0,
    CLASS_NAME,
    CLASS_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    nullptr,
    hMenu,
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
  ); if (hEdit == nullptr) return -1;

  SendMessageW(hEdit, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hEdit, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hEdit, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETUSETABS, FALSE, 0);
  // WではなくAを使うのはScintillaの仕様・文字列にＬはつけないことに注意
  SendMessageA(hEdit, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Consolas");

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
