#pragma once

#include <stdio.h>

struct cfile {
   cfile(const char* filename, const char* mode);

   ~cfile();

   void printf(char const* const format, ...) const;

   void write(const void* bytes, size_t count);

   explicit operator bool() const noexcept;

private:
   FILE* file = nullptr;
};