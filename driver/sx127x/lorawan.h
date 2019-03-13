#ifndef LORAWAN_H
#define LORAWAN_H

#define LORAWAN_SYNC_WORD 0x34

enum lorawan_bandwidth {
	BW7800, BW10400, BW15600, BW20800, BW31250, BW41700, BW62500, BW125000,
	BW250000, BW500000
};

enum lorawan_coding_rate {
	CR4_5 = 1,
	CR4_6 = 2,
	CR4_7 = 3,
	CR4_8 = 4,
};

struct lorawan_data_rate {
	uint8_t sf;
	enum lorawan_bandwidth bw;
};
typedef struct lorawan_data_rate lorawan_data_rate_t;

struct lorawan_channel {
	uint32_t freq; // Hz
	enum lorawan_coding_rate cr;
	uint8_t min_dr;
	uint8_t max_dr;
};
typedef struct lorawan_channel lorawan_channel_t;

#endif // LORAWAN_H
