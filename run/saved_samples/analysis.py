import os
import numpy as np
import pandas as pd
import re
from matplotlib import pyplot as plt

# custom parameters
job_id = 38248896
data_directory = "apr06_2000_100million_U233/"
# num_indices = 50  # only 50 indices (rows)
num_indices = 28  # only 28 indices (rows)
# num_gamma = 1e9
num_gamma = 1e8
gamma_energy = 13.8 # in MeV
num_groups = 28
num_runs_per_group = 1

# fixed parameters
process_names = [
    "Decay", "NoProcess", "Radioactivation", "Rayl", "Transportation", "annihil", 
    "compt", "conv", "eIoni", "hIoni", "hadElastic", "ionIoni", "msc", "nCapture", "nFission",
    "neutronInelastic", "phot", "photonNuclear"
]
# Particle names
particle_names = ["anti_nu_e", "e+", "e-", "gamma", "neutron"]

# Initialize NumPy arrays (2D: 50 rows for indices, 2 columns for count and total Emean)
particle_data_creation = np.zeros((num_indices, len(particle_names), 2))  # [count, total Emean]
particle_data_detection = np.zeros((num_indices, len(particle_names), 2))  # [count, total Emean]

# size_maps = {"5 cm":50, "1 cm":10, "5 mm":5, "1 mm":1} # "10 cm":100 exxcluded
# size_maps = data = {"1 cm": 10, "1.1 cm": 11, "1.2 cm": 12, "1.3 cm": 13, "1.4 cm": 14, "1.5 cm": 15, "1.6 cm": 16, "1.7 cm": 17, "1.8 cm": 18, "1.9 cm": 19, "2 cm": 20, "2.1 cm": 21, "2.2 cm": 22, "2.3 cm": 23, "2.4 cm": 24, "2.5 cm": 25, "2.6 cm": 26, "2.7 cm": 27, "2.8 cm": 28, "2.9 cm": 29, "3 cm": 30, "3.1 cm": 31, "3.2 cm": 32, "3.3 cm": 33, "3.4 cm": 34, "3.5 cm": 35, "3.6 cm": 36, "3.7 cm": 37, "3.8 cm": 38, "3.9 cm": 39, "4 cm": 40}
size_maps = {
  "0.2 cm": 2, "0.4 cm": 4, "0.6 cm": 6, "0.8 cm": 8, "1 cm": 10, "1.2 cm": 12, "1.4 cm": 14,
  "1.6 cm": 16, "1.8 cm": 18, "2 cm": 20, "2.2 cm": 22, "2.4 cm": 24, "2.6 cm": 26,
  "2.8 cm": 28, "3 cm": 30, "3.2 cm": 32, "3.4 cm": 34, "3.6 cm": 36, "3.8 cm": 38,
  "4 cm": 40, "4.2 cm": 42, "4.4 cm": 44, "4.6 cm": 46, "4.8 cm": 48, "5 cm": 50,
  "5.2 cm": 52, "5.4 cm": 54, "5.6 cm": 56, "5.8 cm": 58, "6 cm": 60, "6.2 cm": 62,
  "6.4 cm": 64, "6.6 cm": 66, "6.8 cm": 68, "7 cm": 70, "7.2 cm": 72, "7.4 cm": 74,
  "7.6 cm": 76, "7.8 cm": 78, "8 cm": 80, "8.2 cm": 82, "8.4 cm": 84, "8.6 cm": 86,
  "8.8 cm": 88, "9 cm": 90, "9.2 cm": 92, "9.4 cm": 94, "9.6 cm": 96, "9.8 cm": 98,
  "10 cm": 100
}


num_processes = len(process_names)

# Initialize NumPy array to store process counts
process_data = np.zeros((num_indices, num_processes+2), dtype=object)

# Regular expression to extract process calls
absorber_regex = re.compile(r"The Absorber is (\d*\.?\d+\s*\w+)  of")
particle_regex = re.compile(r"(\S+):\s+(\d+)\s+Emean\s*=\s*([\d\.]+)\s*(MeV|keV)")
process_regex = re.compile(r"\s*([\S]+)\s*=\s*(\d+)\s*")

# Loop over indices
for idx in range(num_indices):
    pattern = f"sim5_{idx}_{job_id}_"
    
    # Find matching files
    files = [f for f in os.listdir(data_directory) if f.startswith(pattern) and f.endswith(".out")]

    for file_name in files:
        file_path = os.path.join(data_directory, file_name)

        with open(file_path, "r") as f:
            lines = f.readlines()

        absorber_thickness = "unknown"  # Default if not found
        for line in lines:
            absorber_match = absorber_regex.search(line)
            if absorber_match:
                absorber_thickness = absorber_match.group(1).strip()
                break  # Stop after finding the first occurrence

        # Find block containing process calls
        start_idx = None
        end_idx = None

        for i, line in enumerate(lines):
            if "Run terminated." in line:
                start_idx = i  # Start from "Run terminated."
            elif "List of particles at creation :" in line and start_idx is not None:
                end_idx = i  # Stop at "List of particles at creation :"
                break

        # Extract process data
        if start_idx is not None and end_idx is not None:
            block_lines = lines[start_idx:end_idx]

            process_start = None
            process_end = None

            for i, line in enumerate(block_lines):
                if "Process calls frequency :" in line:
                    process_start = i + 1
                elif "Mean energy deposit per event" in line and process_start is not None:
                    process_end = i
                    break

            if process_start is not None and process_end is not None:
                for line in block_lines[process_start:process_end]:
                    process_data[idx, 0] = absorber_thickness

                    matches = process_regex.findall(line)
                    for process, count in matches:
                        count = int(count)
                        # if idx==0:
                        #     print(process, count, type(count))
                        if process in process_names:
                            process_idx = process_names.index(process)
                            process_data[idx, process_idx+1] = count
                        else:
                            process_data[idx, num_processes+1] = count  # Add to "Other" category

        creation_block = None
        detection_block = None
        block_start = None

        for i, line in enumerate(lines):
            if "List of particles at creation :" in line:
                block_start = i + 1
            elif "List of particles emerging from the absorber :" in line:
                creation_block = lines[block_start:i] if block_start else None
                block_start = i + 1
            elif line.startswith("...") and block_start:
                detection_block = lines[block_start:i]
                break

        # Process "at creation" block
        if creation_block:
            for line in creation_block:
                match = particle_regex.search(line)
                if match:
                    particle, count, emean, unit = match.groups()
                    
                    if particle not in particle_names:
                        continue
                    count = int(count)  # Convert count to integer
                    emean_value = float(emean)

                    if unit == "keV":
                        emean_value *= 0.001  # Convert keV to MeV
                    particle_idx = particle_names.index(particle)
                    particle_data_creation[idx, particle_idx, 0] += float(count)
                    particle_data_creation[idx, particle_idx, 1] += emean_value


        # Process "at detection" block
        if detection_block:
            for line in detection_block:
                match = particle_regex.search(line)
                if match:
                    particle, count, emean, unit = match.groups()
                    if particle not in particle_names:
                        continue

                    count = int(count)  # Convert count to integer
                    emean_value = float(emean)

                    if unit == "keV":
                        emean_value *= 0.001  # Convert keV to MeV
                    particle_idx = particle_names.index(particle)
                    particle_data_detection[idx, particle_idx, 0] += float(count)
                    particle_data_detection[idx, particle_idx, 1] += emean_value

process_names.append("Other")

# # Define number of groups (5 groups of 10)
# grouped_particle_data_creation = np.zeros((num_groups, len(particle_names), 2))  # [count, total Emean]
# grouped_particle_data_detection = np.zeros((num_groups, len(particle_names), 2))  # [count, total Emean]

# # Aggregate data into 5 groups (Summing every 10 rows)
# for group_idx in range(num_groups):
#     start_idx = group_idx * num_runs_per_group
#     end_idx = start_idx + num_runs_per_group

#     grouped_particle_data_creation[group_idx] = np.sum(particle_data_creation[start_idx:end_idx], axis=0)
#     grouped_particle_data_detection[group_idx] = np.sum(particle_data_detection[start_idx:end_idx], axis=0)

# grouped_particle_data_creation[:, :, 1] /= num_runs_per_group
# grouped_particle_data_detection[:, :, 1] /= num_runs_per_group

# # Convert to structured NumPy array and group by absorber thickness
# absorbers = process_data[:, 0]  # First column (absorber thickness)
# process_counts = process_data[:, 1:].astype(int)  # Convert rest to integers

# # Find unique absorber thickness values
# unique_absorbers = np.unique(absorbers)

# # Initialize grouped data array
# grouped_data = np.zeros((len(unique_absorbers), process_counts.shape[1] + 1), dtype=object)

# # Aggregate values by absorber thickness
# for i, absorber in enumerate(list(size_maps.keys())[::-1]):
#     mask = absorbers == absorber  # Find rows with the same absorber
#     grouped_data[i, 0] = size_maps[absorber]  # Keep absorber name
#     grouped_data[i, 1:] = np.sum(process_counts[mask], axis=0)  # Sum process counts

# for i, rad in enumerate(list(size_maps.keys())[::-1]):
#     print(f"\n\n\n============= {rad} =============")

#     print("Process Calls:")
#     for j, process in enumerate(process_names):
#         print(f"{process.ljust(30)}{grouped_data[i, j+1]}")

#     print("\nParticles at CREATION")
#     for j, particle in enumerate(particle_names):
#         a = grouped_particle_data_creation[i, j, 0]
#         b = round(grouped_particle_data_creation[i, j, 1], 8)
#         c = a * b / (num_gamma)
#         print(f"{particle.ljust(20)} |      Count: {str(int(a)).ljust(15)} |    Mean Energy {str(b).ljust(12)} MeV     |     Total Energy of all particles per gamma {str(c).ljust(12)} MeV")
    
#     print("\nParticles at DETECTION")
#     for j, particle in enumerate(particle_names):
#         a = grouped_particle_data_detection[i, j, 0]
#         b = round(grouped_particle_data_detection[i, j, 1], 8)
#         c = a * b / (num_gamma)
#         print(f"{particle.ljust(20)} |      Count: {str(int(a)).ljust(15)} |    Mean Energy {str(b).ljust(12)} MeV     |     Total Energy of all particles per gamma {str(c).ljust(12)} MeV")

# # Convert first column to numeric values for sorting
# numeric_bar_names = grouped_data[:, 0].astype(int)  
# values = grouped_data[:, 1:].astype(int)

# # Sort indices based on numeric values
# sorted_indices = np.argsort(numeric_bar_names)
# numeric_bar_names = numeric_bar_names[sorted_indices]
# values = values[sorted_indices]

# os.makedirs(f"{data_directory}merged_histograms/process_calls", exist_ok=True)

# # Generate bar charts for each column
# for i, process in enumerate(process_names):
#     plt.figure(figsize=(8, 6))
#     bars = plt.bar(numeric_bar_names.astype(str), values[:, i], color='skyblue')

#     for bar in bars:
#         height = bar.get_height()
#         plt.text(bar.get_x() + bar.get_width() / 2, height, f"{height:,}", ha='center', va='bottom', fontsize=10)

#     # Add values on top of bars
#     # for j, val in enumerate(values[:, i]):
#     #     plt.text(numeric_bar_names[j], val, f"{val:,}", ha='center', va='bottom', fontsize=10, rotation=90)

#     plt.xlabel("Absorber Thickness")
#     plt.ylabel("Count")
#     plt.title(f"Process: {process}")
#     plt.xticks(rotation=45)

#     # Save each chart with its process name
#     plt.tight_layout()
#     plt.savefig(f"{data_directory}merged_histograms/process_calls/{process}.png")
#     plt.close()  # Close figure to avoid overlap

# Save as CSV for verification
np.savetxt(data_directory+"process_data.csv", process_data[:,1:], delimiter=",", header=",".join(process_names), fmt="%d")
