import matplotlib.pyplot as plt
import subprocess

TESTS = 100
N_VALUES = [2, 4, 8, 16]
M_VALUES = [2, 4, 8, 16]


def measure_process(n, m):
    print(f"n={n} m={m}")

    acc = 0.0
    for i in range(TESTS):
        cmd = ["mpirun", "-np", str(n), "./ex2.out", str(m)]
        out = subprocess.run(cmd, capture_output=True)
        acc += float(out.stdout)
    avg = round(acc / TESTS, 3)

    return avg


def main():
    # Generating data...
    x1 = []
    y1 = []
    x2 = []
    y2 = []

    m = 5
    for n in N_VALUES:
        x1.append(n)
        y1.append(measure_process(n, m))

    n = 5
    for m in M_VALUES:
        x2.append(m)
        y2.append(measure_process(n, m))

    # Plotting...
    fig, axs = plt.subplots(2)

    axs[0].scatter(x1, y1)
    axs[0].set_xlabel("p")
    axs[0].set_ylabel("time (seconds)")

    axs[1].scatter(x2, y2)
    axs[1].set_xlabel("trips")
    axs[1].set_ylabel("time (seconds)")

    plt.tight_layout()
    plt.savefig("fig.png")


if __name__ == "__main__":
    main()
