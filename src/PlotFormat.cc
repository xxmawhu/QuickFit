#ifndef __CINT__
//#include "RooGlobalFunc.h"
#endif

#include <TAxis.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH1F.h>
#include <TF1.h>
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TArrow.h"
#include "TH1D.h"
//#include <math>
#include "RooPlot.h"
#include "PlotFormat.hh"
using namespace RooFit ;
using std::cout;
using std::endl;
ClassImp(PlotFormat)

    PlotFormat::PlotFormat(){
    }

PlotFormat::~PlotFormat(){

}

void PlotFormat::Axis(TAxis * axis)
{
    axis->SetLabelFont(42);
    axis->SetLabelSize(0.05);
    axis->SetLabelOffset(0.02);
    axis->SetNdivisions(505);
    axis->SetTitleFont(42);
    axis->SetTitleColor(1);
    axis->SetTitleSize(0.05);
    axis->SetNoExponent(kFALSE);
    axis->CenterTitle();
}
void PlotFormat::Frame(RooPlot *frame ){
	Axis(frame->GetXaxis());
	Axis(frame->GetYaxis());
}
void PlotFormat::SetStyle()
{
    // No Canvas Border
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    // White BG
    gStyle->SetCanvasColor(10);
    // PlotFormat for axes
    gStyle->SetLabelFont(42,"xyz");
    gStyle->SetLabelSize(0.04,"xyz");
    gStyle->SetLabelOffset(0.02,"y");
    gStyle->SetLabelOffset(0.02,"x");
    gStyle->SetNdivisions(510,"xyz");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetTitleColor(1,"xyz");
    gStyle->SetTitleSize(0.09,"xyz");
    gStyle->SetTitleOffset(1.35,"x");
    gStyle->SetTitleOffset(1.2,"y");
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetHistLineWidth(1.85);
    // No pad borders
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBorderSize(0);
    // White BG
    gStyle->SetPadColor(10);
    // Margins for labels etc.
    gStyle->SetPadLeftMargin(0.25);
    gStyle->SetPadBottomMargin(0.25);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadTopMargin(0.12);
    // No error bars in x direction
    gStyle->SetErrorX(0);
    // PlotFormat legend
    gStyle->SetLegendBorderSize(0);
    //
    gStyle->SetPadTickX(0);
    gStyle->SetPadTickY(0);
}




