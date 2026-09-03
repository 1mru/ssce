/* ssce - simple source code editor
 * See LICENSE.txt for copyright and license details.
 * 
 * ssceは、Windows向けに開発された高速で軽量なテキストエディタです。
 * Win32 APIを直接利用し、Scintillaを編集コンポーネントとして使用
 * することによって、起動時間の短縮および操作性の向上を目指します。
 */

#define _WIN32_WINNT 0x0A00 // 解像度をユーザの画面に合わせる
#define WIN32_LEAN_AND_MEAN // Win32 APIの不要な部分を取り除く

#include <fstream> // ファイル操作（標準ライブラリ）
#include <windows.h> // Win32 API
#include <commdlg.h> // ダイアログボックス（「ファイル」→「開く」のダイアログなど） 
#include "Scintilla.h"
#include "SciLexer.h"
#include "ILexer.h"
#include "Lexilla.h"

/* ファイルメニューのマクロ */
#define IDM_OPEN      1 // 開く
#define IDM_SAVE      2 // 保存
#define IDM_SAVEAS    3 // 名前をつけて保存
#define IDM_QUIT      4 // 終了

/* 編集メニューのマク*/
#define IDM_UNDO      5 // 元に戻す
#define IDM_CUT       6 // 切り取り
#define IDM_COPY      7 // コピー
#define IDM_PASTE     8 // 貼り付け
#define IDM_SELECTALL 9 // すべて選択

/* アプリのアイコン */
#define IDI_ICON1 101

/* （注）グローバル変数の宣言はできる限り避ける */
/* グローバル変数 */
HWND hEdit = nullptr; // wndProcの多くの処理で必要
wchar_t path[MAX_PATH] = {}; // ファイルを保存する時の処理で必要

/* メインウィンドウのコールバック関数。wWinMainで直接呼び出すことはないが、
 * Windowsが必要な時に自動的に呼び出す。wWinMainでウィンドウクラスを
 * 登録する際に必要なので、宣言はwWinMainの前に書く。 */
LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  
  case WM_SIZE:
    MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
    return 0;
  
  case WM_NOTIFY: {
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
  
  case WM_COMMAND: {
    switch (LOWORD(wParam)) {
      
    case IDM_OPEN: {     
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
    
    case IDM_SAVE: {
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
    
    case IDM_SAVEAS: {    
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
    
    case IDM_QUIT: {
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
    
    case IDM_UNDO:
      SendMessageW(hEdit, SCI_UNDO, 0, 0);
      return 0;
  
    case IDM_CUT:
      SendMessageW(hEdit, SCI_CUT, 0, 0);
      return 0;
    
    case IDM_COPY:
      SendMessageW(hEdit, SCI_COPY, 0, 0);
      return 0;
    
    case IDM_PASTE:
      SendMessageW(hEdit, SCI_PASTE, 0, 0);
      return 0;
    
    case IDM_SELECTALL:
      SendMessageW(hEdit, SCI_SELECTALL, 0, 0);
      return 0;
    
    } // switch (LOWORD(wParam))
  
  } // WM_COMMAND
  
  } // switch (uMsg)
  
  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}  // wndProc

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  const wchar_t CLASS_NAME[] = L"ssce";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(IDI_ICON1));
  wc.lpszClassName = CLASS_NAME;
  if (!RegisterClassW(&wc)) return -1;

  HMENU hFileMenu = CreatePopupMenu();
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_OPEN, L"開く(&O)\tCtrl+O")) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVE, L"保存(&S)\tCtrl+S")) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVEAS, L"名前を付けて保存(&A)\tCtrl+Shift+S")) return -1;
  if (!AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr)) return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_QUIT, L"終了(&X)\tCtrl+Q")) return -1;
  
  HMENU hEditMenu = CreatePopupMenu();
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_UNDO, L"元に戻す(&U)\tCtrl+Z")) return -1;
  if (!AppendMenuW(hEditMenu, MF_SEPARATOR, 0, nullptr)) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_CUT, L"切り取り(&T)\tCtrl+X")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_COPY, L"コピー(&C)\tCtrl+C")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_PASTE, L"貼り付け(&P)\tCtrl+V")) return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_SELECTALL, L"すべて選択(&S)\tCtrl+A")) return -1;
  
  HMENU hMenu = CreateMenu();
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"ファイル(&F)")) return -1;
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"編集(&E)")) return -1;
  
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

  HMODULE hLexilla = LoadLibraryW(L"Lexilla.dll");
  if (hLexilla == nullptr) return -1;
  
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
    /* ファイルメニュー */
    { FVIRTKEY | FCONTROL, 'O', IDM_OPEN },
    { FVIRTKEY | FCONTROL, 'S', IDM_SAVE },
    { FVIRTKEY | FCONTROL | FSHIFT, 'S', IDM_SAVEAS },
    { FVIRTKEY | FCONTROL, 'Q', IDM_QUIT },

    /* 編集メニュー */
    { FVIRTKEY | FCONTROL, 'Z', IDM_UNDO },
    { FVIRTKEY | FCONTROL, 'X', IDM_CUT },
    { FVIRTKEY | FCONTROL, 'C', IDM_COPY },
    { FVIRTKEY | FCONTROL, 'V', IDM_PASTE },
    { FVIRTKEY | FCONTROL, 'A', IDM_SELECTALL },
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
