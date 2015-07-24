.PHONY: main run clean

PLATFORM := $(shell uname -s)

build:
ifeq ($(PLATFORM),Linux)
	gcc -fpic -c -o inject.o inject.c
	gcc -shared -o inject.so inject.o -ldl
	gcc -rdynamic testProgram.c -o testProgram
else ifeq ($(PLATFORM),Darwin)
	clang -dynamiclib inject.c -o inject.dylib
	clang testProgram.c -o testProgram
endif

run:
ifeq ($(PLATFORM),Linux)
	LD_PRELOAD=$(PWD)/inject.so ./testProgram
else ifeq ($(PLATFORM),Darwin)
	DYLD_INSERT_LIBRARIES=inject.dylib ./testProgram
endif

clean:
	rm -f testProgram inject.o inject.dylib inject.so
