#pragma once

#include <stdint.h>

enum exe_id {
   exe_id_BF2_modtools,
   exe_id_GoG,
   exe_id_Steam,

   EXE_COUNT,
   EXE_INVALID = 0xffffffff,
};

struct exe_table_entry {
   const char* name = "";
   exe_id id = EXE_INVALID;

   bool id_address_is_file_offset = false;

   uintptr_t id_address = 0;
   uint64_t expected_id = 0;
};

extern exe_id current_exe_id;
extern const exe_table_entry exe_table[EXE_COUNT];