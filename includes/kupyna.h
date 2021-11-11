/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kupyna.h                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/09 10:59:24 by yhetman                                  */
/*   Updated: 2021/11/11 17:23:29 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#ifndef KUPYNA_H
# define KUPYNA_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <time.h>


typedef struct  		s_8bytes
{
    uint64_t     		*bytes8;
}               		t_8bytes; 


typedef struct  		s_hashes
{
	char 				*generated_key;
	t_8bytes			hash;
	struct s_hashes 	*next_hash;
}						t_hashes;

typedef struct 		s_kupyna
{
	char			standart_name[10];
	size_t 			block;
	size_t 			block_bytes;
	size_t 			double_block;
	size_t 			diggest;
	size_t 			rounds;
	size_t 			state;
	size_t 			message_size;
} 					t_kupyna;

typedef enum 		e_kupyna
{
	kupyna256,
	kupyna384,
	kupyna512
}					t_kupyna_enum;

extern const unsigned char s_box[4][16][16];
extern uint8_t mix_columns_matrix[8][8];

# define ROWS 8

# define UINT64_TBYTES sizeof(uint64_t)

# define HIGH(x)	(LOW(x) >> 4)

# define LOW(x)		(x & 0xF0)

# define XOR(x, y) (x^y)

# define XOR_ARR(x, y, z, rows) {				\
	for(size_t i = 0; i < rows; i++)			\
		{(x)[i] = XOR((y)[i], (z)[i]);}}


# define XOR_KEY(state, round, rows) {			\
	for(size_t i = 0; i < rows; i++)			\
		{((unsigned char *)&(state)[i])[0] = 	\
			XOR((i << 4), round);}}


# define XOR_MULTIPLE(x, y ,z, t ,rows) {		\
	for(size_t i = 0; i < rows; i++)			\
	{(x)[i] = (y)[i] ^ (z)[i] ^ (t)[i];}}		\


# define ADD_KEY(state, round, rows) {			\
	for(size_t i = 0; i < rows; i++)			\
		{state[i] += XOR(0x00F0F0F0F0F0F0F3, 	\
			((rows - i - 1) << 4));}}


# define MACROS_SET(s, i, k) {					\
	((unsigned char *)&s[k])[i] = 				\
	((unsigned char *)&s[k - 1])[i];}


# define SHIFT_ROWS_SUBBLOCK(shift, s, rows) {	\
	for (int j = 0; j < (int)shift; j++) {		\
        unsigned char tmp = 					\
        ((unsigned char *)&s[rows - 1])[i];		\
        for (int k = rows - 1; k > 0; k--)		\
        {    	MACROS_SET(s, i, k)}			\
        ((unsigned char *)&s[0])[i] = tmp;}}



# define SUB_BYTES(state, rows) {				\
	for(size_t i = 0; i < rows; i++)			\
		{ unsigned char * tmp = 				\
			(unsigned char *)&state[i];			\
        for (size_t j= 0; j < ROWS; j++)		\
            tmp[j] = 							\
        s_box[j% 4][HIGH(tmp[j])][LOW(tmp[j])];	\
    }}


# define T_XOR(state, rounds, rows){			\
		for(size_t j = 0; j < rows; j++)		\
		{	XOR_KEY(state, j, rows)				\
			SUB_BYTES(state, rows) 				\
			shift_rows(state, rows);			\
			mix_columns(state, 					\
				mix_columns_matrix, rows);}}


# define T_ADD(state, rounds, rows){			\
		for(size_t j = 0; j < rows; j++)		\
		{	ADD_KEY(state, j, rows)				\
			SUB_BYTES(state, rows)				\
			shift_rows(state, rows); 			\
			mix_columns(state, 					\
				mix_columns_matrix, rows);}}



void		add_zero_block(t_kupyna *kupyna, 	\
			uint64_t * message, uint64_t size);


void		kupyna_hash(t_kupyna *kupyna, 		\
			uint64_t * message, size_t size,	\
			uint64_t * output);


void		shift_rows(uint64_t * state,		\
			size_t rows);


void		mix_columns(uint64_t *state, 		\
			uint8_t matrix[8][8], size_t rows);


void		count_execution_time(t_kupyna *k);

void		find_collision(t_kupyna *kupyna);

#endif
