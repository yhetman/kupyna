/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kupyna.h                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/09 10:59:24 by yhetman                                  */
/*   Updated: 2021/11/09 10:59:25 by yhetman                                  */
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

typedef struct 		s_kupyna
{
	char			standart_name[10];
	size_t 			block;
	size_t 			block_bytes;
	size_t 			uint64_bytesize;
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

t_kupyna			kupyna_configs[3] = {
	{"kupyna256", 512,  64,  8,  32, 10, 8 , 8 },
	{"kupyna384", 1024, 128, 16, 48, 14, 16, 8 },
	{"kupyna512", 1024, 128, 16, 64, 14, 16, 8 }
};


# define UINT64_TBYTES sizeof(uint64_t)

#endif
