CFLAGS = `sdl2-config --cflags`  -O2 -Wall -Wextra -pedantic -Wfloat-equal
CFWIN = -O2 -Wall -Wextra -pedantic -Wfloat-equal
TARGET = sdl
TARGET2 = sdlwin
WIN = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -w
SOURCES = sdltest.c parser.c
INCS = turtlebay.h
LIBS =  `sdl2-config --libs` -lSDL2_ttf -lm
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

$(TARGET2): $(SOURCES)
		$(CC) $(SOURCES) $(INCS) $(WIN) -o $(TARGET2) $(CFWIN) -lm

clean:
	rm -f $(TARGET)
	del -f $(TARGET2)

run: all
	./$(TARGET)
