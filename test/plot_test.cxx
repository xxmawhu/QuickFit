using namespace RooFit;
using namespace std;

void calculateChisq(RooPlot *frame, double &m_chisq, int &m_ndf) {
    RooCurve *curve = (RooCurve *)frame->getObject(1);
    RooHist *histo = reinterpret_cast<RooHist *>(frame->getObject(0));
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
        if (tmp_y < 10) continue;
        xa[ndf] = x[i];
        ya[ndf] = tmp_y;
        tmp_y = 0;
        ndf++;
    }
    if (tmp_y != 0) {
        xa[ndf] = x[histo->GetN() - 1];
        ya[ndf] = tmp_y;
    }
    double xstart, xstop, ystart, ystop;
    curve->GetPoint(0, xstart, ystart);
    curve->GetPoint(curve->GetN() - 1, xstop, ystop);
    for (int i = 0; i < histo->GetN(); i++) {
        if (x[i] > xstop || x[i] < xstart) continue;
        x_fit[i] = x[i];
        y_fit[i] = curve->average(x[i] - exl[i], x[i] + exh[i]);
    }

    double y_fb[1000];
    int num = 0;
    for (int i = 0; i < histo->GetN(); i++) {
        if (y[i] < 1) continue;

        if (x[i] <= xa[num]) {
            y_fb[num] += y_fit[i];
        } else {
            num++;
            y_fb[num] += y_fit[i];
        }
    }
    double tx[1000] = {0};
    double tx_err[1000] = {0};
    double ty[1000] = {0};
    double chi_err[1000] = {0};
    double chisq = 0;

    for (int tk = 0; tk != histo->GetN(); tk++) {
        tx[tk] = x[tk];
        if (y[tk] == 0) continue;
        if (y[tk] > y_fit[tk]) {
            ty[tk] = (y[tk] - y_fit[tk]) / eyl[tk];
        } else {
            ty[tk] = (y[tk] - y_fit[tk]) / eyh[tk];
        }
        if (fabs(ty[tk]) < 10.) chisq += ty[tk] * ty[tk];
    }
    m_chisq = chisq;
    m_ndf = ndf;
}

void FormatAxis(TAxis *axis) {
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

void SetStyle() {
    // No Canvas Border
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    // White BG
    gStyle->SetCanvasColor(10);
    // PlotFormat for axes
    gStyle->SetLabelFont(42, "xyz");
    gStyle->SetLabelSize(0.04, "xyz");
    gStyle->SetLabelOffset(0.02, "y");
    gStyle->SetLabelOffset(0.02, "x");
    gStyle->SetNdivisions(510, "xyz");
    gStyle->SetTitleFont(42, "xyz");
    gStyle->SetTitleColor(1, "xyz");
    gStyle->SetTitleSize(0.09, "xyz");
    gStyle->SetTitleOffset(1.35, "x");
    gStyle->SetTitleOffset(1.2, "y");
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

void plot_SigPdfName() {
    SetStyle();
    TFile *f = new TFile("output.root", "read");
    RooWorkspace *_wspace = (RooWorkspace *)f->Get("myWs");
    RooRealVar *x = _wspace->var("val");
    RooAbsPdf *model = (RooAbsPdf *)_wspace->pdf("model");
    RooDataSet *data = (RooDataSet *)_wspace->data("data");

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    RooPlot *frame = x->frame(20);
    data->plotOn(frame, MarkerColor(1));
    model->plotOn(frame, LineColor(kBlue), LineStyle(1), LineWidth(2));

    // plot Components
    bool plot_sig_and_bkg = true;
    if (plot_sig_and_bkg) {
        model->plotOn(frame, Components("m_sigpdfName"), LineColor(kRed),
                      LineWidth(2), LineStyle(kDashed));

        model->plotOn(frame, Components("bkgpdf"), LineColor(kGreen),
                      LineWidth(2), LineStyle(2));
    }

    // format the frame
    frame->SetMaximum(frame->GetMaximum() / 0.78);
    frame->SetMinimum(0.0001);
    FormatAxis(frame->GetXaxis());
    FormatAxis(frame->GetYaxis());
    frame->GetYaxis()->SetTitleOffset(1.2);
    frame->Draw();

    // draw signal area
    double mean = m_mean, meanErr = m_meanErr;
    double sigma = m_sigma, sigmaErr = m_sigmaErr;
    double x_low = mean - 3 * sigma;
    double x_high = mean + 3 * sigma;
    TArrow *arrowl =
        new TArrow(x_low, frame->GetMaximum() / 10., x_low, 1, 0.01, "|>");
    TArrow *arrowr =
        new TArrow(x_high, frame->GetMaximum() / 10., x_high, 1, 0.01, "|>");
    arrowl->SetLineColor(kRed);
    arrowr->SetLineColor(kRed);
    arrowl->Draw();
    arrowr->Draw();
    // add a pt to show the fit parameter and signal yields
    TPaveText *pt = new TPaveText(0.6, 0.7, 0.88, 0.88, "blNDC");
    pt->SetTextFont(132);
    pt->SetTextAlign(12);
    pt->SetBorderSize(1);
    pt->SetFillColor(0);
    char par_nsig[100], par_nbkg[100], par_chi[100];
    RooRealVar *nsig = _wspace->var("nsig");
    RooRealVar *nbkg = _wspace->var("nbkg");

    // get the signal yields and background in the signal region
    RooRealVar *x = _wspace->var("val");
    x->setRange("bounds", x_low, x_high);
    RooAbsPdf *sigpdf = _wspace->pdf("m_sigpdfName");
    RooAbsPdf *bkgpdf = _wspace->pdf("bkgpdf");
    RooAbsReal *yield =
        sigpdf->createIntegral(*x, NormSet(*x), Range("bounds"));
    RooAbsReal *bkgnum =
        bkgpdf->createIntegral(*x, NormSet(*x), Range("bounds"));
    sprintf(par_nsig, "#sig= %7.0f #pm %5.0f", nsig->getVal() * yield->getVal(),
            nsig->getError() * yield->getVal());
    sprintf(par_nbkg, "#bkg= %7.0f #pm %5.0f",
            nbkg->getVal() * bkgnum->getVal(),
            nbkg->getError() * bkgnum->getVal());

    double chisq;
    int ndf;
    calculateChisq(frame, chisq, ndf);
    sprintf(par_chi, "#chi^{2}= %.1f/%d =%1.1f ", chisq, ndf, chisq / ndf);

    char par_mean[100], par_sigma[100];
    sprintf(par_mean, "mean = %.3e #pm %.3e", mean, meanErr);
    sprintf(par_sigma, "#sigma = %.3e #pm %.3e", sigma, sigmaErr);
    TString unit = "m_unit";
    if (unit == "GeV") {
        sprintf(par_mean, "mean= %7.4f #pm %1.4f GeV", mean, meanErr);
        sprintf(par_sigma, "#sigma= %4.4f #pm %1.4f GeV", sigma, sigmaErr);
    } else if (unit == "MeV") {
        sprintf(par_mean, "mean= %7.4f #pm %1.4f MeV", 1000 * mean,
                1000 * meanErr);
        sprintf(par_sigma, "#sigma= %4.4f #pm %1.4f MeV", 1000 * sigma,
                1000 * sigmaErr);
    }
    pt->AddText(par_chi);
    pt->AddText(par_nsig);
    pt->AddText(par_nbkg);
    pt->AddText(par_mean);
    pt->AddText(par_sigma);

    pt->Draw();

    c1->Print("cname.eps");
    c1->Print("cname.png");
    return;
}
