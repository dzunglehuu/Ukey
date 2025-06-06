include config.mk

SRC = ukey.c util.c mnpl.c
OBJ = ${SRC:.c=.o}

all: options ukey

options:
	@echo ukey build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

ukey: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ukey ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ukey ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/ukey

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/ukey

.PHONY: all options clean dist install uninstall
