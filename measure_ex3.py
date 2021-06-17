import matplotlib.pyplot as plt
from subprocess import check_output

TESTS = 100

COMMAND = "mpirun -np {0} ./ex3.out"

tests = []
for p in range(1, 16):
	print("Process {}".format(p))
	total = 0.0

	for i in range(TESTS):
		out = check_output(COMMAND.format(p).split()).decode("utf-8")
		total += float(out.split()[1])
	
	total /= TESTS

	tests.append(total)

plt.figure(figsize=(10, 8))
plt.title("Execution time vs. number of processes")
plt.ylabel("Execution time (ms)")
plt.xlabel("Number of processes")
plt.plot([i for i in range(1, 16)], tests)
plt.savefig("ex3.png")
