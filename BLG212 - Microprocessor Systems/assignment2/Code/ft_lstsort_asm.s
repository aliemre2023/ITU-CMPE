; Function: ft_lstsort_asm
; Parameters:
;   R0 - Pointer to the list (address of t_list *)
;   R1 - Pointer to comparison function (address of int (*f_comp)(int, int))
        AREA    Sorting_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  ft_lstsort_asm

ft_lstsort_asm FUNCTION
		PUSH	{R0-R1, LR}
		;LDR	R3, [SP]	 ; you will see the first argument
								; of the function in R3
		;LDR	R3, [SP, #4] ; you will see the second argument
								; of the function in R3
		; Given a singly linked list,
			; sort it using bubble sort by swapping nodes

        ; assume R1 holds the head address
		
		LDR		R1, =0x20000004
			
		B		bubble_sort_start
	
        
					
bubble_sort_start
        ; R2 ve R3 headin valuelarini tutacak, Node* ptr1 = head;
        LDR     R2, [R1]        ;head->val
        ADDS    R3, R1, #4      ;R3 address tutmali
		
        ; copy of head, ana while döngüsünün bitip bitmediğini kontrol edicek
		MOV		R6, R1          ; head val
		MOV 	R7, R3          ; head address
		

        ; bu islemi her firsT_while'a girmeden önce kontrol ettirmeliyim
        CMP     R3, #0          ;if (head == nullptr) return;
        BEQ     bubble_sort_finish

        ; sonraki islemler için null bir value, Node* ptr2 = nullptr;
        MOVS    R4, #0      ; make R4 0
        MOVS    R5, #0      ; make R5 0

        B       bs_first_while

bs_first_while
		PUSH	{R6, R7}        ; R6 ve R7'yi depola

		LDR     R2, [R1]        ;head->val
        ADDS    R3, R1, #4      ;R3 address tutmali
		
        ; ptr2'yi head olarak atama
        MOV     R4, R2      ; Hold val
		MOV		R5, R3 ; hold only next
		
        ; bu ptr2'nin nexti var mi kontrolü, bu her döngüde check edilicek
        ; burda R2, ve R3, ü kullanicam next'i tutmak için, sonra poplarim
        PUSH    {R2, R3}        ; save R2, R3
            
        ; bunu her seconWhile'dan sona yapmam lazim
        CMP     R5, #0      ; while (ptr2->next != nullptr) {
        BEQ     bs_first_while_reoccur  ; end of first while
        B       bs_second_while         ; second while ile devam, no null vlaue

bs_second_while
	
		; nexts
		LDR     R2, [R5]    ; R5 (copy of R3tü) next addressi tutyou zaten böylece R2ye nextval atandi
		ADDS	R3, R2, #4	; next
		LDR		R2, [R2]	; val
		;LDR		R3, [R3] I only want real ->next address
            ; wait

        CMP     R4, R2      ; if (ptr2->val > ptr2->next->val) {
        BLE     bs_if_stop  ; swaplama işlemi olmayacak

        MOV     R6, R4      ; temp val to hold R4 val
		SUBS 	R7, R5, #4	; r5 was next address I need to hold val address
		STR 	R2, [R7]    ; swap 1
		LDR		R7, [R5]	; this is the next node's val address
		STR		R6, [R7]    ; swap 2
	
        B       bs_if_stop  ; if'ten çık , end of second while

bs_if_stop
        ; ptr2 = ptr2->next;
		LDR 	R4, [R5]    ; R4 is nexts address    
		ADDS    R5, R4, #4  ; nexts val
        LDR     R4, [R4]    ; nexts next
     

        ; while (ptr2->next != nullptr) {
		LDR 	R7, [R5]                ; controll if next bull
        CMP     R7, #0                  ; 0 means null
        BEQ     bs_first_while_reoccur  ; secodn while end
        B       bs_second_while         ; continue
        

bs_first_while_reoccur
        ; ptr1 = ptr1->next;
        POP     {R2, R3}    ; take back r2, r3
		POP		{R6, R7}    ; take back R6, R7 
		
		LDR 	R6, [R7]    ; make next hthe copy of head for finish the first loop
		ADDS    R7, R6, #4  ; load next address
        LDR     R6, [R6]    ; load next value 

        ; while (ptr1 != nullptr)
		LDR 	R4, [R7]        ; R7 is the head(copy)'s next   
		;LDR		R4, [R4]
        CMP     R4, #0      ;0 is null
        BEQ     bubble_sort_finish      ; stop, sorted
        B       bs_first_while          ; continue

bubble_sort_finish
		POP		{R0-R1, PC}             ; take back r1 r2 and cntniue
        ENDFUNC
		END