#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include "TChain.h"
#include "TH1F.h"
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
    tag = ivarname+"_"+cuts; tag.ReplaceAll("_1",""); tag.ReplaceAll(".",""); 
    tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")",""); 
    tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); 
    tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
    tag.ReplaceAll(">",""); tag.ReplaceAll("<",""); tag.ReplaceAll("=","");
    tag.ReplaceAll("+",""); tag.ReplaceAll("s_pt1510000",""); 
  }
  TString title, varname, tag, cuts;
  int nbins;
  float minx, maxx, cut;
  vector<int> samples;
};

void hltdistributions(TString folder="root/hlt/noiso") { 
  styles style("Standard"); style.nDivisions = 708; style.setDefaultStyle();
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
  qcd_tt_w_sig.push_back(0); //(1025,625)
  qcd_tt_w_sig.push_back(5); //Wjets
  qcd_tt_w_sig.push_back(3); //QCD
  qcd_tt_w_sig.push_back(4); //tt

  vector<int> mini_sig;
  mini_sig.push_back(1); //(1200,800)
  mini_sig.push_back(2); //(1500,100)

  // Variables only in root/hlt/sep10/ht200
  if(folder.Contains("ht200")){
    // vars.push_back(hfeats("mindr_mu",25,0,2.5, qcd_tt_sig, "Minimum #DeltaR(#mu,jet)"));
    // vars.push_back(hfeats("mindr2_mu",30,0,6, qcd_tt_sig, "2nd minimum #DeltaR(#mu,jet)"));
    // vars.push_back(hfeats("mindr2_mu",30,0,6, qcd_tt_sig, "2nd minimum #DeltaR(#mu,jet)","Sum$(jets_pt>40)>=5"));
    vars.push_back(hfeats("Max$(genels_pt)",29,5,150,mini_sig,"Leading gen e p_{T} (GeV)"));
    vars.push_back(hfeats("Max$(genmus_pt)",29,5,150,mini_sig,"Leading #mu gen p_{T} (GeV)"));
  } else {
    vars.push_back(hfeats("onht",50,0,2500, allsamples, "HLT H_{T} (GeV)"));
    vars.push_back(hfeats("onmet",50,0,750, allsamples,"HLT MET (GeV)"));
    vars.push_back(hfeats("abs(jets_phi[0]-jets_phi[1])*(abs(jets_phi[0]-jets_phi[1])<3.1416)+(2*3.1416-abs(jets_phi[0]-jets_phi[1]))*(abs(jets_phi[0]-jets_phi[1])>=3.1416)",63,0,3.15, qcd_tt_sig, "#Delta#phi(jet1,jet2)"));
    vars.push_back(hfeats("Sum$(jets_pt>40)",16,-0.5,15.5, allsamples, "Number of jets with p_{T} > 40 GeV","onht>400"));
    vars.push_back(hfeats("Sum$(jets_pt>60)",16,-0.5,15.5, allsamples, "Number of jets with p_{T} > 60 GeV","onht>400"));

    vars.push_back(hfeats("Max$(els_pt)",29,5,150,qcd_tt_sig,"Leading e p_{T} (GeV)"));
    vars.push_back(hfeats("Max$(els_pt)",29,5,150,qcd_tt_sig,"Leading e p_{T} (GeV)","iso5x"));
    vars.push_back(hfeats("Min$(els_trackiso+(els_pt<15)*10000)",26,0,0.26,qcd_tt_w_sig,"Electron track relative isolation","1",0.05));
    vars.push_back(hfeats("Min$(els_ecaliso+(els_pt<15)*10000)",51,0,1.02,qcd_tt_w_sig,"Electron ECAL relative isolation","1",0.2));
    vars.push_back(hfeats("Min$(els_hcaliso+(els_pt<15)*10000)",56,0,0.56,qcd_tt_w_sig,"Electron HCAL relative isolation","1",0.11));
    vars.push_back(hfeats("Min$(els_reliso+(els_pt<15)*10000)",50,0,1.7,qcd_tt_w_sig,"Electron total relative isolation","1",0.36));
  
    vars.push_back(hfeats("Max$(mus_pt)",29,5,150,qcd_tt_sig,"Leading #mu HLT p_{T} (GeV)"));
    vars.push_back(hfeats("Max$(mus_pt)",29,5,150,qcd_tt_sig,"Leading #mu HLT p_{T} (GeV)","iso5x"));
    vars.push_back(hfeats("Min$(mus_reliso+(mus_pt<15)*10000)",51,0,1.02,qcd_tt_w_sig,"Muon total relative isolation","1",0.15));
  }


  double legX = 0.56, legY = 0.93;
  double legW = 0.12, legH = 0.061*2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TLine line; line.SetLineColor(28); line.SetLineWidth(2); line.SetLineStyle(2);
  TString eliso5x = "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15";
  TString muiso5x = "Max$(mus_pt*(mus_reliso<1))>15";

  int hcolors[] = {8,28,6,1,4,2};
  int hstyles[] = {2,2,2,1,1,1};
  int hwidths[] = {4,4,4,2,2,2};

  vector< vector<TH1F*> > histo[2];
  vector<TH1F*> varhisto;
  TChain *chain[NSam];
  TString lepfolders[] = {"el15", "mu15"}, currentfolder, legnames[NSam];
  TString hname, pname, variable, leghisto, totCut, title;
  for(int lep(inilep); lep<endlep; lep++){
    currentfolder = folder + "/" + lepfolders[lep];
    ReadChains(chain, currentfolder, legnames);
    for(unsigned var(0); var<vars.size(); var++){
      title = vars[var].cuts; if(title=="1") title = "";
      title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
      title.ReplaceAll("Sum$(jets_pt>40)","n_{jets}^{40}");title.ReplaceAll("abs(lep_id)==13&&","");
      title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
      title.ReplaceAll("onmet", "MET"); title.ReplaceAll("onht", "H_{T}"); 
      if(lepfolders[lep].Contains("mu")) title.ReplaceAll("iso5x", "RelIso < 1");
      if(lepfolders[lep].Contains("el")) title.ReplaceAll("iso5x", "RelIso < 1.7");
      leg.Clear();
      float maxhisto(-999);

      variable = vars[var].varname;
      varhisto.resize(0);
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	hname = "histo"; hname += lep; hname += var; hname += sam;
	varhisto.push_back(new TH1F(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[lep].push_back(varhisto);
      if((vars[var].varname.Contains("els")||vars[var].varname.Contains("11")) 
	 && !lepfolders[lep].Contains("el")) continue;
      if((vars[var].varname.Contains("mu")||vars[var].varname.Contains("13"))  
	 && !lepfolders[lep].Contains("mu")) continue;

      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	histo[lep][var][sam]->SetLineColor(hcolors[vars[var].samples[sam]]);
	histo[lep][var][sam]->SetLineStyle(hstyles[vars[var].samples[sam]]);
	histo[lep][var][sam]->SetLineWidth(hwidths[vars[var].samples[sam]]);
	totCut = "weight*("+vars[var].cuts+")";
	if(variable.Contains("iso") && vars[var].samples[sam]==0){
	  if(lepfolders[lep].Contains("el")) totCut = "weight*wl1ht200*(Max$(genels_pt[0])>0&&"+vars[var].cuts+")";
	  else totCut = "weight*wl1ht200*(Max$(genmus_pt[0])>0&&"+vars[var].cuts+")";
	}      
	if(lepfolders[lep].Contains("el")) totCut.ReplaceAll("iso5x", eliso5x);
	else 	totCut.ReplaceAll("iso5x", muiso5x);
	if(vars[var].samples[sam]==1||vars[var].samples[sam]==2){
	  if(variable == "Max$(mus_pt)") variable = "onmupt";
	  if(variable == "Max$(els_pt)") variable = "onelpt";
	}
	chain[vars[var].samples[sam]]->Project(histo[lep][var][sam]->GetName(), variable, totCut);
	if(variable.Contains("iso")){
	  histo[lep][var][sam]->SetBinContent(vars[var].nbins,
					      histo[lep][var][sam]->GetBinContent(vars[var].nbins)+
					      histo[lep][var][sam]->GetBinContent(vars[var].nbins+1));
	}
	float histo_integral = histo[lep][var][sam]->Integral();
	if(histo_integral) histo[lep][var][sam]->Scale(100./histo_integral);
	if(sam==0){
	  histo[lep][var][sam]->SetXTitle(vars[var].title);
	  histo[lep][var][sam]->SetYTitle("Entries (%)");
	  histo[lep][var][sam]->Draw();
	} else histo[lep][var][sam]->Draw("same");
	if(maxhisto < histo[lep][var][sam]->GetMaximum()) maxhisto = histo[lep][var][sam]->GetMaximum();
	leghisto = legnames[vars[var].samples[sam]]+" [#mu = ";
	int digits(1);
	if(variable.Contains("iso")) digits = 2;
	leghisto += RoundNumber(histo[lep][var][sam]->GetMean(),digits) + "]";
	leg.AddEntry(histo[lep][var][sam], leghisto);
      } // Loop over samples
      histo[lep][var][0]->SetMaximum(maxhisto*1.1);
      legH = 0.061*vars[var].samples.size(); leg.SetY1NDC(legY-legH);
      leg.Draw(); 
      if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*1.1);
      pname = "plots/hltstudy/"+lepfolders[lep]+"_"+vars[var].tag+".png";
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
     folder+"/*T1tttt*1200_*", 
     folder+"/*T1tttt*1500_*", 
     folder+"/*QCD*",
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
