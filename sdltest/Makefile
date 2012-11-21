#TODO: Move this to root
CFLAGS=$(shell sdl-config --cflags)
LDFLAGS=$(shell sdl-config --libs) -lGL -lGLU
all : sdltest

sdltest: main.o
	$(CC) -o $@ $^ $(LDFLAGS) 

.PHONY : clean

clean:
	$(RM) *.o
	$(RM) sdltest
