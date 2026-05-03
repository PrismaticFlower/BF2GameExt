#pragma once

#include <stdint.h>

#include "exe_table.hpp"

#include "lib/slim_vector.hpp"

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

struct byte_patch {
   byte_patch() = default;

   // Typesafe constructor for byte_patch. The stored byte byte_count will be one less than the
   // input as it's intended to be passed string literals.
   template<size_t byte_count>
   byte_patch(uintptr_t address, const char (&expected_bytes)[byte_count],
              const char (&replacement_bytes)[byte_count])
      : address{address},
        byte_count{byte_count - 1},
        expected_bytes{expected_bytes},
        replacement_bytes{replacement_bytes}
   {
   }

   uintptr_t address = 0;
   // Count of bytes pointer to by both expected_bytes and replacement_bytes.
   size_t byte_count = 0;
   const char* expected_bytes = nullptr;
   const char* replacement_bytes = nullptr;
};

struct patch_set {
   const char* name = "";
   slim_vector<patch> patches;
   slim_vector<byte_patch> byte_patches;
};

extern const slim_vector<patch_set> patch_lists[EXE_COUNT];