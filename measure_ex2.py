import matplotlib.pyplot as plt
import subprocess

TESTS = 100
N_VALUES = [2, 3, 4, 5]
M_VALUES = [5, 10, 15, 20]


def main():
    x = []
    y = []
    z = []

    for n in N_VALUES:
        for m in M_VALUES:
            print(f"n={n} m={m}")

            acc = 0.0
            for i in range(TESTS):
                cmd = ["mpirun", "-np", str(n), "./ex2.out", str(m)]
                out = subprocess.run(cmd, capture_output=True)
                acc += float(out.stdout)
            avg = round(acc / TESTS, 3)

            x.append(n)
            y.append(m)
            z.append(avg)

    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")
    ax.scatter(x, y, z)
    ax.set_xlabel("p")
    ax.set_ylabel("trips")
    ax.set_zlabel("time (seconds)")
    plt.show()


if __name__ == "__main__":
    main()
