
/*
 * matrix.h
 *
 * Created: 26-3-2019 09:46:24
 *  Author: Tom Martens
 */ 

void twi_start(void);
int loadMatrix(void);
void twi_stop(void);
void twi_tx(unsigned char);
void clear_column(int);
void fill_column(int, int);
void clear_board(void);
void fill_board(void);
void twi_init(void);

