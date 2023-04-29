all: out out/test

clean:
	rm -Rf out
	make -C sub/cbase clean

glad/glad.o: glad/glad.c
	gcc -c -o $@ $^

out/binary.o: bin/VGA-ROM.F08 bin/VGA.vertex-shader bin/VGA.fragment-shader
	ld -z noexecstack -r -b binary -o $@ $^

out/%.o: src/%.c
	gcc -c -o $@ $^

out/lib.a: out/main.o out/text.o glad/glad.o
	ar rcs $@ $^

out/%_debug.o: src/%.c
	gcc -D DEBUG= -c -o $@ $^

out/debug.a: out/main_debug.o out/text_debug.o glad/glad.o
	ar rcs $@ $^

subinit:
	git submodule init
	git submodule update

subdate:
	git submodule foreach git pull origin master

sub/cbase/out/lib.a:
	make -C sub/cbase lib

out/test: src/test.c out/debug.a sub/cbase/out/lib.a out/binary.o
	gcc `pkg-config --cflags glfw3` -o $@ $^ -lGL `pkg-config --static --libs glfw3`

glad:
	glad --profile core --out-path glad --api "gl=3.0" --generator c --local-files

out:
	mkdir out

test: out/test
	out/test
