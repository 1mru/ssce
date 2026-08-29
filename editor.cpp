#include "editor.h"
#include <stdexcept>
#include <windows.h>
#include "Scintilla.h"

HWND Editor::get() {
  return hwndEditor;
}

void Editor::configure() {
  if (hwndEditor == nullptr) {
    throw std::runtime_error("Editor window is not created");
  }

  SendMessageW(hwndEditor, SCI_SETMARGINWIDTHN, 0, 50);
  SendMessageW(hwndEditor, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
  SendMessageA( // Note A rather than W
    hwndEditor,
    SCI_STYLESETFONT,
    STYLE_DEFAULT,
    (LPARAM) "Cascadia Code" // Note lack of L before font name
  );
  SendMessageW(hwndEditor, SCI_STYLESETSIZE, STYLE_DEFAULT, 12);
  SendMessageW(hwndEditor, SCI_SETTABWIDTH, 2, 0);
  SendMessageW(hwndEditor, SCI_SETUSETABS, FALSE, 0);
}

Editor::Editor(HINSTANCE hInstance, HWND hwndParent) {
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
    hwndParent,
    nullptr,
    hInstance,
    nullptr
  );

  if (hwndEditor == nullptr) {
    throw std::runtime_error("Failed to create Scintilla editor");
  }

  configure();
}

Editor::~Editor() {
  if (hwndEditor != nullptr) {
    DestroyWindow(hwndEditor);
    hwndEditor = nullptr;
  }

  if (hmodScintilla != nullptr) {
    FreeLibrary(hmodScintilla);
    hmodScintilla = nullptr;
  }
}
