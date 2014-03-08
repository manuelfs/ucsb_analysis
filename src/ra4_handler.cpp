#include "small_tree.hpp"
#include "styles.hpp"
#include "in_json_2012.hpp"
#include "ra4_handler.hpp"
#include "timer.hpp"
#include <cfloat>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTree.h"

using namespace std;
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

#define NTrigEfficiencies 14
#define NTrigReduced 19

void ra4_handler::ReduceTree(int Nentries, string outFilename){
  TFile outFile(outFilename.c_str(), "recreate");
  outFile.cd();
  TString energy, SampleName;
  SampleName = ParseSampleName(outFilename, energy);

  // Reduced tree
  small_tree tree;

  // Setting up desired triggers
  vector <string> triggername;
  TString trigname, trigEffName, word, model;
  string TriggerName[] = {"Mu17", "Mu40", "Mu40_eta2p1", "Mu40_PFHT350", "Mu40_PFNoPUHT350", "Mu40_PFHT350",  	// 0-6
			  "PFHT350_Mu15_PFMET45", "PFHT350_Mu15_PFMET45", "PFNoPUHT350_Mu15_PFMET45", 		// 7-9
			  "PFHT400_Mu5_PFMET45", "PFNoPUHT400_Mu5_PFMET45", 					// 10-11
			  "Ele80_CaloIdVT_TrkIdT",    "CleanPFHT300_Ele40_CaloIdVT_TrkIdT",  			// 12-13
			  "CleanPFNoPUHT300_Ele40_CaloIdVT_TrkIdT", 						// 14
			  "Ele27_WP80", "CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET4", 		// 15
			  "CleanPFNoPUHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45", 			// 16
			  "CleanPFHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45",  			// 17
			  "CleanPFNoPUHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"}; 			// 18
 
  int TriggerIndex[NTrigReduced], AllTriggers(0);
  GetEntry(0);
  model = model_params->c_str();
  for(int ieff(0); ieff < NTrigReduced; ieff++){
    TriggerIndex[ieff] = -1; 
    triggername.push_back(TriggerName[ieff]);
    tree.trigger.push_back(-1);
    for(unsigned int tri(0); tri < trigger_decision->size(); tri++){
      trigname = trigger_name->at(tri);
      trigEffName = "HLT_"; trigEffName += triggername[ieff]; trigEffName += "_v";
      if(trigname.Contains(trigEffName)) {
	TriggerIndex[ieff] = tri;
	break;
      }
    } // Loop over all trigger in event
    AllTriggers += TriggerIndex[ieff];
  } // Loop over desired triggers
  if(AllTriggers == -NTrigReduced) {
    cout<<"No desired triggers exist in this ntuple"<<endl;
    return;
  }

  const float luminosity = 19600, xsec_t1tttt_8tev(0.00695169), xsec_t1tttt_13tev(0.121755);
  const float xsec_tt_8tev(245.8), xsec_tt_13tev(818.8);
  Float_t pt_thresh[] = {30, 40, 50, 60, 70, 80};
  int nthresh = 6;
  tree.njets.resize(nthresh);

  vector<float> wpu_min, wpu_max, wpu;
  ifstream wpu_file("txt/weights_sms_pu.txt");
  if(sampleName.find("8TeV_T1tttt")!=std::string::npos){
    for(int iword(0); iword<3; iword++) wpu_file>>word;
    while(wpu_file>>word){
      wpu_min.push_back(word.Atof());
      wpu_file>>word;
      wpu_max.push_back(word.Atof());
      wpu_file>>word;
      wpu.push_back(word.Atof());
    }
  }
  Timer timer(Nentries);
  timer.Start();
  for(int entry(0); entry < Nentries; entry++){
    if(entry%5000==0 && entry!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    GetEntry(entry);

    /////////  Calculating branches  /////////
    vector<int> signal_electrons = GetElectrons();
    vector<int> veto_electrons = GetElectrons(false);
    vector<int> signal_muons = GetMuons();
    vector<int> veto_muons = GetMuons(false);
    tree.nel  = signal_electrons.size();
    tree.nvel = veto_electrons.size();
    tree.nmu  = signal_muons.size();
    tree.nvmu = veto_muons.size();
    AllTriggers = 0;
    for(int ieff(0); ieff < NTrigReduced; ieff++){
      if(TriggerIndex[ieff] >= 0){
	tree.trigger[ieff] = static_cast<int> (trigger_decision->at(TriggerIndex[ieff]));
	AllTriggers += tree.trigger[ieff];
      } else tree.trigger[ieff] = -1;
    }
    if(AllTriggers == 0) continue; // No desired triggers passed

    ////////////////   Jets   ////////////////
    tree.jets_pt.resize(0);
    tree.jets_eta.resize(0);
    tree.jets_phi.resize(0);
    for(int ith(0); ith < nthresh; ith++) 
      tree.njets[ith] = 0;
    for(uint ijet = 0; ijet<jets_AK5PFclean_pt->size(); ijet++) {
      if(!isGoodJet(ijet, 30)) continue;
      tree.jets_pt.push_back(jets_AK5PFclean_pt->at(ijet));
      tree.jets_eta.push_back(jets_AK5PFclean_eta->at(ijet));
      tree.jets_phi.push_back(jets_AK5PFclean_phi->at(ijet));
      for(int ith(0); ith < nthresh; ith++) 
	if(jets_AK5PFclean_pt->at(ijet) >= pt_thresh[ith]) tree.njets[ith]++;
    }

    ////////////////   METS   ////////////////
    tree.met = pfTypeImets_et->at(0);
    tree.metsig = pfmets_fullSignif;
    // Setting up online MET
    int index_onmet(-1);
    for(unsigned int tri(0); tri < standalone_triggerobject_pt->size(); tri++){
      trigname = standalone_triggerobject_collectionname->at(tri); 
      //cout<<tri<<": "<<trigname<<endl;
      if(trigname.BeginsWith("hltPFMETnoMu")) continue;
      if(trigname.Contains("MuORNoMu")) continue;
      if(trigname.BeginsWith("hltPFMET")) {
	index_onmet = tri;
	break;
      }
    }
    if(index_onmet >= 0) tree.onmet = standalone_triggerobject_et->at(index_onmet);
    else tree.onmet = -999;


    ////////////////   Pile-up   ////////////////
    for(unsigned int bc(0); bc<PU_bunchCrossing->size(); ++bc){
      if(PU_bunchCrossing->at(bc)==0){
	tree.ntrupv = PU_TrueNumInteractions->at(bc);
	break;
      }
    }
    ////////////////   Weights   ////////////////
    tree.wpu = 1;
    tree.wlumi = 1;
    if(energy == "8" && SampleName == "T1ttt"){
      tree.wpu = -99;
      for(unsigned int bin(0); bin<wpu.size(); bin++)
	if(tree.ntrupv >= wpu_min[bin] && tree.ntrupv < wpu_max[bin]){
	  tree.wpu = wpu[bin];
	  break;
	}
      tree.wlumi = xsec_t1tttt_8tev*luminosity / static_cast<double>(Nentries);
    } 
    if(energy == "14" && SampleName == "T1ttt") tree.wlumi = xsec_t1tttt_13tev*luminosity / static_cast<double>(Nentries);
    if(energy == "8" && SampleName == "t#bar{t}") tree.wlumi = xsec_tt_8tev*luminosity / static_cast<double>(Nentries);
    if(energy == "13" && SampleName == "t#bar{t}") tree.wlumi = xsec_tt_13tev*luminosity / static_cast<double>(Nentries);

    tree.weight = tree.wpu*tree.wlumi;

    
    tree.Fill();
  }

  tree.Write();

  // Global tree
  int ntriggers(NTrigReduced);
  TTree treeglobal("treeglobal", "treeglobal");
  treeglobal.Branch("ntriggers", &ntriggers);
  treeglobal.Branch("noriginal", &Nentries);
  treeglobal.Branch("triggername", &triggername);
  treeglobal.Branch("model", &model);
  treeglobal.Branch("nthresh", &nthresh);
  treeglobal.Branch("pt_thresh", pt_thresh, "pt_thresh[nthresh]/F");

  treeglobal.Fill();
  treeglobal.Write();

  outFile.Close();
  cout<<"Reduced tree in "<<outFilename<<endl;
}


void ra4_handler::CalTrigEfficiency(int Nentries, string outFilename){

  styles style; style.setDefaultStyle();
  TFile outFile(outFilename.c_str(), "recreate");
  outFile.cd();

  TString TriggerName[][2] = {{"Mu40", "Mu40_PFHT350"},         {"Mu40", "Mu40_PFNoPUHT350"},
			      {"Mu40_eta2p1", "Mu40_PFHT350"},  {"Mu17", "PFHT350_Mu15_PFMET45"},
			      {"Mu40", "PFHT350_Mu15_PFMET45"}, {"Mu40", "PFNoPUHT350_Mu15_PFMET45"},
			      {"Mu40", "PFHT400_Mu5_PFMET45"},  {"Mu40", "PFNoPUHT400_Mu5_PFMET45"},
			      {"Ele80_CaloIdVT_TrkIdT",    "CleanPFHT300_Ele40_CaloIdVT_TrkIdT"}, 
			      {"Ele80_CaloIdVT_GsfTrkIdT", "CleanPFNoPUHT300_Ele40_CaloIdVT_TrkIdT"},
			      {"Ele27_WP80",               "CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET4"}, 
			      {"Ele27_WP80",               "CleanPFNoPUHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"},
			      {"Ele27_WP80",               "CleanPFHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"}, 
			      {"Ele27_WP80",               "CleanPFNoPUHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"}};
  int nBins[] = {30,25}, TrigEffDecision[NTrigEfficiencies][2], TriggerIndex[NTrigEfficiencies][2];
  float Range[2][2] = {{0, 600}, {0, 250}};

  TH1F *hTrigEff[NTrigEfficiencies][3][2];
  TH2F *h2TrigEff[NTrigEfficiencies][3];
  TString hName, trigname, trigEffName, hisPrefix[] = {"Num_", "Den_", "Eff_"}, trigType[] = {"HT", "MET"};
  TString xTitle[] = {"H_{T} (GeV)", "MET (GeV)"};
  float cutHtMet[] = {450, 120}, HtMet[2];
  bool IsHtMet[NTrigEfficiencies][2], trigCombExists[NTrigEfficiencies], noTriggers = true;
  GetEntry(0);
  for(int ieff(0); ieff < NTrigEfficiencies; ieff++){
    TriggerIndex[ieff][0] = -1; TriggerIndex[ieff][1] = -1; 
    for(unsigned int tri(0); tri < trigger_decision->size(); tri++){
      trigname = trigger_name->at(tri);
      for(int ind(0); ind<2; ind++){
	trigEffName = "HLT_"; trigEffName += TriggerName[ieff][ind]; trigEffName += "_v";
	if(trigname.Contains(trigEffName)) TriggerIndex[ieff][ind] = tri;
      }
      if(TriggerIndex[ieff][0]>=0 && TriggerIndex[ieff][1]>=0) break;
    }
    if(TriggerIndex[ieff][0]<0 || TriggerIndex[ieff][1]<0) {
      trigCombExists[ieff] = false;
      continue;
    }
    trigCombExists[ieff] = true;
    noTriggers = false;
    for(int his(0); his < 3; his++){
      for(int typ(0); typ < 2; typ++){
	if(TriggerName[ieff][1].Contains(trigType[typ])) IsHtMet[ieff][typ] = true;
	else{IsHtMet[ieff][typ] = false;  continue;}
	hName = hisPrefix[his]; hName+= trigType[typ]; hName += "_";
	hName += TriggerName[ieff][0]; hName += "_Vs_";	hName += TriggerName[ieff][1];
	hTrigEff[ieff][his][typ] = new TH1F(hName, hName, nBins[typ], Range[typ][0], Range[typ][1]);
	hTrigEff[ieff][his][typ]->SetXTitle(xTitle[typ]);
      }
      if(IsHtMet[ieff][0] && IsHtMet[ieff][1]) {
	if(his==2) continue;
	hName = hisPrefix[his]; hName+= trigType[0]; hName+= trigType[1]; hName += "_";
	hName += TriggerName[ieff][0]; hName += "_Vs_";	hName += TriggerName[ieff][1];
	h2TrigEff[ieff][his] = new TH2F(hName, hName, nBins[0], Range[0][0], Range[0][1], nBins[1], Range[1][0], Range[1][1]);
      }
    } // Loop over histos
  } // Loop over trigger efficiencies
  if(noTriggers) {
    cout<<"No triggers from the list found in this ntuple"<<endl;
    return;
  }

  Timer timer(Nentries);
  timer.Start();
  for(int entry(0); entry < Nentries; entry++){
    if(entry%5000==0 && entry!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    GetEntry(entry);

    // Event clean-up
    if(!PassesJSONCut()) continue;
    if(!PassesPVCut()) continue;
    if(!PassesMETCleaningCut()) continue;

    // Baseline selection
    HtMet[1] = 0;
    if(pfTypeImets_et->size()>0) HtMet[1] = pfTypeImets_et->at(0);
    vector<int> signal_electrons = GetElectrons();
    vector<int> veto_electrons = GetElectrons(false);
    vector<int> signal_muons = GetMuons();
    vector<int> veto_muons = GetMuons(false);
    vector<int> good_jets = GetJets(signal_electrons, signal_muons, HtMet[0]);

    //     cout<<entry<<": HT "<<HtMet[0]<<", MET "<<HtMet[1]<<", "<<good_jets.size()<<" jets, "
    //  	<<signal_muons.size()<<" signal muons, "
    //  	<<veto_muons.size()<<" veto muons, "<<signal_electrons.size()<<" signal elecs, "
    //  	<<veto_electrons.size()<<" veto elecs. "<<endl<<"======"<<endl;

    if(good_jets.size() < 3) continue;
    if (!((signal_muons.size() == 0 && veto_muons.size() == 0 && signal_electrons.size() == 1 && veto_electrons.size() == 1) || 
	  (signal_muons.size() == 1 && veto_muons.size() == 1 && signal_electrons.size() == 0 && veto_electrons.size() == 0)))
      continue; 

    // Looping over trigger efficiencies
    for(int ieff(0); ieff < NTrigEfficiencies; ieff++){
      if(!trigCombExists[ieff]) continue;
      for(int ind(0); ind<2; ind++){
	TrigEffDecision[ieff][ind] = 0;
	trigEffName = "HLT_"; trigEffName += TriggerName[ieff][ind]; trigEffName += "_v";
	int tri = TriggerIndex[ieff][ind];
	trigname = trigger_name->at(tri);
	if(trigname.BeginsWith(trigEffName)) {
	  TrigEffDecision[ieff][ind]++;  // Trigger exists >=1
	  if(trigger_decision->at(tri)==1) {
	    TrigEffDecision[ieff][ind]++;  // Trigger passes ==2
	  }
	  continue;
	}
      } // Loop over RefTrig and Target Trig

      if(TrigEffDecision[ieff][0]==2 && TrigEffDecision[ieff][1]>=1){ // Reference passed, trigger exist
	if(IsHtMet[ieff][0] && IsHtMet[ieff][1]){ // This trigger has both HT and MET
	  h2TrigEff[ieff][1]->Fill(HtMet[0], HtMet[1]);
	  if(TrigEffDecision[ieff][1]==2) h2TrigEff[ieff][0]->Fill(HtMet[0], HtMet[1]);
	  for(int typ(0); typ < 2; typ++){
	    if(IsHtMet[ieff][typ] && HtMet[(typ+1)%2] > cutHtMet[(typ+1)%2]){
	      hTrigEff[ieff][1][typ]->Fill(HtMet[typ]);
	      if(TrigEffDecision[ieff][1]==2) hTrigEff[ieff][0][typ]->Fill(HtMet[typ]);
	    }
	  } // Loop over types
	} else { // This trigger only has either HT or MET
	  for(int typ(0); typ < 2; typ++){
	    if(IsHtMet[ieff][typ]){
	      hTrigEff[ieff][1][typ]->Fill(HtMet[typ]);
	      if(TrigEffDecision[ieff][1]==2) hTrigEff[ieff][0][typ]->Fill(HtMet[typ]);
	    }
	  } // Loop over types
	}
      }
    } // Loop over trigger efficiencies
  } // Loop over events

  // Finding efficiency histograms with Poisson errors. TEfficiency to be implemented
  for(int ieff(0); ieff < NTrigEfficiencies; ieff++) {
    if(!trigCombExists[ieff]) continue;
    for(int typ(0); typ < 2; typ++)
      if(IsHtMet[ieff][typ])
	DivideHistosEff(hTrigEff[ieff][0][typ], hTrigEff[ieff][1][typ], hTrigEff[ieff][2][typ]);
    if(IsHtMet[ieff][0] && IsHtMet[ieff][1]) {
      hName = hisPrefix[2]; hName+= trigType[0]; hName+= trigType[1]; hName += "_";
      hName += TriggerName[ieff][0]; hName += "_Vs_";	hName += TriggerName[ieff][1];
      h2TrigEff[ieff][2] = (TH2F*) h2TrigEff[ieff][0]->Clone(hName);
      h2TrigEff[ieff][2]->Divide(h2TrigEff[ieff][1]);
    }
  }
  outFile.Write();
  outFile.Close();
  cout<<"Finished saving file "<<outFilename<<endl;
}

void ra4_handler::PrintAllTriggers(string outName){
  GetEntry(0);
  ofstream outTrigger(outName.c_str());
  for(unsigned int tri(0); tri < trigger_decision->size(); tri++)
    outTrigger<<trigger_name->at(tri)<<": decision "<<trigger_decision->at(tri)<<", prescale "<<trigger_prescalevalue->at(tri)<<endl;
  cout<<"Printed list of triggers in "<<outName.c_str()<<endl;
}

vector<int> ra4_handler::GetMuons(bool doSignal){
  vector<int> muons;
  for(uint index=0; index<mus_pt->size(); index++)
    if(doSignal){
      if(passedMuonSelection(index)) muons.push_back(index);
    }	else {
      if(passedMuonVetoSelection(index)) muons.push_back(index);
    }
  return muons;
}

vector<int> ra4_handler::GetElectrons(bool doSignal){
  vector<int> electrons;
  for(uint index=0; index<els_pt->size(); index++)
    if(doSignal){
      if(passedElectronSelection(index)) electrons.push_back(index);
    }	else {
      if(passedElectronVetoSelection(index)) electrons.push_back(index);
    }
  return electrons;
}

vector<int> ra4_handler::GetJets(vector<int> electrons, vector<int> muons, float &HT){
  vector<int> jets;
  HT = 0;
  for(uint ijet = 0; ijet<jets_AK5PFclean_pt->size(); ijet++) {
    if(!isGoodJet(ijet)) continue;
    double tmpdR;
    bool useJet = true;
    for(uint index = 0; index < electrons.size(); index++) {
      tmpdR = dR(jets_AK5PFclean_eta->at(ijet), els_eta->at(electrons[index]),
		 jets_AK5PFclean_phi->at(ijet), els_phi->at(electrons[index]));	
      if(tmpdR < 0.3){useJet = false; break;}
    }      
    if(!useJet) continue;
    for(uint index = 0; index < muons.size();index++) {
      tmpdR = dR(jets_AK5PFclean_eta->at(ijet), mus_eta->at(muons[index]),
		 jets_AK5PFclean_phi->at(ijet), mus_phi->at(muons[index]));
	
      if(tmpdR < 0.1){useJet = false; break;}
    }
    if(!useJet) continue;

    if(jets_AK5PFclean_pt->at(ijet) > JetPTThresholdHT) HT += jets_AK5PFclean_pt->at(ijet);
    if(jets_AK5PFclean_pt->at(ijet) > JetPTThresholdNJ) jets.push_back(ijet);
  } // Loop over jets	    
  return jets;
}

float ra4_handler::GetMuonIsolation(uint imu){
  if(imu >= mus_pt->size()) return -999;
  double sumEt = mus_pfIsolationR03_sumNeutralHadronEt->at(imu) + mus_pfIsolationR03_sumPhotonEt->at(imu) 
    - 0.5*mus_pfIsolationR03_sumPUPt->at(imu);
  if(sumEt<0.0) sumEt=0.0;
  return (mus_pfIsolationR03_sumChargedHadronPt->at(imu) + sumEt)/mus_pt->at(imu);
}

bool ra4_handler::passedBaseMuonSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  float d0PV = mus_tk_d0dum->at(imu)-pv_x->at(0)*sin(mus_tk_phi->at(imu))+pv_y->at(0)*cos(mus_tk_phi->at(imu));
  int pfIdx=-1;
  
  //   cout<<"Muon "<<imu<<" => global "<<mus_isGlobalMuon->at(imu)<<", isPFMuon "<< mus_isPFMuon->at(imu)
  //        <<", GlobalMuonPromptTight "<< mus_id_GlobalMuonPromptTight->at(imu)
  //        <<", LayersWithMeasurement "<< mus_tk_LayersWithMeasurement->at(imu)<<", numvalPixelhits "<< mus_tk_numvalPixelhits->at(imu)
  //        <<", numberOfMatchedStations "<<mus_numberOfMatchedStations->at(imu) <<", dB "<< mus_dB->at(imu)
  //        <<", pt "<< mus_pt->at(imu)<<", MuonPTThreshold "<< MuonPTThreshold
  //        <<", eta "<< mus_eta->at(imu)
  //        <<", hasPFMatch "<< hasPFMatch(imu, particleId::muon, pfIdx)<<", tk_vx "<< mus_tk_vx->at(imu)<<endl;
  return (mus_isGlobalMuon->at(imu) > 0
	  && mus_isPFMuon->at(imu) > 0
	  && mus_id_GlobalMuonPromptTight->at(imu)> 0 
	  && mus_tk_LayersWithMeasurement->at(imu) > 5
	  && mus_tk_numvalPixelhits->at(imu) > 0
	  && mus_numberOfMatchedStations->at(imu) > 1
	  //&& fabs(mus_dB->at(imu)) < 0.02
	  && fabs(d0PV) < 0.02
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5
	  && mus_pt->at(imu) >= MuonPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.4
	  && hasPFMatch(imu, particleId::muon, pfIdx)); 
}

bool ra4_handler::passedMuonSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  float relIso = GetMuonIsolation(imu);  
  return (passedBaseMuonSelection(imu) && relIso < 0.12); 
}

bool ra4_handler::passedMuonVetoSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  int pfIdx=-1; 
  float relIso = GetMuonIsolation(imu);
  
  return ((mus_isGlobalMuon->at(imu) >0 || mus_isTrackerMuon->at(imu) >0)
	  && mus_isPFMuon->at(imu) > 0
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5 
	  && mus_pt->at(imu) >= MuonVetoPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.5
	  && relIso < 0.2
	  && hasPFMatch(imu, particleId::muon, pfIdx));      
}

float ra4_handler::GetElectronIsolation(uint iel){
  double sumEt = els_PFphotonIsoR03->at(iel) + els_PFneutralHadronIsoR03->at(iel) 
    - rho_kt6PFJetsForIsolation2011 * GetEffectiveArea(els_scEta->at(iel), IsMC());
  if(sumEt<0.0) sumEt=0;
  return (els_PFchargedHadronIsoR03->at(iel) + sumEt)/els_pt->at(iel);
}

bool ra4_handler::passedBaseElectronSelection(uint iel){
  if(iel >= els_pt->size()) return false;

  //   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
  //       <<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  int pfIdx=-1;
  
  //   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
  //       <<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && !els_hasMatchedConversion->at(iel)
	  && els_n_inner_layer->at(iel) <= 1
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && hasPFMatch(iel, particleId::electron, pfIdx) 
	  && fabs(d0PV) < 0.02 
	  && ((els_isEB->at(iel)
	       && fabs(els_dEtaIn->at(iel)) < 0.004
	       && fabs(els_dPhiIn->at(iel)) < 0.06
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.12 )
	      || (els_isEE->at(iel)
		  && fabs(els_dEtaIn->at(iel)) < 0.007
		  && fabs(els_dPhiIn->at(iel)) < 0.03
		  && els_sigmaIEtaIEta->at(iel) < 0.03
		  && els_hadOverEm->at(iel) < 0.10 ))
	  );
}

bool ra4_handler::passedElectronSelection(uint iel){
  if(iel >= els_pt->size()) return false;

  double relIso = GetElectronIsolation(iel);
  return (passedBaseElectronSelection(iel) && relIso < 0.15);
}

bool ra4_handler::passedElectronVetoSelection(uint iel){
  if(iel >= els_pt->size()) return false;

  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  double sumEt = els_PFphotonIsoR03->at(iel) + els_PFneutralHadronIsoR03->at(iel) 
    - rho_kt6PFJetsForIsolation2011 * GetEffectiveArea(els_scEta->at(iel), IsMC());
  if(sumEt<0.0) sumEt=0;
  double relIso = (els_PFchargedHadronIsoR03->at(iel) + sumEt)/els_pt->at(iel);
  
  return (els_pt->at(iel) > ElectronVetoPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && relIso < 0.15
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.2
	  && fabs(d0PV) < 0.04 
	  && ((els_isEB->at(iel)
	       && fabs(els_dEtaIn->at(iel)) < 0.007
	       && fabs(els_dPhiIn->at(iel)) < 0.8
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.15)
	      || (els_isEE->at(iel)
		  && fabs(els_dEtaIn->at(iel)) < 0.01
		  && fabs(els_dPhiIn->at(iel)) < 0.7
		  && els_sigmaIEtaIEta->at(iel) < 0.03))
	  );  
}


float ra4_handler::GetEffectiveArea(float SCEta, bool isMC){
  float EffectiveArea;

  if(isMC) {
    if (fabs(SCEta) >= 0.0 && fabs(SCEta) < 1.0 ) EffectiveArea = 0.110;
    if (fabs(SCEta) >= 1.0 && fabs(SCEta) < 1.479 ) EffectiveArea = 0.130;
    if (fabs(SCEta) >= 1.479 && fabs(SCEta) < 2.0 ) EffectiveArea = 0.089;
    if (fabs(SCEta) >= 2.0 && fabs(SCEta) < 2.2 ) EffectiveArea = 0.130;
    if (fabs(SCEta) >= 2.2 && fabs(SCEta) < 2.3 ) EffectiveArea = 0.150;
    if (fabs(SCEta) >= 2.3 && fabs(SCEta) < 2.4 ) EffectiveArea = 0.160;
    if (fabs(SCEta) >= 2.4) EffectiveArea = 0.190;
  }
  else {
    //kEleGammaAndNeutralHadronIso03 from 2011 data
    //obtained from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.3&view=markup
    if (fabs(SCEta) >= 0.0 && fabs(SCEta) < 1.0 ) EffectiveArea = 0.100;
    if (fabs(SCEta) >= 1.0 && fabs(SCEta) < 1.479 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 1.479 && fabs(SCEta) < 2.0 ) EffectiveArea = 0.085;
    if (fabs(SCEta) >= 2.0 && fabs(SCEta) < 2.2 ) EffectiveArea = 0.110;
    if (fabs(SCEta) >= 2.2 && fabs(SCEta) < 2.3 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 2.3 && fabs(SCEta) < 2.4 ) EffectiveArea = 0.120;
    if (fabs(SCEta) >= 2.4) EffectiveArea = 0.130;
  }
  return EffectiveArea;
}

bool ra4_handler::hasPFMatch(int index, particleId::leptonType type, int &pfIdx){
  double deltaRVal = 999.;
  double deltaPT = 999.;
  double leptonEta = 0, leptonPhi = 0, leptonPt = 0;
  if(type == particleId::muon ) {
    leptonEta = mus_eta->at(index);
    leptonPhi = mus_phi->at(index);
    leptonPt = mus_pt->at(index);
  }  else if(type == particleId::electron) {
    leptonEta = els_scEta->at(index);
    leptonPhi = els_phi->at(index);
    leptonPt = els_pt->at(index);
  }
  
  for(unsigned iCand=0; iCand<pfcand_pt->size(); iCand++) {
    //     cout<<"Repetition "<<iCand<<": particleId "<<pfcand_particleId->at(iCand)
    // 	<<", deltaRVal "<<deltaRVal<<", deltaPT "<<deltaPT
    // 	<<", eta "<<pfcand_eta->at(iCand)<<", phi "<<pfcand_phi->at(iCand)<<endl;
    if(pfcand_particleId->at(iCand)==type) {
      double tempDeltaR = dR(leptonEta, pfcand_eta->at(iCand), leptonPhi, pfcand_phi->at(iCand));
      if(tempDeltaR < deltaRVal) {
	deltaRVal = tempDeltaR;
	deltaPT = fabs(leptonPt-pfcand_pt->at(iCand));
	pfIdx=iCand;
      }
    }
  }
 
  //   cout<<"Lepton "<<index<<" => type "<<type
  //       <<", leptonEta "<< leptonEta<<", leptonPhi "<< leptonPhi
  //       <<", leptonPt "<< leptonPt<<", deltaPT "<< deltaPT<<endl;
  if(type == particleId::electron) return (deltaPT<10);
  else return (deltaPT<5);
}

double ra4_handler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

int ra4_handler::GetNumGoodJets(double ptThresh) const{
  int numGoodJets(0);
  for(uint ijet(0); ijet<jets_AK5PFclean_pt->size(); ++ijet){
    if(isGoodJet(ijet, ptThresh, 2.4)) ++numGoodJets;
  }
  return numGoodJets;
}

bool ra4_handler::isGoodJet(const unsigned int ijet, const double ptThresh, const double etaThresh) const{
  if(jets_AK5PFclean_pt->size()<=ijet) return false;
  if(!passedPFJetSelection(ijet)) return false;
  if(jets_AK5PFclean_pt->at(ijet)<ptThresh || fabs(jets_AK5PFclean_eta->at(ijet))>etaThresh) return false;
  return true;
}

bool ra4_handler::passedPFJetSelection(const unsigned int ijet) const{
  double rawRatio =(jets_AK5PFclean_rawPt->at(ijet)/jets_AK5PFclean_pt->at(ijet)); // Same as jets_AK5PFclean_corrFactorRaw
  const double jetenergy = jets_AK5PFclean_energy->at(ijet) * rawRatio;
  double NEF = -999., CEF = -999., NHF=-999., CHF=-999.;
  double chgMult=jets_AK5PFclean_chg_Mult->at(ijet);
  double numConst=jets_AK5PFclean_mu_Mult->at(ijet)+jets_AK5PFclean_neutral_Mult->at(ijet)+jets_AK5PFclean_chg_Mult->at(ijet);
   
  if(jetenergy > 0){
    NEF = jets_AK5PFclean_neutralEmE->at(ijet)/jetenergy;
    CEF = jets_AK5PFclean_chgEmE->at(ijet)/jetenergy;
    NHF = jets_AK5PFclean_neutralHadE->at(ijet)/jetenergy;
    CHF = jets_AK5PFclean_chgHadE->at(ijet)/jetenergy;   
  }
   
  //    cout<<"Jet "<<ijet<<" => NEF "<<NEF<<", CEF "<< CEF
  //        <<", NHF "<<NHF <<",  CHF "<< CHF<<", chgMult "<< chgMult
  //        <<", pt "<<jets_AK5PFclean_pt->at(ijet)<<", eta "<<jets_AK5PFclean_eta->at(ijet)
  //        <<", numConst "<< numConst<<endl;
  return (NEF < 0.99 && CEF < 0.99 && NHF < 0.99 && CHF > 0 &&
	  chgMult > 0 && numConst > 1);
}

bool ra4_handler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
}

bool ra4_handler::PassesJSONCut() const {
  if(sampleName.find("Run2012")!=std::string::npos){
    if(sampleName.find("PromptReco")!=std::string::npos
       &&!inJSON(VRunLumiPrompt, run, lumiblock)) return false;
    if(sampleName.find("24Aug")!=std::string::npos
       && !inJSON(VRunLumi24Aug, run, lumiblock)) return false;
    if(sampleName.find("13Jul")!=std::string::npos
       && !inJSON(VRunLumi13Jul, run, lumiblock)) return false;
    return true;
  }else{
    return true;
  }
}

bool ra4_handler::PassesMETCleaningCut() const{
  //   cout<<", cschalofilter "<< cschalofilter_decision<<",  hbhefilter"<< hbhefilter_decision<<", hcallaserfilter "<<hcallaserfilter_decision 
  //       <<", ecalTPfilter "<< ecalTPfilter_decision <<",  trackingfailurefilter"<< trackingfailurefilter_decision 
  //       <<", eebadscfilter "<< eebadscfilter_decision  <<", scrapingVeto_decision "<<scrapingVeto_decision<<endl;
  return cschalofilter_decision
    && hbhefilter_decision
    && hcallaserfilter_decision 
    && ecalTPfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision 
    && scrapingVeto_decision;
}

bool ra4_handler::IsMC(){
  return (sampleName.find("Run201") == string::npos);
}

ra4_handler::ra4_handler(const std::string &fileName, const bool isList, const bool fastMode):
  cfA(fileName, isList){
  if (fastMode) { // turn off unnecessary branches
    //chainA.SetBranchStatus("triggerobject_*",0);
    //chainA.SetBranchStatus("standalone_t*",0);
    chainA.SetBranchStatus("L1trigger_*",0);
    chainA.SetBranchStatus("passprescale*",0);
    chainA.SetBranchStatus("softjetUp_*",0);
    chainA.SetBranchStatus("photon_*",0);
    chainB.SetBranchStatus("Ntcmets",0);
    chainB.SetBranchStatus("tcmets_*",0);
    chainB.SetBranchStatus("Nphotons",0);
    chainB.SetBranchStatus("photons_*",0);
    chainB.SetBranchStatus("Nmets*",0);
    chainB.SetBranchStatus("taus*",0);
    chainB.SetBranchStatus("tracks*",0);
    chainB.SetBranchStatus("mets*",0);
    //chainB.SetBranchStatus("pfmets*",0);
    chainB.SetBranchStatus("mets_AK5_et",0);
    chainB.SetBranchStatus("Nmc*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("Nmc_*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("jets_AK5PF_*",0);
    chainB.SetBranchStatus("Njets_AK5PF",0);
  }
}

