// Copyright (c) 2019-2-25 maxx
#ifndef __CINT__
//#include "RooGlobalFunc.h"
#endif

#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooCBShape.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooConstVar.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooPolynomial.h"
#include "TString.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TArrow.h"
#include "TPaveText.h"
#include "TH1D.h"

// #include <math>
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooRealIntegral.h"
#include "QuickFit.hh"
#include "PlotFormat.hh"
#include "currentPath.hh"
#include <string>
// #include "/besfs/users/maxx/home/head/bes3plotstyle_1.C"
using namespace RooFit;
using std::cout;
using std::endl;
using std::to_string;
ClassImp(QuickFit)
QuickFit::QuickFit(TString file, TString tree, TString leaf) {
    m_file = file;
    m_tree = tree;
    m_branch = leaf;
    m_method = "bined";
    m_bins = 100;
    m_signalShape = "gauss";
    m_sigOrder = 2;
    m_bkgShape = "chebychev";
    m_bkgOrder = 2;
    m_cut = "";
    m_figname = "c.eps";
    m_output = "out.root";
    _result = new RooFitResult();
}
QuickFit::~QuickFit() {
    delete _result;
}

void QuickFit::SetSignalPDF(TString sigshape, int order ) {
    m_signalShape = sigshape;
    m_sigOrder = order;
}

void QuickFit::SetBackgroundPDF(TString bkgshape, int order) {
    m_bkgShape = bkgshape;
    m_bkgOrder = order;
}
void QuickFit::SetCut(TCut cut) {
    m_cut = cut;
}
void QuickFit::SetXTitle(TString xtitle) {
    m_xtitle = xtitle;
}
void QuickFit::SaveAs(TString output, TString sigshapeName) {
    m_output = output;
    m_sigpdfName = sigshapeName;
}
void QuickFit::SetFitRange(double xlow, double xhigh) {
    m_low = xlow;
    m_high = xhigh;
}

void QuickFit::AddSignalModel(RooWorkspace *_wspace) {
    cout << "Inf:: AddSignalModel " << m_signalShape << endl;
    RooRealVar x(m_branch, m_xtitle, m_low, m_high);
    if (m_signalShape == "gauss") {
        cout << "Inf::the signal model is " << m_sigOrder
             <<  " gaussi function" << endl;
        RooRealVar mean1("mean1_"+m_sigpdfName, "mean",
                (m_high + m_low)/2, m_low, m_high);
        RooRealVar sigma1("sigma1_"+m_sigpdfName,
                "sigma", (m_high - m_low)/10, 0, m_high - m_low);
        if (m_sigOrder == 1) {
            RooGaussian sigpdf(m_sigpdfName, "Gaussian function", x, mean1, sigma1);
            _wspace->import(sigpdf);
            return;
        }
        RooGaussian g1("g1_" + m_sigpdfName,
                "gaussianpdf", x, mean1, sigma1);
        RooRealVar mean2("mean2_" + m_sigpdfName, "mean",
                (m_high + m_low)/2, m_low, m_high);
        RooRealVar sigma2("sigma2_" + m_sigpdfName, "sigma",
                (m_high - m_low)/6, 0, m_high - m_low);
        RooGaussian g2("g2_" + m_sigpdfName, "gaussianpdf", x, mean2, sigma2);
        RooRealVar frac1("frac1_" + m_sigpdfName, "", 0.5, 0, 1);
        if (m_sigOrder == 2) {
            RooAddPdf sigpdf(m_sigpdfName, "Double gaussian function",
                    RooArgList(g1, g2), RooArgList(frac1));
            _wspace->import(sigpdf);
            return;
        }
        RooRealVar mean3("mean3_" + m_sigpdfName,  "mean3",
                (m_low + m_high)/2,   m_low,  m_high);
        RooRealVar sigma3("sigma3_" + m_sigpdfName, "sigma",
                (m_high - m_low)/8, 0, m_high - m_low);
        RooGaussian g3("g3_" + m_sigpdfName,
                "gaussianpdf", x, mean3, sigma3);
        RooRealVar frac2("frac2_" + m_sigpdfName,
                "frac2", 0.16,   0.0, 1.0);
        if (m_sigOrder == 3) {
            RooAddPdf sigpdf(m_sigpdfName, "three gaussian function",
                    RooArgList(g1,  g2, g3),
                    RooArgList(frac1, frac2) );
            _wspace->import(sigpdf);
            return;
        }
        RooRealVar mean4("mean4_" + m_sigpdfName,
                "mean4" ,
                (m_low + m_high)/2,  m_low, m_high);
        RooRealVar sigma4("sigma4_" + m_sigpdfName,
                "sigma", (m_high - m_low)/6, 0, m_high - m_low);
        RooGaussian g4("g4_" + m_sigpdfName,
                "gaussianpdf", x, mean4, sigma4);
        RooRealVar frac3("frac3_" + m_sigpdfName,
                "frac3", 0.05,   0.0, 1.0);
        if (m_sigOrder == 4) {
            RooAddPdf sigpdf(m_sigpdfName, "four gaussian function", RooArgList(g1,  g2, g3,  g4),
                    RooArgList(frac1, frac2, frac3) );
            _wspace->import(sigpdf);
            return;
        }
    }
}
void QuickFit::AddBackgroundModel(RooWorkspace *_wspace) {
    cout << "Inf:: AddBackgroundModel:" << m_bkgShape << endl;
    RooRealVar *x = _wspace->var(m_branch);
    if (m_bkgShape == "None") {
        RooRealVar p0("p0", "poly 0", 0, -1, 1);
        RooChebychev  bkgpdf("bkgpdf", "1st chebychev",  *x, RooArgList(p0));
        _wspace->import(bkgpdf);
        return;
    }
    if (m_bkgShape == "chebychev") {
        RooRealVar p0("p0", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 1) {
            RooChebychev  bkgpdf("bkgpdf", "1st chebychev",  *x, RooArgList(p0));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p1("p1", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 2) {
            RooChebychev  bkgpdf("bkgpdf", "2nd chebychev", *x, RooArgList(p0,  p1));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p2("p2", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 3) {
            RooChebychev  bkgpdf("bkgpdf", "3rd chebychev",  *x,
                    RooArgList(p0, p1, p2));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p3("p3", "poly 3", 0, -1, 1);
        if (m_bkgOrder == 4) {
            RooChebychev  bkgpdf("bkgpdf", "4th chebychev",  *x,
                    RooArgList(p0, p1, p2, p3));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p4("p4", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 5) {
            RooChebychev  bkgpdf("bkgpdf", "5th chebychev",  *x,
                    RooArgList(p0, p1,  p2,  p3,  p4));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p5("p5", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 6) {
            RooChebychev  bkgpdf("bkgpdf", "6th chebychev",  *x,
                    RooArgList(p0, p1,  p2,  p3,  p4,  p5));
            _wspace->import(bkgpdf);
            return;
        }
        RooRealVar p6("p6", "poly 0", 0, -1, 1);
        if (m_bkgOrder == 7) {
            RooChebychev  bkgpdf("bkgpdf", "7th chebychev",  *x,
                    RooArgList(p0, p1,  p2,  p3,  p4,  p5,  p6));
            _wspace->import(bkgpdf);
            return;
        }
    }
    _wspace->Print();
}
void QuickFit::AddModel(RooWorkspace *_wspace) {
    RooAbsPdf *sigpdf = _wspace->pdf(m_sigpdfName);
    RooAbsPdf *bkgpdf = _wspace->pdf("bkgpdf");
    RooRealVar nsig("nsig", "#signal events", m_totevts/2,  0, m_totevts);
    RooRealVar nbkg("nbkg",  "#bkg events",    m_totevts/2,  0, m_totevts);
    RooAddPdf model("model", "s+b",   RooArgList((*sigpdf),  (*bkgpdf) ),
            RooArgList(nsig, nbkg));
    if (m_bkgShape == "None") {
        nbkg.setVal(0);
        nbkg.setConstant();
        RooRealVar *p0 = _wspace->var("p0");
        p0->setConstant();
    }
    _wspace->import(model);
    cout << "total events\t" << m_totevts << endl;
    return;
}
void QuickFit::AddData(RooWorkspace *_wspace) {
    cout << "Inf::Add Data from " << m_file << endl;
    TChain ch(m_tree);
    ch.Add(m_file);
    TTree* t = (TTree*)ch.CopyTree(m_cut);
    m_totevts = t ->GetEntries();
    if (m_totevts == 0) {
      cout << "Error: the data Set is empty!!!" << endl;
      exit(0);
    }
    RooRealVar *x = _wspace->var(m_branch);
    if (m_method != "unbinned") {
        cout << "Inf::You choose the bined fit method" << endl;
        cout << "Inf::Aim to fit " << m_branch << endl;
        TH1D* m1 = new TH1D("m1", "m1", m_bins , m_low, m_high);
        t->Project("m1", m_branch);
        RooDataHist data("data", "data sample",  *x,  m1);
        _wspace->import(data);
        delete m1;
        return;
    }
    else {
        cout << "Inf::You choose the unbined fit method" << endl;
        RooDataSet data("data", "data sample", t, *x);
        _wspace->import(data);

        // TH1F* h1 = new TH1F("h1","h1",m_bins ,m_low,m_high);
        // t->Project("h1",m_branch,m_cut);
        // RooDataHist data("data","data sample", *x, h1 );
        _wspace->import(data);
        return;
    }
}
void QuickFit::Fit(RooWorkspace *_wspace) {
    RooAbsPdf *model = (RooAbsPdf*)_wspace->pdf("model");
    if (m_method != "unbinned") {
        RooDataSet *data = (RooDataSet*)_wspace->data("data");
        _result = model->fitTo(*data, Save(kTRUE),  Extended(kTRUE));
        _result->Print();
        for (int i = 0; i < 1E2; i++) {
            double edm = _result->edm();
            cout << "Inf: fit " << i << " times" << endl << i << " times" << endl;
            _result->randomizePars();
            int covQual = _result->covQual();
            int status = _result->status();
            // int numStatus = result->numStatusHistory();
            // int MIGRAD = result->statusCodeHistory(0);
            // int HESSE = result->statusCodeHistory(1);
            if (covQual == 3 && status == 0) {
                break;
            }
            _result = model->fitTo(*data, Save(kTRUE),  Extended());
            _result->Print();
        }
    }
    else {
        RooDataHist *data = (RooDataHist*)_wspace->data("data");
        for (int i = 0; i < 1E2; i++) {
            _result = model->fitTo(*data, Save(kTRUE),  Extended());
            double edm = _result->edm();
            _result->randomizePars();
            int covQual = _result->covQual();
            int status = _result->status();
            if (covQual == 3 && status == 0) {
                break;
            }
            cout << "Inf: fit " << i << " times" << endl << i << " times" << endl;
            _result->Print();
            // model->fitTo(*data,Save(kTRUE), Extended())->Print();
        }
        // _result = model->fitTo(*data,Save(kTRUE), Extended() );
        // _result->Print();
        return;
    }
}
void  QuickFit::FitToData(TString  method) {
    m_method = method;
    RooWorkspace *myWs = new RooWorkspace("myWs");
    cout << "Inf::start to fit !" << endl;
    // gSystem->Load("libRooFit") ;
    AddSignalModel(myWs);
    myWs -> Print();
    AddBackgroundModel(myWs);
    myWs -> Print();
    AddData(myWs);
    myWs ->Print();
    AddModel(myWs);
    myWs ->Print();
    Fit(myWs);
    MakePlots(myWs);
    cout << "Inf: plot success" << endl;
    Export(myWs);
    cout << "Inf: export success" << endl;
    _result->Print();
    delete myWs;
}

void QuickFit::SetUnit(TString unit) {
    m_unit = unit;
}
void QuickFit::SetFigureName(TString epsname) {
    m_figname = epsname;
}
void QuickFit::SetSignalArea(double low , double hig) {
    m_signallow = low;
    m_signalhig = hig;
}
void QuickFit::MakePlots(RooWorkspace *_wspace) {
    PlotFormat plotForm;
    plotForm.SetStyle();
    RooRealVar *x = _wspace->var(m_branch);
    RooAbsPdf *model = (RooAbsPdf*)_wspace->pdf("model");
    RooDataSet *data = (RooDataSet*)_wspace->data("data");

    RooPlot *frame = x->frame(m_bins);

    data->plotOn(frame, MarkerColor(1));

    model->plotOn(frame,
            LineColor(kBlue),
            LineStyle(1),
            LineWidth(2) );

    // plot Components
    if (m_bkgShape != "None") {
       model->plotOn(frame,
               Components(m_sigpdfName),
               LineColor(kRed),
                LineWidth(2),
               LineStyle(kDashed) );

        model->plotOn(frame,
                Components("bkgpdf"),
                LineColor(kGreen),
                LineWidth(2),
                LineStyle(2) );
    }

    frame->SetMaximum(frame->GetMaximum()/0.74);
    frame->SetMinimum(0.0001);
    PlotTitle(_wspace, frame);

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

    frame->Draw();
    TPaveText pt(0.6, 0.7, 0.88, 0.88, "blNDC");
    PlotPaveText(pt, _wspace, frame);
//pt.Draw();

    c1->SaveAs(m_figname);
    this->GenerateCxx(_wspace, frame);
   
    delete c1;
    return;
}


void QuickFit::GenerateCxx(RooWorkspace *_wspace, RooPlot* frame) {
    system("echo 'make plot'");
    TString outcxx = "plot_" + m_sigpdfName + ".cxx";
    TString com1 = "cat ";
    com1 += QuickPath"/test/plot_test.cxx >" + outcxx;
    system(com1);
    cout << com1 << endl;
    system("sed -i 's/SigPdfName/" + m_sigpdfName + "/g' " + outcxx);
    system("sed -i 's/output.root/" + m_output + "/g' " + outcxx);
    cout << "sed -i 's/SigPdfName/" + m_sigpdfName + "/g' " + outcxx << endl;
    
    system("sed -i 's/val/" + m_branch + "/g' " + outcxx);
    system("sed -i 's/m_sigpdfName/" + m_sigpdfName + "/g' " + outcxx);
    system("sed -i 's/m_sigpdfName/" + m_sigpdfName + "/g' " + outcxx);
    if (m_bkgShape == "None") {
        system("sed -i 's/true/false/g' " + outcxx);
    }
    double chisq;
    int ndf = 100;
    calculateChisq(frame, chisq, ndf);
    // system("sed -i 's/chisq/" + to_string(chisq) + "/g' " + outcxx);
    // system("sed -i 's/ndf/" + to_string(ndf) + "/g' " + outcxx);
    // cout << "sed -i 's/chisq/" + to_string(chisq) + "/g' " + outcxx << endl;
    // cout << "sed -i 's/ndf/" + to_string(ndf) + "/g' " + outcxx << endl;
    
    double mean[2], sigma[2];
    getSigmaAndMean(_wspace, mean, sigma);
    system("sed -i 's/m_meanErr/" + to_string(mean[1]) + "/g' " + outcxx);
    system("sed -i 's/m_sigmaErr/" + to_string(sigma[1]) + "/g' " + outcxx);
    system("sed -i 's/m_mean/" + to_string(mean[0]) + "/g' " + outcxx);
    system("sed -i 's/m_sigma/" + to_string(sigma[0]) + "/g' " + outcxx);
    cout << "sed -i 's/m_meanErr/" + to_string(mean[1]) + "/g' " + outcxx << endl;
    cout << "sed -i 's/m_sigmaErr/" + to_string(sigma[1]) + "/g' " + outcxx << endl;
    cout << "sed -i 's/m_mean/" + to_string(mean[0]) + "/g' " + outcxx << endl;
    cout << "sed -i 's/m_sigma/" + to_string(sigma[0]) + "/g' " + outcxx << endl;
    
    cout << "Signal region: [ " << x_low << " , " << x_high << " ]" << endl;
    system("sed -i 's/m_unit/" + m_unit + "/g' " + outcxx);
    cout << "sed -i 's/m_unit/" + m_unit + "/g' " + outcxx << endl;
    system("sed -i 's/cname/c" + m_sigpdfName + "/g' " + outcxx);
    cout << "sed -i 's/cname/c" + m_sigpdfName + "/g' " + outcxx << endl;
    return;
}


void QuickFit::PlotPaveText(TPaveText &pt, RooWorkspace *_wspace, RooPlot *frame) {
    // chisq
    double chisq;
    int ndf = 100;
    calculateChisq(frame, chisq, ndf);
    char par_chi[100];
    sprintf(par_chi, "#chi^{2}= %.1f/%d =%1.1f ",
            chisq,  ndf, chisq/(ndf));


    // number of events
    TString Q;
    // get mean and sigma
    double _mean[2], _sigma[2];
    getSigmaAndMean(_wspace, _mean, _sigma);
    cout << "Inf:: mean: [ " << _mean[0]  << "," << _mean[1]  << "]" << endl;
    cout << "If:: sigma: [ " << _sigma[0] << "," << _sigma[1] << "]" << endl;

    char par_mean[100], par_sigma[100];
    sprintf(par_mean, "mean = %.3e #pm %.3e",  _mean[0],  _mean[1]);
    sprintf(par_sigma, "#sigma = %.3e #pm %.3e",  _sigma[0],  _sigma[1]);
    if (m_unit == "GeV") {
        sprintf(par_mean, "mean= %7.4f #pm %1.4f GeV",
                _mean[0], _mean[1]);
        sprintf(par_sigma, "#sigma= %4.4f #pm %1.4f GeV",
                _sigma[0], _sigma[1]);
    }
    else if (m_unit == "MeV") {
        cout << "Inf:: unit is " << m_unit << endl;
        sprintf(par_mean, "mean= %.3f #pm %.3f MeV",
                _mean[0]*1000, _mean[1]*1000);
        sprintf(par_sigma, "#sigma= %.3f #pm %.3f MeV",
                _sigma[0]*1000, _sigma[1]*1000);
    }

    // draw signal area
    x_low = _mean[0] + m_signallow * _sigma[0];
    x_high = _mean[0] + m_signalhig * _sigma[0];
    TArrow* arrowl = new TArrow(x_low, frame->GetMaximum()/10., x_low, 1, 0.01, "|>");
    TArrow* arrowr = new TArrow(x_high, frame->GetMaximum()/10., x_high, 1, 0.01, "|>");
    arrowl->SetLineColor(kRed);
    arrowr->SetLineColor(kRed);
    arrowl->Draw();
    arrowr->Draw();
    RooRealVar *nsig = _wspace->var("nsig");
    RooRealVar *nbkg = _wspace->var("nbkg");
    char par_nsig[100];
    sprintf(par_nsig, "#sig= %7.0f #pm %5.0f", nsig->getVal(), nsig->getError());
    char par_nbkg[100];
    sprintf(par_nbkg, "#bkg= %7.0f #pm %5.0f", nbkg->getVal(),  nbkg->getError());
    RooRealVar *x = _wspace->var(m_branch);
    x->setRange("bounds", x_low, x_high);
    RooAbsPdf *sigpdf = _wspace->pdf(m_sigpdfName);
    RooAbsPdf *bkgpdf = _wspace->pdf("bkgpdf");

    RooAbsReal* yield = sigpdf->createIntegral(*x, NormSet(*x), Range("bounds"));
    RooAbsReal* bkgnum = bkgpdf->createIntegral(*x, NormSet(*x), Range("bounds"));
    double ns = nsig->getVal();
    double nb = nbkg->getVal();
    double comp1 = yield->getVal();
    double comp2 = bkgnum->getVal();
    sprintf(par_nsig, "#sig=%.0f#pm%.0f", ns*comp1, nsig->getError());
    sprintf(par_nbkg, "#bkg=%.0f#pm%.0f", nb*comp2, nbkg->getError());
    pt.SetTextFont(132);
    pt.SetTextAlign(12);  // zuo dui qi
    pt.SetBorderSize(1);
    pt.SetFillColor(0);
    if (m_bkgShape != "None") {
        pt.AddText(par_chi);
        pt.AddText(par_nsig);
        pt.AddText(par_nbkg);
    }
    else {
        pt.AddText(par_mean);
        pt.AddText(par_sigma);
    }
    // pt.Draw();
    return;
}
void QuickFit::PlotTitle(RooWorkspace *_wspace, RooPlot *frame) {
    char  ytitle[100];
    sprintf(ytitle, "Entries /%.1e",  (m_high-m_low)/m_bins);
    if (m_unit == "GeV") {
        cout << "Inf:: unit is " << m_unit << endl;
        sprintf(ytitle, "Events /%2.4f GeV",  (m_high-m_low)/m_bins);
    }
    else if (m_unit == "MeV") {
        cout << "Inf:: unit is " << m_unit << endl;
        sprintf(ytitle, "Events /%.2f MeV",  (m_high-m_low)/m_bins*1000);
    }

    frame->SetYTitle(ytitle);
    frame->SetXTitle(m_xtitle);
    PlotFormat plotForm;
    plotForm.Axis(frame->GetXaxis());
    plotForm.Axis(frame->GetYaxis());
    TGaxis* xaxis = (TGaxis*)frame->GetYaxis();
    xaxis->SetMaxDigits(3);
    return;
}
void QuickFit::Export(RooWorkspace *_wspace) {
    RooRealVar *x = _wspace->var(m_branch);
    RooAbsPdf *sigpdf = (RooAbsPdf*) _wspace->pdf(m_sigpdfName);
    RooAbsPdf *model = (RooAbsPdf*) _wspace->pdf("model");
    RooAbsPdf *bkgpdf = (RooAbsPdf*) _wspace->pdf("bkgpdf");
    // sigpdf->SetName(m_sigpdfName);
    RooArgSet *para = model ->getVariables();
    TIterator *iter = para->createIterator();
    RooRealVar *par(0);
    while(0 !=(par =(RooRealVar*) iter->Next())) {
        cout << "Name:\t" << par->GetName()
             << " value:\t" << par->getVal() << endl;
        par->setConstant();
    }
    // _wspace->writeToFile(m_output);
    TFile *f = new TFile(m_output, "recreate");
    _wspace->Write();
    sigpdf->SetName(m_sigpdfName);
    sigpdf->Write(m_sigpdfName);
    bkgpdf->Write();
    f->Close();
    delete f;
    delete iter;
}


void QuickFit::calculateChisq(RooPlot* frame,
        double& m_chisq, int& m_ndf) {
    RooCurve *curve = (RooCurve*)frame->getObject(1);
    RooHist *histo = reinterpret_cast<RooHist*>(frame->getObject(0));
    double x[1000], y[1000];
    double xa[1000], ya[1000];
    double y_fit[1000], x_fit[1000];
    double tmp_y = 0;
    double eyl[1000], eyh[1000], exl[1000], exh[1000];
    int ndf = 0;
    for (int i = 0; i < histo->GetN(); i++) {
        histo->GetPoint(i, x[i], y[i]);
        eyl[i] = histo->GetEYlow()[i];
        eyh[i] = histo->GetEYhigh()[i];
        exl[i] = histo->GetEXlow()[i];
        exh[i] = histo->GetEXhigh()[i];
    }
    for (int i = 0; i < histo->GetN(); i++) {
        tmp_y += y[i];
        if ( tmp_y < 10 ) continue;
        xa[ndf] = x[i];
        ya[ndf] = tmp_y;
        tmp_y = 0;
        ndf++;
    }
    if ( tmp_y != 0 ) {
        xa[ndf] = x[histo->GetN()-1];
        ya[ndf] = tmp_y;
    }
    double xstart, xstop, ystart, ystop;
    curve->GetPoint(0, xstart, ystart);
    curve->GetPoint(curve->GetN()-1, xstop, ystop);
    for (int i = 0; i < histo->GetN(); i++) {
        if (x[i] > xstop || x[i] < xstart ) continue;
        x_fit[i] = x[i];
        y_fit[i] = curve->average(x[i]-exl[i], x[i]+exh[i]);
    }

    double y_fb[1000];
    int num = 0;
    for (int i=0; i < histo->GetN(); i++) {
        if ( y[i] < 1 ) continue;

        if ( x[i] <= xa[num] ) {
            y_fb[num] += y_fit[i];
        } else {
            num++;
            y_fb[num] += y_fit[i];
        }
    }
    double tx[1000]={0};
    double tx_err[1000]={0};
    double ty[1000]={0};
    double chi_err[1000]={0};
    double chisq = 0;

    for (int tk = 0; tk != histo->GetN(); tk++) {
        tx[tk] = x[tk];
        if (y[tk] == 0) continue;
        if (y[tk] > y_fit[tk]) {
            ty[tk]=(y[tk] - y_fit[tk])/eyl[tk];
        } else {
            ty[tk]=(y[tk] - y_fit[tk])/eyh[tk];
        }
        if (fabs(ty[tk]) < 10.) chisq += ty[tk] * ty[tk];
    }
    m_chisq = chisq;
    m_ndf = ndf;
}

void QuickFit::getSigmaAndMean(RooWorkspace *_wspace, double *_mean,
        double *_sigma) {
    // sigPdf->Print("v");
    RooRealVar *x = _wspace->var(m_branch);
    RooAbsPdf *sigPdf = _wspace->pdf(m_sigpdfName);
    // if the m_sigOrder is one, we can read the mean and sigma,
    // directly
    if (m_sigOrder == 1) {
        RooRealVar *meanVar = _wspace->var("mean1_" + m_sigpdfName);
        RooRealVar *sigamVar = _wspace->var("sigma1_" + m_sigpdfName);
        _mean[0] = meanVar->getVal();
        _mean[1] = meanVar->getError();
        _sigma[0] = sigamVar->getVal();
        _sigma[1] = sigamVar->getError();
        return;
    }

    //
    const int nTimes = 1000;
    double * Means = new double[nTimes];
    double * Sigmas = new double[nTimes];
    double MaxMean, MinMean, MaxSigma, MinSigma;
    TIterator *itr;
    for (int i = 0 ; i < nTimes; i++) {
        RooArgList check = _result->randomizePars();
        itr = check.createIterator();
        RooRealVar *par(0);
        while ((par = (RooRealVar*) itr->Next()) !=0) {
            _wspace->var(par->GetName())->setVal(par->getVal());
        }
        RooDataSet* data_toy = sigPdf->generate(*x, 1E5);
        Means[i] = data_toy->mean(*x);
        Sigmas[i] = data_toy->sigma(*x);
        if (Means[i] > MaxMean) MaxMean = Means[i];
        if (Means[i] < MinMean) MinMean = Means[i];
        if (Sigmas[i] > MaxSigma) MaxSigma = Sigmas[i];
        if (Sigmas[i] < MinSigma) MinSigma = Sigmas[i];
    }
    delete itr;
    TH1D h1("h1", "",  40,  MinMean -0.05*(MaxMean - MinMean),
            MaxMean + 0.05 * (MaxMean - MinMean));
    TH1D h2("h2", "",  40,  MinSigma -0.05*(MaxSigma - MinSigma),
            MaxSigma + 0.05 * (MaxSigma - MinSigma));
    for (int i = 0 ; i < nTimes;  i++) {
        h1.Fill(Means[i]);
        h2.Fill(Sigmas[i]);
    }

    // get mean and sigma
    _mean[0] = h1.GetMean();
    _mean[1] = h1.GetRMS();
    _sigma[0] = h2.GetMean();
    _sigma[1] = h2.GetRMS();
    delete Means;
    delete Sigmas;

    return;
}


void QuickFit::SetBins(int n) {
    m_bins = n;
}


double QuickFit::Chisq(RooAbsPdf *totFit, RooAbsData *data) {
    std::vector<double>  ndata, nfit;
    //this->CheckMin(ndata, nfit, 10);
}
