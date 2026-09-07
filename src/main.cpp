/* ssce - simple source code editor
 * See LICENSE.txt for copyright and license details.
 *
 * ssceは、Windows向けに開発された高速で軽量なテキストエディタです。
 * Win32 APIを直接利用し、Scintillaを編集コンポーネントとして使用
 * することによって、起動時間の短縮および操作性の向上を目指します。
 */

#define _WIN32_WINNT 0x0A00  // Windows 10向けのAPIを使用
#define WIN32_LEAN_AND_MEAN  // Win32 APIの不要な部分を取り除く

// #include <commdlg.h>  //
// ダイアログボックス（「ファイル」→「開く」のダイアログなど）
#include <windows.h>  // Win32 API

#include <filesystem>
#include <fstream>  // ファイル操作（標準ライブラリ）

#include "ILexer.h"
#include "Lexilla.h"
#include "SciLexer.h"
#include "Scintilla.h"
#include "accel.h"
#include "editor.h"
#include "menu.h"
#include "wndproc.h"

/* （注）グローバル変数の宣言はできる限り避ける */
/* グローバル変数 */
HWND hEdit = nullptr;           // wndProcの多くの処理で必要
extern wchar_t path[MAX_PATH];  // ファイルを保存する時の処理で必要

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR,
                    _In_ int nCmdShow) {
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

  Scintilla_RegisterClasses(hInst);

  const wchar_t CLASS_NAME[] = L"ssce";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  if (!RegisterClassW(&wc)) return -1;

  HMENU hMenu = createMenu();

  HWND hMain = CreateWindowExW(0, CLASS_NAME, CLASS_NAME, WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, nullptr, hMenu, hInst, nullptr);
  if (hMain == nullptr) return -1;

  hEdit = CreateWindowExW(0, L"Scintilla", L"",
                          WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
                          0, 0, 0, 0, hMain, nullptr, hInst, nullptr);
  if (hEdit == nullptr) return -1;

  configureEditor(hEdit);

  ShowWindow(hMain, nCmdShow);
  SetFocus(hEdit);

  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    if (!TranslateAcceleratorW(hMain, hAccel, &msg)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  return 0;
}  // wWinMain
