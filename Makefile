CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lsimlib -lm

TARGET = proj

all: $(TARGET)

$(TARGET): proj.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGET)

run:
	./$(TARGET)
