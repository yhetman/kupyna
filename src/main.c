/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                                                   */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/09 10:57:22 by yhetman                                  */
/*   Updated: 2021/11/09 10:57:23 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kupyna.h"

static int
get_flags(int argc, char ** argv, FILE **input, \
	size_t * filesize, int * zero, t_kupyna_enum  *version)
{
    int 	flag;

    while ((flag = getopt(argc, argv, "i:s:p:ch")) != -1)
        switch (flag)
        {
            case 'i':
                *input = fopen(optarg, "r+");
                fseek(*input, 0L, SEEK_END);
                *filesize = ftell(*input);
                fseek(*input, 0L, SEEK_SET);
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
            case 'p':
                *zero = atoi(optarg);
                break;
            case 'c':
            	// find_collision(*version)
            	break;
            case 'h':
                printf("Usage:  ./kupyna -i ./input/file/path [flags] \n\n\
Flags:\n\
    -i  --  specify input file,\n\
    -s  --  standart name. Allowed options: kupyna256, kupyna384, kupyna512,\n\
    -p  --  [size] - start proof of work process using size zero in hash result.\n\
    -c	--	start collision testing.\n\
    -h  --  display help message.\n");
                return 1;
        }
    return 0;
}


static void
add_zero_block(t_kupyna *kupyna, unsigned char * message, uint64_t size)
{
    size_t		zero_to_add,
    			i;
    
    i = (size % kupyna->block) / 8;
    zero_to_add = kupyna->block / 8 - i - 12;
    message[i] = 0x80;
    memset(message + i + 1, 0, zero_to_add);

    for (i = 0; i < 12; i++)
        *(message + kupyna->block_bytes - i - 1) =  (i < UINT64_TBYTES) ? \
    ((unsigned char *)&size)[i] : 0;
}



int
main(int argc, char **argv)
{
	FILE            *input;
    size_t          filesize,
                    blocks;
    int             zero = -1,
    				i;
    t_kupyna_enum   version = kupyna256;
    t_kupyna        *kupyna;
    unsigned char 	*buffer,
    				*writer;
    

    if (argc < 2)
    {
        printf("Error! Wrong number of arguments, check -h flag.\n");
        exit(1);
    }
    if ((get_flags(argc, argv, &input, &filesize, &zero, &version) != 0))
    	return 1;

    kupyna = &kupyna_configs[version];

    blocks = (filesize / kupyna->block_bytes + 3) * kupyna->block_bytes;

    buffer = (unsigned char *)malloc(sizeof(unsigned char) * blocks);
    writer = (unsigned char *)malloc(sizeof(unsigned char) * kupyna->diggest + 1);

    if ((blocks = fread(buffer, sizeof(unsigned char), blocks, input)) != 0)
    {
        add_zero_block(kupyna, (unsigned char *)(buffer + blocks), 8 * blocks);

		for (i = 0; i < (int)(kupyna->diggest/UINT64_TBYTES); i++)
            if (printf("%16lx", ((uint64_t *)writer)[i]) == 0)
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
