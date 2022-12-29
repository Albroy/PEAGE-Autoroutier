CC=gcc
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)
FALG= -W -Wall -Wextra -pedantic -ansi -std=c99 

AddressSanitizer= -fsanitize=address -fno-omit-frame-pointer

ThreadSanitizer= -fsanitize=thread -fno-omit-frame-pointer

UndefinedBehaviourSanitizer = -fsanitize=undefined \
                              -fsanitize=shift \
                              -fsanitize=shift-exponent \
                              -fsanitize=shift-base \
                              -fsanitize=integer-divide-by-zero \
                              -fsanitize=unreachable \
                              -fsanitize=vla-bound \
                              -fsanitize=null \
                              -fsanitize=return \
                              -fsanitize=bounds \
                              -fsanitize=alignment \
                              -fsanitize=object-size \
                              -fsanitize=float-cast-overflow \
                              -fsanitize=nonnull-attribute \
                              -fsanitize=returns-nonnull-attribute \
                              -fsanitize=bool \
                              -fsanitize=enum \
                              -fsanitize=vptr \
                              -fsanitize=pointer-overflow \
                              -fsanitize=builtin \
                              -fsanitize-address-use-after-scope \
                              -fno-omit-frame-pointer \
                              -fstack-protector-strong \
                              -fstack-check

main: $(OBJ)
	@$(CC) -o $@ $^ $(FLAG)
	@if [ ! -d ./exec ] || [ ! -d ./obj ]; then mkdir exec obj; fi
	@mv $@ exec/
	@mv $(OBJ) obj/


%.o: %.c *.h
	@$(CC) -o $@ -c $< $(FLAG)

clean: 
	@rm -f *.o main
	@if [ -d ./exec ] || [ -d ./obj ]; then rm -r exec obj; fi
