
BUILDTIME=$(shell date +'\"%Y-%m-%d %H:%M\"')

CC = gcc
CXX = g++
STRIP = strip
SDL_CFLAGS  := `sdl-config --cflags`

OUTPUTNAME = boot-logo

DEFINES = -DHAVE_STDINT_H -DVERSION_MIYOO
INCLUDES = -Iinclude $(SDL_CFLAGS)
EXTRA_LDFLAGS = -lmpg123 -Wl,--as-needed -Wl,--gc-sections -flto -s

CFLAGS = $(DEFINES) $(INCLUDES) $(OPT_FLAGS) -std=gnu11 
CXXFLAGS = $(DEFINES) $(INCLUDES) $(OPT_FLAGS) -std=gnu++11 
LDFLAGS = -Wl,--start-group -lSDL -lSDL_image -lpng -ljpeg -lSDL_mixer -lfreetype -lSDL_ttf -logg -lm -pthread -lz -lstdc++ $(EXTRA_LDFLAGS) -Wl,--end-group

# Redream (main engine)
OBJS =  \
	main.o
	
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $< 
	
.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
	
all: executable

executable : $(OBJS)
	$(CC) -o $(OUTPUTNAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(OBJS) $(OUTPUTNAME)
