#include "pch.h"

#include "../game_tables.hpp"

#include "../lib/cfile.hpp"

#include <d3d9.h>

// This shim is patched in on Steam/GoG builds to fix the broken screenshot hotkey.

static bool get_screenshot_path(char (&file_path)[32])
{
   for (int i = 0; i < 10000; ++i) {
      snprintf(&file_path[0], sizeof(file_path), R"(ScreenShots\screenshot_%04d.tga)", i);

      if (GetFileAttributesA(file_path) == INVALID_FILE_ATTRIBUTES) return true;
   }

   return false;
}

static void save_screenshot(const D3DLOCKED_RECT& locked, const UINT width, const UINT height)
{
   CreateDirectoryA("ScreenShots", nullptr);

   char filePath[32] = {};

   if (not get_screenshot_path(filePath)) return;

   cfile file{filePath, "wb"};

   if (not file) return;

   // Super simple .TGA writer.
   constexpr uint8_t rgba_image_type = 2;

   struct tga_header {
      uint8_t image_id_length = 0;
      uint8_t color_map_type = 0;
      uint8_t image_type = rgba_image_type;
      uint8_t color_map_index[2] = {0, 0};
      uint8_t color_map_length[2] = {0, 0};
      uint8_t color_map_entry_size = 0;
      uint16_t image_x_origin = 0;
      uint16_t image_y_origin = 0;
      uint16_t image_width;
      uint16_t image_height;
      uint8_t image_pixel_depth = 24;
      uint8_t image_description = 0;
   };

   tga_header header = {.image_width = (uint16_t)width, .image_height = (uint16_t)height};

   file.write(&header, sizeof(header));

   const char* src_bytes = (const char*)locked.pBits;

   for (INT y = (INT)height - 1; y >= 0; --y) {
      for (INT x = 0; x < (INT)width; ++x) {
         UINT texel = 0;

         memcpy(&texel, src_bytes + y * locked.Pitch + x * sizeof(UINT), sizeof(texel));

         file.write(&texel, 3);
      }
   }
}

void Screenshot_RequestScreenshot_Shim()
{
   IDirect3DDevice9* device = *variable_table.d3dDevice;

   IDirect3DSurface9* backbuffer_surface = nullptr;
   IDirect3DTexture9* intermediate_target_texture = nullptr;
   IDirect3DSurface9* intermediate_target_surface = nullptr;
   IDirect3DSurface9* sys_mem_surface = nullptr;

   D3DSURFACE_DESC backbuffer_desc = {};

   if (FAILED(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer_surface))) {
      goto CLEANUP;
   }

   if (FAILED(backbuffer_surface->GetDesc(&backbuffer_desc))) {
      goto CLEANUP;
   }

   if (FAILED(device->CreateTexture(backbuffer_desc.Width, backbuffer_desc.Height, 1,
                                    D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                    &intermediate_target_texture, nullptr))) {
      goto CLEANUP;
   }

   if (FAILED(intermediate_target_texture->GetSurfaceLevel(0, &intermediate_target_surface))) {
      goto CLEANUP;
   }

   if (FAILED(device->CreateOffscreenPlainSurface(backbuffer_desc.Width, backbuffer_desc.Height, D3DFMT_A8R8G8B8,
                                                  D3DPOOL_SYSTEMMEM, &sys_mem_surface, nullptr))) {
      goto CLEANUP;
   }

   if (FAILED(device->StretchRect(backbuffer_surface, nullptr, intermediate_target_surface, nullptr,
                                  D3DTEXF_NONE))) {
      goto CLEANUP;
   }

   if (FAILED(device->GetRenderTargetData(intermediate_target_surface, sys_mem_surface))) {
      goto CLEANUP;
   }

   if (D3DLOCKED_RECT locked{}; SUCCEEDED(sys_mem_surface->LockRect(&locked, nullptr, D3DLOCK_READONLY))) {
      save_screenshot(locked, backbuffer_desc.Width, backbuffer_desc.Height);

      sys_mem_surface->UnlockRect();
   }

CLEANUP:
   if (backbuffer_surface) backbuffer_surface->Release();
   if (intermediate_target_texture) intermediate_target_texture->Release();
   if (intermediate_target_surface) intermediate_target_surface->Release();
   if (sys_mem_surface) sys_mem_surface->Release();
}