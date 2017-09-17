all:
	gcc -std=c99 -g -m32 -z execstack ms.c malloc.c fork_accept.c -o minesweeper
	@echo "Done"

clean:
	rm -rf *.o *dSYM minesweeper
