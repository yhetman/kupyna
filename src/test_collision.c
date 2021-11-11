/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   test_collision.c                                                         */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/08 20:05:03 by yhetman                                  */
/*   Updated: 2021/11/11 17:21:54 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "kupyna.h"

int
compare_kupyna(t_8bytes *content1, t_8bytes *content2)
{
    size_t  i;

    for (i = 0; i < 32; i++)
    {
        if (content1->bytes8[i] < content2->bytes8[i])
            return -1;
        else if (content1->bytes8[i] > content2->bytes8[i])
            return 1;
    }
    return 0;
}


char*
random_string(char *str, size_t size)
{
	char 	charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t 	n;

	for (n = 0; n < size; n++)
	{
		int key = rand() % (int) (sizeof charset - 1);
		str[n] = charset[key];
	}
	str[size] = '\0';
  return str;
}


t_hashes *
init_t_hash(t_kupyna *kupyna)
{	t_hashes 		*hash;
	size_t 			blocks;

    hash = (t_hashes*)malloc(sizeof(t_hashes));
	
	hash->generated_key = (char *)malloc(sizeof(char) * 32);
	hash->generated_key = random_string(hash->generated_key, 32);
	
	hash->hash.bytes8 = (uint64_t *)malloc(sizeof(unsigned char) * 32);
    
    blocks = 32 / kupyna->block_bytes + 1;

    add_zero_block(kupyna, (uint64_t*)(hash->generated_key + 32), 8 * 32);
    kupyna_hash(kupyna, (uint64_t*)hash->generated_key, blocks, (uint64_t*)hash->hash.bytes8);
    free(hash->generated_key);
    return hash;
}


int
check_hash_list(t_hashes *h, t_hashes *new)
{
	t_hashes	*tmp;

	if (h == NULL)
		return 1;
	tmp = h;
	tmp->next_hash = h->next_hash;
	while (tmp->next_hash != NULL)
	{
		if (compare_kupyna(&tmp->hash, &new->hash) == 0)
			return -1;
		tmp = tmp->next_hash;
	}
	return 1;
}


void
find_collision(t_kupyna *kupyna)
{
	t_hashes 	*hashed_keys;
	t_hashes    *new_hash;
	t_hashes	*tmp;
	int 		success_counter = 0;

	hashed_keys = init_t_hash(kupyna);
	while (true)
	{
		new_hash = init_t_hash(kupyna);

		if (check_hash_list(hashed_keys, new_hash) != -1)
		{
			tmp = new_hash;
			tmp->next_hash = new_hash->next_hash;
			hashed_keys->next_hash = tmp;
			success_counter++;
		}
		else
			break;

		if (success_counter % 100000 == 0)
			printf("Collision test in progree: current amount of succesfull tries : %i .\n", success_counter);

	}
	printf("The Algorithm made %i succesfull hashings before collision.\n", success_counter);
}


void
count_execution_time(t_kupyna *kupyna)
{
	clock_t t;

	t = clock();
	find_collision(kupyna);
	t = clock() - t;

	double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Testing for collision took %f seconds of execution \n", time_taken);
}
