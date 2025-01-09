        AREA    Timing_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  Systick_Start_asm
        EXPORT  Systick_Stop_asm
		EXPORT	SysTick_Handler ; When the correct time comes,
									; remove this from the comments.
		EXTERN	ticks

SysTick_Handler FUNCTION
        PUSH    {LR}       ; save the LR 
        LDR     R0, =ticks      ; load the address of 'ticks'
        LDR     R1, [R0]        ; load the current value of 'ticks'
        ADDS    R1, R1, #1       ; increment ticks by 1
        STR     R1, [R0]        ;store the incremented value back
        POP     {PC}            ; restore the PC
        ENDFUNC

Systick_Start_asm FUNCTION
        PUSH    {LR}        ; save the LR to come back
        LDR     R0, =0xE000E010   ; SysTick Control and Status Register base address
        MOVS    R1, #249           ; Pre-calculated reload value: (SystemCoreClock / 100000) - 1
        STR     R1, [R0, #4]      ; Write the reload value to the SysTick Reload Value Register
        MOVS    R1, #0            ; Clear the SysTick Current Value Register
        STR     R1, [R0, #8]
        MOVS    R1, #0x7          ; Enable SysTick with processor clock and interrupts
        STR     R1, [R0]          ; Write to the Control Register
        LDR     R0, =ticks        ; Reset the global tticks variable
        MOVS    R1, #0
        STR     R1, [R0]
        POP     {PC}             ; restore the PC
        ENDFUNC

Systick_Stop_asm FUNCTION
        PUSH    {LR}          ; save the LR to come back
        LDR     R0, =0xE000E010 ; SysTick Control and Status Register base address
        MOVS    R1, #0x0       ;  7 -> 0 disable
        STR     R1, [R0]        ; Write to the Control Register
        LDR     R0, =ticks      ; load the address tick
        LDR     R1, [R0]        ; Read the current value of ticks in its address
        MOVS    R2, #0          ; Reset ticks to 0, for other iterations
        STR     R2, [R0]        ; Store 0 in ticks, adr
        MOV     R0, R1          ; current value of ticks appear in R0
        POP     {PC}         ; restore the PC
        ENDFUNC

        END
