#ifndef MENU_H
#define MENU_H

#include <windows.h>

/* ファイルメニューのマクロ */
#define IDM_OPEN 1    // 開く
#define IDM_SAVE 2    // 保存
#define IDM_SAVEAS 3  // 名前をつけて保存
#define IDM_QUIT 4    // 終了

/* 編集メニューのマク*/
#define IDM_UNDO 5       // 元に戻す
#define IDM_CUT 6        // 切り取り
#define IDM_COPY 7       // コピー
#define IDM_PASTE 8      // 貼り付け
#define IDM_SELECTALL 9  // すべて選択

HMENU createMenu();

#endif  // MENU_H
