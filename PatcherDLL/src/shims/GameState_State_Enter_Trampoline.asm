.model  flat, C

PUBlIC GameState_ShellState_Enter_Trampoline
PUBlIC GameState_MissionState_Enter_Trampoline

GameState_ShellState_Enter_Shim PROTO 
GameState_MissionState_Enter_Shim PROTO

.data

.code

GameState_ShellState_Enter_Trampoline proc
	push ecx

	call GameState_ShellState_Enter_Shim
	
	pop ecx

	ret
GameState_ShellState_Enter_Trampoline endp

GameState_MissionState_Enter_Trampoline proc
	push ecx

	call GameState_MissionState_Enter_Shim
	
	pop ecx

	ret
GameState_MissionState_Enter_Trampoline endp

end