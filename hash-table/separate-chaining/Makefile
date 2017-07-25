.PHONY: clean

CFLAGS = -Wall -Werror -Wextra -g

build: clean
	${CC} ${CFLAGS} -o build/hash_table src/main.c src/hash_table.c src/xmalloc.c

clean:
	rm -rf build/
	mkdir -p build/

test: clean
	${CC} ${CFLAGS} -o build/hash_table_test src/hash_table.c src/hash_table_test.c src/xmalloc.c
	./build/hash_table_test
