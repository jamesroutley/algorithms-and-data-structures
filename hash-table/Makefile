.PHONY: clean

CFLAGS = -Wall -Werror -g

build: clean
	${CC} ${CFLAGS} -o hash_table hash_table.c xmalloc.c

clean:
	rm -f hash_table hash_table_test
	rm -rf *.dSYM

test: clean
	${CC} ${CFLAGS} -o hash_table_test hash_table.c hash_table_test.c xmalloc.c
	./hash_table_test
