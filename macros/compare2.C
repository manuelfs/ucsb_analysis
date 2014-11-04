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
	     TString cuts="1", bool dolog=false, TString treename="tree") { 
  styles style("Standard"); style.PadTopMargin = 0.07; style.setDefaultStyle(); gStyle->SetPadTickY(1); 
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

    totCut = "1.4e-2/19600*weight*(" + cuts + "&&els_genpt>=0)";
    //totCut = "1.4e-2/19600*weight*(" + cuts + ")";
    chain.push_back(new TChain(treename));
    chain[his]->Add(chainname[his]);
    //cout<<totCut<<endl;
    chain[his]->Project(hname, variable, totCut);

    histo[his]->SetBinContent(nbins, histo[his]->GetBinContent(nbins)+
    			      histo[his]->GetBinContent(nbins+1));

    nentries[his] = histo[his]->Integral(0,nbins+1);
    //leglabel[his] += " [" + RoundNumber(nentries[his],0) + "]";
    leglabel[his] += " [#mu = " + RoundNumber(histo[his]->GetMean(),3) + "]";
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

  histo[0]->SetMinimum(0);
  // histo[0]->SetMaximum(1.5);
  // histo[0]->Divide(histo[1]);
  // histo[0]->Draw();

  can.SetLogy(dolog);
  TString pname = "plots/" + tag + ".eps";
  can.SaveAs(pname);
  for(unsigned his(0); his<chainname.size(); his++) histo[his]->Delete();
}

void compare2(){

  // vector<TString> qcd;
  // qcd.push_back("root/hlt/nov2_noseed/el15/QCD_Pt-120to170_Tune4C*");
  // qcd.push_back("root/hlt/nov2_noseedgt/el15/QCD_Pt-120to170_Tune4C*");
  // vector<TString> legnames;
  // legnames.push_back("720 unseeded, auto:upgradePLS1");
  // legnames.push_back("720 unseeded, MCRUN2_72_V1A::All");

  // compare(qcd,"onht", 100, 0, 1500, "15 GeV non-isolated HLT e, QCD", 
  // 	  "H_{T} (GeV)", "qcd_onht_el15_noseed", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)||abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  vector<TString> signal;
  signal.push_back("root/hlt/oct30_716elid/el15/*1500_*");
  signal.push_back("root/hlt/oct30_720elid/el15/*1500_*");
  signal.push_back("root/hlt/nov2_noseedgt/el15/*1500_*");
  vector<TString> legnames;
  legnames.push_back("716");
  legnames.push_back("720pre8");
  legnames.push_back("720 unseeded");

  compare(signal,"els_clustershape", 60, 0, 0.03, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", 
  	  "#sigmai#eta#eta", "signal_els_clustershape_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_clustershape", 70, 0, 0.07, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", 
  	  "#sigmai#eta#eta", "signal_els_clustershape_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  compare(signal,"els_he", 100, 0, 1, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "H/E", 
  	  "signal_els_he_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_he", 100, 0, 1, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "H/E", 
  	  "signal_els_he_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  compare(signal,"els_eminusp", 60, 0, 0.09, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "1/E-1/p (GeV^{-1})", 
  	  "signal_els_eminusp_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_eminusp", 60, 0, 0.09, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "1/E-1/p (GeV^{-1})", 
  	  "signal_els_eminusp_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_deta<0.010&&els_dphi<0.03)");


  compare(signal,"els_deta", 50, 0, 0.03, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "#Delta#eta", 
  	  "signal_els_deta_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_dphi<0.03)");
  compare(signal,"els_deta", 50, 0, 0.03, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "#Delta#eta", 
  	  "signal_els_deta_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_dphi<0.03)");


  compare(signal,"els_dphi", 50, 0, 0.15, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "#Delta#phi", 
  	  "signal_els_dphi_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005)");
  compare(signal,"els_dphi", 50, 0, 0.15, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "#Delta#phi", 
  	  "signal_els_dphi_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010)");


  compare(signal,"els_hcaliso", 50, 0, 1.5, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron HCAL relative isolation", 
  	  "signal_els_hcaliso_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_hcaliso", 50, 0, 1.5, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron HCAL relative isolation", 
  	  "signal_els_hcaliso_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");

  compare(signal,"els_ecaliso", 50, 0, 2, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron ECAL relative isolation", 
  	  "signal_els_ecaliso_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_ecaliso", 50, 0, 2, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron ECAL relative isolation", 
  	  "signal_els_ecaliso_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");

  compare(signal,"els_trackiso", 50, 0, 1, "Barrel, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron track relative isolation", 
  	  "signal_els_trackiso_elid_barrel", legnames,
  	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  compare(signal,"els_trackiso", 50, 0, 1, "Endcap, 15 GeV non-isolated HLT e, T1tttt(1500,100)", "Electron track relative isolation", 
  	  "signal_els_trackiso_elid_endcap", legnames,
  	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");

  // vector<TString> qcd;
  // qcd.push_back("root/hlt/oct30_716elid/el15/QCD_Pt-120to170_Tune4C*");
  // qcd.push_back("root/hlt/oct30_720elid/el15/QCD_Pt-120to170_Tune4C*");
  // qcd.push_back("root/hlt/nov2_noseedgt/el15/QCD_Pt-120to170_Tune4C*");
  // vector<TString> legnames;
  // legnames.push_back("716");
  // legnames.push_back("720pre8");
  // legnames.push_back("720 unseeded");

  // compare(qcd,"els_clustershape", 60, 0, 0.03, "Barrel, 15 GeV non-isolated HLT e, QCD", 
  // 	  "#sigmai#eta#eta", "qcd_els_clustershape_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_clustershape", 70, 0, 0.07, "Endcap, 15 GeV non-isolated HLT e, QCD", 
  // 	  "#sigmai#eta#eta", "qcd_els_clustershape_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  // compare(qcd,"els_he", 100, 0, 1, "Barrel, 15 GeV non-isolated HLT e, QCD", "H/E", 
  // 	  "qcd_els_he_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_he", 100, 0, 1, "Endcap, 15 GeV non-isolated HLT e, QCD", "H/E", 
  // 	  "qcd_els_he_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  // compare(qcd,"els_eminusp", 60, 0, 0.09, "Barrel, 15 GeV non-isolated HLT e, QCD", "1/E-1/p (GeV^{-1})", 
  // 	  "qcd_els_eminusp_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_eminusp", 60, 0, 0.09, "Endcap, 15 GeV non-isolated HLT e, QCD", "1/E-1/p (GeV^{-1})", 
  // 	  "qcd_els_eminusp_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_deta<0.010&&els_dphi<0.03)");


  // compare(qcd,"els_deta", 50, 0, 0.03, "Barrel, 15 GeV non-isolated HLT e, QCD", "#Delta#eta", 
  // 	  "qcd_els_deta_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_dphi<0.03)");
  // compare(qcd,"els_deta", 50, 0, 0.03, "Endcap, 15 GeV non-isolated HLT e, QCD", "#Delta#eta", 
  // 	  "qcd_els_deta_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_dphi<0.03)");


  // compare(qcd,"els_dphi", 50, 0, 0.15, "Barrel, 15 GeV non-isolated HLT e, QCD", "#Delta#phi", 
  // 	  "qcd_els_dphi_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005)");
  // compare(qcd,"els_dphi", 50, 0, 0.15, "Endcap, 15 GeV non-isolated HLT e, QCD", "#Delta#phi", 
  // 	  "qcd_els_dphi_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010)");


  // compare(qcd,"els_hcaliso", 50, 0, 1.5, "Barrel, 15 GeV non-isolated HLT e, QCD", "Electron HCAL relative isolation", 
  // 	  "qcd_els_hcaliso_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_hcaliso", 50, 0, 1.5, "Endcap, 15 GeV non-isolated HLT e, QCD", "Electron HCAL relative isolation", 
  // 	  "qcd_els_hcaliso_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");

  // compare(qcd,"els_ecaliso", 50, 0, 2, "Barrel, 15 GeV non-isolated HLT e, QCD", "Electron ECAL relative isolation", 
  // 	  "qcd_els_ecaliso_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_ecaliso", 50, 0, 2, "Endcap, 15 GeV non-isolated HLT e, QCD", "Electron ECAL relative isolation", 
  // 	  "qcd_els_ecaliso_elid_endcap", legnames,
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");

  // compare(qcd,"els_trackiso", 50, 0, 1, "Barrel, 15 GeV non-isolated HLT e, QCD", "Electron track relative isolation", 
  // 	  "qcd_els_trackiso_elid_barrel", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)");
  // compare(qcd,"els_trackiso", 50, 0, 1, "Endcap, 15 GeV non-isolated HLT e, QCD", "Electron track relative isolation", 
  // 	  "qcd_els_trackiso_elid_endcap", legnames,  
  // 	  "abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");




  // compare(qcd,"els_eta", 80, -3, 3, "15 GeV non-isolated HLT e, QCD", "All e HLT eta", 
  // 	  "qcd_els_eta_elid", legnames,
  // 	  "abs(els_eta)<1.479&&(els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03)||abs(els_eta)>=1.479&&(els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03)");


  // vector<TString> tt;
  // tt.push_back("../ra4_code/archive/14-10-18/*TTJet*");
  // vector<TString> legnames;
  // legnames.push_back("TTJets_MSDecaysCKM_central_Tune4C_13TeV");
  // compare(tt,"(mc_type&0x000F) + (mc_type&0xF00)/0x100 - (mc_type&0x00F0)/0x10", 3, -0.5, 2.5, 
  // 	  "MET > 125 GeV", "Number of e/#mu/#tau from W", "nlep_tt", legnames, false, "met>125");
  // vector<TString> el15_qcd;
  // el15_qcd.push_back("root/hlt/noiso/el15/*QCD*");
  // el15_qcd.push_back("root/hlt/oct10/el15/*QCD*");
  // el15_qcd.push_back("root/hlt/oct24_720pre8/el15/*QCD*");
  // vector<TString> el15_sig;
  // el15_sig.push_back("root/hlt/noiso/el15/*1025*");
  // el15_sig.push_back("root/hlt/oct10/el15/*1025*");
  // el15_sig.push_back("root/hlt/oct24_720pre8/el15/*1025*");
  // vector<TString> legnames;
  // legnames.push_back("CMSSW_7_1_6");
  // legnames.push_back("CMSSW_7_2_0_pre6, auto:startup_GRun");
  // legnames.push_back("CMSSW_7_2_0_pre8, MCRUN2_72_V1A::All");

  // // QCD el15
  // compare(el15_qcd,"els_eta", 80, -3, 3, "15 GeV non-isolated HLT e, QCD", "All e HLT eta", 
  // 	  "els_eta_el15_qcd", legnames);
  // compare(el15_qcd,"jets_eta", 80, -5, 5, "15 GeV non-isolated HLT e, QCD", "All jets HLT eta", 
  // 	  "jets_eta_el15_qcd", legnames);
  // compare(el15_qcd, "els_eta", 80, -3, 3, "15 GeV isolated HLT e, QCD", "All e HLT eta", "els_eta_isoel15_qcd", 
  // 	  legnames, false, 
  // 	  "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15");

  // // T1tttt(1025,625) el15
  // compare(el15_sig,"els_eta", 50, -3, 3, "15 GeV non-isolated HLT e, T1tttt(1025,625)", "All e HLT eta", 
  // 	  "els_eta_el15_sig", legnames);

  // compare(el15_sig, "onht", 50, 0, 1500, "15 GeV non-isolated HLT e, T1tttt(1025,625)", "HLT PF H_{T} (GeV)", 
  // 	  "ht_el15_sig", legnames);

  // vector<TString> mu15_qcd;
  // mu15_qcd.push_back("root/hlt/noiso/mu15/*QCD*");
  // mu15_qcd.push_back("root/hlt/oct10/mu15/*QCD*");

  // // QCD mu15
  // compare(mu15_qcd,"mus_eta", 80, -3, 3, "15 GeV non-isolated HLT #mu, QCD", "All #mu HLT eta", 
  // 	  "mus_eta_mu15_qcd", legnames);
  // compare(mu15_qcd,"jets_eta", 80, -5, 5, "15 GeV non-isolated HLT #mu, QCD", "All jets HLT eta", 
  // 	  "jets_eta_mu15_qcd", legnames);

}
