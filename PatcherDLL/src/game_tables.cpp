#include "pch.h"

#include "game_tables.hpp"

static const uintptr_t unrelocated_executable_base = 0x400000;

// These tables (variable_table_init and function_table_init) store unrelocated virtual addresses.
// Basically the addresses a disassembler/decompiler like Ghidra will display. This makes populating
// them very easy, just copy the address of the variable/function from Ghidra.

struct variable_table_init {
   uintptr_t textureTable = 0;

   uintptr_t terrainNullDetailTexture = 0;
   uintptr_t terrainWhiteTexture = 0;

   uintptr_t d3dDevice = 0;

   uintptr_t _pcLoggingEnabled = 0;
};

struct function_table_init {
   uintptr_t PblHashTable_Find = 0;

   uintptr_t RedRenderer_pcGetViewportExtents = 0;

   uintptr_t ReadTerrain = 0;

   uintptr_t BlurEffect_Render = 0;

   uintptr_t RedWarning_Init = 0;
   uintptr_t RedWarning_SetDestinationMinSeverity = 0;
};

static variable_table_init variable_init_tables[EXE_COUNT] = {
   // exe_id_BF2_modtools
   variable_table_init{
      .textureTable = 0x00d4f994,

      .terrainNullDetailTexture = 0x00edd114,
      .terrainWhiteTexture = 0x00edd11c,

      .d3dDevice = 0x00d63fd4,

      ._pcLoggingEnabled = 0x00adb69e,
   },

   // exe_id_GoG
   variable_table_init{
      .textureTable = 0x008f022c,

      .terrainNullDetailTexture = 0x009ca6cc,
      .terrainWhiteTexture = 0x009ca6d0,

      .d3dDevice = 0x007f6dec,

      ._pcLoggingEnabled = 0x01eb029a,
   },

   // exe_id_Steam
   variable_table_init{
      .textureTable = 0x008eed8c,

      .terrainNullDetailTexture = 0x009c922c,
      .terrainWhiteTexture = 0x009c9230,

      .d3dDevice = 0x007f594c,

      ._pcLoggingEnabled = 0x01eaede6,
   },
};

static function_table_init function_init_tables[EXE_COUNT] = {
   // exe_id_BF2_modtools
   function_table_init{
      .PblHashTable_Find = 0x007e1a40,

      .RedRenderer_pcGetViewportExtents = 0x00805f40,

      .ReadTerrain = 0x007e5e10,

      .BlurEffect_Render = 0x0077a930,

      .RedWarning_Init = 0x007e3c20,
      .RedWarning_SetDestinationMinSeverity = 0x007e3560,
   },

   // exe_id_GoG
   function_table_init{
      .PblHashTable_Find = 0x00727ed0,

      .RedRenderer_pcGetViewportExtents = 0x006b9730,

      .ReadTerrain = 0x006c34f0,

      .BlurEffect_Render = 0x0040f8d0,

      .RedWarning_Init = 0x006f7f70,
      .RedWarning_SetDestinationMinSeverity = 0x006f8350,
   },

   // exe_id_Steam
   function_table_init{
      .PblHashTable_Find = 0x00726e00,

      .RedRenderer_pcGetViewportExtents = 0x006b86a0,

      .ReadTerrain = 0x006c2460,

      .BlurEffect_Render = 0x0040f8d0,

      .RedWarning_Init = 0x006f6ea0,
      .RedWarning_SetDestinationMinSeverity = 0x006f7280,
   },
};

static auto resolve_variable_address(const uintptr_t virtual_address,
                                     const uintptr_t relocated_executable_base) -> void*
{
   // Unlike resolve_address in apply_patches.cpp, we support null addresses to ease development.
   // Allows for a variable to be filled out and tested for one exe at a time.
   if (virtual_address == 0) return nullptr;

   return (void*)((virtual_address - unrelocated_executable_base) + relocated_executable_base);
}

static void initialize_variable_table(const exe_id exe, const uintptr_t relocated_executable_base)
{
   const variable_table_init& init_table = variable_init_tables[exe];

// This macro may look scary but is actually quite simple. All it does is allow typing the variable name once for both tables
// and do a C-style cast using decltype to get the pointer from resolve_variable_address into the correct type.
#define INIT_TABLE_ENTRY(entry)                                                                    \
   .entry = (decltype(variable_table_t::entry))resolve_variable_address(init_table.entry,          \
                                                                        relocated_executable_base)

   variable_table = {
      INIT_TABLE_ENTRY(textureTable),

      INIT_TABLE_ENTRY(terrainNullDetailTexture),
      INIT_TABLE_ENTRY(terrainWhiteTexture),

      INIT_TABLE_ENTRY(d3dDevice),

      INIT_TABLE_ENTRY(_pcLoggingEnabled),
   };

#undef INIT_TABLE_ENTRY
}

static void initialize_function_table(const exe_id exe, const uintptr_t relocated_executable_base)
{
   const function_table_init& init_table = function_init_tables[exe];

// See comment for INIT_TABLE_ENTRY in initialize_variable_table.
#define INIT_TABLE_ENTRY(entry)                                                                    \
   .entry = (decltype(function_table_t::entry))resolve_variable_address(init_table.entry,          \
                                                                        relocated_executable_base)

   function_table = {
      INIT_TABLE_ENTRY(PblHashTable_Find),

      INIT_TABLE_ENTRY(RedRenderer_pcGetViewportExtents),

      INIT_TABLE_ENTRY(ReadTerrain),

      INIT_TABLE_ENTRY(BlurEffect_Render),

      INIT_TABLE_ENTRY(RedWarning_Init),
      INIT_TABLE_ENTRY(RedWarning_SetDestinationMinSeverity),
   };
}

void initialize_game_tables(const exe_id exe, const uintptr_t relocated_executable_base)
{
   initialize_variable_table(exe, relocated_executable_base);
   initialize_function_table(exe, relocated_executable_base);
}

variable_table_t variable_table;
function_table_t function_table;