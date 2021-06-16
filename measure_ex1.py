from subprocess import check_output

TESTS = 100
COMMAND = "mpirun -np 2 --mca opal_warn_on_missing_libcuda 0 ./ex1.out"

tests = [0] * TESTS
for i in range(TESTS):
    out = check_output(COMMAND.split()).decode("utf-8")
    tests[i] = float(out.split()[1])

average = sum(tests) / TESTS

print(average)
