#include "editor.h"

#include <windows.h>

#include "ILexer.h"
#include "Lexilla.h"
#include "SciLexer.h"
#include "Scintilla.h"

void configureEditor(HWND hEdit) {
  SendMessageW(hEdit, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageW(hEdit, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hEdit, SCI_STYLESETSIZE, STYLE_DEFAULT, 13);
  SendMessageW(hEdit, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETUSETABS, FALSE, 0);
  SendMessageA(hEdit, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM) "Cascadia Code");
  SendMessageA(hEdit, SCI_STYLECLEARALL, 0, 0);
  SendMessageW(hEdit, SCI_SETCARETLINEVISIBLE, TRUE, 0);
  SendMessageW(hEdit, SCI_SETCARETLINEBACK, RGB(248, 248, 242), 0);
  SendMessageW(hEdit, SCI_SETCARETWIDTH, 2, 0);
  SendMessageW(hEdit, SCI_SETCARETFORE, RGB(40, 40, 40), 0);
  SendMessageW(hEdit, SCI_SETEDGEMODE, EDGE_LINE, 0);
  SendMessageW(hEdit, SCI_SETEDGECOLUMN, 100, 0);
  SendMessageW(hEdit, SCI_SETEDGECOLOUR, RGB(225, 225, 220), 0);
  SendMessageW(hEdit, SCI_SETUNDOCOLLECTION, TRUE, 0);
  SendMessageW(hEdit, SCI_SETLAYOUTCACHE, SC_CACHE_PAGE, 0);
  SendMessageW(hEdit, SCI_SETADDITIONALSELECTIONTYPING, TRUE, 0);
  
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
}
