.model  flat, C

PUBlIC BlurEffect_Render_Trampoline
BlurEffect_Render_Shim PROTO

.data

.code

BlurEffect_Render_Trampoline proc
	push DWORD PTR [esp + 4h]
	push ecx

	call BlurEffect_Render_Shim
	
	pop ecx
	pop ecx

	ret 4
BlurEffect_Render_Trampoline endp

end