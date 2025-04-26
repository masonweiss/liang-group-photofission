import ROOT
import array
import numpy as np

# custom parameters
data_directory = "apr11_1100_100_evt_U233/"
# histos = "merged_histograms/"
histos = ""
counts = "process_data.csv"
num_groups = 11  # = # of rows in COUNTS and # of root files in HISTOS
process_names = ["nFission", "photonNuclear", "conv"]
process_names_idx = [14,17,7]
hist_idxs = ["54", "53", "55"]
pFission_hist_idx = "46"
out_file = "comparison_process_data.csv"

if "merged" in histos:
    prefix = "merged"
else:
    prefix = "sim5"

results = np.loadtxt(data_directory+histos+counts,delimiter=",")

i = 0
with open(data_directory+out_file, "w") as f:
    for p in range(len(process_names_idx)):
        # nFission, photonNuclear, pairProduction

        for i in range(num_groups):
            # process calls counts
            if process_names[p] == "photonNuclear":
                file = ROOT.TFile.Open(data_directory+prefix+f"_{i}.root")
                hist = file.Get(pFission_hist_idx)
                num_entries = hist.GetEntries()
                f.write(str(num_entries/2))
            else:
                f.write(str(results[i, process_names_idx[p]]))
            if i != num_groups-1:
                f.write(",")
        f.write("\n")

        for i in range(num_groups):
            # charged particle counts
            file = ROOT.TFile.Open(data_directory+prefix+f"_{i}.root")
            hist = file.Get(hist_idxs[p])
            num_entries = hist.GetEntries()
            f.write(str(num_entries))
            if i != num_groups-1:
                f.write(",")
        f.write("\n")

        for i in range(num_groups):
            # means
            file = ROOT.TFile.Open(data_directory+prefix+f"_{i}.root")
            hist = file.Get(hist_idxs[p])
            mean = hist.GetMean()
            f.write(str(mean))
            if i != num_groups-1:
                f.write(",")
        f.write("\n\n")
        
            
            
