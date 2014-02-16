//----------------------------------------------------------------------------
// styles.cpp description:
//      styles - Class to set default plotting styles, read from a text file
//  
// Author List:
//      Manuel Franco Sevilla                     Stanford University/UCSB
//      Wells Wulsin                              Stanford University
//      Jochen Dingfelder                         SLAC
//
// History:
//      14/02/15  manuelf -- Got rid of RooFit dependences
//      10/10/22  manuelf -- Adapted to Donut's needs
//      10/08/09  wulsin  -- Copied from SetStyles.C.  Made into a proper class.  
//----------------------------------------------------------------------------

#ifndef STYLES_HH
#define STYLES_HH

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TPad.h"
#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TROOT.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using std::cout;
using std::endl;

class styles {
public: 
  styles();  
  void setPadsStyle(int numberPads = 1);
  void testGlobalStyle(bool fixY = true, float scale = 1000.); 
  void setGlobalStyle();
  void applyStyle();
  void printValues();
  void fixYAxis(TH1 *h, TPad *pad);
  void styleHist(TH1 *h, Int_t color = 1, Int_t fillstyle = 0,
		 Int_t symbol = 8,Double_t size = 0.7, Int_t width = 1);
  void setMarkers(TH1 *h, float Msize=0.6, int Mstyle=20) ;
  void setTitles(TH1 *h, TString xTitle="", TString yTitle="", TString Left="", TString Right="");
  void setTitleSizes(TH1 *h,  float size, float lsize, int font=62, 
		     float xoff=1., float yoff=1., int divisions=405);
  void parseConfFile(TString group, TString fnames[], float *fvalues[], int nFloat, 
		     TString inames[], int *ivalues[], int nInt);

  TString confFile;
  int nFont, nPads, nDivisions;
  int CanvasW, CanvasH;
  float TextSize, TitleSize, LabelSize, xTitleOffset, yTitleOffset;
  float PadRightMargin, PadBottomMargin, PadLeftMargin, PadTopMargin;
};

styles::styles() {
  confFile = "txt/plot_configuration.txt";
  TString inames[] = {"nFont", "nDivisions"};
  int *ivalues[] = {&nFont, &nDivisions};
  parseConfFile("General", 0, 0, 0, inames, ivalues, 2);
  nPads      = 1;
}

// Default styles for each possible number of pads
// The values of PadLeftMargin and yTitleOffset are set in between the optimal
// values for 4 and 3 digits in the y axis
void styles::setPadsStyle(int numberPads) {
  nPads = abs(numberPads);
  TString Section = "Pads_"; Section += nPads;
  if(numberPads<0) Section += "b";

  TString inames[] = {"CanvasW", "CanvasH"};
  TString fnames[] = {"TextSize", "TitleSize", "LabelSize", "PadRightMargin", "PadTopMargin", "PadBottomMargin",
		      "xTitleOffset", "PadLeftMargin", "yTitleOffset"};
  int   *ivalues[] = {&CanvasW, &CanvasH};
  float *fvalues[] = {&TextSize,&TitleSize,&LabelSize,&PadRightMargin,&PadTopMargin,&PadBottomMargin,
		      &xTitleOffset,&PadLeftMargin,&yTitleOffset};
  parseConfFile(Section, fnames, fvalues, 9, inames, ivalues, 2);
}

// ----------------------------------------------------------------------
void styles::fixYAxis(TH1 *h, TPad *pad){
  float maxi = h->GetMaximum()*1.15;
  int digits = (int)(log(maxi)/log(10.)+0.001)+1;
  if(digits<2) digits = 2;
  TString Section = "Pads_"; Section += nPads;
  Section += "_Digits_"; Section += digits;  
  TString fnames[] = {"PadLeftMargin", "yTitleOffset"};
  float *fvalues[] = {&PadLeftMargin, &yTitleOffset};
  parseConfFile(Section, fnames, fvalues, 2, 0, 0, 0);

  h->SetTitleOffset(yTitleOffset,"y");
  pad->SetLeftMargin(PadLeftMargin);
}

// Test the global style settings for a generic histogram.  
void styles::testGlobalStyle(bool fixY, float scale) {
  
  setPadsStyle(nPads); setGlobalStyle(); applyStyle();
  
  TH1* h = new TH1F("h", "h", 50, 0, 50);
  TH1* hc[6];
  for (int i=1; i<=50; i++) {
    double value = scale*exp(-0.5*pow(((i-25.)/5.),2));  // Gaussian shape
    h->SetBinContent(i, value);
  }

  TCanvas c;
  if(nPads == 2) c.Divide(2);
  if(nPads == 3) c.Divide(3);
  if(nPads == 4) c.Divide(2,2);
  if(nPads == 6) c.Divide(3,2);
  TPad *cPad = (TPad *)c.cd(1); h->Draw();
  if(fixY) fixYAxis(h,cPad);
  setTitles(h, "D^{(*)0/+} channels", "xlabel^{2}_{miss} (GeV^{2})", "Events/(10 MeV^{2})");
  float scales[] = {0.1, 10, 0.01};
  for(int pads = 2; pads<=4; pads++){
    if(nPads>=pads){
      cPad = (TPad *)c.cd(pads); 
      hc[pads-2] = (TH1F*)h->Clone();
      hc[pads-2]->Scale(scales[pads-2]); 
      if(fixY) fixYAxis(hc[pads-2],cPad);
      hc[pads-2]->Draw();
      setTitles(hc[pads-2], "D^{(*)0/+} channels", "xlabel^{2}_{miss} (GeV^{2})", "Events/(1000 MeV^{2})");
    }
  }
  TString epsName = "babar_code/styles/Plot_"; epsName += nPads; epsName += "Pads.eps";
  c.Print(epsName);
  
}

void styles::applyStyle() {
  setGlobalStyle();
  gStyle->SetCanvasDefW(CanvasW);
  gStyle->SetCanvasDefH(CanvasH);
  gStyle->SetTextSize(TextSize);            // Set global text size
  gStyle->SetTitleSize(TitleSize,"xy");     // Set the 2 axes title size
  gStyle->SetLabelSize(LabelSize,"xy");     // Set the 2 axes label size

  gStyle->SetTitleOffset(xTitleOffset,"x");     
  gStyle->SetTitleOffset(yTitleOffset,"y");     
  gStyle->SetPadRightMargin (PadRightMargin);    
  gStyle->SetPadBottomMargin(PadBottomMargin); 
  gStyle->SetPadTopMargin(PadTopMargin); 
  gStyle->SetPadLeftMargin  (PadLeftMargin); 
  gStyle->SetNdivisions(nDivisions, "xy");   // 5 primary ticks and 4 secondary ticks

  gStyle->SetTitleFont(nFont,"xy");          // Set the all 2 axes title font
  gStyle->SetLabelFont(nFont,"xy");          // Set the all 2 axes label font
  gStyle->SetTextFont(nFont);                // Set global text font
}

// Set default styles globally.   
void styles::setGlobalStyle() {
  gStyle->SetPalette(1);              // Decent colors for 2D plots
  gStyle->SetOptStat(0);              // No Stats box
  gStyle->SetPadTickX(0);             // No ticks at the right
  gStyle->SetPadTickY(0);             // No ticks at the top
}

void styles::printValues() {
  cout<<"nFont           = " << nFont           << endl;
  cout<<"nPads           = " << nPads           << endl;
  cout<<"nDivisions      = " << nDivisions      << endl;
  cout<<"CanvasW         = " << CanvasW         << endl;   
  cout<<"CanvasH         = " << CanvasH         << endl;   
  cout<<"TextSize        = " << TextSize        << endl;   
  cout<<"TitleSize       = " << TitleSize       << endl;
  cout<<"LabelSize       = " << LabelSize       << endl;
  cout<<"PadRightMargin  = " << PadRightMargin  << endl;
  cout<<"PadTopMargin    = " << PadTopMargin    << endl;
  cout<<"PadBottomMargin = " << PadBottomMargin << endl;
  cout<<"xTitleOffset    = " << xTitleOffset    << endl;
  cout<<"PadLeftMargin   = " << PadLeftMargin   << endl;
  cout<<"yTitleOffset    = " << yTitleOffset    << endl;

}

// ----------------------------------------------------------------------
void styles::setMarkers(TH1 *h, float Msize, int Mstyle) {
  h->SetMarkerStyle(Mstyle);
  h->SetMarkerSize(Msize);
}

// ----------------------------------------------------------------------
void styles::setTitles(TH1 *h, TString xTitle, TString yTitle, TString Left, TString Right) {
  if (0==h) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetTitle(""); h->SetXTitle(xTitle); h->SetYTitle(yTitle);
    TLatex label; label.SetNDC(kTRUE);
    label.SetTextAlign(13);
    label.DrawLatex(PadLeftMargin+0.04,1-PadTopMargin-0.03,Left);  
    label.SetTextAlign(33);
    label.DrawLatex(1-PadRightMargin-0.02,1-PadTopMargin-0.03,Right);  
 }
}

// ----------------------------------------------------------------------
void styles::styleHist(TH1 *h, Int_t color, Int_t fillstyle,
		       Int_t symbol, Double_t size, Int_t width) {
  h->SetLineColor(color);   
  h->SetLineWidth(width);
  h->SetMarkerColor(color); 
  h->SetMarkerStyle(symbol);  
  h->SetMarkerSize(size); 
  h->SetStats(kFALSE); 
  h->SetFillStyle(fillstyle); 
  h->SetFillColor(color);
}

// ----------------------------------------------------------------------
void styles::setTitleSizes(TH1 *h,  float size, float lsize, int font,
			   float xoff, float yoff, int divisions) {
  if (0==h) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size,   "x");      h->SetTitleSize(size,   "y");
    h->SetLabelSize(lsize,  "x");      h->SetLabelSize(lsize,  "y");
    h->SetLabelFont(font,   "x");      h->SetLabelFont(font,   "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(divisions,"X");   h->SetNdivisions(divisions,   "Y");
  }
}

void styles::parseConfFile(TString group, TString fnames[], float *fvalues[], int nFloat, 
			   TString inames[], int *ivalues[], int nInt){
  ifstream file(confFile);
  TString word, s_value;
  while(file >> word){
    if(word.Contains("[")) {
      word.ReplaceAll("[",""); word.ReplaceAll("]",""); 
      if(word == group){
	while(file >> word){
	  if(word.Contains("[")) break;
	  file >> s_value; file >> s_value;
	  for(int var(0); var < nFloat; var++){
	    if(word == fnames[var]){
	      *fvalues[var] = s_value.Atof();
	      break;
	    }
	  } // Loop finding requested float variables
	  for(int var(0); var < nInt; var++){
	    if(word == inames[var]){
	      *ivalues[var] = s_value.Atoi();
	      break;
	    }
	  } // Loop finding requested int variables
	} // Loop over group variables
	break;
      }
    }
  } // Loop over all words
}


#endif	/* STYLES_HH */
