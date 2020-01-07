#include <map>
#include "RooChebychev.h"
#include <string>
using std::map;
using std::string;
using namespace RooFit;
gSystem->Load("../lib/libSimpleFit.so");
void fit(TString inputFile = "val/alpha.root", double xLow = -8e-3,
         double xHigh = 4e-3, TString xtitle = "#alpha_{J#psi}",
         TString figName = "fig/alpha.eps") {
    QuickFit fit(inputFile, "sig", "val");
    fit.SetSignalPDF("gauss", 1);
    fit.SetCut("");
    fit.SetBackgroundPDF("None");
    fit.SetFitRange(xLow, xHigh);
    fit.SetSignalArea(-3, 3);
    fit.SetUnit("None");
    fit.SetBins(40);
    fit.SetFigureName(figName);
    fit.SetXTitle(xtitle);
    fit.SaveAs("Example.root", "toyFit");
    fit.FitToData("unbinned");
}
void pull() { fit("f.root", -5E-4, 0., "#Delta A_{CP}", "afit.eps"); }
