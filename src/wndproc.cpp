#include "wndproc.h"

// #include <commdlg.h>  //
// ダイアログボックス（「ファイル」→「開く」のダイアログなど）
#include <windows.h>  // Win32 API

#include <filesystem>
#include <fstream>  // ファイル操作（標準ライブラリ）

#include "ILexer.h"
#include "Lexilla.h"
#include "SciLexer.h"
#include "Scintilla.h"
#include "menu.h"

extern HWND hEdit;

wchar_t path[MAX_PATH] = {};

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
      NMHDR *nmhdr = reinterpret_cast<NMHDR *>(lParam);
      if (nmhdr->hwndFrom != hEdit) return 0;

      if (nmhdr->code == SCN_UPDATEUI) {
        int lines = SendMessageW(hEdit, SCI_GETLINECOUNT, 0, 0);
        int digit =
            SendMessageA(hEdit, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM) "9");
        int width = 0;
        if (lines < 1000)
          width = 4 * digit;
        else if (lines < 10000)
          width = 5 * digit;
        else if (lines < 100000)
          width = 6 * digit;
        else if (lines < 1000000)
          width = 7 * digit;
        else
          width = 8 * digit;
        SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, width);
      }  // SCN_UPDATEUI

      if (nmhdr->code == SCN_MODIFIED) {
        wchar_t title[MAX_PATH + 16];
        wsprintfW(title, L"%s* - ssce", path);
        SetWindowTextW(hwnd, title);
      }
      return 0;
    }  // WM_NOTIFY

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

          /* ファイル拡張子を取得 */
          std::filesystem::path filePath(path);
          std::wstring ext = filePath.extension().wstring();

          /* C++ファイルならシンタクスハイライトをつける */
          if (ext == L".cpp" || ext == L".h" || ext == L".cc" ||
              ext == L".hpp") {
            ILexer5 *lexer = CreateLexer("cpp");
            SendMessageA(hEdit, SCI_SETILEXER, 0, (LPARAM)lexer);

            SendMessageA(
            hEdit, SCI_SETKEYWORDS, 0,
            (LPARAM) "alignas alignof auto bool break case catch char class "
                     "const constexpr continue default delete do double else "
                     "enum explicit export extern false float for friend goto "
                     "if inline int long mutable namespace new noexcept "
                     "nullptr "
                     "operator private protected public register "
                     "reinterpret_cast "
                     "return short signed sizeof static static_assert "
                     "static_cast "
                     "struct switch template this throw true try typedef "
                     "typename union unsigned using virtual void volatile "
                     "wchar_t "
                     "while");
          }

          std::string text((std::istreambuf_iterator<char>(file)), {});
          SendMessageA(hEdit, SCI_SETTEXT, 0, (LPARAM)text.c_str());
          SendMessageA(hEdit, SCI_SETSAVEPOINT, 0, 0);
          wchar_t title[MAX_PATH + 16];
          wsprintfW(title, L"%s - ssce", path);
          SetWindowTextW(hwnd, title);
          return 0;
        }  // IDM_OPEN

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
        }  // IDM_SAVE

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
        }  // IDM_SAVEAS

        case IDM_QUIT: {
          if (SendMessageW(hEdit, SCI_GETMODIFY, 0, 0)) {
            int result = MessageBoxW(
                hwnd, L"保存されていない変更があります。\n終了しますか？",
                L"ssce", MB_YESNO | MB_ICONWARNING);
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

      }  // switch (LOWORD(wParam))

    }  // WM_COMMAND

  }  // switch (uMsg)

  return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}  // wndProc
