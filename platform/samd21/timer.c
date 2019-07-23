#include <platform/timer.h>
#include <platform.h>

void timer_setup(timer_t *timer) {
    Tcc *tcc;
    timer->tcc = (Tcc *)(((void *)TCC0) + (timer->num * 0x400UL));

    GCLK->CLKCTRL.reg = (
            GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TCC0_TCC1_Val + (timer->num / 2)) |
            GCLK_CLKCTRL_GEN(GCLK_GEN_MAIN) |
            GCLK_CLKCTRL_CLKEN);

    PM->APBCMASK.reg |= (PM_APBCMASK_TCC0 << timer->num);


    tcc->CTRLA.bit.ENABLE = 0;
    while(tcc->SYNCBUSY.bit.ENABLE);
    tcc->CTRLA.bit.SWRST = 1;
    while(tcc->SYNCBUSY.bit.SWRST);

    tcc->CTRLA.bit.RESOLUTION = TCC_CTRLA_RESOLUTION(TCC_CTRLA_RESOLUTION_NONE_val);

    tcc->CTRLA.bit.ENABLE = 1;
    while(tcc->SYNCBUSY.bit.ENABLE);
}
