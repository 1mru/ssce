#ifndef EDIT_H
#define EDIT_H

#include <windows.h>
#include "Scintilla.h"

HWND makeEdit(HWND hMain, HINSTANCE hInst);
void optEdit(HWND hEdit);
void resizeEdit(HWND hEdit, int width, int height);
void resizeMarginEdit(HWND hEdit);

#endif