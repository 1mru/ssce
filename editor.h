#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

class Editor {
public:
  HWND get();
  void configure();
  Editor(HINSTANCE hInstance, HWND hwndParent);
  ~Editor();
private:
  HWND hwndEditor;
  HMODULE hmodScintilla;
};

#endif // EDITOR_H
