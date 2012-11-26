#TODO: Move this to root
CFLAGS=$(shell sdl-config --cflags)
LDFLAGS=$(shell sdl-config --libs) -lGL -lGLU

TARGET=bin/sdltest

all : $(TARGET)

vpath %.c sdltest
vpath %.h sdltest

$(TARGET): $(BINDIR) main.o
	$(shell mkdir bin)
	$(CC) -o $@ $^ $(LDFLAGS) 

.PHONY : clean

clean:
	$(RM) *.o
	$(RM) $(TARGET)
