/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
void interrupt_handler(){}

void main() {
	//volatile int *sw_ptr = (int *)0xff04;//スイッチの状態
	volatile int *led_ptr = (int *)0xff08;//LEDの状態
	for(;;)
		*led_ptr = kypd_scana();
}


int kypd_scand() {
        volatile int *iod_ptr = (int *)0xff18;
        *iod_ptr = 0x07;                /* 0111 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iod_ptr & 0x80) == 0)
                return 0x1;
        if ((*iod_ptr & 0x40) == 0)
            return 0x4;
        if ((*iod_ptr & 0x20) == 0)
                return 0x7;
        if ((*iod_ptr & 0x10) == 0)
                return 0x0;
        *iod_ptr = 0x0b;                /* 1011 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iod_ptr & 0x80) == 0)
                return 0x2;
        if ((*iod_ptr & 0x40) == 0)
                return 0x5;
        if ((*iod_ptr & 0x20) == 0)
                return 0x8;
        if ((*iod_ptr & 0x10) == 0)
                return 0xf;
        *iod_ptr = 0x0d;                /* 1101 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iod_ptr & 0x80) == 0)
                return 0x3;
        if ((*iod_ptr & 0x40) == 0)
                return 0x6;
        if ((*iod_ptr & 0x20) == 0)
                return 0x9;
        if ((*iod_ptr & 0x10) == 0)
                return 0xe;
        *iod_ptr = 0x0e;                /* 1110 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iod_ptr & 0x80) == 0)
                return 0xa;
        if ((*iod_ptr & 0x40) == 0)
                return 0xb;
        if ((*iod_ptr & 0x20) == 0)
                return 0xc;
        if ((*iod_ptr & 0x10) == 0)
                return 0xd;
        return 0;
}

int kypd_scana() {
        volatile int *ioa_ptr = (int *)0xff10;
        *ioa_ptr = 0x07;                /* 0111 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*ioa_ptr & 0x80) == 0)
                return 0x1;
        if ((*ioa_ptr & 0x40) == 0)
            return 0x4;
        if ((*ioa_ptr & 0x20) == 0)
                return 0x7;
        if ((*ioa_ptr & 0x10) == 0)
                return 0x0;
        *ioa_ptr = 0x0b;                /* 1011 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*ioa_ptr & 0x80) == 0)
                return 0x2;
        if ((*ioa_ptr & 0x40) == 0)
                return 0x5;
        if ((*ioa_ptr & 0x20) == 0)
                return 0x8;
        if ((*ioa_ptr & 0x10) == 0)
                return 0xf;
        *ioa_ptr = 0x0d;                /* 1101 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*ioa_ptr & 0x80) == 0)
                return 0x3;
        if ((*ioa_ptr & 0x40) == 0)
                return 0x6;
        if ((*ioa_ptr & 0x20) == 0)
                return 0x9;
        if ((*ioa_ptr & 0x10) == 0)
                return 0xe;
        *ioa_ptr = 0x0e;                /* 1110 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*ioa_ptr & 0x80) == 0)
                return 0xa;
        if ((*ioa_ptr & 0x40) == 0)
                return 0xb;
        if ((*ioa_ptr & 0x20) == 0)
                return 0xc;
        if ((*ioa_ptr & 0x10) == 0)
                return 0xd;
        return 0;
}