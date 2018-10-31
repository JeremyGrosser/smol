#include <platform/gpio.h>
#include <platform/i2c.h>
#include <platform.h>
#include "ssd1306.h"
#include "font.h"

static void ssd1306_reset(ssd1306_t *dev) {
	gpio_write(dev->reset, 0);
	platform_delay(10);
	gpio_write(dev->reset, 1);
	platform_delay(10);
}

static void ssd1306_cmd(ssd1306_t *dev, uint8_t cmd) {
	i2c_write(dev->i2c, dev->address, &cmd, 1);
}

int ssd1306_init(ssd1306_t *dev) {
	gpio_setup(dev->reset);
	ssd1306_reset(dev);

	i2c_setup(dev->i2c);

	// 1/32 Duty (0x0F~0x3F)
	ssd1306_cmd(dev, SSD1306_SETMULTIPLEX);
	ssd1306_cmd(dev, 0x1F);

	// Shift Mapping RAM Counter (0x00~0x3F)
	ssd1306_cmd(dev, SSD1306_SETDISPLAYOFFSET);
	ssd1306_cmd(dev, 0x00);

	// Set Mapping RAM Display Start Line (0x00~0x3F)
	ssd1306_cmd(dev, SSD1306_SETSTARTLINE);

	// Set Column Address 0 Mapped to SEG0
	ssd1306_cmd(dev, SSD1306_SEGREMAP);

	// Set COM/Row Scan Scan from COM63 to 0
	ssd1306_cmd(dev, SSD1306_COMSCANDEC);

	// Set COM Pins hardware configuration
	ssd1306_cmd(dev, SSD1306_SETCOMPINS);
	ssd1306_cmd(dev, 0x02);

	ssd1306_cmd(dev, SSD1306_SETCONTRAST);
	ssd1306_cmd(dev, 0x8F);

	// Disable Entire display On
	ssd1306_cmd(dev, SSD1306_DISPLAYON);
	ssd1306_cmd(dev, SSD1306_NORMALDISPLAY);

	// Set Display Clock Divide Ratio / Oscillator Frequency (Default => 0x80)
	ssd1306_cmd(dev, SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_cmd(dev, 0x80);

	// Enable charge pump regulator
	ssd1306_cmd(dev, SSD1306_SETPRECHARGE);
	ssd1306_cmd(dev, 0x14);

	// Set VCOMH Deselect Level
	ssd1306_cmd(dev, SSD1306_SETVCOMDETECT);
	ssd1306_cmd(dev, 0x40); // Default => 0x20 (0.77*VCC)

	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	ssd1306_cmd(dev, SSD1306_SETPRECHARGE);
	ssd1306_cmd(dev, 0xF1);

	ssd1306_cmd(dev, SSD1306_DISPLAYON);

	ssd1306_cmd(dev, SSD1306_SWITCHCAPVCC);
	ssd1306_cmd(dev, 0x3C);

	return 0;
}

int ssd1306_write(ssd1306_t *dev, char *msg, size_t len) {
	uint8_t *cp;
	char c;
	int i;

	for(i = 0; i < len; i++) {
		c = msg[i];
		if(c < 0x7F) {
			cp = font_table[c - 32];
			i2c_write(dev->i2c, dev->address, (cp + 1), cp[0]);
			i2c_write(dev->i2c, dev->address, 0x00, 1);
		}
	}

	return 0;
}
