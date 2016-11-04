CFLAGS = -Wall -g
OFILES = mymake.o build.o

mymake: $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o mymake

%.o : %.c mymake.h
	gcc $(CFLAGS) -c $*.c

.PHONY:clean
clean:
	rm -f *.o mymake *.gcov *.gcno *.gcda
