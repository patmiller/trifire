PYTHON=python3
MSRC =  \
	bmp.c \
	bricks.c \
	coin.c \
	penrose.c \
	play.c \
	render.c \
	sha1.c \
	state.c
SRC = ${MSRC} main.c
OBJ = ${SRC:.c=.o}
HEADERS = ${shell ls ${MSRC:.c=.h}}
OTHER = \
	Makefile \
	movie.py \
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

.PHONY: movie
movie: movie.mp4
movie.mp4: movie.py frame-1.bmp
	${PYTHON} movie.py frame-*.bmp

.PHONY: pytest
pytest:
	$(PYTHON) -m pip install .
	$(PYTHON) pytest.py rrrrslslslrrrsss

.PHONY: clean
clean:
	rm -rf trifire.egg-info build trifire output.bmp ${OBJ} *~ state-*.json frame-*.bmp movie.mp4

.PHONY: gitadd
gitadd:
	git add ${OTHER} ${SRC} ${HEADERS}
	@git status

bmp.o: bmp.c bmp.h
bricks.o: bricks.c bricks.h
coin.o: coin.c coin.h
main.o: main.c bmp.h bricks.h coin.h render.h state.h
penrose.o: penrose.c penrose.h
play.o: play.c play.h penrose.h render.h sha1.h state.h 
render.o: render.c render.h
sha1.o: sha1.c sha1.h

trifire: $(OBJ)
	$(CC) -o trifire $(OBJ)

.PHONY: install_luxcedia
install_luxcedia:
	cp trifire.html /var/www/html
	cp trifire.cgi  /usr/lib/cgi-bin/trifire
	chmod +x /usr/lib/cgi-bin/trifire
	cp trifire.bmp trifire.mp4 /usr/lib/cgi-bin
	chmod +x /usr/lib/cgi-bin/trifire.bmp
	chmod +x /usr/lib/cgi-bin/trifire.mp4
