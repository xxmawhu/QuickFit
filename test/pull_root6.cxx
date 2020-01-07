/* ====================================================
#   Copyright (C)2020 All rights reserved.
#
#   Author        : Xin-Xin Ma
#   Email         : xxmawhu@163.com
#   File Name     : pull_root6.cxx
#   Create Time   : 2020-01-07 18:28
#   Last Modified : 2020-01-07 18:28
#   Describe      :
#
# ====================================================*/

#include "RooChebychev.h"
#include <map>
#include <string>
using std::map;
using std::string;
using namespace RooFit;
R__LOAD_LIBRARY(../lib/libSimpleFit.so)
void fit(TString inputFile, double xLow,
         double xHigh, TString xtitle,
         TString figName);
void pull_root6() {
    // use the function fit
    fit("f.root", -8E-4, 4e-4, "A_{CP}", "afit.eps");
}
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
