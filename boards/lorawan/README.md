# Write the arduino zero bootloader with J-Link

	openocd -f openocd.cfg -c 'program samd21_sam_ba.bin verify reset'

# Write the application using the bootloader
Double-tap the reset button on the board to get into bootloader mode.

	bossac -e -w -v -R -b smol-m0-mini.bin


