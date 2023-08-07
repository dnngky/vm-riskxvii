TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -Wall -Wvla -Werror -O0 -g -std=c11
ASAN_FLAGS = -fsanitize=address
SRC1       = vm_riskxvii.c
SRC2	   = stdvmmem.c
SRC3	   = stdvmparse.c
SRC4	   = stdvmop.c
OBJ1       = $(SRC1:.c=.o)
OBJ2	   = $(SRC2:.c=.o)
OBJ2	   = $(SRC3:.c=.o)
OBJ2	   = $(SRC4:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -Os -s $(SRC1) $(SRC2) $(SRC3) $(SRC4) -o $@

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(SRC1) $(SRC2) $(SRC3) $(SRC4)

debug:
	$(CC) $(ASAN_FLAGS) -o $(TARGET)-debug $(SRC1) $(SRC2) $(SRC3) $(SRC4)

summary:
	cd programs && cat README.md

run:
	bash run_tests.sh

clean:
	rm -f *.o *.obj $(TARGET) $(TARGET)-debug
