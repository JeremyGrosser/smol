#include "sx127x.h"
#include "sx127x_internal.h"

#include <stdio.h>

void sx127x_modem_config(sx127x_t *dev, uint8_t sf, uint8_t bw, uint8_t cr, uint8_t syncword) {
	RegModemConfig1_t mc1;
	RegModemConfig2_t mc2;
	RegModemConfig3_t mc3;

	mc1.reg = sx127x_readreg(dev, RegModemConfig1);
	mc2.reg = sx127x_readreg(dev, RegModemConfig2);
	mc3.reg = sx127x_readreg(dev, RegModemConfig3);

	mc1.bit.Bw = bw;
	mc1.bit.CodingRate = cr;
	mc2.bit.SpreadingFactor = sf;

	sx127x_writereg(dev, RegModemConfig1, mc1.reg);
	sx127x_writereg(dev, RegModemConfig2, mc2.reg);
	sx127x_writereg(dev, RegModemConfig3, mc3.reg);
	sx127x_writereg(dev, RegSyncWord, syncword);
}

/*
static void sx127x_modem_status(sx127x_t *dev) {
	RegModemStat_t status;

	status.reg = sx127x_readreg(dev, RegModemStat);
	if(status.bit.SignalDetected) {
		printf("DETECT ");
	}
	if(status.bit.SignalSync) {
		printf("SYNC ");
	}
	if(status.bit.RxOngoing) {
		printf("RX ");
	}
	if(status.bit.HeaderValid) {
		printf("HDR ");
	}
	if(status.bit.ModemClear) {
		printf("CLR ");
	}
	printf("\r\n");
}
*/

int sx127x_set_channel(sx127x_t *dev, lorawan_channel_t *channels, lorawan_data_rate_t *data_rates, uint8_t chan_num, uint8_t dr_num) {
	lorawan_channel_t chan;
	lorawan_data_rate_t dr;
	RegOpMode_t opmode;

	opmode.reg = sx127x_readreg(dev, RegOpMode);
	if(opmode.bit.Mode > MODE_STANDBY) {
		dev->errmsg = "Cannot change channel outside of SLEEP and STANDBY modes";
		return -1;
	}

	chan = channels[chan_num];
	if(dr_num < chan.min_dr || dr_num > chan.max_dr) {
		dev->errmsg = "Invalid data rate for channel";
		return -2;
	}
	dr = data_rates[dr_num];

	sx127x_set_frequency(dev, chan.freq);
	sx127x_modem_config(dev, dr.sf, dr.bw, chan.cr, LORAWAN_SYNC_WORD);

	printf("CHAN:%2u DR:%2u (SF:%2u BW:%2u CR:%2u FRF:%ld)\r\n",
			chan_num, dr_num, dr.sf, dr.bw, chan.cr, chan.freq);

	return 0;
}
