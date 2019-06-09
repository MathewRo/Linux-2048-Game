CC =gcc
CFLAGS = -g -Wall
TARGET = 2048

all: $(TARGET)
$(TARGET): $(TARGET).c
	     $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean: 
	$(RM) $(TARGET)
