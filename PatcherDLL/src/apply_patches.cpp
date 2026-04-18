#include "pch.h"

#include "apply_patches.hpp"
#include "cfile.hpp"
#include "exe_table.hpp"
#include "game_tables.hpp"
#include "patch_table.hpp"

static const uintptr_t unrelocated_executable_base = 0x400000;

static bool memeq(const void* left, size_t left_size, const void* right, size_t right_size)
{
   if (left_size != right_size) return false;

   return memcmp(left, right, left_size) == 0;
}

static auto resolve_file_address(uintptr_t offset, const slim_vector<section_info>& sections) -> char*
{
   for (const section_info& section : sections) {
      if (offset >= section.file_start and offset < section.file_end) {
         return section.memory_start + (offset - section.file_start);
      }
   }

   return nullptr;
}

static auto resolve_address(uintptr_t virtual_address, const uintptr_t relocated_executable_base) -> char*
{
   return (char*)((virtual_address - unrelocated_executable_base) + relocated_executable_base);
}

static bool apply_patch(const patch& patch, const uintptr_t relocated_executable_base,
                        const slim_vector<section_info>& sections)
{
   char* patch_address = patch.flags.file_offset
                            ? resolve_file_address(patch.address, sections)
                            : resolve_address(patch.address, relocated_executable_base);

   const uint32_t expected_value = patch.flags.expected_is_va
                                      ? patch.expected_value - unrelocated_executable_base + relocated_executable_base
                                      : patch.expected_value;

   if (not memeq(patch_address, sizeof(expected_value), &expected_value, sizeof(expected_value))) {
      return false;
   }

   const uint32_t replacement_value =
      patch.flags.replacement_relative_for_call
         ? (intptr_t)patch.replacement_value - (intptr_t)patch_address - sizeof(uint32_t)
         : patch.replacement_value;

   memcpy(patch_address, &replacement_value, sizeof(replacement_value));

   return true;
}

static bool select_executable(const uintptr_t relocated_executable_base,
                              const slim_vector<section_info>& sections, cfile& log)
{
   for (const exe_table_entry& entry : exe_table) {
      log.printf("Checking executable against patch list: %s\n", entry.name);

      if (entry.id_address_is_file_offset) {
         const char* id_address = resolve_file_address(entry.id_address, sections);

         if (not id_address or not memeq(id_address, sizeof(entry.expected_id), &entry.expected_id,
                                         sizeof(entry.expected_id))) {
            continue;
         }
      }
      else {
         if (not memeq(resolve_address(entry.id_address, relocated_executable_base),
                       sizeof(entry.expected_id), &entry.expected_id, sizeof(entry.expected_id))) {
            continue;
         }
      }

      if (entry.id == EXE_INVALID) {
         log.printf("Bad executable table entry: %s", entry.name);

         continue;
      }

      log.printf("Identified executable as: %s\nApplying patches.\n", entry.name);

      current_exe_id = entry.id;

      return true;
   }

   return false;
}

bool apply_patches(const uintptr_t relocated_executable_base, const slim_vector<section_info>& sections)
{
   cfile log{"BF2GameExt.log", "w"};

   if (not log) return false;

   if (not select_executable(relocated_executable_base, sections, log)) {
      log.printf("Couldn't identify executable. Unable to patch.\n");

      return false;
   }

   initialize_game_tables(current_exe_id, relocated_executable_base);

   for (const patch_set& set : patch_lists[current_exe_id]) {
      log.printf("Applying patch set: %s\n", set.name);

      for (const patch& patch : set.patches) {
         if (not apply_patch(patch, relocated_executable_base, sections)) {
            log.printf(R"(Failed to apply patch
   address = %x 
   expected_value = %x 
   replacement_value = %x
   flags = {.file_offset = %i, .expected_is_va = %i, replacement_relative_for_call = %i}
)",
                       patch.address, patch.expected_value, patch.replacement_value,
                       (int)patch.flags.file_offset, (int)patch.flags.expected_is_va,
                       (int)patch.flags.replacement_relative_for_call);

            return false;
         }
      }
   }

   return true;
}
