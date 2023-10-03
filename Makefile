VERSION = v0.6

###########################################################################
# DATAPATH is where XRoads dumps it's data, and BINPATH is where it puts
# the executable
###########################################################################

DATAPATH = ./#/usr/games/xroads/
BINPATH = /usr/X11R6/bin/
# I imagine you would use this BINPATH for Solaris:
#BINPATH = /usr/openwin/bin/

###########################################################################
# Add any flags your C compiler may want... Such as -O3 for extra
# optimizations. Check the manual for your compiler.
###########################################################################

#CFLAGS = -fomit-frame-pointer -O2 -Wall -DXRMPATH=\"$(DATAPATH)\"
#CFLAGS = -O6 -mpentium -g -Wall -DXRMPATH=\"$(DATAPATH)\"
CFLAGS = -O6  -g -Wall -DXRMPATH=\"$(DATAPATH)\"

###########################################################################
# Links necessary for X11                                                 
# Note: I have not been able to get this program to compile for AccelX, if
# you have a way to fix that, please tell me.. I assume it has something  
# to do with this line... (All the machines I have access to use XFree86, 
# so I don't know about other X servers like MetroX... please inform me if
# you have a way to work it on those platforms)
#
# If there is no appropriate line for your computer, you may have to
# improvise the -L directory should be wherever libX11.so is located, and
# the -I directory (assuming it's not already in your linker's path)
# should be the directory with X11/Xlib.h
###########################################################################

### Linux machines use this line:
LINK = -L/usr/X11R6/lib -I/usr/X11R6/include -lX11 -lm

### This line should work on Sun machines:
#LINK = -L/usr/openwin/lib -I/usr/openwin/include -lX11 -lm

###########################################################################
# Your GNU programs. These defaults should work fine
###########################################################################

# Your C compiler
#CC = egcs
CC = gcc

# Your tar
TAR = tar

# Your compression program (generally gzip, compress, bzip2, etc) and any
# flags you wish to send to it (eg: "gzip -9")
ZIPR = gzip

#########################################################################
############################ Rules for Targets ##########################
#########################################################################

## Note: Don't change anythign below here unless you need to for some 
## reason... you'll probably just mess it all up :P

OBJECTS = randommaze.o \
          chardata.o \
          tiledata.o \
          fileops.o \
          ai.o \
          screens.o \
          xrtext.o \
          xroads.o

## "all" of course builds XRoads.
all: $(OBJECTS)
	$(CC) $(FLAGS) -o xroads $(OBJECTS) $(LINK)

install: all
	rm -fr $(DATAPATH)
	mkdir $(DATAPATH)
	cp -f *.xrm $(DATAPATH)
	cp -f README $(DATAPATH)
	cp -f xroads $(BINPATH)

uninstall:
	rm -fr $(DATAPATH)
	rm -fr $(BINPATH)xroads

## "clean" cleans up all the junk and leaves the uncompiled code alone
clean:
	rm -f xroads *.o core a.out *~

## "almost-clean" cleans up spare parts, but leaves the binary
almost-clean:

	rm -f *.o core a.out

## "distribution" wraps everything up into a neat little tarball suitable
## to upload/email/ftp/etc. It even creates a nice little directory
## structure for when people untar-gz it.. how about that! (Note: it has
## to call "clean" first to get rid of any ./xroads file so it can make
## the directory... this will be fixed later (./xroads0.7/ or something
## for a directory name)
distribution:
	mkdir xroads-$(VERSION)
	cp *.c xroads-$(VERSION)
	cp *.h xroads-$(VERSION)
	cp Makefile xroads-$(VERSION)
	cp BUGS xroads-$(VERSION)
	cp README xroads-$(VERSION)
	cp CHANGELOG xroads-$(VERSION)
	cp TODO xroads-$(VERSION)
	cp -r font xroads-$(VERSION)
	cp map??.xrm xroads-$(VERSION)
	$(TAR) -cvf xroads-$(VERSION).tar xroads-$(VERSION)/
	$(ZIPR) xroads-$(VERSION).tar
	rm -rf ./xroads-$(VERSION)

## "dist" is shorter than "distribution". I'm lazy.
dist: distribution
