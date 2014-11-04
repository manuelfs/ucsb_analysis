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

#define NSam 6

using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[], TString folder, TString LegNames[]);

class hfeats {
public:
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, vector<int> isamples,
	 TString ititle="", TString icuts="1", float icut=-1){
    varname = ivarname; nbins = inbins; minx = iminx; maxx = imaxx; title = ititle;
    cuts = icuts; cut = icut; samples = isamples;
    tag = ivarname+"_"+cuts; 
    if(cuts.Contains(">=1.479")) tag += "Endcap";
    if(cuts.Contains("<1.479")) tag += "Barrel";
    int cmax(tag.Index("Max$")), c15(tag.Index(">=15"));
    if(cmax>=0 && c15>=0) tag.Remove(cmax, c15+4-cmax);
    int cmin(tag.Index("Min$(")), c10000(tag.Index("10000")), ceta(tag.Index("+(abs("));
    if(cmin>=0){
      tag.Remove(ceta, c10000+6-ceta);
      tag.ReplaceAll("Min$(","");
    }
    tag.ReplaceAll("_1",""); tag.ReplaceAll(".",""); 
    tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")",""); 
    tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); tag.ReplaceAll("|",""); 
    tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
    tag.ReplaceAll(">","g"); tag.ReplaceAll("<","s"); tag.ReplaceAll("=","");
    tag.ReplaceAll("+",""); 
  }
  TString title, varname, tag, cuts;
  int nbins;
  float minx, maxx, cut;
  vector<int> samples;
};

//void hltdistributions(TString folder="root/hlt/oct24_720pre8/") { 
void hltdistributions(TString folder="root/hlt/nov2_noseedgt/") { 
  styles style("Standard"); style.nDivisions = 708; style.setDefaultStyle(); gStyle->SetPadTickY(1); 
  vector<hfeats> vars;
  TCanvas can;
  int inilep(0), endlep(2);

  vector<int> allsamples;
  allsamples.push_back(0); //(1025,625)
  allsamples.push_back(1); //(1200,800)
  allsamples.push_back(2); //(1500,100)
  allsamples.push_back(5); //Wjets
  allsamples.push_back(3); //QCD
  allsamples.push_back(4); //tt

  vector<int> qcd_tt_sig;
  qcd_tt_sig.push_back(0); //(1025,625)
  qcd_tt_sig.push_back(3); //QCD
  qcd_tt_sig.push_back(4); //tt

  vector<int> qcd_tt_w_sig;
  qcd_tt_w_sig.push_back(2); //(1500,100)
  qcd_tt_w_sig.push_back(0); //(1025,625)
  qcd_tt_w_sig.push_back(5); //Wjets
  qcd_tt_w_sig.push_back(3); //QCD
  qcd_tt_w_sig.push_back(4); //tt

  vector<int> mini_sig;
  mini_sig.push_back(1); //(1200,800)
  mini_sig.push_back(2); //(1500,100)

    // vars.push_back(hfeats("Min$(els_trackiso+(els_pt<15)*10000+(abs(els_eta)>1.479)*10000)",21,0,0.21,qcd_tt_w_sig,
    // 			  "Electron track relative isolation","Max$(els_pt*(abs(els_eta)<1.479))>=15",0.05));
    // vars.push_back(hfeats("Min$(els_trackiso+(els_pt<15)*10000+(abs(els_eta)<=1.479)*10000)",21,0,0.21,qcd_tt_w_sig,
    // 			  "Electron track relative isolation","Max$(els_pt*(abs(els_eta)>=1.479))>=15",0.05));

    // vars.push_back(hfeats("Min$(els_ecaliso+(els_pt<15)*10000+(abs(els_eta)>1.479)*10000)",33,0,0.66,qcd_tt_w_sig,
    // 			  "Electron ECAL relative isolation","Max$(els_pt*(abs(els_eta)<1.479))>=15",0.16));
    // vars.push_back(hfeats("Min$(els_ecaliso+(els_pt<15)*10000+(abs(els_eta)<=1.479)*10000)",49,0,0.49,qcd_tt_w_sig,
    // 			  "Electron ECAL relative isolation","Max$(els_pt*(abs(els_eta)>=1.479))>=15",0.12));

    // vars.push_back(hfeats("Min$(els_hcaliso+(els_pt<15)*10000+(abs(els_eta)>1.479)*10000)",41,0,0.82,qcd_tt_w_sig,
    //  			  "Electron HCAL relative isolation","Max$(els_pt*(abs(els_eta)<1.479))>=15",0.2));
    // vars.push_back(hfeats("Min$(els_hcaliso+(els_pt<15)*10000+(abs(els_eta)<=1.479)*10000)",41,0,1.23,qcd_tt_w_sig,
    // 			  "Electron HCAL relative isolation","Max$(els_pt*(abs(els_eta)>=1.479))>=15",0.3));

    // vars.push_back(hfeats("Min$(mus_reliso+(mus_pt<15)*10000+(abs(mus_eta)>1.479)*10000)",51,0,1.02,qcd_tt_w_sig,
    //  			  "Muon total relative isolation","Max$(mus_pt*(abs(mus_eta)<1.479))>=15",0.15));
    // vars.push_back(hfeats("Min$(mus_reliso+(mus_pt<15)*10000+(abs(mus_eta)<=1.479)*10000)",51,0,1.02,qcd_tt_w_sig,
    //  			  "Muon total relative isolation","Max$(mus_pt*(abs(mus_eta)>=1.479))>=15",0.15));

  /////////// With Electron ID /////////////////
  vars.push_back(hfeats("Min$(els_ecaliso+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_he>=0.15||els_eminusp>=0.012||els_deta>=0.005||els_dphi>=0.03)*10000)",33,0,0.66,qcd_tt_w_sig,
  			"Electron ECAL relative isolation","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03))>=15",0.16));
  vars.push_back(hfeats("Min$(els_ecaliso+(abs(els_eta)<1.479||els_clustershape>=0.033||els_he>=0.20||els_eminusp>=0.009||els_deta>=0.010||els_dphi>=0.03)*10000)",33,0,0.66,qcd_tt_w_sig,
  			"Electron ECAL relative isolation","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03))>=15",0.16));

  vars.push_back(hfeats("Min$(els_hcaliso+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_he>=0.15||els_eminusp>=0.012||els_deta>=0.005||els_dphi>=0.03)*10000)",41,0,0.82,qcd_tt_w_sig,
  			"Electron HCAL relative isolation","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03))>=15",0.2));
  vars.push_back(hfeats("Min$(els_hcaliso+(abs(els_eta)<1.479||els_clustershape>=0.033||els_he>=0.20||els_eminusp>=0.009||els_deta>=0.010||els_dphi>=0.03)*10000)",41,0,1.23,qcd_tt_w_sig,
  			"Electron HCAL relative isolation","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03))>=15",0.3));

  vars.push_back(hfeats("Min$(els_clustershape+(abs(els_eta)>=1.479||els_he>=0.15||els_eminusp>=0.012||els_deta>=0.005||els_dphi>=0.03)*10000)",60,0,0.03,qcd_tt_w_sig,
  			"Electron #sigmai#eta#eta","Max$(els_pt*(abs(els_eta)<1.479&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03))>=15",0.011));
  vars.push_back(hfeats("Min$(els_clustershape+(abs(els_eta)<1.479||els_he>=0.20||els_eminusp>=0.009||els_deta>=0.010||els_dphi>=0.03)*10000)",70,0,0.07,qcd_tt_w_sig,
  			"Electron #sigmai#eta#eta","Max$(els_pt*(abs(els_eta)>=1.479&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03))>=15",0.033));

  vars.push_back(hfeats("Min$(els_he+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_eminusp>=0.012||els_deta>=0.005||els_dphi>=0.03)*10000)",31,0,0.155,qcd_tt_w_sig,
  			"Electron H/E","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_eminusp<0.012&&els_deta<0.005&&els_dphi<0.03))>=15"));
  vars.push_back(hfeats("Min$(els_he+(abs(els_eta)<1.479||els_clustershape>=0.033||els_eminusp>=0.009||els_deta>=0.010||els_dphi>=0.03)*10000)",41,0,0.205,qcd_tt_w_sig,
  			"Electron H/E","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_eminusp<0.009&&els_deta<0.010&&els_dphi<0.03))>=15"));
    
  vars.push_back(hfeats("Min$(els_eminusp+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_he>=0.15||els_deta>=0.005||els_dphi>=0.03)*10000)",25, 0, 0.0125,qcd_tt_w_sig,
  			"Electron 1/E-1/p (GeV^{-1})","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_he<0.15&&els_deta<0.005&&els_dphi<0.03))>=15"));
  vars.push_back(hfeats("Min$(els_eminusp+(abs(els_eta)<1.479||els_clustershape>=0.033||els_he>=0.20||els_deta>=0.010||els_dphi>=0.03)*10000)",19, 0, 0.0095,qcd_tt_w_sig,
  			"Electron 1/E-1/p (GeV^{-1})","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_he<0.20&&els_deta<0.010&&els_dphi<0.03))>=15"));

  vars.push_back(hfeats("Min$(els_deta+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_he>=0.15||els_eminusp>=0.012||els_dphi>=0.03)*10000)",21, 0, 0.00525,qcd_tt_w_sig,
  			"Electron #Delta#eta","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_dphi<0.03))>=15"));
  vars.push_back(hfeats("Min$(els_deta+(abs(els_eta)<1.479||els_clustershape>=0.033||els_he>=0.20||els_eminusp>=0.009||els_dphi>=0.03)*10000)",21,  0, 0.0105,qcd_tt_w_sig,
  			"Electron #Delta#eta","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_dphi<0.03))>=15"));

  vars.push_back(hfeats("Min$(els_dphi+(abs(els_eta)>=1.479||els_clustershape>=0.011||els_he>=0.15||els_eminusp>=0.012||els_deta>=0.005)*10000)",31, 0, 0.031,qcd_tt_w_sig,
  			"Electron #Delta#phi","Max$(els_pt*(abs(els_eta)<1.479&&els_clustershape<0.011&&els_he<0.15&&els_eminusp<0.012&&els_deta<0.005))>=15"));
  vars.push_back(hfeats("Min$(els_dphi+(abs(els_eta)<1.479||els_clustershape>=0.033||els_he>=0.20||els_eminusp>=0.009||els_deta>=0.010)*10000)",31, 0, 0.031,qcd_tt_w_sig,
  			"Electron #Delta#phi","Max$(els_pt*(abs(els_eta)>=1.479&&els_clustershape<0.033&&els_he<0.20&&els_eminusp<0.009&&els_deta<0.010))>=15"));


  // Variables only in root/hlt/sep10/ht200
  if(folder.Contains("ht200")){
    // vars.push_back(hfeats("mindr_mu",25,0,2.5, qcd_tt_sig, "Minimum #DeltaR(#mu,jet)"));
    // vars.push_back(hfeats("mindr2_mu",30,0,6, qcd_tt_sig, "2nd minimum #DeltaR(#mu,jet)"));
    // vars.push_back(hfeats("mindr2_mu",30,0,6, qcd_tt_sig, "2nd minimum #DeltaR(#mu,jet)","Sum$(jets_pt>40)>=5"));
    vars.push_back(hfeats("Max$(genels_pt)",29,5,150,mini_sig,"Leading gen e p_{T} (GeV)"));
    vars.push_back(hfeats("Max$(genmus_pt)",29,5,150,mini_sig,"Leading #mu gen p_{T} (GeV)"));
  } else {
    // vars.push_back(hfeats("onht",50,0,2500, allsamples, "HLT H_{T} (GeV)"));
    // vars.push_back(hfeats("onmet",50,0,750, allsamples,"HLT MET (GeV)"));
    // vars.push_back(hfeats("abs(jets_phi[0]-jets_phi[1])*(abs(jets_phi[0]-jets_phi[1])<3.1416)+(2*3.1416-abs(jets_phi[0]-jets_phi[1]))*(abs(jets_phi[0]-jets_phi[1])>=3.1416)",63,0,3.15, qcd_tt_sig, "#Delta#phi(jet1,jet2)"));
    // vars.push_back(hfeats("Sum$(jets_pt>40)",16,-0.5,15.5, allsamples, "Number of jets with p_{T} > 40 GeV","onht>400"));
    // vars.push_back(hfeats("Sum$(jets_pt>60)",16,-0.5,15.5, allsamples, "Number of jets with p_{T} > 60 GeV","onht>400"));

    // vars.push_back(hfeats("Max$(els_pt)",29,5,150,qcd_tt_sig,"Leading e p_{T} (GeV)"));
    // vars.push_back(hfeats("Max$(els_pt)",29,5,150,qcd_tt_sig,"Leading e p_{T} (GeV)","iso5x"));
    // vars.push_back(hfeats("Min$(els_reliso+(els_pt<15)*10000)",50,0,1.7,qcd_tt_w_sig,"Electron total relative isolation","1",0.36));
  
    // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,qcd_tt_sig,"Leading #mu HLT p_{T} (GeV)"));
    // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,qcd_tt_sig,"Leading #mu HLT p_{T} (GeV)","iso5x"));





  }


  double legX = 0.52, legY = 0.92;
  double legW = 0.12, legH = 0.061*5;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TLine line; line.SetLineColor(28); line.SetLineWidth(2); line.SetLineStyle(2);
  TString eliso5x = "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15";
  TString muiso5x = "Max$(mus_pt*(mus_reliso<1))>15";

  int hcolors[] = {8,28,6,1,4,2};
  int hstyles[] = {2,2,2,1,1,1};
  int hwidths[] = {4,4,4,2,2,2};

  vector< vector<TH1D*> > histo[2];
  vector<TH1D*> varhisto;
  TChain *chain[NSam];
  TString lepfolders[] = {"el15", "mu15"}, currentfolder, legnames[NSam];
  TString hname, pname, variable, leghisto, totCut, title;
  for(int lep(inilep); lep<endlep; lep++){
    currentfolder = folder + "/" + lepfolders[lep];
    ReadChains(chain, currentfolder, legnames);
    for(unsigned var(0); var<vars.size(); var++){
      title = vars[var].cuts; if(title=="1") title = "";
      if(title.Contains(">=1.479")) title += "Endcap";
      if(title.Contains("<1.479")) title += "Barrel";
      int cmax(title.Index("Max$")), c15(title.Index(">=15"));
      if(cmax>=0 && c15>=0) title.Remove(cmax, c15+4-cmax);
      title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
      title.ReplaceAll("Sum$(jets_pt>40)","n_{jets}^{40}");title.ReplaceAll("abs(lep_id)==13&&","");
      title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
      title.ReplaceAll("onmet", "MET"); title.ReplaceAll("onht", "H_{T}"); 
      if(lepfolders[lep].Contains("mu")) title.ReplaceAll("iso5x", "RelIso < 1");
      if(lepfolders[lep].Contains("el")) title.ReplaceAll("iso5x", "RelIso < 1.7");
      leg.Clear();
      float maxhisto(-999);

      varhisto.resize(0);
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	hname = "histo"; hname += lep; hname += var; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[lep].push_back(varhisto);
      if((vars[var].varname.Contains("els")||vars[var].varname.Contains("11")) 
	 && !lepfolders[lep].Contains("el")) continue;
      if((vars[var].varname.Contains("mu")||vars[var].varname.Contains("13"))  
	 && !lepfolders[lep].Contains("mu")) continue;

      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	variable = vars[var].varname; // Assigned here to change it for signal
	histo[lep][var][sam]->SetLineColor(hcolors[vars[var].samples[sam]]);
	histo[lep][var][sam]->SetLineStyle(hstyles[vars[var].samples[sam]]);
	histo[lep][var][sam]->SetLineWidth(hwidths[vars[var].samples[sam]]);
	totCut = "weight*("+vars[var].cuts+")";
	if(legnames[vars[var].samples[sam]].Contains("T1tttt")){
	  if(lepfolders[lep].Contains("el")) {
	    totCut.ReplaceAll("els_pt*(","els_pt*(els_genpt>=0&&");
	    variable.ReplaceAll("1.479","1.479||els_genpt<0");
	  } else {
	    totCut.ReplaceAll("mus_pt*(","els_pt*(mus_genpt>=0&&");
	    variable.ReplaceAll("1.479","1.479&&mus_genpt>=0");
	  }
	}      
	if(lepfolders[lep].Contains("el")) totCut.ReplaceAll("iso5x", eliso5x);
	else 	totCut.ReplaceAll("iso5x", muiso5x);
	chain[vars[var].samples[sam]]->Project(histo[lep][var][sam]->GetName(), variable, totCut);
	//if(variable.Contains("iso"))
	  histo[lep][var][sam]->SetBinContent(vars[var].nbins,
					      histo[lep][var][sam]->GetBinContent(vars[var].nbins)+
					      histo[lep][var][sam]->GetBinContent(vars[var].nbins+1));
	
	float histo_integral = histo[lep][var][sam]->Integral();
	if(histo_integral) histo[lep][var][sam]->Scale(100./histo_integral);
	if(sam==0){
	  histo[lep][var][sam]->SetXTitle(vars[var].title);
	  histo[lep][var][sam]->SetYTitle("Entries (%)");
	  histo[lep][var][sam]->Draw();
	} else histo[lep][var][sam]->Draw("same");
	if(maxhisto < histo[lep][var][sam]->GetMaximum()) maxhisto = histo[lep][var][sam]->GetMaximum();
	leghisto = legnames[vars[var].samples[sam]]+" [#mu = ";
	int digits(3);
	if(variable.Contains("iso")) digits = 2;
	leghisto += RoundNumber(histo[lep][var][sam]->GetMean(),digits) + "]";
	leg.AddEntry(histo[lep][var][sam], leghisto);
      } // Loop over samples
      histo[lep][var][0]->SetMaximum(maxhisto*1.1);
      legH = 0.061*vars[var].samples.size(); leg.SetY1NDC(legY-legH);
      leg.Draw(); 
      if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*1.1);
      pname = "plots/hltstudy/"+lepfolders[lep]+"_"+vars[var].tag+".eps";
      can.SaveAs(pname);
    }// Loop over variables
  }// Loop over el15 and mu15

  for(int lep(inilep); lep<endlep; lep++)
    for(unsigned var(0); var<vars.size(); var++){
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
  	if(histo[lep][var][sam]) histo[lep][var][sam]->Delete();
    }
}

void ReadChains(TChain *chain[], TString folder, TString LegNames[]){
  TString FileNames[] = 
    {folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1200_*PU20*", 
     folder+"/*T1tttt*1500_*PU20*", 
     folder+"/*QCD*",
     //folder+"/*QCD*300to*",
     folder+"/*TT*",
     folder+"/W*"};
  LegNames[0] = "T1tttt(1025,625)";
  LegNames[1] = "T1tttt(1200,800)";
  LegNames[2] = "T1tttt(1500,100)";
  LegNames[3] = "QCD";
  LegNames[4] = "t#bar{t}";
  LegNames[5] = "W#rightarrowl#nu";

  for(int sam(0); sam < NSam; sam++){
    //cout<<"Adding "<<FileNames[sam]<<endl;
    chain[sam] = new TChain("tree");
    chain[sam]->Add(FileNames[sam]);
  }// Loop over samples
}
