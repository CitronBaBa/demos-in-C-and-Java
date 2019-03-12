CFLAGS = `sdl2-config --cflags`  -O2 -Wall -Wextra -pedantic -Wfloat-equal
CFWIN = -O2 -Wall -Wextra -pedantic -Wfloat-equal -static-libgcc -lm
TARGET = turtlebay
TARGET2 = turtlebay_win
WIN = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -w
SOURCES = turtlebay.c parser.c main.c drawing.c rabbit.c
INCS = turtlebay.h
LIBS =  `sdl2-config --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm
CC = gcc
all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

$(TARGET2): $(SOURCES)
		$(CC) $(SOURCES) $(INCS) $(WIN) -o $(TARGET2) $(CFWIN)

clean:
	rm -f $(TARGET)
	del -f $(TARGET2)

run: all
	./$(TARGET)
