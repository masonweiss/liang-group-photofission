# Python script to read a .txt file and strip left whitespace from each line

# Define the file path
file_path = 'feb21_1200_10million/radius_1cm.txt'

# Open the file for reading
with open(file_path, 'r') as file:
    # Read all lines and strip left whitespace
    lines = [line.lstrip() for line in file]

# # Optionally, print the processed lines or do something with them
# for line in lines:
#     print(line, end='')  # Printing each line without adding extra newline

# Write the modified lines to a new file
with open('feb21_1200_10million/radius_1cm_clean.txt', 'w') as output_file:
    output_file.writelines(lines)
