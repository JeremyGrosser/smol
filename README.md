# Simple Microcontroller Operation Library (SMOL)

## Quick start

Use the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads/7-2018-q2-update).

Put your application code in src/

Platform-independent methods can be included from include/platform/

Board support is in boards/

    make BOARD=samd21-xpro

## Supported Boards

| Name | Status | Description | Link |
| ---- | ----------- | ---- |
| samd21-xpro | STABLE | Atmel SAM D21 Xplained Pro | https://www.microchip.com/developmenttools/ProductDetails/ATSAMD21-XPRO |
| m0-mini | BETA | Arduino M0 clones (various) | https://www.aliexpress.com/item/SAMD21-M0-Mini-32-bit-ARM-Cortex-M0-Core-Compatible-with-Arduino-Zero-Form-Mini/32890639630.html |
| rpi | UNSTABLE | RaspberryPi | https://www.raspberrypi.org/ |
| clock3 | UNSTABLE | My desk clock, ADA3010 with peripherals | https://www.adafruit.com/product/3010 |
