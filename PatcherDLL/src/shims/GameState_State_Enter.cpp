#include "pch.h"

#include "../game_tables.hpp"

static bool shell_state_ever_entered = false;

extern "C" void GameState_ShellState_Enter_Shim(GameState::State* self)
{
   shell_state_ever_entered = true;

   function_table.GameState_ShellState_Enter(self);
}

extern "C" void GameState_MissionState_Enter_Shim(GameState::State* self)
{
   if (not shell_state_ever_entered) {
      variable_table.GameState_shellState->Enter();
      variable_table.GameState_shellState->Exit();
   }

   function_table.GameState_MissionState_Enter(self);
}
