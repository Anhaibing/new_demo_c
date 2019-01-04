ifndef CC
		CC=gcc
	endif

ifndef CFLAGS
		CFLAGS=-MMD -O2 -Wall -g
	else
		CFLAGS += -I$(DIR_INC) -O2 -g
	endif

TARGET = anbin

$(TARGET):
	$(CC) -o $@ *.c *.h

clean:
	rm -f *.o *.d $(TARGET) *.o

.PHONY: clean
