import zlib
import numpy as np
import matplotlib.pyplot as plt

# NEUTRON FISSION CROSSECTIONS
data_dir = "Fission"
# Define files for uranium isotopes
files = {
    "U-233": "92_233_Uranium.z",
    "U-235": "92_235_Uranium.z",
    "U-238": "92_238_Uranium.z"
}

# Colors for each isotope
colors = ["b", "g", "r"]

fig1, ax1 = plt.subplots(figsize=(8, 6))

for (label, file), color in zip(files.items(), colors):
    try:
        # Read and decompress the Zlib file
        with open(data_dir+"/"+file, "rb") as f:
            compressed_data = f.read()
        decompressed_data = zlib.decompress(compressed_data).decode()

        # Split lines and remove empty spaces
        lines = decompressed_data.strip().split("\n")

        # Skip metadata (first line contains metadata, actual data starts from line 2)
        data_lines = lines[1:]  # Exclude the first line

        # Flatten the values into a single list
        raw_numbers = []
        print("==================")
        for line in data_lines:
            if label == "U-238":
                print(line)
            values = line.strip().split()
            raw_numbers.extend(map(float, values))  # Convert to floats

        # Reshape data into (E, XS) pairs
        num_pairs = len(raw_numbers) // 2  # Ensure it's even
        data = np.array(raw_numbers[: 2 * num_pairs]).reshape(-1, 2)

        # Extract photon energy and cross-section
        photon_energy = data[:, 0]  # Energy in eV
        photon_energy = photon_energy / 1e6 # Energy in MeV
        cross_section = data[:, 1]  # Cross-section in barns

        # Plot full range
        ax1.plot(photon_energy, cross_section, label=label, color=color)

    except Exception as e:
        print(f"Error processing {file}: {e}")

ax1.set_xscale("log")  # Log scale for energy
ax1.set_yscale("log")  # Log scale for cross-section
ax1.set_xlabel("Neutron Energy (MeV)")
ax1.set_ylabel("Cross-Section (Barns)")
ax1.set_title("Neutron Fission Cross-Section (Full Range)")
ax1.legend()
ax1.grid(True, which="both", linestyle="--", linewidth=0.5)

# Show the first figure
plt.savefig('nFissionXS_full.png')

# ===== Figure 2: Copy and Zoom-In (0.01 - 20 MeV) =====
fig2, ax2 = plt.subplots(figsize=(8, 6))

# Copy everything from the first figure to the second
lowval = 1
highval = 20
for line in ax1.get_lines():
    x_data, y_data = line.get_xdata(), line.get_ydata()
    mask = (x_data >= lowval) & (x_data <= highval)  # Zoom in
    ax2.plot(x_data[mask], y_data[mask], label=line.get_label(), color=line.get_color())

#ax2.set_xscale("log")  # Log scale for energy
#ax2.set_yscale("log")  # Log scale for cross-section
ax2.set_xlabel("Neutron Energy (MeV)")
ax2.set_ylabel("Cross-Section (Barns)")
ax2.set_title("Neutron Fission Cross-Section (zoomed)")
ax2.legend()
ax2.grid(True, which="both", linestyle="--", linewidth=0.5)

# Show the plot
plt.savefig("nFissionXS.png")