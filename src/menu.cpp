#include "menu.h"

#include <windows.h>

HMENU createContextMenu() {
  HMENU hCtx = CreatePopupMenu();
  AppendMenuW(hCtx, MF_STRING, IDM_UNDO, L"元に戻す(&U)");
  AppendMenuW(hCtx, MF_STRING, IDM_REDO, L"やり直し(&R)");
  AppendMenuW(hCtx, MF_SEPARATOR, 0, nullptr);
  AppendMenuW(hCtx, MF_STRING, IDM_CUT, L"切り取り(&T)");
  AppendMenuW(hCtx, MF_STRING, IDM_COPY, L"コピー(&C)");
  AppendMenuW(hCtx, MF_STRING, IDM_PASTE, L"貼り付け(&P)");
  AppendMenuW(hCtx, MF_STRING, IDM_SELECTALL, L"すべて選択(&S)");
  return hCtx;
}

HMENU createMenu() {
  HMENU hFileMenu = CreatePopupMenu();
  AppendMenuW(hFileMenu, MF_STRING, IDM_NEW, L"新規作成(&N)\tCtrl+N");
  AppendMenuW(hFileMenu, MF_STRING, IDM_OPEN, L"開く(&O)\tCtrl+O");
  AppendMenuW(hFileMenu, MF_STRING, IDM_SAVE, L"保存(&S)\tCtrl+S");
  AppendMenuW(hFileMenu, MF_STRING, IDM_SAVEAS,
              L"名前を付けて保存(&A)\tCtrl+Shift+S");
  AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr);
  AppendMenuW(hFileMenu, MF_STRING, IDM_QUIT, L"終了(&X)\tCtrl+Q");

  HMENU hEditMenu = CreatePopupMenu();
  AppendMenuW(hEditMenu, MF_STRING, IDM_UNDO, L"元に戻す(&U)\tCtrl+Z");
  AppendMenuW(hEditMenu, MF_STRING, IDM_REDO, L"やり直し(&R)\tCtrl+Y");
  AppendMenuW(hEditMenu, MF_SEPARATOR, 0, nullptr);
  AppendMenuW(hEditMenu, MF_STRING, IDM_CUT, L"切り取り(&T)\tCtrl+X");
  AppendMenuW(hEditMenu, MF_STRING, IDM_COPY, L"コピー(&C)\tCtrl+C");
  AppendMenuW(hEditMenu, MF_STRING, IDM_PASTE, L"貼り付け(&P)\tCtrl+V");
  AppendMenuW(hEditMenu, MF_STRING, IDM_SELECTALL, L"すべて選択(&S)\tCtrl+A");

  HMENU hMenu = CreateMenu();
  AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"ファイル(&F)");
  AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"編集(&E)");
  return hMenu;
}
