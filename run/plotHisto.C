#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"

void plotHisto() {
    // 1. Open the existing ROOT file containing the histogram
    TFile *inputFile = TFile::Open("saved_samples/feb21_1200_10million/radius_10cm.root");  // Replace with your file name

    // 2. Retrieve the existing histogram
    TH1F *h = (TH1F*)inputFile->Get("16");  // Replace with your histogram name

    if (!h) {
        std::cout << "Histogram not found!" << std::endl;
        return;
    }

    // 3. Rebin the histogram to 50 bins (you can choose an appropriate rebinning factor)
    int currentBins = h->GetNbinsX();
    int newBins = 20; // Desired number of bins
    int rebinFactor = currentBins / newBins;  // Calculate rebinning factor

    h->Rebin(rebinFactor);  // Rebin the histogram

    // 4. Create a canvas to draw the histogram
    TCanvas *c = new TCanvas("c", "Energy Spectrum Fit", 800, 600);

    // Draw the rebinned histogram
    h->Draw();

    // 5. Create a fit function (e.g., Gaussian)
    TF1 *fitFunc = new TF1("fitFunc", "gaus", h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());  // Gaussian fit

    // 6. Fit the histogram with the chosen function
    h->Fit(fitFunc, "R");  // Fit the histogram with the Gaussian function

    // 7. Save the result to a new ROOT file
    TFile *outputFile = new TFile("rebinned_and_fitted_histogram.root", "RECREATE");
    h->Write();  // Write the rebinned histogram
    fitFunc->Write();  // Write the fit function to the ROOT file
    outputFile->Close();

    // 8. Save the canvas as an image (optional)
    c->SaveAs("rebinned_fit_histogram.png");

    // 9. Print fit parameters
    fitFunc->Print();
}
