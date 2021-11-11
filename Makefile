#* ************************************************************************** *#
#*                                                                            *#
#*                                                                            *#
#*   Makefile                                                                 *#
#*                                                                            *#
#*   By: yhetman <yhetman@student.unit.ua>                                    *#
#*                                                                            *#
#*   Created: 2021/11/09 10:57:52 by yhetman                                  *#
#*   Updated: 2021/11/09 10:57:54 by yhetman                                  *#
#*                                                                            *#
#* ************************************************************************** *#

SRCS = const.c kupyna.c test_collision.c

SRCS_DIR = src/

OBJ_DIR = obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

OBJ_DIR = obj/

LIB_NAME = libKUPYNA.a

KUPYNA = KUPYNA

FLAGS = -I includes -Wall -Wextra -Werror

DEBUG_FLAGS = -g3 -fsanitize=address

KUPYNA_MAIN = src/main.c

all: make_obj_dir $(KUPYNA)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	gcc $(FLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	ar -rv $(LIB_NAME) $^
	ranlib $(LIB_NAME)

$(KUPYNA): $(LIB_NAME) $(KUPYNA_MAIN)
	gcc $(FLAGS) $(KUPYNA_MAIN) $(LIB_NAME) -o $(KUPYNA)

make_obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(KUPYNA)
	rm -f $(LIB_NAME)

re: fclean all

.PHONY: all clean flcean re debug