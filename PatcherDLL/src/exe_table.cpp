#include "pch.h"

#include "exe_table.hpp"

extern exe_id current_exe_id = EXE_INVALID;

const exe_table_entry exe_table[EXE_COUNT] = {
   exe_table_entry{
      .name = "BF2_modtools",
      .id = exe_id_BF2_modtools,
      .id_address_is_file_offset = true,
      .id_address = 0x62b59c,
      .expected_id = 0x746163696c707041,
   },

   exe_table_entry{
      .name = "BattlefrontII.exe GoG",
      .id = exe_id_GoG,
      .id_address_is_file_offset = true,
      .id_address = 0x39f298,
      .expected_id = 0x746163696c707041,
   },

   exe_table_entry{
      .name = "BattlefrontII.exe Steam",
      .id = exe_id_Steam,
      .id_address_is_file_offset = true,
      .id_address = 0x39e234,
      .expected_id = 0x746163696c707041,
   },
};
