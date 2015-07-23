.PHONY: main run clean

main:
	gcc -dynamiclib inject.c -o inject.dylib
	gcc testProgram.c -o testProgram

run:
	DYLD_INSERT_LIBRARIES=inject.dylib ./testProgram

clean:
	rm testProgram inject.dylib