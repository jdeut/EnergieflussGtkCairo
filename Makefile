SYS:=$(shell uname) 
PKGS=gtk+-3.0 glib-2.0 gmodule-2.0 
CFLAGS=-g3 `pkg-config --cflags $(PKGS)`
CC=gcc
CFILES=$(shell find . -iname "*.c")
OBJ=$(patsubst %.c, %.o, $(CFILES))

ifneq (, $(findstring MINGW, $(SYS)))
  LDLIBS=-L/mingw64/bin -static -lgtk-3.dll -lgdk-3.dll -lfontconfig.dll -lgdk_pixbuf-2.0.dll -lglib-2.0.dll -lgobject-2.0.dll 
else
  LDLIBS=`pkg-config --libs $(PKGS)`
endif

main: $(OBJ)
	$(CC) -o $@ $^ $(LDLIBS)

win32bundle: main
ifneq (, $(findstring MINGW, $(SYS)))
	  mkdir -p bundle
	  #ldd main.exe | perl -ne 'if (/\s(\/mingw64\/\S*dll)/) { print $$1, " " }'
	  cp `ldd main.exe | perl -ne 'if (/\s(\/mingw64\/\S*dll)/) { print $$1, " " }'` bundle
	  cp main.exe style.css ui.glade bundle
else	
endif


clean:
	rm -f $(OBJ) main
