CC = gcc
#CFLAGS = -Wall -Wextra -std=c99
TARGET = web-server

C_FILES := $(wildcard *.c)
OBJ_FILES  = $(patsubst %.c, %.o, $(C_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	cd plugins/ && make all && cd -
	$(CC) -L./plugins -lgenerror $^ -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -ldl $< -c

clean:
	cd plugins/ && make clean && cd -
	rm -f *.o $(TARGET)
