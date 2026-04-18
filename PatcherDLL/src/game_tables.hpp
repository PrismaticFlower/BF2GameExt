#pragma once

#include <stdint.h>

#include "exe_table.hpp"
#include "slim_vector.hpp"

const uint32_t texture_table_size = 0x2000;

// Names guessed either from context or from BF1 Mac executable. Could be wrong.
struct PblHashTable;
struct RedTexture;

// Holds pointers that can be used to access information about the game in patch functions without
// having to handle exe differences in those functions.
struct variable_table_t {
   PblHashTable* textureTable = nullptr;

   RedTexture** terrainNullDetailTexture = nullptr;
   RedTexture** terrainWhiteTexture = nullptr;
};

// Holds function pointers from the game's executable. Can be used in patch functions.
struct function_table_t {
   // Utility Functions
   void* (*PblHashTable_Find)(void* table, uint32_t tableSize, uint32_t hash) = nullptr;

   // Chunk Reader Functions
   void (*ReadTerrain)(void* reader) = nullptr;
};

void initialize_game_tables(const exe_id exe, const uintptr_t relocated_executable_base);

// initialize_game_tables must be called before using either of these.
extern variable_table_t variable_table;
extern function_table_t function_table;
