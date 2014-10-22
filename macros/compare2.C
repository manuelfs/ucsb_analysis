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

#define NHis 2

using namespace std;
using std::cout;
using std::endl;

void compare(TString ntuple1, TString ntuple2, TString variable, int nbins, float minX, float maxX, 
	     TString title, TString xtitle, TString tag, TString leg1, TString leg2, 
	     bool dolog=false, TString cuts="weight", TString treename="tree") { 
  styles style("Standard"); style.PadTopMargin = 0.07; style.setDefaultStyle();
  TCanvas can;

  double legX = 0.15, legY = 0.93;
  double legW = 0.12, legH = 0.061*NHis;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  float nentries[NHis], maxhisto(0);
  int color[] = {4, 2};
  TChain *chain[NHis];
  TString chainname[] = {ntuple1, ntuple2}, leglabel[] = {leg1, leg2};
  TH1D *histo[NHis];
  TString hname;
  for(int his(0); his<NHis; his++){
    hname = "histo"; hname += his;
    histo[his] = new TH1D(hname,"",nbins, minX, maxX);
    histo[his]->SetLineWidth(3);
    histo[his]->SetLineColor(color[his]);
    histo[his]->SetTitle(title);
    histo[his]->SetXTitle(xtitle);
    histo[his]->Sumw2();

    chain[his] = new TChain(treename);
    chain[his]->Add(chainname[his]);
    chain[his]->Project(hname, variable, cuts);

    nentries[his] = histo[his]->Integral(0,nbins+1);
    //leglabel[his] += " [" + RoundNumber(nentries[his],0) + "]";
    leg.AddEntry(histo[his], leglabel[his]);
    if(histo[his]->GetMaximum() > maxhisto) maxhisto = histo[his]->GetMaximum();
  }
  for(int his(0); his<NHis; his++){
    if(his==0) {
      histo[his]->SetMaximum(maxhisto*1.3);
      if(dolog) histo[his]->SetMaximum(maxhisto*100);
      histo[his]->Draw();
    } else histo[his]->Draw("same");
  }
  leg.Draw();

  can.SetLogy(dolog);
  TString pname = "plots/" + tag + ".eps";
  can.SaveAs(pname);
  for(int his(0); his<NHis; his++) histo[his]->Delete();
}

void compare2(){
  compare("root/hlt/noiso/mu15/*QCD*", "root/hlt/oct10/mu15/*QCD*", "mus_eta", 80, -3, 3, 
  	  "15 GeV non-isolated HLT #mu, QCD", "All #mu HLT eta", "mus_eta_qcd", 
  	  "CMSSW_7_1_6", "CMSSW_7_2_0_pre6, bad GT");
  compare("root/hlt/noiso/el15/*QCD*", "root/hlt/oct10/el15/*QCD*", "els_eta", 80, -3, 3, 
  	  "15 GeV non-isolated HLT e, QCD", "All e HLT eta", "els_eta_qcd", 
  	  "CMSSW_7_1_6", "CMSSW_7_2_0_pre6, bad GT");
  compare("root/hlt/noiso/mu15/*QCD*", "root/hlt/oct10/mu15/*QCD*", "jets_eta", 80, -5, 5, 
  	  "15 GeV non-isolated HLT #mu, QCD", "All PF jets HLT eta", "jets_eta_mu15_qcd", 
  	  "CMSSW_7_1_6", "CMSSW_7_2_0_pre6, bad GT");
  compare("root/hlt/noiso/el15/*QCD*", "root/hlt/oct10/el15/*QCD*", "jets_eta", 80, -5, 5, 
  	  "15 GeV non-isolated HLT e, QCD", "All PF jets HLT eta", "jets_eta_el15_qcd", 
  	  "CMSSW_7_1_6", "CMSSW_7_2_0_pre6, bad GT");
  // compare("root/hlt/noiso/el15/*QCD*", "root/hlt/oct10/el15/*QCD*", "onht", 80, 0, 1500, 
  // 	  "15 GeV non-isolated HLT e, QCD", "HLT PF H_{T} (GeV)", "onht_el15_qcd", 
  // 	  "CMSSW_7_1_6", "CMSSW_7_2_0_pre6, bad GT", true);

}
