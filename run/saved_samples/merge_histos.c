#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"

void merge_histos() {
    const int num_radii = 4;      // rad_idx ranges from 0 to 3
    const int num_samples = 10;   // sample_idx ranges from 0 to 9
    const int hist_count = 52;    // 52 histograms named 1 to 52
    
    std::string rootfile_dir = "mar02_2000_1billion_U233"; // Root Directory
    std::string output_dir = "merged_histograms"; // Output directory

    system(("mkdir -p " + rootfile_dir + "/" + output_dir).c_str()); // Create output directory if it doesn't exist

    // Loop over radius indices
    for (int rad_idx = 0; rad_idx < num_radii; rad_idx++) {
        std::vector<TH1D*> merged_hists(hist_count, nullptr);

        // Loop over samples
        for (int sample_idx = 0; sample_idx < num_samples; sample_idx++) {
            std::string file_name = rootfile_dir + "/sim5_" + std::to_string(rad_idx) + "_" + std::to_string(sample_idx) + ".root";
            TFile* file = TFile::Open(file_name.c_str(), "READ");

            if (!file || file->IsZombie()) {
                std::cerr << "Error: Cannot open " << file_name << std::endl;
                continue;
            }

            // Loop over histograms (named 1 to 52)
            for (int hist_idx = 1; hist_idx <= hist_count; hist_idx++) {
                std::string hist_name = std::to_string(hist_idx);
                TH1D* hist = (TH1D*)file->Get(hist_name.c_str());

                if (!hist) {
                    std::cerr << "Warning: Histogram " << hist_name << " not found in " << file_name << std::endl;
                    continue;
                }

                // Merge histograms
                if (!merged_hists[hist_idx - 1]) {
                    merged_hists[hist_idx - 1] = (TH1D*)hist->Clone();
                    merged_hists[hist_idx - 1]->SetDirectory(0); // Detach from file
                } else {
                    merged_hists[hist_idx - 1]->Add(hist);
                }
            }
            file->Close();
            delete file;
        }

        // Save merged histograms
        std::string output_file_name = rootfile_dir + "/" + output_dir + "/merged_" + std::to_string(rad_idx) + ".root";
        TFile* output_file = new TFile(output_file_name.c_str(), "RECREATE");

        for (int hist_idx = 0; hist_idx < hist_count; hist_idx++) {
            if (merged_hists[hist_idx]) merged_hists[hist_idx]->Write();
        }

        output_file->Close();
        delete output_file;

        std::cout << "Saved merged histograms for rad_idx " << rad_idx << " -> " << output_file_name << std::endl;
    }

    std::cout << "Merging complete." << std::endl;
}
