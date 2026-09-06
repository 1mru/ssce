#ifndef WNDPROC_H
#define WNDPROC_H

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

#include "menu.h"

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // WNDPROC_H
