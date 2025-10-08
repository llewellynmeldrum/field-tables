.PHONY: run-c89 build-c89 run-c99 build-c99 run-c11 build-c11 run-c17 build-c17 run-c23 build-c23 run build clean 

CC:= clang
CFLAGS:= -Iinclude

SRC:=$(wildcard src/*.c)
PROG:= table

#DEFAULT 
run: run-c23
build: build-c23



# CLEAN
#
clean: 
	@rm -rf $(PROG_C89) $(PROG_C99) $(PROG_C11) $(PROG_C17) $(PROG_C23) $(PROG)

# C89
CFLAGS_C89:= $(CFLAGS) -std=c89
PROG_C89:= $(PROG)-c89

build-c89: 
	$(CC) $(CFLAGS_C89) $(SRC) -o $(PROG_C89)

run-c89: build-c89
	./$(PROG_C89) $(ARGS)


# C99
CFLAGS_C99:= $(CFLAGS) -std=c99
PROG_C99:= $(PROG)-c99

build-c99: 
	$(CC) $(CFLAGS_C99) $(SRC) -o $(PROG_C99)

run-c99: build-c99
	./$(PROG_C99) $(ARGS)

# C11
CFLAGS_C11:= $(CFLAGS) -std=c11
PROG_C11:= $(PROG)-c11

build-c11: 
	$(CC) $(CFLAGS_C11) $(SRC) -o $(PROG_C11)

run-c11: build-c11
	./$(PROG_C11) $(ARGS)

# C17
CFLAGS_C17:= $(CFLAGS) -std=c17
PROG_C17:= $(PROG)-c17

build-c17: 
	$(CC) $(CFLAGS_C17) $(SRC) -o $(PROG_C17)

run-c17: build-c17
	./$(PROG_C17) $(ARGS)

# C23
CFLAGS_C23:= $(CFLAGS) -std=c2x
PROG_C23:= $(PROG)-c23

build-c23: 
	$(CC) $(CFLAGS_C23) $(SRC) -o $(PROG_C23)

run-c23: build-c23
	./$(PROG_C23) $(ARGS)

