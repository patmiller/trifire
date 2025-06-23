SRC =  \
	bmp.c \
	bricks.c \
	coin.c \
	main.c \
	penrose.c \
	play.c \
	render.c \
	sha1.c \
	state.c
OBJ = ${SRC:.c=.o}
HEADERS = ${shell ls ${SRC:.c=.h}}
OTHER = \
	Makefile \
	README.md \
	state.h \
	gameplay.bmp

.PHONY: run
run: trifire init
	./trifire 0 s
	./trifire 1 s
	./trifire 2 r
	./trifire 3 r

.PHONY: init
init: trifire
	./trifire -1 i

.PHONY: clean
clean:
	rm -f trifire output.bmp ${OBJ} *~ state-*.json

.PHONY: headers
	grep '#include "' ${SRC} | tr ' ' '\012' | sort -u | grep -v '#include' | tr -d '"'

.PHONY: gitadd
gitadd:
	git add ${OTHER} ${SRC} ${HEADERS}
	@git status

bmp.o: bmp.c bmp.h
bricks.o: bricks.c bricks.h
coin.o: coin.c coin.h
main.o: main.c coin.h bmp.h bricks.h state.h
penrose.o: penrose.c penrose.h
play.o: play.c play.h sha1.h state.h render.h penrose.h
render.o: render.c render.h
sha1.o: sha1.c sha1.h

trifire: $(OBJ)
	$(CC) -o trifire $(OBJ)
