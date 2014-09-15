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
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, TString ititle="", TString icuts="1", 
	 float icut=-1){
    varname = ivarname; nbins = inbins; minx = iminx; maxx = imaxx; title = ititle;
    cuts = icuts; cut = icut;
    tag = ivarname+"_"+cuts; tag.ReplaceAll("_1",""); tag.ReplaceAll(".",""); 
    tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")",""); 
    tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); 
    tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
    tag.ReplaceAll(">",""); tag.ReplaceAll("<",""); tag.ReplaceAll("=","");
  }
  TString title, varname, tag, cuts;
  int nbins;
  float minx, maxx, cut;
};

void hltdistributions(TString folder="root/hlt/mu15noiso") { 
  styles style("Standard"); style.nDivisions = 708; style.setDefaultStyle();
  vector<hfeats> vars;
  TCanvas can;
  int inilep(0), endlep(1);

   vars.push_back(hfeats("onht",50,0,2500,"HLT H_{T} (GeV)"));
   vars.push_back(hfeats("onmet",50,0,750,"HLT MET (GeV)"));
  // vars.push_back(hfeats("onht",50,0,2500,"HLT H_{T} (GeV)","Sum$(jets_pt>40)>=5"));
  // vars.push_back(hfeats("onmet",50,0,750,"HLT MET (GeV)","Sum$(jets_pt>40)>=5"));
  //vars.push_back(hfeats("mindr_mu",25,0,2.5,"Minimum #DeltaR(#mu,jet)"));
  //vars.push_back(hfeats("mindr2_mu",30,0,6,"2nd minimum #DeltaR(#mu,jet)","Sum$(jets_pt>40)>=5"));
  //vars.push_back(hfeats("abs(jets_phi[0]-jets_phi[1])*(abs(jets_phi[0]-jets_phi[1])<3.1416)+(2*3.1416-abs(jets_phi[0]-jets_phi[1]))*(abs(jets_phi[0]-jets_phi[1])>=3.1416)",63,0,3.15,"#Delta#phi(jet1,jet2)"));
   // vars.push_back(hfeats("Sum$(jets_pt>40)",16,-0.5,15.5,"Number of jets with p_{T} > 40 GeV","onht>500"));
   // vars.push_back(hfeats("Sum$(jets_pt>60)",16,-0.5,15.5,"Number of jets with p_{T} > 60 GeV","onht>500"));
   // vars.push_back(hfeats("Sum$(jets_pt>40)",16,-0.5,15.5,"Number of jets with p_{T} > 40 GeV"));
   // vars.push_back(hfeats("Sum$(jets_pt>60)",16,-0.5,15.5,"Number of jets with p_{T} > 60 GeV"));
  // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,"Leading #mu HLT p_{T} (GeV)"));
  // vars.push_back(hfeats("Max$(mus_pt)",29,5,150,"Leading #mu HLT p_{T} (GeV)","onht>500"));
  // vars.push_back(hfeats("Max$(genmus_pt)",29,5,150,"Leading #mu gen p_{T} (GeV)"));
  //  vars.push_back(hfeats("Max$(lep_pt*(abs(lep_id)==13&&lep_reliso<0.12))",29,5,150,"Leading #mu reco p_{T} (GeV)",
  //   			"abs(lep_id)==13&&lep_reliso<0.12"));

  // vars.push_back(hfeats("Max$(els_pt)",29,5,150,"Leading e p_{T} (GeV)"));
  // vars.push_back(hfeats("Max$(els_pt)",29,5,150,"Leading e p_{T} (GeV)","onht>500&&onmet>50"));
  // vars.push_back(hfeats("Max$(genels_pt)",29,5,150,"Leading gen e p_{T} (GeV)"));
  // vars.push_back(hfeats("Max$(lep_pt*(abs(lep_id)==11&&lep_reliso<0.15))",29,5,150,"Leading e reco p_{T} (GeV)",
  //  			"abs(lep_id)==11&&lep_reliso<0.15"));

  // vars.push_back(hfeats("Min$(els_trackiso+(els_pt<15)*10000)",26,0,0.26,"Electron track relative isolation","1",0.05));
  // vars.push_back(hfeats("Min$(els_ecaliso+(els_pt<15)*10000)",51,0,1.02,"Electron ECAL relative isolation","1",0.2));
  // vars.push_back(hfeats("Min$(els_hcaliso+(els_pt<15)*10000)",56,0,0.56,"Electron HCAL relative isolation","1",0.11));
  // vars.push_back(hfeats("Min$(els_reliso+(els_pt<15)*10000)",50,0,1.7,"Electron total relative isolation","1",0.36));
  
  vars.push_back(hfeats("Min$(mus_reliso+(mus_pt<15)*10000)",51,0,1.02,"Muon total relative isolation","1",0.15));

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

  TLine line; line.SetLineColor(28); line.SetLineWidth(2); line.SetLineStyle(2);

  int hcolors[] = {8,28,6,1,4,2};
  int hstyles[] = {2,2,2,1,1,1};
  int hwidths[] = {4,4,4,2,2,2};
  vector<TH1F*> histo[2][NSam];
  TChain *chain[NSam];
  TString lepfolders[] = {"el15", "mu15"}, currentfolder, legnames[NSam];
  TString hname, pname, variable, leghisto, totcut, title;
  if(folder.Contains("mu15")) {inilep = 1; endlep = 2;}
  for(int lep(inilep); lep<endlep; lep++){
    currentfolder = folder;// + lepfolders[lep];
    ReadChains(chain, currentfolder, legnames);
    for(unsigned var(0); var<vars.size(); var++){
      if((vars[var].varname.Contains("els")||vars[var].varname.Contains("11")) 
	 && !lepfolders[lep].Contains("el")) continue;
      if((vars[var].varname.Contains("mus")||vars[var].varname.Contains("13"))  
	 && !lepfolders[lep].Contains("mu")) continue;

      title = vars[var].cuts; if(title=="1") title = "";
      title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
      title.ReplaceAll("Sum$(jets_pt>40)","n_{jets}^{40}");title.ReplaceAll("abs(lep_id)==13&&","");
      title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
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
	if(variable.Contains("iso") && indchain[sam]==0){
	  if(lepfolders[lep].Contains("el")) totcut = "weight*wl1ht200*(Max$(genels_pt[0])>0&&"+vars[var].cuts+")";
	  else totcut = "weight*wl1ht200*(Max$(genmus_pt[0])>0&&"+vars[var].cuts+")";
	  cout<<totcut<<endl;
	}
	if(indchain[sam]==1||indchain[sam]==2||indchain[sam]==5){
	  if(variable == "Max$(mus_pt)") variable = "onmupt";
	  if(variable == "Max$(els_pt)") variable = "onelpt";
	}
	chain[indchain[sam]]->Project(hname, variable, totcut);
	histo[lep][var][sam]->SetBinContent(vars[var].nbins,
					    histo[lep][var][sam]->GetBinContent(vars[var].nbins)+
					    histo[lep][var][sam]->GetBinContent(vars[var].nbins+1));
	if(legnames[indchain[sam]].Contains("QCD")) histo[lep][var][sam]->Scale(100./histo[lep][var][sam]->Integral());
	else histo[lep][var][sam]->Scale(100./histo[lep][var][sam]->Integral());
	if(sam==0){
	  histo[lep][var][sam]->SetXTitle(vars[var].title);
	  histo[lep][var][sam]->SetYTitle("Entries (%)");
	  histo[lep][var][sam]->Draw();
	} else histo[lep][var][sam]->Draw("same");
	if(maxhisto < histo[lep][var][sam]->GetMaximum()) maxhisto = histo[lep][var][sam]->GetMaximum();
	leghisto = legnames[indchain[sam]]+" [#mu = ";
	leghisto += RoundNumber(histo[lep][var][sam]->GetMean(),2) + "]";
	leg.AddEntry(histo[lep][var][sam], leghisto);
      } // Loop over samples
      histo[lep][var][0]->SetMaximum(maxhisto*1.1);
      leg.Draw(); 
      if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*1.1);
      pname = "plots/hltstudy/"+lepfolders[lep]+"_"+vars[var].tag+".eps";
      can.SaveAs(pname);
    }// Loop over variables
  }// Loop over el15 and mu15

  for(int lep(inilep); lep<endlep; lep++)
    for(unsigned var(0); var<vars.size(); var++){
      if((vars[var].varname.Contains("els")||vars[var].varname.Contains("11")) 
	 && !lepfolders[lep].Contains("el")) continue;
      if((vars[var].varname.Contains("mus")||vars[var].varname.Contains("13"))  
	 && !lepfolders[lep].Contains("mu")) continue;
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
     folder+"/W*"};
  LegNames[0] = "T1tttt(1025,625)";
  LegNames[1] = "T1tttt(1200,800)";
  LegNames[2] = "T1tttt(1500,100)";
  LegNames[3] = "QCD";
  LegNames[4] = "t#bar{t}";
  LegNames[5] = "W#rightarrowl#nu";

  for(int sam(0); sam < NSam; sam++){
    chain[sam] = new TChain("tree");
    chain[sam]->Add(FileNames[sam]);
  }// Loop over samples
}
