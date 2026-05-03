#include "pch.h"

#include "../game_tables.hpp"

#include "../lib/utility.hpp"

const static size_t downsize_factor_offset = 0x30;

extern "C" void BlurEffect_Render_Shim(BlurEffect* self, uint32_t unkn_flags)
{
   float vpMinX = 0.0f;
   float vpMinY = 0.0f;
   float vpMaxX = 0.0f;
   float vpMaxY = 0.0f;

   function_table.RedRenderer_pcGetViewportExtents(&vpMinX, &vpMinY, &vpMaxX, &vpMaxY);

   float* downsize_factor = (float*)((unsigned char*)self + downsize_factor_offset);

   const float viewport_max_length = max(vpMaxX - vpMinX, vpMaxY - vpMinY);
   const float max_downsize_resolution = 512.0f;
   const float current_downsized_resolution = *downsize_factor * viewport_max_length;

   if (current_downsized_resolution > max_downsize_resolution) {
      *downsize_factor *= (max_downsize_resolution / current_downsized_resolution);
   }

   function_table.BlurEffect_Render(self, unkn_flags);
}