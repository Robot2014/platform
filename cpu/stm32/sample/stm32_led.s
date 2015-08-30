.equ stack_top , 0x20000800
.text
.global _start
.code 16
.syntax unifi ed
.type _start , function
_start :
	mov r0 , #0
	mov r1 , #0
	mov r2 , #0
	mov r3 , #0
	mov r4 , #0 
	mov r5 , #0
	ldr r0 , =hello
	bl puts
	mov r0 , #0x4
	bl putc
deadloop:
	b deadloop
hello:
	.ascii "hello world \n"
	.byte 0
	.align

puts :
	push {r0 ,r1 ,lr }
	mov r1, r0

putsloop:
	ldrb.w r0,[r1],#1
	cbz r0, putsloopexit
	bl putc
	b  putsloop

putsloopexit :
	pop { r0 ,r1 ,pc}
	.equ GPIO_DATA ,40013804
	.equ :
	.equ UART


