import matplotlib.pyplot as plt

def parse_and_plot_histogram(filename):
    values = []

    with open(filename, 'r') as file:
        for line in file:
            parts = line.split('>')
            if len(parts) > 1:
                try:
                    value = float(parts[1].strip())
                    values.append(value)
                except ValueError:
                    continue

    # Plot the histogram using Matplotlib
    plt.hist(values, bins=20, edgecolor='black')
    plt.title('Neutron Energy Distribution')
    plt.xlabel('Energy (MeV)')
    plt.ylabel('#')
    plt.grid(True)
    plt.savefig('../out/neutron_counts.png')
    plt.show()

# Call the function with the filename
filename = '../out/neutron_counts.txt'  # Replace with the actual file name
parse_and_plot_histogram(filename)
