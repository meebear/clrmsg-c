all:
	gcc -Werror -Wall -g -o test test.c clrmsg.c -D__STANDALONE_TEST__
