#pragma once

#include <stdint.h>

#include "exe_table.hpp"
#include "slim_vector.hpp"

struct patch_flags {
   /// @brief Address represents a file offset instead of a virtual an unrelocated virtual address.
   bool file_offset : 1 = false;

   /// expected_value is an unrelocated virtual address (what would be displayed in tools like Ghidra/IDA)
   bool expected_is_va : 1 = false;

   /// Replacement value is a function ptr and should be adjusted for use as in a relative call instruction.
   bool replacement_relative_for_call : 1 = false;
};

struct patch {
   uintptr_t address = 0;
   uint32_t expected_value = 0;
   uint32_t replacement_value = 0;
   patch_flags flags = {};
};

struct patch_set {
   const char* name = "";
   slim_vector<patch> patches;
};

extern const slim_vector<patch_set> patch_lists[EXE_COUNT];