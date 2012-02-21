PROG=	dialog
SRCS=	dialog.c 

CFLAGS+= -I/usr/pkg/include -Wall -pedantic

LDADD=	-lndialog -lpanel -lncurses -L/usr/pkg/lib/

.include <bsd.prog.mk>

clean: requirethis
	rm -f .depend dialog.d dialog.o

requirethis:
