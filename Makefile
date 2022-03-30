CC=gcc 
CFLAGS=-Wall -Werror -g -std=gnu99
LIBS=-lpthread
INCLUDE_HEADERS_DIRECTORY=-Iheaders
INCLUDE_CUNIT = -I$(HOME)/local/include
LINK_CUNIT = -L$(HOME)/local/lib

fec: main.o  ./src/block.o  ./src/message.o  ./src/system.o  ./src/tinymt32.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f src/*.o
	@rm -f tests/*.o
	@rm -f fec
	@rm -f ./bin/*
	@rm -f ./*.o
	@rm -f ./testing

#new: src/system.o src/tinymt32.o
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) -o ./bin/$@ $^ -g
#	./bin/$@

test_tinymn32: ./tests/test_tinymt32.o ./src/tinymt32.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/$@ $^ -lcunit
	./bin/$@

test_system:  ./src/tinymt32.o tests/test_system.o src/system.o 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/$@ $^ -lcunit
	./bin/$@

tests: ./tests/testRun.o     ./tests/test_tinymt32.o ./src/tinymt32.o       tests/test_system.o src/system.o     tests/test_block.o  src/block.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./testing $^ -lcunit
	./testing

check:
	@cppcheck ./src/system.c
	@cppcheck ./src/block.c
	@cppcheck ./src/message.c
	@cppcheck ./src/tinymt32.c
	@cppcheck ./main.c
	@make tests
	@make
	@valgrind ./testing
	@valgrind ./fec ./binary_exemple -f here.txt
	@make clean

run:  ./src/block.o  ./src/system.o ./src/message.o  ./src/tinymt32.o
	$(CC) $(CCFLAGS) $(INCLUDE_HEADERS_DIRECTORY) -o open $^
	./open

.PHONY: clean tests
