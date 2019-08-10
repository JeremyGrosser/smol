# 1 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
# 23 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
    .syntax unified
    .arch armv6-m
# 33 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
    .section .stack







    .align 3
    .equ Stack_Size, 2048

    .globl __StackTop
    .globl __StackLimit
__StackLimit:
    .space Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3





    .equ Heap_Size, 2048

    .globl __HeapBase
    .globl __HeapLimit
__HeapBase:
    .if Heap_Size
    .space Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long __StackTop
    .long Reset_Handler
    .long NMI_Handler
    .long HardFault_Handler
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long SVC_Handler
    .long 0
    .long 0
    .long PendSV_Handler
    .long SysTick_Handler


    .long POWER_CLOCK_IRQHandler
    .long RADIO_IRQHandler
    .long UART0_IRQHandler
    .long SPI0_TWI0_IRQHandler
    .long SPI1_TWI1_IRQHandler
    .long 0
    .long GPIOTE_IRQHandler
    .long ADC_IRQHandler
    .long TIMER0_IRQHandler
    .long TIMER1_IRQHandler
    .long TIMER2_IRQHandler
    .long RTC0_IRQHandler
    .long TEMP_IRQHandler
    .long RNG_IRQHandler
    .long ECB_IRQHandler
    .long CCM_AAR_IRQHandler
    .long WDT_IRQHandler
    .long RTC1_IRQHandler
    .long QDEC_IRQHandler
    .long LPCOMP_IRQHandler
    .long SWI0_IRQHandler
    .long SWI1_IRQHandler
    .long SWI2_IRQHandler
    .long SWI3_IRQHandler
    .long SWI4_IRQHandler
    .long SWI5_IRQHandler
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

    .size __isr_vector, . - __isr_vector



    .equ NRF_POWER_RAMON_ADDRESS, 0x40000524
    .equ NRF_POWER_RAMONB_ADDRESS, 0x40000554
    .equ NRF_POWER_RAMONx_RAMxON_ONMODE_Msk, 0x3

    .text
    .thumb
    .thumb_func
    .align 1
    .globl Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:

    MOVS R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk

    LDR R0, =NRF_POWER_RAMON_ADDRESS
    LDR R2, [R0]
    ORRS R2, R1
    STR R2, [R0]

    LDR R0, =NRF_POWER_RAMONB_ADDRESS
    LDR R2, [R0]
    ORRS R2, R1
    STR R2, [R0]
# 163 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
    ldr r1, =__etext
    ldr r2, =__data_start__
    ldr r3, =__bss_start__

    subs r3, r3, r2
    ble .L_loop1_done

.L_loop1:
    subs r3, r3, #4
    ldr r0, [r1,r3]
    str r0, [r2,r3]
    bgt .L_loop1

.L_loop1_done:
# 206 "third_party/nrfx/mdk/gcc_startup_nrf51.S"
    bl SystemInit







    bl _start

    .pool
    .size Reset_Handler,.-Reset_Handler

    .section ".text"




    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    b .
    .size NMI_Handler, . - NMI_Handler


    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    b .
    .size HardFault_Handler, . - HardFault_Handler


    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    b .
    .size SVC_Handler, . - SVC_Handler


    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    b .
    .size PendSV_Handler, . - PendSV_Handler


    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    b .
    .size SysTick_Handler, . - SysTick_Handler




    .globl Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b .
    .size Default_Handler, . - Default_Handler

    .macro IRQ handler
    .weak \handler
    .set \handler, Default_Handler
    .endm

    IRQ POWER_CLOCK_IRQHandler
    IRQ RADIO_IRQHandler
    IRQ UART0_IRQHandler
    IRQ SPI0_TWI0_IRQHandler
    IRQ SPI1_TWI1_IRQHandler
    IRQ GPIOTE_IRQHandler
    IRQ ADC_IRQHandler
    IRQ TIMER0_IRQHandler
    IRQ TIMER1_IRQHandler
    IRQ TIMER2_IRQHandler
    IRQ RTC0_IRQHandler
    IRQ TEMP_IRQHandler
    IRQ RNG_IRQHandler
    IRQ ECB_IRQHandler
    IRQ CCM_AAR_IRQHandler
    IRQ WDT_IRQHandler
    IRQ RTC1_IRQHandler
    IRQ QDEC_IRQHandler
    IRQ LPCOMP_IRQHandler
    IRQ SWI0_IRQHandler
    IRQ SWI1_IRQHandler
    IRQ SWI2_IRQHandler
    IRQ SWI3_IRQHandler
    IRQ SWI4_IRQHandler
    IRQ SWI5_IRQHandler

  .end
