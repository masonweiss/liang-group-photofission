import ROOT
import numpy as np
from matplotlib import pyplot as plt

# custom parameters
data_directory = "apr06_2000_100million_U233/"
# histos = "merged_histograms/"
histos = ""
counts = "process_data.csv"
run_idxs = [2, 3]  # which run indexes to do
rad_idxs = ["1.2 cm", "1.3 cm"]

particle_labels = ["e-", "e+", "neutron", "gamma", "fission product"]
hist_creation_idxs = [5, 6, 7, 4, 48] # e-, e+, neutrons, gammas, fission products
hist_emerging_idxs = [16, 17, 18, 15, 22] # e-, e+, neutrons, gammas, fission products
uselog_idxs = [True, False, False, True, False]
out_file = "comparison_process_data.csv"

if "merged" in histos:
    prefix = "merged"
else:
    prefix = "sim5"

idxcount = 0
for j in range(len(run_idxs)):
    file = ROOT.TFile.Open(data_directory+prefix+f"_{run_idxs[j]}.root")
    for k in range(len(particle_labels)):
        for m in range(2):
            if m == 0:
                hist = file.Get(str(hist_creation_idxs[k]))
            else:
                hist = file.Get(str(hist_emerging_idxs[k]))
            n_bins = hist.GetNbinsX()
            bin_edges = [hist.GetBinLowEdge(i+1) for i in range(n_bins)]
            bin_edges.append(hist.GetBinLowEdge(n_bins+1))  # last edge
            bin_widths = np.array([hist.GetBinWidth(i+1) for i in range(n_bins)])
            bin_centers = np.array([hist.GetBinCenter(i+1) for i in range(n_bins)])
            bin_contents = np.array([hist.GetBinContent(i+1) for i in range(n_bins)])
            bin_errors = np.array([hist.GetBinError(i+1) for i in range(n_bins)])

            nonzero_mask = bin_contents != 0  # only plot entries with nonzero values
            
            max_e_idx = np.argmax(bin_contents)
            max_energy = bin_centers[max_e_idx]

            # Plot with matplotlib
            fig = plt.figure(figsize=(16,8))
            #plt.plot(bin_centers, bin_contents)
            plt.errorbar(bin_centers[nonzero_mask], bin_contents[nonzero_mask], xerr=bin_widths[nonzero_mask] / 2, yerr=bin_errors[nonzero_mask], fmt='o', markersize=1, linewidth=0, capsize=0, label="Histogram with errors")
            plt.xlabel("Energy [MeV]")
            plt.ylabel("Frequency")
            if uselog_idxs[k]:
                plt.yscale("log")
            plt.title(f"Energy Mode at {round(max_energy,5)} MeV | Mean at {round(hist.GetMean(),5)} MeV")
            plt.suptitle(f"\n\n  Radius: {rad_idxs[j]} | Energy Distribution for {particle_labels[k]} at {'creation' if m==0 else 'detector'}")
            plt.grid(True)
            # plt.ylim([0,1e5])
            plt.savefig(data_directory+f"/plots/{run_idxs[j]}_{particle_labels[k][0:2]}_{'creation' if m==0 else 'detector'}",dpi=600)
            plt.close(fig)
            
