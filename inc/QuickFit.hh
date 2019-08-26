#ifndef QuickFit_H 
#define QuickFit_H

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
#include "TString.h"
//#include <math>
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooRealIntegral.h"
#include "RooWorkspace.h"
//#include "/besfs/users/maxx/home/head/bes3plotstyle_1.C"
using namespace RooFit ;

class QuickFit {
    public:
        QuickFit(TString file, TString tree, TString leaf);
        virtual ~QuickFit();
        void SetSignalPDF(TString, int order=2);//"gauss"

        void SetBackgroundPDF(TString, int order=2);  //
        void SetCut(TCut cut );
        void SetUnit(TString unit= "GeV");
        void SetFitRange(double,double);
        void SetFigureName(TString);
        void SetXTitle(TString);
        void SetSignalArea(double low = -3.0, double hig = 3.0);
        void SetBins(int bins=100);
        void SaveAs(TString file="out.root", TString sigshapeName="sigpdf");
        void FitToData(TString method = "binned"); //or unbined
        double Chisq(RooAbsPdf *totFit, RooAbsData *data);
    private:
        RooWorkspace* _wspace;
        void AddSignalModel(RooWorkspace*);
        void AddBackgroundModel(RooWorkspace*);
        void AddModel(RooWorkspace*);
        void AddData(RooWorkspace*);
        void Fit(RooWorkspace*);
        void MakePlots(RooWorkspace*);
        void PlotPaveText(TPaveText &, RooWorkspace *_wspace, RooPlot *frame);
        void PlotTitle(RooWorkspace *_wspace, RooPlot *frame);
        void Export(RooWorkspace *);
        void calculateChisq(RooPlot* frame, double&, int&);
        void GenerateCxx(RooWorkspace* _wspace, RooPlot*);
        void getSigmaAndMean(RooWorkspace *, double*, double*);
        TString m_file, m_tree, m_branch, m_output;
        TCut  m_cut;
        double m_low, m_high;
        int m_bins, m_totevts;

        TString m_signalShape, m_bkgShape;
        int m_bkgOrder, m_sigOrder;
        TString m_sigpdfName;

        bool m_sigarea,m_getQ,m_setxt;
        TString m_method, m_txt, che_g, m_xtitle, m_figname, 
                m_unit;
        int   txt_place;
        double m_chisq,m_minFCN;
        double x_low, x_high;
        int m_ndf,c_ndf;
        int m_signallow, m_signalhig;
    private:
        RooFitResult *_result;
        ClassDef(QuickFit, 1)
};
#endif



