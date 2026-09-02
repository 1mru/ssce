#include "edit.h"
#include <windows.h>
#include "Scintilla.h"

HWND makeEdit(HWND hMain, HINSTANCE hInst) {
  return CreateWindowExW(
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
  );
}

void optEdit(HWND hEdit) {
  SendMessageW(hEdit, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hEdit, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hEdit, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETUSETABS, FALSE, 0);
  SendMessageA(hEdit, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Consolas");
}

void resizeEdit(HWND hEdit, int width, int height) {
  MoveWindow(hEdit, 0, 0, width, height, TRUE);
}

void resizeMarginEdit(HWND hEdit) {
    int lines = SendMessageW(hEdit, SCI_GETLINECOUNT, 0, 0);
    int digit = SendMessageA(hEdit, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"9");
    int width = 0;
    if(lines < 1000) width = 4 * digit;
    else if (lines < 10000) width = 5 * digit;
    else if (lines < 100000) width = 6 * digit;
    else if (lines < 1000000) width = 7 * digit;
    else width = 8 * digit;
    SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, width);
}
