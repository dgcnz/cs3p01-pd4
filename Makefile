run: compile
	@mpirun -np 2 ./$(TARGET).out

compile:
	@mpicc ${TARGET}.cpp -o ${TARGET}.out

clean:
	@rm ${TARGET}.out
