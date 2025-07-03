# Copyright 2025 Patrick Miller
PYTHON=python3
CORE_SRC = \
	state.c \
	play.c \
	sha1.c \
	trajectory.c
RENDER_SRC = \
	ball.c \
	bmp.c \
	bricks.c \
	coin.c \
	explosion.c \
	penrose.c \
	penrosecoin.c \
	render.c
SRC = ${CORE_SRC} ${RENDER_SRC}
OBJ = ${SRC:.c=.o}

EXECUTABLES = \
	json2bmp \
	json2json \
	playcheck \
	showbmp

.PHONY: jsonrun
jsonrun: json2json
	./json2json s ''

.PHONY: sources
sources:
	@echo $(SRC)

.PHONY: all
all: $(EXECUTABLES) check_setup trifire.so

json2json: json2json.o state.o play.o sha1.o trajectory.o
	$(CC) -o json2json $^

json2bmp: json2bmp.o bricks.o render.o ball.o penrose.o state.o play.o sha1.o trajectory.o bmp.o penrosecoin.o explosion.o
	$(CC) -o json2bmp $^

playcheck: playcheck.o monolith.o
	$(CC) -o playcheck $^

monolith.c: ${SRC:.c=.h} $(SRC)
	cat $^ | grep -v '#pragma once' | grep -v '#include "' > monolith.c

showbmp: showbmp.o bricks.o render.o ball.o penrose.o state.o play.o sha1.o trajectory.o bmp.o penrosecoin.o explosion.o
	$(CC) -o showbmp $^

.PHONY: tests
ANSR0='{"turn": 1, "points": 0, "tri_x": 0, "rotation": 1, "cannon_t": 0, "cannon_offset": 0, "coin_x": 409, "coin_y": 164}'
tests: json2json
	./json2json s '' $(ANSR0)

.PHONY: check_setup
check_setup:
	@for src in $(SRC); do \
	    if grep -q "$$src" setup.py; then \
	        echo "$$src found in setup.py"; \
	    else \
	        echo "$$src NOT found in setup.py"; \
		exit 1; \
	    fi; \
	done

.PHONY: pytest
pytest: trifire.so setup.py ${SRC} ${SRC:.c=.h}
	$(PYTHON) pytest.py rrrrslslslrrrsssfrr

trifire.so: setup.py $(SRC)
	$(PYTHON) setup.py build
	cp `find build -name "*.so"` trifire.so

CLEAN = \
	${EXECUTABLES:=.o} \
	${EXECUTABLES} \
	${OBJ} \
	*~ \
	build \
	movie.mp4 \
	pytest.bmp \
	monolith.c \
	monolith.o \
	trifire.so 
.PHONY: clean
clean:
	rm -rf $(CLEAN)

# You can regenerate with % python depends.py
bmp.o: bmp.c bmp.h
bricks.o: bricks.c bricks.h
coin.o: coin.c coin.h
explosion.o: explosion.c explosion.h
penrose.o: penrose.c penrose.h
play.o: play.c play.h sha1.h state.h trajectory.h
render.o: render.c ball.h explosion.h penrose.h penrosecoin.h render.h state.h trajectory.h
sha1.o: sha1.c sha1.h
state.o: state.c state.h
trajectory.o: trajectory.c trajectory.h

# We build trajectories from our generator
trajectory.c: generate_trajectories.py
	$(PYTHON) generate_trajectories.py trajectory.c

# This is local to the Apache2 server on the machine (needs sudo)
.PHONY: install_luxcedia
install_luxcedia: trifire.so
	$(PYTHON) setup.py build
	chmod -x `find build -name "*.so"`
	cp `find build -name "*.so"` /usr/lib/cgi-bin/trifire.so
	cp trifire.html /var/www/html
	cp trifire.cgi  /usr/lib/cgi-bin/trifire
	chmod +x /usr/lib/cgi-bin/trifire
	cp left_button.png right_button.png /var/www/html/image
	cp spin_button.png fire_button.png /var/www/html/image
	cp explosion.png /var/www/html/image
	cp trifire.bmp trifire.json trifire.mp4 /usr/lib/cgi-bin
	chmod +x /usr/lib/cgi-bin/trifire.bmp
	chmod +x /usr/lib/cgi-bin/trifire.json
	chmod +x /usr/lib/cgi-bin/trifire.mp4
