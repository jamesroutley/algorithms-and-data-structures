.PHONY: clean

CFLAGS = -Wall -Werror -Wextra -g

build: clean
	${CC} ${CFLAGS} -shared -o build/hash_table.so -fPIC src/hash_table.c src/xmalloc.c src/prime.c

build-test: clean
	${CC} ${CFLAGS} -o build/hash_table_test src/hash_table.c test/hash_table_test.c src/xmalloc.c src/prime.c
	${CC} ${CFLAGS} -o build/prime_test test/prime_test.c src/prime.c

clean:
	rm -rf build/
	mkdir -p build/
	$(MAKE) -C analytics clean

test: build-test 
	./build/hash_table_test
	./build/prime_test
