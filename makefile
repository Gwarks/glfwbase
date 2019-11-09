all: out out/test

clean:
	rm -Rf out
	make -C sub/cbase clean

glad/glad.o: glad/glad.c
	gcc -c -o $@ $^

out/bin_font.o: bin/VGA-ROM.F08
	ld -r -b binary -o out/bin_font.o bin/VGA-ROM.F08

out/%.o: src/%.c
	gcc -c -o $@ $^

out/lib.a: out/main.o glad/glad.o
	ar rcs $@ $^

out/%_debug.o: src/%.c
	gcc -D DEBUG= -c -o $@ $^

out/debug.a: out/main_debug.o glad/glad.o
	ar rcs $@ $^

subdate:
	git submodule foreach git pull origin master

sub/cbase/out/lib.a:
	make -C sub/cbase lib

out/test: src/test.c out/debug.a sub/cbase/out/lib.a out/bin_font.o
	gcc `pkg-config --cflags glfw3` -o $@ $^ -lGL `pkg-config --static --libs glfw3`

glad:
	glad --profile core --out-path glad --api "gl=3.0" --generator c --local-files

out:
	mkdir out

test: out/test
	out/test
