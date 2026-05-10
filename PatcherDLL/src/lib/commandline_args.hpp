#pragma once

struct commandline_args {
   constexpr static int noindex = -1;

   /// @brief Get the commandline argument at index
   /// @param index The index of the argument to get.
   /// @return The argument or nullptr if index is out of range.
   auto operator[](const int index) noexcept -> const char*;

   /// @brief Get the number of commandline arguments.
   /// @return The number of commandline arguments.
   auto size() const noexcept -> int;

   /// @brief Find the index of a commandline argument.
   /// @param arg The argument to search for.
   /// @return The index of the argument or noindex.
   auto find(const char* arg) const noexcept -> int;
};

extern commandline_args commandline;
