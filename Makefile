NAME = my_blockchain
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
SRCS = my_blockchain.c $(wildcard libs/src/*.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(wildcard libs/headers/*.h)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re