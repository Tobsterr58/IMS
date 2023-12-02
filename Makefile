CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lsimlib -lm

TARGET = proj

all: $(TARGET)

$(TARGET): proj.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

proj.o: proj.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o
