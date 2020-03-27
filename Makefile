CFLAGS = -Wall -Wextra -Ofast
LIBS = -lX11
INSTALLDIR = /usr/bin

all: colorpicker

colorpicker: colorpicker.c
	$(CC) $^ $(CFLAGS) $(LIBS) -o $@
	
install: colorpicker
	cp -f $< $(INSTALLDIR)/$<

clean:
	rm -f colorpicker
