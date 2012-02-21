PROG=	dialog
SRCS=	dialog.c 

CFLAGS+= -I/usr/local/include -Wall -pedantic

LDADD=	-ldialog -L/usr/local/lib/ -liberty

.include <bsd.prog.mk>
