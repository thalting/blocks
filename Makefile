PREFIX  := /usr/local
CC      := cc
CFLAGS  := -pedantic -Wall -Os

all: options blocks

options:
	@echo blocks build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

blocks: blocks.c blocks.def.h blocks.h
	${CC} -o blocks blocks.c ${CFLAGS} ${LDFLAGS}

blocks.h:
	cp blocks.def.h $@

clean:
	rm -f *.o blocks

install: blocks
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f blocks ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/blocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/blocks

.PHONY: all options clean install uninstall
