#include "pch.h"

#include "patch_table.hpp"
#include "shims.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;

static char DLC_mission_table_storage[DLC_mission_size * DLC_mission_patch_limit] = {};
static const uint32_t DLC_mission_table_address = (uint32_t)&DLC_mission_table_storage[0];

// Function names matched from BF1 Mac executable. Could be wrong in cases.

// clang-format off

const slim_vector<patch_set> patch_lists[EXE_COUNT] = {
   // exe_id_BF2_modtools
   slim_vector<patch_set>{
      patch_set{
         .name = "RedMemory Heap Extensions",
         .patches =
            {
               patch{0x337921, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
               patch{0x33792c, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
            },
      },
      
      patch_set{
         .name = "SoundParameterized Layer Limit Extension",
         .patches =
            {
               patch{0x6227c2, 0xa0, 0x2000, {.file_offset = true}},
            },
      },
      
      patch_set{
         .name = "DLC Mission Limit Extension",
         .patches =
            {
               patch{0x4935c, 0xb08308, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                         // SetCurrentMap
               patch{0x493ac, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
               patch{0x49415, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
               patch{0x49472, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
               patch{0x494fb, 0x1f4, DLC_mission_patch_limit, {.file_offset = true, .expected_is_va = true}},                              // AddDownloadableContent
               patch{0x4951f, 0xb08308, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                         // AddDownloadableContent
               patch{0x49542, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x49548, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x49571, 0xb08413, (0xb08413 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x4957d, 0xb08414, (0xb08414 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
            },
      },

      patch_set{
         .name = "Terrain RedTexture* Cleanup",
         .patches =
            {
               patch{0x00452ed2, 0x392f3a, (uintptr_t)&ReadTerrain_Shim, {.replacement_relative_for_call = true}},
            },
      },

      patch_set{
         .name = "SkyObjectClass Limit Extension",
         .patches =
            {
               patch{0x006c23b1, 0x00ba45cc, 0x401f0f /* 4 byte nop */, {.expected_is_va = true}},
            },
            .byte_patches = {
               byte_patch{0x006c23ae, "\x41\x89\x0d", "\x0f\x1f\x00"},
            },
      },

      patch_set{
         .name = "PropGenerator::Update Loop Exit Condition",

         .byte_patches = {
            byte_patch{0x0073d314, "\x49", "\x33"},
            // Swap two instructions so we have a convenient jump target for the above redirection.
            // This allows jumping directly to relevant instructions for checking the cluster object count
            // without doing any other work (and thus greatly reduced risk of introducing bugs).
            byte_patch{0x0073d344, "\x8b\x54\x24\x40", "\x88\x44\x71\x01"},
            byte_patch{0x0073d348, "\x88\x44\x71\x01", "\x8b\x54\x24\x40"},
         },
      },

      patch_set{
         .name = "BlurEffect::Render Downsize Clamp",

         .patches = {
            patch{0x00a69698, 0x00409755, (uintptr_t)(&BlurEffect_Render_Trampoline), {.expected_is_va = true}},
         },
      },
   },
   
   // exe_id_GoG
   slim_vector<patch_set>{
      patch_set{
         .name = "RedMemory Heap Extensions",
         .patches =
            {
               patch{0x217651, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
               patch{0x217667, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
            },
      },
      
      patch_set{
         .name = "SoundParameterized Layer Limit Extension",
         .patches =
            {
               patch{0x3e310c, 0xa0, 0x2000, {.file_offset = true}},
            },
      },
      
      patch_set{
         .name = "DLC Mission Limit Extension",
         .patches =
            {
               patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, {.file_offset = true}},                                                         // AddDownloadableContent
               patch{0x8de9f, 0x1e31f00, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // AddDownloadableContent
               patch{0x8dec3, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8dec9, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8def0, 0x1e3200b, (0x1e3200b - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8def7, 0x1e3200c, (0x1e3200c - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8df28, 0x1e31f00, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // SetCurrentMap
               patch{0x8df68, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
               patch{0x8dfb4, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
               patch{0x8dfce, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
            },
      },

      patch_set{
         .name = "Terrain RedTexture* Cleanup",
         .patches =
            {
               patch{0x0057a57f, 0x148f6d, (uintptr_t)&ReadTerrain_Shim, {.replacement_relative_for_call = true}},
            },
      },

      patch_set{
         .name = "SkyObjectClass Limit Extension",
         .patches =
            {
               patch{0x00639e41, 0x01eb051c, 0x401f0f /* 4 byte nop */, {.expected_is_va = true}},
               patch{0x00639e6a, 0x01eb051c, 0x401f0f /* 4 byte nop */, {.expected_is_va = true}},
            },
            .byte_patches = {
               byte_patch{0x00639e3e, "\x41\x89\x0d", "\x0f\x1f\x00"},
               byte_patch{0x00639e68, "\x40\xa3", "\x66\x90"},
            },
      },

      patch_set{
         .name = "PropGenerator::Update Loop Exit Condition",

         .byte_patches = {
            byte_patch{0x0062bf6b, "\x68", "\x42"},
         },
      },

      patch_set{
         .name = "BlurEffect::Render Downsize Clamp",

         .patches = {
            patch{0x00797f48, 0x0040f8d0, (uintptr_t)(&BlurEffect_Render_Trampoline), {.expected_is_va = true}},
         },
      },

      patch_set{
         .name = "Screenshot::RequestScreenshot Redirect",

         .patches = {
            patch{0x00534291, 0x1c0b7b, (uintptr_t)(&Screenshot_RequestScreenshot_Shim), {.replacement_relative_for_call = true}},
         },
      },

      patch_set{
         .name = "Logging Enablement",

         .patches = {
            patch{0x006186e3, 0xdf889, (uintptr_t)(&RedWarning_Init_Shim), {.replacement_relative_for_call = true}},
         },
      },
   },

   // exe_id_Steam
   slim_vector<patch_set>{
      patch_set{
         .name = "RedMemory Heap Extensions",
         .patches =
            {
               patch{0x2165b1, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
               patch{0x2165c7, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
            },
      },
      
      patch_set{
         .name = "SoundParameterized Layer Limit Extension",
         .patches =
            {
               patch{0x3e170c, 0xa0, 0x2000, {.file_offset = true}},
            },
      },
      
      patch_set{
         .name = "DLC Mission Limit Extension",
         .patches =
            {
               patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, {.file_offset = true}},                                                         // AddDownloadableContent
               patch{0x8de9f, 0x1e30950, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // AddDownloadableContent
               patch{0x8dec3, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8dec9, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8def0, 0x1e30a5b, (0x1e30a5b - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8def7, 0x1e30a5c, (0x1e30a5c - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
               patch{0x8df28, 0x1e30950, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // SetCurrentMap
               patch{0x8df68, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
               patch{0x8dfb4, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
               patch{0x8dfce, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
            },
      },

      patch_set{
         .name = "Terrain RedTexture* Cleanup",
         .patches =
            {
               patch{0x0057980f, 0x148c4d, (uintptr_t)&ReadTerrain_Shim, {.replacement_relative_for_call = true}},
            },
      },

      patch_set{
         .name = "SkyObjectClass Limit Extension",
         .patches =
            {
               patch{0x00638da1, 0x01eaf068, 0x401f0f /* 4 byte nop */, {.expected_is_va = true}},
               patch{0x00638dca, 0x01eaf068, 0x401f0f /* 4 byte nop */, {.expected_is_va = true}},
            },
            .byte_patches = {
               byte_patch{0x00638d9e, "\x41\x89\x0d", "\x0f\x1f\x00"},
               byte_patch{0x00638dc8, "\x40\xa3", "\x66\x90"},
            },
      },

      patch_set{
         .name = "PropGenerator::Update Loop Exit Condition",

         .byte_patches = {
            byte_patch{0x0062aedb, "\x68", "\x42"},
         },
      },

      patch_set{
         .name = "BlurEffect::Render Downsize Clamp",

         .patches = {
            patch{0x00796fa8, 0x0040f8d0, (uintptr_t)(&BlurEffect_Render_Trampoline), {.expected_is_va = true}},
         },
      },

      patch_set{
         .name = "Screenshot::RequestScreenshot Redirect",

         .patches = {
            patch{0x00533521, 0x1c081b, (uintptr_t)(&Screenshot_RequestScreenshot_Shim), {.replacement_relative_for_call = true}},
         },
      },

      patch_set{
         .name = "Logging Enablement",

         .patches = {
            patch{0x0061766c, 0xdf830, (uintptr_t)(&RedWarning_Init_Shim), {.replacement_relative_for_call = true}},
         },
      },
   },
};

