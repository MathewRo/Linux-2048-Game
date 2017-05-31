CC =g++
CFLAGS = -g -Wall
TARGET = 2048

all: $(TARGET)
$(TARGET): $(TARGET).cpp
	     $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean: 
	$(RM) $(TARGET)
