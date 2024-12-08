/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
#include "ChrFont0.h"

void show_ball(char field[][10], int playerCoordinate1, int playerCoordinate2);
void play();
int btn_check_0();
int btn_check_1();
int btn_check_3();
void led_set(int data);
void led_blink();
void lcd_init();
void lcd_putc(int y, int x, int c);
void lcd_sync_vbuf();
void lcd_clear_vbuf();
void shot_set(int y, int inputNumber);
int get_shot_position1(int y);
int get_shot_position2(int y);
void shot_move1(int y);
void shot_move2(int y);
void player1_move();
void player2_move();
void score1_add(int shotNumber);
void score2_add(int shotNumber);
int kypd_scand();
int kypd_scana();

/* LCD related function prototypes */
void lcd_wait(int n);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_pwr_on();
void lcd_set_vbuf_pixel(int row, int col, int r, int g, int b);
void lcd_puts(int y, int x, char *str);


#define INIT    0
#define OPENING 1
#define PLAY    2
#define ENDING  3

int state = INIT, pos = 0;

int score1;
int score2;
int playerCoordinate1;
int playerCoordinate2;
char field[8][10];
char player1Field[10];
char player2Field[10];
int shotCount1;
int shotCount2;



/* interrupt_handler() is called every 100msec */
void interrupt_handler() {
        static int cnt;
        if (state == INIT) {
        } else if (state == OPENING) {
                cnt = 0;
        } else if (state == PLAY) {
                // /* Display a ball */
                // pos = (cnt < 12) ? cnt : 23 - cnt;
                // show_ball(pos);
                // if (++cnt >= 24) {
                //         cnt = 0;
                // }
                show_ball(field, playerCoordinate1, playerCoordinate2);
                get_shot_position1(1);
                shot_move1(1);
                get_shot_position2(2);
                shot_move2(2);
        } else if (state == ENDING) {
        }
        lcd_sync_vbuf();
}


void init() {
        score1 = 0;
        score2 = 0;
        playerCoordinate1 = 0;
        playerCoordinate2 = 0;
        shotCount1 = 0;
        shotCount2 = 0;

        for(int y = 0; y < 8; y++) {
                for(int x = 0; x < 10; x++) {
                        field[y][x] = '0';
                        player1Field[y] = '0';
                        player2Field[y] = '0';
                }
        }
}

void shot_set(int y, int inputNumber) {
        if (inputNumber <= 15) {
                if(shotCount1 < 3) {
                        field[y][0] = inputNumber;
                        shotCount1++;
                }
        } else if (inputNumber >= 17) {
                if(shotCount2 < 3) {
                        field[y][9] = inputNumber; // kypd_scand()内であらかじめ１６足しておく。
                        shotCount2++;
                }
        }
}


int get_shot_position1(int y) {
        for(int i = 0; i < 7; i++) {
                if(field[y][i] >= 1 && field[y][i] <= 15) {
                        return i;
                }
        }
        
        return 0;
}

int get_shot_position2(int y) {
        for(int i = 0; i < 9; i++) {
                if(field[y][i] >= 17) {
                        return i;
                }
        }
        
        return 9;
}

void shot_move1(int y) {
        if(get_shot_position1(y) == sizeof(field[y])/sizeof(field[y][0]) /* 弾が右端に到達していたら左プレイヤーの得点 */) {
                score1_add(field[y][get_shot_position1(y)]);
                field[y][get_shot_position1(y)] = '0';
        } else if(field[y][get_shot_position1(y) + 1] == '0') {
                field[y][get_shot_position1(y) + 1] = field[y][get_shot_position1(y)];
                field[y][get_shot_position1(y)] = 0;
        } else if (get_shot_position1(y) < 4 /* 左半分でぶつかったとき, 左プレイヤーの弾にする */) {
                int tmp1 = field[y][get_shot_position1(y)] - '0';
                int tmp2 = field[y][get_shot_position1(y) + 1] - '0';
                field[y][get_shot_position1(y) + 1] = (tmp1 + tmp2) % 16 + '0';
                field[y][get_shot_position1(y)] = '0';
        } else if (get_shot_position1(y) > 4 /* 右半分でぶつかったとき, 右プレイヤーの弾にする */ ) {
                int tmp1 = field[y][get_shot_position1(y)] - '0';
                int tmp2 = field[y][get_shot_position1(y) + 1] - '0';
                // 16を足すことで右プレイヤーの弾にする
                field[y][get_shot_position1(y) + 1] = (tmp1 + tmp2) % 16 + 16 + '0';
                field[y][get_shot_position1(y)] = '0';
        } else {
                unsigned char tmp = field[y][get_shot_position1(y)];
                field[y][get_shot_position1(y)] = field[y][get_shot_position1(y) + 1];
                field[y][get_shot_position1(y)] = tmp;
        }
}

void shot_move2(int y) {
        if(get_shot_position2(y) == 0 /* 弾が左端に到達していたら右プレイヤーの得点 */) {
                score2_add(field[y][get_shot_position2(y)]);
                field[y][get_shot_position2(y)] = '0';
        } else if(field[y][get_shot_position2(y) - 1] == '0') {
                field[y][get_shot_position2(y) - 1] = field[y][get_shot_position2(y)];
                field[y][get_shot_position2(y)] = 0;
        } else if (get_shot_position2(y) < 5 /* 左半分でぶつかったとき, 左プレイヤーの弾にする */) {
                int tmp1 = field[y][get_shot_position2(y)] - '0';
                int tmp2 = field[y][get_shot_position2(y) - 1] - '0';
                field[y][get_shot_position2(y) - 1] = (tmp1 + tmp2) % 16 + '0';
                field[y][get_shot_position2(y)] = '0';
        } else if (get_shot_position2(y) > 5 /* 右半分でぶつかったとき, 右プレイヤーの弾にする */ ) {
                int tmp1 = field[y][get_shot_position2(y)] - '0';
                int tmp2 = field[y][get_shot_position2(y) - 1] - '0';
                // 16を足すことで右プレイヤーの弾にする
                field[y][get_shot_position2(y) - 1] = (tmp1 + tmp2) % 16 + 16 + '0';
                field[y][get_shot_position2(y)] = '0';
        } else {
                unsigned char tmp = field[y][get_shot_position2(y)];
                field[y][get_shot_position2(y)] = field[y][get_shot_position2(y) - 1];
                field[y][get_shot_position2(y)] = tmp;
        }
}

void player1_move() {
        player1Field[playerCoordinate1] = '0';
        playerCoordinate1++;
        playerCoordinate1 %= 10;
        player1Field[playerCoordinate1] = '*';
        return;
}

void player2_move() {
        player2Field[playerCoordinate2] = '0';
        playerCoordinate2++;
        playerCoordinate2 %= 10;
        player2Field[playerCoordinate2] = '*';
        return;
}


void score1_add(int shotNumber) {
        score1 += shotNumber % 3;
        if(score1 >= 5) {
                state = ENDING;
        }
        return;
}

void score2_add(int shotNumber) {
        score2 += shotNumber % 3;
        if(score2 >= 5) {
                state = ENDING;
        }
        return;
}

int kypd_scand() {
        volatile int *iob_ptr = (int *)0xff18;
        *iob_ptr = 0x07;                /* 0111 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x1;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x4;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x7;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0x0;
        }
        *iob_ptr = 0x0b;                /* 1011 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x2;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x5;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x8;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xf;
        }
        *iob_ptr = 0x0d;                /* 1101 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x3;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x6;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x9;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xe;
        }
        *iob_ptr = 0x0e;                /* 1110 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0xa;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0xb;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0xc;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xd;
        }
        return 0;
}
int kypd_scana() {
        volatile int *iob_ptr = (int *)0xff10;
        *iob_ptr = 0x07;                /* 0111 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x1;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x4;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x7;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0x0;
        }
        *iob_ptr = 0x0b;                /* 1011 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x2;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x5;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x8;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xf;
        }
        *iob_ptr = 0x0d;                /* 1101 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0x3;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0x6;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0x9;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xe;
        }
        *iob_ptr = 0x0e;                /* 1110 */
        for (int i = 0; i < 1; i++);    /* Wait */
        if ((*iob_ptr & 0x80) == 0) {
                return 0xa;
        }
        if ((*iob_ptr & 0x40) == 0) {
                return 0xb;
        }
        if ((*iob_ptr & 0x20) == 0) {
                return 0xc;
        }
        if ((*iob_ptr & 0x10) == 0) {
                return 0xd;
        }
        return 0;
}

// void main() {
//         volatile int *led_ptr = (int *)0xff08;
//         while (1) {
// 	        *led_ptr = kypd_scan();
//                 // led_set(*led_ptr);
//         }
// }
void main() {
        init();
        playerCoordinate1 = 3;
        playerCoordinate2 = 5;
        field[1][0] = 1;
        field[2][9] = 19;

        while (1) {
                if (state == INIT) {
                        lcd_init();
                        state = OPENING;
                } else if (state == OPENING) {
                        state = PLAY;
                } else if (state == PLAY) {
                        play();
                        state = ENDING;
                } else if (state == ENDING) {
                        state = OPENING;
                }
        }
}
void play() {
        while (1) {
                // /* Button0 is pushed when the ball is in the left edge */
                // if (pos == 0 && btn_check_0()) {
                //         led_blink();    /* Blink LEDs when hit */
                // /* Button3 is pushed when the ball is in the right edge */
                // } else if (pos == 11 && btn_check_3()) {
                //         led_blink();    /* Blink LEDs when hit */
                // } else if (btn_check_1()) {
                //         break;          /* Stop the game */
                // }

                if(kypd_scana() == 0) {
                        player1_move();
                } else if(kypd_scana() == 0x1) {
                        shot_set(playerCoordinate1, 1);
                        led_set(0xff08);
                } else if(kypd_scana() == 0x2) {
                        shot_set(playerCoordinate1, 2);
                } else if(kypd_scana() == 0x3) {
                        shot_set(playerCoordinate1, 3);
                } else if(kypd_scana() == 0x4) {
                        shot_set(playerCoordinate1, 4);
                } else if(kypd_scana() == 0x5) {
                        shot_set(playerCoordinate1, 5);
                } else if(kypd_scana() == 0x6) {
                        shot_set(playerCoordinate1, 6);
                } else if(kypd_scana() == 0x7) {
                        shot_set(playerCoordinate1, 7);
                } else if(kypd_scana() == 0x8) {
                        shot_set(playerCoordinate1, 8);
                } else if(kypd_scana() == 0x9) {
                        shot_set(playerCoordinate1, 9);
                } else if(kypd_scana() == 0xa) {
                        shot_set(playerCoordinate1, 10);
                } else if(kypd_scana() == 0xb) {
                        shot_set(playerCoordinate1, 11);
                } else if(kypd_scana() == 0xc) {
                        shot_set(playerCoordinate1, 12);
                } else if(kypd_scana() == 0xd) {
                        shot_set(playerCoordinate1, 13);
                } else if(kypd_scana() == 0xe) {
                        shot_set(playerCoordinate1, 14);
                } else if(kypd_scana() == 0xf) {
                        shot_set(playerCoordinate1, 15);
                }


               if(kypd_scand() == 0) { player2_move(); } 
                else if(kypd_scand() == 0x1) { shot_set(playerCoordinate2, 17); } 
                else if(kypd_scand() == 0x2) { shot_set(playerCoordinate2, 18); } 
                else if(kypd_scand() == 0x3) { shot_set(playerCoordinate2, 19); } 
                else if(kypd_scand() == 0x4) { shot_set(playerCoordinate2, 20); } 
                else if(kypd_scand() == 0x5) { shot_set(playerCoordinate2, 21); } 
                else if(kypd_scand() == 0x6) { shot_set(playerCoordinate2, 22); } 
                else if(kypd_scand() == 0x7) { shot_set(playerCoordinate2, 23); } 
                else if(kypd_scand() == 0x8) { shot_set(playerCoordinate2, 24); } 
                else if(kypd_scand() == 0x9) { shot_set(playerCoordinate2, 25); } 
                else if(kypd_scand() == 0xa) { shot_set(playerCoordinate2, 26); } 
                else if(kypd_scand() == 0xb) { shot_set(playerCoordinate2, 27); } 
                else if(kypd_scand() == 0xc) { shot_set(playerCoordinate2, 28); } 
                else if(kypd_scand() == 0xd) { shot_set(playerCoordinate2, 29); } 
                else if(kypd_scand() == 0xe) { shot_set(playerCoordinate2, 30); } 
                else if(kypd_scand() == 0xf) { shot_set(playerCoordinate2, 31); } 
        }
}
void show_ball(char field[8][10], int playerCoordinate1, int playerCoordinate2){
    lcd_clear_vbuf(); // 画面バッファをクリア
    char hex_char;
    //field[1][0] = 1;
    //field[2][9] = 1;
    //field[1][1] = 2;
    //field[2][2] = 3;
    //field[3][3] = 17;
    //field[4][4] = 19;
    //field[5][5] = 31;
    //field[6][6] = 8;
    //field[7][7] = 14;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 10; x++) {
                // mod 16 を適用
                char mod_value = field[y][x] % 16;

                // 描画する文字を16進数表記に変換
                if(field[y][x] == '0') {
                        hex_char = ' ';
                        led_set(0x8);
                } else {
                        hex_char = (mod_value < 10) ? ('0' + mod_value) : ('A' + (mod_value - 10));
                        led_set(0x4);
                }

                // 描画
                lcd_putc(y, x + 1, hex_char);
        }
    }
    lcd_putc(playerCoordinate1, 0, ' ');
    lcd_putc(playerCoordinate2, 11, ' ');
}
/*
 * Switch functions
 */
int btn_check_0() {
        volatile int *sw_ptr = (int *)0xff04;;
        return (*sw_ptr & 0x10) ? 1 : 0;
}
int btn_check_1() {
        volatile int *sw_ptr = (int *)0xff04;;
        return (*sw_ptr & 0x20) ? 1 : 0;
}
int btn_check_3() {
        volatile int *sw_ptr = (int *)0xff04;;
        return (*sw_ptr & 0x80) ? 1 : 0;
}
/*
 * LED functions
 */
void led_set(int data) {
        volatile int *led_ptr = (int *)0xff08;
        *led_ptr = data;
}
void led_blink() {
        led_set(0xf);                           /* Turn on */
        for (int i = 0; i < 300000; i++);       /* Wait */
        led_set(0x0);                           /* Turn off */
        for (int i = 0; i < 300000; i++);       /* Wait */
        led_set(0xf);                           /* Turn on */
        for (int i = 0; i < 300000; i++);       /* Wait */
        led_set(0x0);                           /* Turn off */
}
/*
 * LCD functions
 */
unsigned char lcd_vbuf[64][96];
void lcd_wait(int n) {
	for (int i = 0; i < n; i++);
}
void lcd_cmd(unsigned char cmd) {
        volatile int *lcd_ptr = (int *)0xff0c;
        *lcd_ptr = cmd;
        lcd_wait(1000);
}
void lcd_data(unsigned char data) {
        volatile int *lcd_ptr = (int *)0xff0c;
        *lcd_ptr = 0x100 | data;
        lcd_wait(200);
}
void lcd_pwr_on() {
        volatile int *lcd_ptr = (int *)0xff0c;
        *lcd_ptr = 0x200;
        lcd_wait(700000);
}
void lcd_init() {
        lcd_pwr_on();   /* Display power ON */
        lcd_cmd(0xa0);  /* Remap & color depth */
        lcd_cmd(0x20);
        lcd_cmd(0x15);  /* Set column address */
        lcd_cmd(0);
        lcd_cmd(95);
        lcd_cmd(0x75);  /* Set row address */
        lcd_cmd(0);
        lcd_cmd(63);
        lcd_cmd(0xaf);  /* Display ON */
}
void lcd_set_vbuf_pixel(int row, int col, int r, int g, int b) {
        r >>= 5; g >>= 5; b >>= 6;
        lcd_vbuf[row][col] = ((r << 5) | (g << 2) | (b << 0)) & 0xff;
}
void lcd_clear_vbuf() {
        for (int row = 0; row < 64; row++)
                for (int col = 0; col < 96; col++)
                        lcd_vbuf[row][col] = 0;
}
void lcd_sync_vbuf() {
        for (int row = 0; row < 64; row++)
                for (int col = 0; col < 96; col++)
                        lcd_data(lcd_vbuf[row][col]);
}
void lcd_putc(int y, int x, int c) {
        for (int v = 0; v < 8; v++)
                for (int h = 0; h < 8; h++)
                        if ((font8x8[(c - 0x20) * 8 + h] >> v) & 0x01)
                                lcd_set_vbuf_pixel(y * 8 + v, x * 8 + h, 0, 255, 0);
}
void lcd_puts(int y, int x, char *str) {
        for (int i = x; i < 12; i++)
                if (str[i] < 0x20 || str[i] > 0x7f)
                        break;
                else
                        lcd_putc(y, i, str[i]);
}
