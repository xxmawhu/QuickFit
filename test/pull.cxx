#include <map>
#include "RooChebychev.h"
#include <string>
using std::map;
using std::string;
using namespace RooFit ;
void pull()
{
    fit("f.root", -5E-4, 0., "#Delta A_{CP}", "afit.eps");
}
void fit(
        TString inputFile = "val/alpha.root",
        double xLow = 0.40, 
        double xHigh = 0.50,
        TString xtitle = "#alpha_{J#psi}",
        TString figName = "fig/alpha.eps"
        )
{
    gSystem->Load("../lib/libSimpleFit.so");
    QuickFit fit(inputFile, "sig", "val");
    fit.SetSignalPDF("gauss", 1);
    fit.SetCut("");
    fit.SetBackgroundPDF("None");
    max(inputFile, xLow, xHigh);
    fit.SetFitRange(xLow, xHigh);
    fit.SetSignalArea(-30, 30);
    fit.SetUnit("None");
    fit.SetBins(40);
    fit.SetFigureName(figName);
    fit.SetXTitle(xtitle);
    fit.SaveAs("Example.root", "toyFit");
    fit.FitToData("unbinned");
}
void max(TString inputFile, double &xLow, double &xHigh) {
    TFile f(inputFile, "open");
    TTree *t = reinterpret_cast<TTree*>(f.Get("sig"));
    double val;
    t->SetBranchAddress("val", &val);
    t->GetEntry(0);
    xLow = val;
    xHigh = val;
    for (int i = 0; i < t->GetEntries(); ++i) {
        t->GetEntry(i);
        if ( val > xHigh) {
            xHigh = val;
        } else if ( val < xLow) {
            xLow = val;
        }
    }
    xLow -= 0.2* (xHigh - xLow);
    xHigh += 0.2* (xHigh - xLow);
    return;
}
