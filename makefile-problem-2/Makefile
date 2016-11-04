FILES = $(wildcard assg*.files)
TAR = $(FILES:files=tar)

%.tar : %.files
	tar -cf $@ $?

.PHONY : all clean

all : $(TAR)

clean : 
	rm -f *.tar