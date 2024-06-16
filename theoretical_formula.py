import numpy as np
import matplotlib.pyplot as plt


def alpha_x(x):
    return (np.sqrt(9 - 8*x) / 2) + x - 1.5


def alpha(s, K):
    x = s / K
    return 1 - x + alpha_x(x)


# Define the range of n
n = np.linspace(1, 20000, 500)

# Define specific values for s and K
s_values = [1, 2, 3, 4, 5]
K = 10

# Plotting
plt.figure(figsize=(10, 6))

for s in s_values:
    alpha_value = alpha(s, K)
    cost = n**alpha_value
    plt.plot(n, cost, label=f's={s}, alpha={alpha_value:.2f}')

plt.xlabel('n')
plt.ylabel('O(n^alpha)')
plt.title('Theoretical Average Cost of Partial Match Query')
plt.legend()
plt.yscale('log')  # Using log scale for better visualization
plt.grid(True)
plt.show()
