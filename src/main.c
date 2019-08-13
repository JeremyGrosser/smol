#include <platform/gpio.h>
#include <platform/i2c.h>
#include <board.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    uint8_t ch;
    int err;

    err = board_init();
    if(err != 0) {
        return 1;
    }

    gpio_t LED_ROW1 = {
        .num = 13,
        .config = {
            .direction = DIR_OUT,
            .drive = DRIVE_LOW,
            .pull = PULL_DISABLE,
        },
    };

    gpio_t LED_COL1 = {
        .num = 4,
        .config = {
            .direction = DIR_OUT,
            .drive = DRIVE_LOW,
            .pull = PULL_DISABLE,
        },
    };

    gpio_setup(&LED_ROW1);
    gpio_setup(&LED_COL1);

    gpio_write(&LED_ROW1, 1);
    gpio_write(&LED_COL1, 0);

    printf("\r\n\r\nsmol microbit ready\r\n");

    if(i2c_reg_read(&I2C, 0x0E, 0x07, &ch) > 0) {
        if(ch != 0xC4) {
            printf("mag3110: incorrect device id 0x%02X\r\n", ch);
        }else{
            printf("mag3110: identified\r\n");
        }
    }else{
        printf("mag3110: i2c read failed\r\n");
    }


    while(1) {
        if(read(STDIN_FILENO, &ch, 1) > 0) {
            if(ch == '\r') {
                write(STDOUT_FILENO, &ch, 1);
                ch = '\n';
            }
            write(STDOUT_FILENO, &ch, 1);
        }
        ch = '.';
        write(STDOUT_FILENO, &ch, 1);
        platform_delay(100);
        gpio_toggle(&LED_COL1);
        __WFI();
    }

    return 0;
}
