/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kupyna.c                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/09 10:57:45 by yhetman                                  */
/*   Updated: 2021/11/09 10:57:46 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kupyna.h"


static unsigned char
multiply_galois_fields(unsigned char a, unsigned char b)
{
    unsigned char result,
            hbit;
    size_t  i;

    result = 0;
    hbit = 0;
    for (i = 0; i < 8; i++)
    {
        if ((b & 0x1) == 1)
            result ^= a;
        hbit = a & 0x80;
        a <<= 1;
        if (hbit == 0x80)
            a ^= 0x011d;
        b >>= 1;
    }
    return result;
}


void
mix_columns(uint64_t *state, uint8_t matrix[8][8], size_t rows)
{
	unsigned char   product = 0;
    uint64_t    	result = 0;
    int         	r, c, i;

    for (c = 0; c < (int)rows; c++, state[c] = result, result = 0)
    {
    	for (r = ROWS - 1; r >= 0; r--, product = 0)
        {
            for (i = ROWS - 1; i >= 0; i--)
                product ^= multiply_galois_fields(((unsigned char *) &state[c])[i], matrix[r][i]);
            result |= (uint64_t) product << (r * ROWS);
        }
    }
}



void
shift_rows(uint64_t * state, size_t rows)
{
    size_t  		i, shift;

    for (i = 0; i < ROWS - 1; i++, shift = i)
		SHIFT_ROWS_SUBBLOCK(shift, state, rows);
    
    shift = (rows * 64) == 512 ? i : 11;

    SHIFT_ROWS_SUBBLOCK(shift, state, rows);
}


static void
kupyna_block(uint64_t *state, uint64_t	*tmp,	\
	uint64_t *h_0, uint64_t *h, t_kupyna *kupyna)
{
	XOR_ARR(h_0, state, tmp, kupyna->state);

    memcpy(h, tmp, kupyna->block_bytes);
	
	T_XOR(h_0, kupyna->rounds, kupyna->state);
	
	T_ADD(h_0, kupyna->rounds, kupyna->state);
    
    XOR_MULTIPLE(state, state,  h_0, h, kupyna->state);
}


void
kupyna_hash(t_kupyna	*kupyna, uint64_t * message,\
	size_t size, uint64_t * output)
{
    uint64_t 	*state,
    			*h_0,
    			*h;

    state = (uint64_t *) malloc(ROWS * kupyna->state);
    h_0 = (uint64_t *) malloc(ROWS * kupyna->state);
    h = (uint64_t *) malloc(ROWS * kupyna->state);

    memset(state, 0, ROWS * kupyna->state);

    ((unsigned char *)&state[0])[0] = 0x80; 

    if (kupyna->block == 512)
        ((unsigned char *)&state[0])[0] >>= 1;

    for (int i = 0; i < (int) size; i++)
    	kupyna_block(state, (message + i * kupyna->double_block),\
			h_0, h, kupyna);

    memcpy(h_0, state, kupyna->block_bytes);
    
    T_XOR(h_0, kupyna->rounds, kupyna->state);
    XOR_ARR(state, state, h_0, kupyna->state);

    memcpy(output, state, kupyna->diggest);

    free(state);
    free(h_0);
    free(h);
}
