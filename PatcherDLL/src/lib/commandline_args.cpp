#include "pch.h"

#include "commandline_args.hpp"

auto commandline_args::operator[](const int index) noexcept -> const char*
{
   if (index < 0 or index >= size()) return nullptr;

   return __argv[index + 1];
}

auto commandline_args::size() const noexcept -> int
{
   if (__argc == 0) return 0;

   return __argc - 1;
}

auto commandline_args::find(const char* arg) const noexcept -> int
{
   for (int i = 1; i < __argc; ++i) {
      if (strcmp(__argv[i], arg) == 0) {
         return i - 1;
      }
   }

   return noindex;
}

constinit commandline_args commandline;