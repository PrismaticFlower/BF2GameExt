#include "pch.h"

#include "../game_tables.hpp"

// This shim is patched in to intiialize the RedTexture* pointers the terrain shader holds between maps.
//
// Doing this terrain's without detail maps being assigned a stale pointer when they're not first in the
// playlist.

void ReadTerrain_Shim(void* reader)
{
   *variable_table.terrainNullDetailTexture =
      (RedTexture*)function_table.PblHashTable_Find(variable_table.textureTable, texture_table_size,
                                                    0xba44f106 /* "null_detailmap" */);
   *variable_table.terrainWhiteTexture =
      (RedTexture*)function_table.PblHashTable_Find(variable_table.textureTable, texture_table_size,
                                                    0xba44f106 /* "white" */);

   function_table.ReadTerrain(reader);
}