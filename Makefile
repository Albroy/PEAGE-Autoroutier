CC=gcc
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)
FALG= -W -Wall -pedantic -ansi -std=c99

main: $(OBJ)
	@$(CC) -o $@ $^ $(FLAG)
	@if [ ! -d ./exec ] && [ ! -d ./obj ]; then mkdir exec obj; fi
	@mv $@ exec/
	@mv $(OBJ) obj/


%.o: %.c *.h
	@$(CC) -o $@ -c $< $(FLAG)

clean: 
	@rm -f *.o main
	@if [ -d ./exec ] && [ -d ./obj ]; then rm -r exec obj; fi
