/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                                                   */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/09 10:57:22 by yhetman                                  */
/*   Updated: 2021/11/11 17:23:46 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kupyna.h"

extern t_kupyna  kupyna_configs[3];


static int
get_flags(int argc, char ** argv, FILE **input, \
	size_t * filesize, bool *collision, t_kupyna_enum  *version)
{
    int 	flag;
    bool    ok = false; 

    while ((flag = getopt(argc, argv, "i:s:p:h")) != -1)
        switch (flag)
        {
            case 'i':
                *input = fopen(optarg, "r+");
                fseek(*input, 0L, SEEK_END);
                *filesize = ftell(*input);
                fseek(*input, 0L, SEEK_SET);
                ok = true;
                break;
            case 's':
                if (strcasecmp(optarg, "kupyna256") == 0)
                    *version = kupyna256;
                else if (strcasecmp(optarg, "kupyna384") == 0)
                    *version = kupyna384;
                else if (strcasecmp(optarg, "kupyna512") == 0)
                    *version = kupyna512;
                else
                    printf("%s wrong standart name, looks -h for supported standarts.\n", optarg);
                    return -1;
                break;
            case 'c':
                *collision = true;
                ok = true;
                break;
            case 'h':
                printf("Usage:  ./kupyna -i ./input/file/path [flags] \n\
                Usage to test for collision:  ./SHA256 -c\n\
Flags:\n\
    -i  --  specify input file,\n\
    -s  --  standart name. Allowed options: kupyna256, kupyna384, kupyna512,\n\
    -c	--	start collision testing.\n\
    -h  --  display help message.\n");
                return 1;
        }
    if (!ok)
        return 1;
    return 0;
}


void
add_zero_block(t_kupyna *kupyna, uint64_t * message, uint64_t size)
{
    size_t			zero_to_add,
    				i;
    unsigned char	*m;

    m = (unsigned char *)message;
    i = (size % kupyna->block) / 8;
    zero_to_add = kupyna->block / 8 - i - 12;
    m[i] = 0x80;
    memset(m + i + 1, 0, zero_to_add);

    for (i = 0; i < 12; i++)
        *(m + kupyna->block_bytes - i - 1) =  (i < UINT64_TBYTES) ? \
    ((unsigned char *)&size)[i] : 0;
}



int
main(int argc, char **argv)
{
	FILE            *input;
    size_t          filesize,
                    blocks,
                    read_blocks;
    bool            collision = false;
    int				i;
    t_kupyna_enum   version = kupyna256;
    t_kupyna        *kupyna;
    unsigned char 	*buffer,
    				*writer;
    

    if (argc < 2)
    {
        printf("Error! Wrong number of arguments, check -h flag.\n");
        exit(1);
    }

    if ((get_flags(argc, argv, &input, &filesize, &collision, &version) != 0))
    	return 1;
    
    kupyna = &kupyna_configs[version];

    if (collision)
    {
        find_collision(kupyna);
        return 0;
    }

    blocks = (filesize / kupyna->block_bytes + 3) * kupyna->block_bytes;

    buffer = (unsigned char *)malloc(sizeof(unsigned char) * blocks);
	
	writer = (unsigned char *)malloc(sizeof(unsigned char) * kupyna->diggest + 1);
    if ((blocks = fread(buffer, sizeof(unsigned char), blocks, input)) != 0)
    {
        read_blocks = blocks / kupyna->block_bytes + 1;

        add_zero_block(kupyna, (uint64_t *)(buffer + blocks), 8 * blocks);

        kupyna_hash(kupyna, (uint64_t *)buffer, read_blocks, (uint64_t *)writer);
		for (i = 0; i < (int)(kupyna->diggest/UINT64_TBYTES); i++)
            if (printf("%16lx\t", ((uint64_t *)writer)[i]) == 0)
            {
                printf("Error occured while writting to the output.\n");
                return -1;
            }
        printf("\n");
    }
    else
        printf("Error occured while reading the file.\n");
    
    free(buffer);
    free(writer);
    fclose(input);
	
	return 0;
}
