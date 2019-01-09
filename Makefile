CC = gcc

TARGET_M = master
TARGET_S = slave
DIR_INC = ./fifo ./log ./msgQueue
OBJECT = ./fifo/*.c ./log/*.c ./msgQueue/*.c

CFLAGS = -I$(DIR_INC) -O2
LDFLAGS = -lpthread

all: $(TARGET_M) $(TARGET_S)
$(TARGET_M): $(OBJECT) test/master.c
	@echo "Build $@..."
	$(CC) -o $@ $^

$(TARGET_S): $(OBJECT) test/slave.c
	@echo "Build $@..."
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm $(TARGET_M) $(TARGET_S)

.PHONY:clean
