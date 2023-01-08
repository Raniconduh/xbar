.POSIX:

include config.mk

BIN=	xbar
SRC=	xbar.c
OBJ=	${SRC:.c=.o}

CC?=	cc

all: ${BIN}

${BIN}: ${OBJ} config.h
	${CC} ${LDFLAGS} ${OBJ} -o $@

.c.o: config.h
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${BIN} ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -Dm755 ${BIN} ${DESTDIR}${PREFIX}/bin/${BIN}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${BIN}

.PHONY: all clean install uninstall
