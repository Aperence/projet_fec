CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iheaders
INCLUDE_CUNIT = -I$(HOME)/local/include
LINK_CUNIT = -L$(HOME)/local/lib

fec: main.c     # add your other object files needed to compile your program here. !! The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)    # this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread

%.o: %.c
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f src/*.o
	@rm -f tests/*.o
	@rm -f fec
	@rm -f ./bin/*

#new: src/system.o src/tinymt32.o
#	$(CC) $(INCLUDE_HEADERS_DIRECTORY) -o ./bin/$@ $^ -g
#	./bin/$@

test_tinymn32: ./tests/test_tinymt32.o ./src/tinymt32.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/$@ $^ -lcunit
	./bin/$@

test_system:  ./src/tinymt32.o tests/test_system.o src/system.o 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/$@ $^ -lcunit
	./bin/$@

tests: ./tests/testRun.o     ./tests/test_tinymt32.o ./src/tinymt32.o       tests/test_system.o src/system.o 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/testing $^ -lcunit
	./bin/testing

check:
	cppcheck ./src/system.c
	@make tests
	valgrind ./bin/testing

.PHONY: clean tests
