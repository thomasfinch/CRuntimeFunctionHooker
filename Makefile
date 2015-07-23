.PHONY: main run clean

PLATFORM := $(shell uname -s)

build:
ifeq ($(PLATFORM),Linux)
	gcc -fpic -c -o inject.o inject.c
	gcc -shared -o inject.so inject.o
	gcc testProgram.c -o testProgram
else
	clang -dynamiclib inject.c -o inject.dylib
	clang testProgram.c -o testProgram
endif

run:
ifeq ($(PLATFORM),Linux)
else
	DYLD_INSERT_LIBRARIES=inject.dylib ./testProgram
endif

clean:
	rm testProgram inject.dylib inject.so
