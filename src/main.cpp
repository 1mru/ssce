/* ssce - simple source code editor
 * See LICENSE.txt for copyright and license details.
 *
 * ssceは、Windows向けに開発された高速で軽量なテキストエディタです。
 * Win32 APIを直接利用し、Scintillaを編集コンポーネントとして使用
 * することによって、起動時間の短縮および操作性の向上を目指します。
 */

#define _WIN32_WINNT 0x0A00 // Windows 10向けのAPIを使用
#define WIN32_LEAN_AND_MEAN // Win32 APIの不要な部分を取り除く

#include "ILexer.h"
#include "Lexilla.h"
#include "SciLexer.h"
#include "Scintilla.h"

#include <windows.h> // Win32 API
#include <commdlg.h> // ダイアログボックス（「ファイル」→「開く」のダイアログなど）
#include <filesystem>
#include <fstream>   // ファイル操作（標準ライブラリ）

#include "wndproc.h"
#include "menu.h"

/* （注）グローバル変数の宣言はできる限り避ける */
/* グローバル変数 */
HWND hEdit = nullptr;        // wndProcの多くの処理で必要

extern wchar_t path[MAX_PATH]; // ファイルを保存する時の処理で必要

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR,
                    _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  Scintilla_RegisterClasses(hInst);

  const wchar_t CLASS_NAME[] = L"ssce";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  if (!RegisterClassW(&wc))
    return -1;

  HMENU hFileMenu = CreatePopupMenu();
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_OPEN, L"開く(&O)\tCtrl+O"))
    return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVE, L"保存(&S)\tCtrl+S"))
    return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_SAVEAS,
                   L"名前を付けて保存(&A)\tCtrl+Shift+S"))
    return -1;
  if (!AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr))
    return -1;
  if (!AppendMenuW(hFileMenu, MF_STRING, IDM_QUIT, L"終了(&X)\tCtrl+Q"))
    return -1;

  HMENU hEditMenu = CreatePopupMenu();
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_UNDO, L"元に戻す(&U)\tCtrl+Z"))
    return -1;
  if (!AppendMenuW(hEditMenu, MF_SEPARATOR, 0, nullptr))
    return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_CUT, L"切り取り(&T)\tCtrl+X"))
    return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_COPY, L"コピー(&C)\tCtrl+C"))
    return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_PASTE, L"貼り付け(&P)\tCtrl+V"))
    return -1;
  if (!AppendMenuW(hEditMenu, MF_STRING, IDM_SELECTALL,
                   L"すべて選択(&S)\tCtrl+A"))
    return -1;

  HMENU hMenu = CreateMenu();
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"ファイル(&F)"))
    return -1;
  if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"編集(&E)"))
    return -1;

  HWND hMain = CreateWindowExW(0, CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, nullptr, hMenu, hInst, nullptr);
  if (hMain == nullptr)
    return -1;

  hEdit = CreateWindowExW(0, L"Scintilla", L"",
                          WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
                          0, 0, 0, 0, hMain, nullptr, hInst, nullptr);
  if (hEdit == nullptr)
    return -1;

  SendMessageW(hEdit, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hEdit, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hEdit, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETUSETABS, FALSE, 0);
  // WではなくAを使うのはScintillaの仕様・文字列にＬはつけないことに注意
  SendMessageA(hEdit, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM) "Consolas");
  SendMessageW(hEdit, SCI_STYLESETFORE, STYLE_LINENUMBER, RGB(100, 100, 100));
  SendMessageW(hEdit, SCI_STYLESETBACK, STYLE_LINENUMBER, RGB(248, 248, 246));

  SendMessageW(hEdit, SCI_STYLESETBACK, STYLE_DEFAULT, RGB(255, 255, 255));
  SendMessageW(hEdit, SCI_STYLESETFORE, STYLE_DEFAULT, RGB(45, 45, 45));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_COMMENT, RGB(95, 135, 95));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_COMMENTLINE, RGB(95, 135, 95));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_WORD, RGB(120, 50, 120));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_WORD2, RGB(120, 50, 120));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_STRING, RGB(150, 90, 40));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_CHARACTER, RGB(150, 90, 40));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_NUMBER, RGB(50, 120, 140));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_PREPROCESSOR, RGB(120, 80, 120));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_OPERATOR, RGB(45, 45, 45));
  SendMessageW(hEdit, SCI_STYLESETFORE, SCE_C_GLOBALCLASS, RGB(50, 110, 120));

  ShowWindow(hMain, nCmdShow);
  SetFocus(hEdit);

  ACCEL accel[] = {
      /* ファイルメニュー */
      {FVIRTKEY | FCONTROL, 'O', IDM_OPEN},
      {FVIRTKEY | FCONTROL, 'S', IDM_SAVE},
      {FVIRTKEY | FCONTROL | FSHIFT, 'S', IDM_SAVEAS},
      {FVIRTKEY | FCONTROL, 'Q', IDM_QUIT},

      /* 編集メニュー */
      {FVIRTKEY | FCONTROL, 'Z', IDM_UNDO},
      {FVIRTKEY | FCONTROL, 'X', IDM_CUT},
      {FVIRTKEY | FCONTROL, 'C', IDM_COPY},
      {FVIRTKEY | FCONTROL, 'V', IDM_PASTE},
      {FVIRTKEY | FCONTROL, 'A', IDM_SELECTALL},
  };
  HACCEL hAccel = CreateAcceleratorTableW(accel, ARRAYSIZE(accel));

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    if (!TranslateAcceleratorW(hMain, hAccel, &msg)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  return 0;
} // wWinMain
