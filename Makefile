EXEC = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

.PHONY: compile

# Se compilan todos los ejecutables
compile: $(EXEC)

# Al correr `make x` se compila x.cpp, produce x.out y se ejecuta x.out
# De preferencia usar `run.sh x` como alternativa
%: %.out
	mpirun -np 2 ./$<

# Al correr `make x.out` se compila x.cpp y se produce x.out
%.out: %.cpp
	mpicc $< -o $@ -lstdc++

clean:
	rm $(EXEC)
