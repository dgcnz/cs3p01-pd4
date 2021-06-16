EXEC = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

.PHONY: compile

# Se compilan todos los ejecutables
compile: $(EXEC)

# Al correr `make x.out` se compila x.cpp y se produce x.out
%.out: %.cpp
	mpic++ $< -o $@

clean:
	rm $(EXEC)
