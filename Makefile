CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iheaders
INCLUDE_CUNIT = -I$(HOME)/local/include
LINK_CUNIT = -L$(HOME)/local/lib

fec: main.c     # add your other object files needed to compile your program here. !! The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)    # this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread

%.o: %.c              # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f src/*.o
	@rm -f tests/*.o
	@rm -f fec
	@rm -f ./bin/*

test_system: tests/test_system.o src/system.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/$@ $^ -lcunit

tests:
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(INCLUDE_CUNIT) $(LINK_CUNIT) -o ./bin/test_tinymt32 ./tests/test_tinymt32.c  -lcunit
	./bin/test_tinymt32
	make test_system
	./bin/test_system

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests
