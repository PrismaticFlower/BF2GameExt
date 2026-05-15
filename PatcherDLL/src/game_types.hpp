#pragma once

namespace GameState {

struct State {
   virtual ~State() = 0;

   virtual void Enter() = 0;

   virtual void Update(float deltaTime, float systemDeltaTime) = 0;

   virtual void Exit() = 0;
};

}
