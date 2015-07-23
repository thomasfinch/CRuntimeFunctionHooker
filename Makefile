.PHONY: main run clean

main:
	clang -dynamiclib inject.c -o inject.dylib
	clang testProgram.c -o testProgram

run:
	DYLD_INSERT_LIBRARIES=inject.dylib ./testProgram

clean:
	rm testProgram inject.dylib