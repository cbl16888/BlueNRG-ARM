					AREA MicroDelay, CODE
					EXPORT MicroSecondDelay
					ALIGN
		
MicroSecondDelay	PROC
					PUSH {r4, lr}
					MOVS R1, #6
					MULS R0, R1, R0
loop				SUBS R0, R0, #1
					CMP R0, #0
					BNE loop
					POP {r4,pc}
					ENDP
						
					END
						
