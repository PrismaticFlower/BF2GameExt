#include "pch.h"

#include "../game_tables.hpp"

#include "../lib/commandline_args.hpp"

const static int file_destination = 2;

void RedWarning_Init_Shim()
{
   function_table.RedWarning_Init();

   if (commandline.find("/log") != commandline.noindex) {
      function_table.RedWarning_SetDestinationMinSeverity(file_destination, 0);

      *variable_table._pcLoggingEnabled = true;
   }
}
