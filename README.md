# Simple Microcontroller Operation Library (SMOL)

## Quick start

Use the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads/7-2018-q2-update).

Put your application code in src/

Platform-independent methods can be included from include/platform/

Board support is in boards/

    make BOARD=samd21-xpro

## Supported Boards

| Name | Description | Link |
| ---- | ----------- | ---- |
| samd21-xpro | Atmel SAM D21 Xplained Pro | https://www.microchip.com/developmenttools/ProductDetails/ATSAMD21-XPRO |
| m0-mini | Arduino M0 clones (various) | https://www.aliexpress.com/item/SAMD21-M0-Mini-32-bit-ARM-Cortex-M0-Core-Compatible-with-Arduino-Zero-Form-Mini/32890639630.html |
