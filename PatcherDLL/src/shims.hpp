#pragma once

// Forward declarations for shims to save having a header file for each.

void ReadTerrain_Shim(void* reader);

void Screenshot_RequestScreenshot_Shim();

void RedWarning_Init_Shim();

int __stdcall RedWarning_DialogBoxParamA(void* hInstance, const char* lpTemplateName,
                                         void* hWndParent, void* lpDialogFunc, long dwInitParam);

// .asm declarations
//
// These functions may have __thiscall non-standard calling conventions. Do not try call them directly.

extern "C" {

// void __thiscall BlurEffect_Render_Trampoline(BlurEffect* self, uint32_t unkn_flags);
void BlurEffect_Render_Trampoline();
}