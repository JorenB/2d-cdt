MAIN	:= cdt2d.x

.PHONY: all build test clean

all: test

build:
	@make --no-print-directory -C ..
	@cp ../$(MAIN) .

clean:
	$(RM) $(MAIN) out/*.dat

test: build
	./$(MAIN) config.txt
