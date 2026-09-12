#ifndef ACCEL_H
#define ACCEL_H

#include <windows.h>

#include "accel.h"
#include "menu.h"

ACCEL accel[] = {
    /* ファイルメニュー */
    {FVIRTKEY | FCONTROL, 'O', IDM_OPEN},
    {FVIRTKEY | FCONTROL, 'S', IDM_SAVE},
    {FVIRTKEY | FCONTROL | FSHIFT, 'S', IDM_SAVEAS},
    {FVIRTKEY | FCONTROL, 'Q', IDM_QUIT},

    /* 編集メニュー */
    {FVIRTKEY | FCONTROL, 'Z', IDM_UNDO},
    {FVIRTKEY | FCONTROL, 'Y', IDM_REDO},
    {FVIRTKEY | FCONTROL, 'X', IDM_CUT},
    {FVIRTKEY | FCONTROL, 'C', IDM_COPY},
    {FVIRTKEY | FCONTROL, 'V', IDM_PASTE},
    {FVIRTKEY | FCONTROL, 'A', IDM_SELECTALL},
};

HACCEL hAccel = CreateAcceleratorTableW(accel, ARRAYSIZE(accel));

#endif  // ACCEL_H
