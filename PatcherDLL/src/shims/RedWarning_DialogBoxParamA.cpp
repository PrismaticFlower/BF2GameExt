#include "pch.h"

#include "../../resource.h"

#include "../game_tables.hpp"

extern "C" IMAGE_DOS_HEADER __ImageBase;

int __stdcall RedWarning_DialogBoxParamA(void* hInstance, const char* lpTemplateName,
                                         void* hWndParent, void* lpDialogFunc, long dwInitParam)
{
   return DialogBoxParamA((HINSTANCE)&__ImageBase, MAKEINTRESOURCEA(IDD_GAMEMESSAGEDIALOG),
                          (HWND)hWndParent, (DLGPROC)lpDialogFunc, dwInitParam);
}