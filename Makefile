CC = g++
PACKAGES = libpng gl glfw3
CFLAGS = -Wall -Wno-write-strings $(shell pkg-config --cflags $(PACKAGES))
LDLIBS = $(shell pkg-config --libs $(PACKAGES))

c-intro: src/c-intro.cpp src/engine.cpp src/readpng.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)
