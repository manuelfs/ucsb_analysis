#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"

#define NSam 6

using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[], TString folder, TString LegNames[]);

class hfeats {
public:
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, TString ititle="", TString icuts="1"){
    varname = ivarname; nbins = inbins; minx = iminx; maxx = imaxx; title = ititle;
    cuts = icuts;
    tag = ivarname+"_"+cuts; tag.ReplaceAll("_1",""); 
    tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")",""); 
    tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
    tag.ReplaceAll(">",""); tag.ReplaceAll("<",""); tag.ReplaceAll("=","");
  }
  TString title, varname, tag, cuts;
  int nbins;
  float minx, maxx;
};

void hltdistributions(TString folder="root/hlt/ht200/") { 
  styles style("Standard"); style.nDivisions = 708; style.setDefaultStyle();
  vector<hfeats> vars;
  TCanvas can;

  //vars.push_back(hfeats("mindr_mu",30,0,3,"Minimum #DeltaR(#mu,jet)","onht>500"));
  //vars.push_back(hfeats("mindr2_mu",25,0,5,"2nd minimum #DeltaR(#mu,jet)","onht>500"));
  vars.push_back(hfeats("Sum$(jets_pt>40)",15,-0.5,14.5,"Number of jets with p_{T} > 40 GeV","onht>500"));
  //vars.push_back(hfeats("Sum$(jets_pt>60)",15,-0.5,14.5,"Number of jets with p_{T} > 60 GeV","onht>500"));
  // vars.push_back(hfeats("Sum$(jets_pt>40)",15,-0.5,14.5,"Number of jets with p_{T} > 40 GeV"));
  // vars.push_back(hfeats("Sum$(jets_pt>60)",15,-0.5,14.5,"Number of jets with p_{T} > 60 GeV"));
  // vars.push_back(hfeats("onht",50,0,2500,"HLT H_{T} (GeV)"));
  // vars.push_back(hfeats("onmet",50,0,750,"HLT MET (GeV)"));
  // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,"Leading #mu p_{T} (GeV)"));
  // vars.push_back(hfeats("Max$(els_pt)",29,5,150,"Leading e p_{T} (GeV)"));
  // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,"Leading #mu p_{T} (GeV)","onht>500&&onmet>50"));
  // vars.push_back(hfeats("Max$(els_pt)",29,5,150,"Leading e p_{T} (GeV)","onht>500&&onmet>50"));
  // vars.push_back(hfeats("Max$(genmus_pt)",29,5,150,"Leading gen #mu p_{T} (GeV)","onht>500&&onmet>50"));
  // vars.push_back(hfeats("Max$(genels_pt)",29,5,150,"Leading gen e p_{T} (GeV)","onht>500&&onmet>50"));
  // vars.push_back(hfeats("Max$(lep_pt*(abs(lep_id)==13))",29,5,150,"Leading reco #mu p_{T} (GeV)",
  //  			"onht>500&&onmet>50&&abs(lep_id)==13"));
  // vars.push_back(hfeats("Max$(lep_pt*(abs(lep_id)==11))",29,5,150,"Leading reco e p_{T} (GeV)",
  //  			"onht>500&&onmet>50&&abs(lep_id)==11"));

  vector<int> indchain;
  indchain.push_back(0); //(1025,625)
  indchain.push_back(1); //(1200,800)
  indchain.push_back(2); //(1500,100)
  indchain.push_back(5); //Wjets
  indchain.push_back(3); //QCD
  indchain.push_back(4); //tt

  double legX = 0.56, legY = 0.93;
  double legW = 0.12, legH = 0.061*indchain.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  int hcolors[] = {28,8,6,1,4,2};
  int hstyles[] = {2,2,2,1,1,1};
  int hwidths[] = {4,4,4,2,2,2};
  vector<TH1F*> histo[2][NSam];
  TChain *chain[NSam];
  TString lepfolders[] = {"el15", "mu15"}, currentfolder, legnames[NSam];
  TString hname, pname, variable, leghisto, totcut, title;
  for(int lep(0); lep<2; lep++){
    currentfolder = folder + lepfolders[lep];
    ReadChains(chain, currentfolder, legnames);
    for(unsigned var(0); var<vars.size(); var++){
      if(vars[var].varname.Contains("el") && !lepfolders[lep].Contains("el")) continue;
      if(vars[var].varname.Contains("mu") && !lepfolders[lep].Contains("mu")) continue;

      title = vars[var].cuts; if(title=="1") title = "";
      title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
      title.ReplaceAll("onmet", "MET"); title.ReplaceAll("onht", "H_{T}"); 
      leg.Clear();
      float maxhisto(-999);
      for(unsigned sam(0); sam < indchain.size(); sam++){
	hname = "histo"; hname += lep; hname += var; hname += sam;
	histo[lep][var].push_back(new TH1F(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
	histo[lep][var][sam]->SetLineColor(hcolors[indchain[sam]]);
	histo[lep][var][sam]->SetLineStyle(hstyles[indchain[sam]]);
	histo[lep][var][sam]->SetLineWidth(hwidths[indchain[sam]]);
	totcut = "weight*("+vars[var].cuts+")";
	variable = vars[var].varname;
	if(indchain[sam]==1||indchain[sam]==2||indchain[sam]==5){
	  if(variable == "Max$(mus_pt)") variable = "onmupt";
	  if(variable == "Max$(els_pt)") variable = "onelpt";
	}
	chain[indchain[sam]]->Project(hname, variable, totcut);
	histo[lep][var][sam]->Scale(100./histo[lep][var][sam]->Integral());
	if(sam==0){
	  histo[lep][var][sam]->SetXTitle(vars[var].title);
	  histo[lep][var][sam]->SetYTitle("Entries (%)");
	  histo[lep][var][sam]->Draw();
	} else histo[lep][var][sam]->Draw("same");
	if(maxhisto < histo[lep][var][sam]->GetMaximum()) maxhisto = histo[lep][var][sam]->GetMaximum();
	leghisto = legnames[indchain[sam]]+" [#mu = ";
	leghisto += RoundNumber(histo[lep][var][sam]->GetMean(),1) + "]";
	leg.AddEntry(histo[lep][var][sam], leghisto);
      } // Loop over samples
      histo[lep][var][0]->SetMaximum(maxhisto*1.1);
      leg.Draw();
      pname = "plots/hltstudy/"+lepfolders[lep]+"_"+vars[var].tag+".eps";
      can.SaveAs(pname);
    }// Loop over variables
  }// Loop over el15 and mu15

  for(int lep(0); lep<2; lep++)
    for(unsigned var(0); var<vars.size(); var++){
      if(vars[var].varname.Contains("el") && !lepfolders[lep].Contains("el")) continue;
      if(vars[var].varname.Contains("mu") && !lepfolders[lep].Contains("mu")) continue;
      for(unsigned sam(0); sam < indchain.size(); sam++)
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
     folder+"/*WJets*"};
  LegNames[0] = "T1tttt(1025,625)";
  LegNames[1] = "T1tttt(1200,800)";
  LegNames[2] = "T1tttt(1500,100)";
  LegNames[3] = "QCD";
  LegNames[4] = "t#bar{t}";
  LegNames[5] = "W+jets";

  for(int sam(0); sam < NSam; sam++){
    chain[sam] = new TChain("tree");
    chain[sam]->Add(FileNames[sam]);
  }// Loop over samples
}
