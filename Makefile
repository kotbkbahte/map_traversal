CC = gcc
CFLAGS= -Wall -g
LDFLAGS=

SRC = main.c map.c alg.c
OBJ = $(SRC:.c=.o)
EXEC = a.out

all: $(SRC) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o a.out
