#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"

#define NHis 3

using namespace std;
using std::cout;
using std::endl;

void compare(vector<TString> chainname, 
	     TString variable, int nbins, float minX, float maxX, 
	     TString title, TString xtitle, TString tag, vector<TString> leglabel, 
	     bool dolog=false, TString cuts="weight", TString treename="tree") { 
  styles style("Standard"); style.PadTopMargin = 0.07; style.setDefaultStyle();
  TCanvas can;

  double legX = 0.15, legY = 0.93;
  double legW = 0.12, legH = 0.061*NHis;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  float nentries[NHis], maxhisto(0);
  int color[] = {4, 2, 8};
  vector<TChain*> chain;
  vector<TH1D*> histo;
  TString hname, totCut;
  for(unsigned his(0); his<chainname.size(); his++){
    hname = "histo"; hname += his;
    histo.push_back(new TH1D(hname,"",nbins, minX, maxX));
    histo[his]->SetLineWidth(3);
    histo[his]->SetLineColor(color[his]);
    histo[his]->SetTitle(title);
    histo[his]->SetXTitle(xtitle);
    histo[his]->SetYTitle("HLT rate [Hz] for L = 1.4 #times 10^{34} cm^{-2}s^{-1}");
    // histo[his]->Sumw2();

    totCut = "1.4e-2/19600*weight*(" + cuts + ")";
    chain.push_back(new TChain(treename));
    chain[his]->Add(chainname[his]);
    chain[his]->Project(hname, variable, totCut);

    nentries[his] = histo[his]->Integral(0,nbins+1);
    //leglabel[his] += " [" + RoundNumber(nentries[his],0) + "]";
    leg.AddEntry(histo[his], leglabel[his]);
    if(histo[his]->GetMaximum() > maxhisto) maxhisto = histo[his]->GetMaximum();
  }
  for(unsigned his(0); his<chainname.size(); his++){
    if(his==0) {
      histo[his]->SetMaximum(maxhisto*1.3);
      if(dolog) histo[his]->SetMaximum(maxhisto*100);
      histo[his]->Draw();
    } else histo[his]->Draw("same");
  }
  leg.Draw();

  // histo[0]->SetMaximum(1.5);
  // histo[0]->Divide(histo[1]);
  // histo[0]->Draw();

  can.SetLogy(dolog);
  TString pname = "plots/" + tag + ".eps";
  can.SaveAs(pname);
  for(unsigned his(0); his<chainname.size(); his++) histo[his]->Delete();
}

void compare2(){

  vector<TString> el15_qcd;
  el15_qcd.push_back("root/hlt/noiso/el15/*QCD*");
  el15_qcd.push_back("root/hlt/oct10/el15/*QCD*");
  el15_qcd.push_back("root/hlt/oct24_720pre8/el15/*QCD*");
  vector<TString> el15_sig;
  el15_sig.push_back("root/hlt/noiso/el15/*1025*");
  el15_sig.push_back("root/hlt/oct10/el15/*1025*");
  el15_sig.push_back("root/hlt/oct24_720pre8/el15/*1025*");
  vector<TString> legnames;
  legnames.push_back("CMSSW_7_1_6");
  legnames.push_back("CMSSW_7_2_0_pre6, auto:startup_GRun");
  legnames.push_back("CMSSW_7_2_0_pre8, MCRUN2_72_V1A::All");

  // QCD el15
  compare(el15_qcd,"els_eta", 80, -3, 3, "15 GeV non-isolated HLT e, QCD", "All e HLT eta", 
	  "els_eta_el15_qcd", legnames);
  compare(el15_qcd,"jets_eta", 80, -5, 5, "15 GeV non-isolated HLT e, QCD", "All jets HLT eta", 
	  "jets_eta_el15_qcd", legnames);
  compare(el15_qcd, "els_eta", 80, -3, 3, "15 GeV isolated HLT e, QCD", "All e HLT eta", "els_eta_isoel15_qcd", 
  	  legnames, false, 
	  "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15");

  // T1tttt(1025,625) el15
  compare(el15_sig,"els_eta", 50, -3, 3, "15 GeV non-isolated HLT e, T1tttt(1025,625)", "All e HLT eta", 
	  "els_eta_el15_sig", legnames);

  compare(el15_sig, "onht", 50, 0, 1500, "15 GeV non-isolated HLT e, T1tttt(1025,625)", "HLT PF H_{T} (GeV)", 
	  "ht_el15_sig", legnames);

  vector<TString> mu15_qcd;
  mu15_qcd.push_back("root/hlt/noiso/mu15/*QCD*");
  mu15_qcd.push_back("root/hlt/oct10/mu15/*QCD*");

  // QCD mu15
  compare(mu15_qcd,"mus_eta", 80, -3, 3, "15 GeV non-isolated HLT #mu, QCD", "All #mu HLT eta", 
	  "mus_eta_mu15_qcd", legnames);
  compare(mu15_qcd,"jets_eta", 80, -5, 5, "15 GeV non-isolated HLT #mu, QCD", "All jets HLT eta", 
	  "jets_eta_mu15_qcd", legnames);

}
