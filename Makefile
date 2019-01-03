ifndef CC
		CC=gcc
	endif

ifndef CFLAGS
		CFLAGS=-MMD -O2 -Wall -g
	else
		CFLAGS += -I$(DIR_INC) -O2 -g
	endif

TARGET = anbin
OBJECTS = main.o slog.o

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -f *.o *.d $(TARGET) *.o

.PHONY: clean
