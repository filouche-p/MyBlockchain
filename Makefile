NAME = my_blockchain
CC = /opt/homebrew/opt/llvm/bin/clang
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -fno-omit-frame-pointer
SRCS = my_blockchain.c $(wildcard libs/src/*.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(wildcard libs/headers/*.h)
# CC = /opt/homebrew/etc/clang

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	dsymutil $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re