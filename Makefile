tshader: main.o shader.o program.o x11.o gl.o ani.o files.o
	@echo showing symbols
	@nm *.o | grep -E ' T |:|U gl|U X| B '
	@echo making tshader
	@g++ -o tshader main.o shader.o x11.o gl.o ani.o files.o program.o -lGL -lGLU -lX11 -lboost_system -lboost_filesystem -O2

main.o: main.cpp shader.h x11.h
	@echo compiling main.o
	@g++ -c main.cpp -O2

shader.o: shader.h shader.cpp
	@echo compiling shader.o
	@g++ -c shader.cpp -O2

program.o: shader.h program.cpp program.h
	@echo compiling program.o
	@g++ -c program.cpp -O2

x11.o: x11.cpp x11.h ani.h
	@echo compiling x11.o
	@g++ -c x11.cpp -O2

gl.o: gl.cpp shader.h gl.h program.h ani.h
	@echo compiling gl.o
	@g++ -c gl.cpp -O2

ani.o: ani.cpp ani.h
	@echo compiling ani.o
	@g++ -c ani.cpp -O2

files.o: files.cpp files.h shader.h
	@echo compiling files.o
	@g++ -c files.cpp -O2

clean: .PHONY
	@echo cleaning tree ^^
	@rm -f *.o
	@rm tshader

.PHONY:
