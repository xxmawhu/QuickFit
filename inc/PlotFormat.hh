#ifndef PlotFormat_H 
#define PlotFormat_H

#ifndef __CINT__
#include "RooGlobalFunc.h"
//#include "dsgauss.h"
#endif

#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooCBShape.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TCut.h"
#include "TChain.h"
//#include <math>
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooRealIntegral.h"
#include "RooWorkspace.h"
//#include "/besfs/users/maxx/home/head/bes3plotstyle_1.C"
using namespace RooFit ;

class PlotFormat {
    public:
        PlotFormat();
        virtual ~PlotFormat();
        void Frame(RooPlot *frame );
        void SetStyle();
        void Axis(TAxis * axis);
    private:
        ClassDef(PlotFormat, 1)
};
#endif



