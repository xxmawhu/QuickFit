/* ====================================================
#   Copyright (C)2020 All rights reserved.
#
#   Author        : Xin-Xin MA
#   Email         : xxmawhu@163.com
#   File Name     : pull_root5.cxx
#   Create Time   : 2020-01-07 18:54
#   Last Modified : 2020-01-07 18:54
#   Describe      :
#
# ====================================================*/

#include <map>
#include "RooChebychev.h"
#include <string>
using std::map;
using std::string;
using namespace RooFit;
void fit(TString inputFile = "val/alpha.root", double xLow = -12e-3,
         double xHigh = 8e-3, TString xtitle = "#alpha_{J#psi}",
         TString figName = "fig/alpha.eps") {
    QuickFit quickFit(inputFile, "sig", "val");
    quickFit.SetSignalPDF("gauss", 1);
    quickFit.SetCut("");
    quickFit.SetBackgroundPDF("None");
    quickFit.SetFitRange(xLow, xHigh);
    quickFit.SetSignalArea(-3, 3);
    quickFit.SetUnit("None");
    quickFit.SetBins(40);
    quickFit.SetFigureName(figName);
    quickFit.SetXTitle(xtitle);
    quickFit.SaveAs("Example.root", "toyFit");
    quickFit.FitToData("unbinned");
}
void pull_root5() { 
    gSystem->Load("../lib/libSimpleFit.so");
    fit("f.root", -8E-4, 4e-4, "A_{CP}", "afit.eps");
}
