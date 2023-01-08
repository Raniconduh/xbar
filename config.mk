PREFIX=	/usr/local

CFLAGS=	-I/usr/local/include -std=c99 -Wall -Wextra -pedantic
LDFLAGS= -L/usr/local/lib -lX11
