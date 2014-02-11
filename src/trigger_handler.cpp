#include "in_json_2012.hpp"
#include "trigger_handler.hpp"
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
#include "TCanvas.h"

using namespace std;
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

#define NTrigEfficiencies 12

void trigger_handler::CalTrigEfficiency(int Nentries, string outFilename){

  TFile outFile(outFilename.c_str(), "recreate");
  outFile.cd();

  TString TriggerName[][2] = {{"Mu40", "Mu40_PFHT350"},         {"Mu40", "Mu40_PFNoPUHT350"},
			      {"Mu40", "PFHT350_Mu15_PFMET45"}, {"Mu40", "PFNoPUHT350_Mu15_PFMET45"},
			      {"Mu40", "PFHT400_Mu5_PFMET45"},  {"Mu40", "PFNoPUHT400_Mu5_PFMET45"},
			      {"Ele27_WP80",               "CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET4"}, 
			      {"Ele27_WP80",               "CleanPFNoPUHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"},
			      {"Ele27_WP80",               "CleanPFHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"}, 
			      {"Ele27_WP80",               "CleanPFNoPUHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45"},
			      {"Ele80_CaloIdVT_TrkIdT",    "CleanPFHT300_Ele40_CaloIdVT_TrkIdT"}, 
			      {"Ele80_CaloIdVT_GsfTrkIdT", "CleanPFNoPUHT300_Ele40_CaloIdVT_TrkIdT"}};
  int nBins = 30, TrigEffDecision[NTrigEfficiencies][2];
  float Range[2] = {0, 600};

  TH1F *hTrigEff[NTrigEfficiencies][3];
  TString hName, trigname, trigEffName, hisPrefix[] = {"Num_", "Den_", "Eff_"};
  for(int ieff(0); ieff < NTrigEfficiencies; ieff++){
    for(int his(0); his < 3; his++){
      hName = hisPrefix[his]; hName += TriggerName[ieff][0]; hName += "_Vs_";
      hName += TriggerName[ieff][1];
      hTrigEff[ieff][his] = new TH1F(hName, hName, nBins, Range[0], Range[1]);
    }
  }

  Timer timer(Nentries);
  timer.Start();
  for(int entry(0); entry < Nentries; entry++){
    if(entry%1000==0 && entry!=0){
      timer.PrintRemainingTime();
    }
    timer.Iterate();
    GetEntry(entry);

    // Event clean-up
    if(!PassesJSONCut()) continue;
    if(!PassesPVCut()) continue;
    if(!PassesMETCleaningCut()) continue;

    // Jet selection
    vector<int> good_muons_jet_cleaning;
    vector<int> veto_muons_jet_cleaning;
    vector<int> signal_elecs;
    vector<int> veto_elecs; 
    for(uint imu=0; imu<mus_pt->size(); imu++){
      if(passedMuonSelection(imu)) good_muons_jet_cleaning.push_back(imu);
      if(passedMuonVetoSelection(imu)) veto_muons_jet_cleaning.push_back(imu);
    }
    for(uint iel=0; iel<els_pt->size(); iel++){
      if(passedElectronSelection(iel)) signal_elecs.push_back(iel);
      if(passedElectronVetoSelection(iel)) veto_elecs.push_back(iel);
    }

    double htPF = 0;
    vector<int> good_jets;
    for(uint ijet = 0; ijet<jets_AK5PFclean_pt->size(); ijet++) {
      if(!isGoodJet(ijet)) continue;
      
      bool isElectron = false;     
      for(uint cit = 0; cit < signal_elecs.size(); cit++) {
	double tmpdR = dR( jets_AK5PFclean_eta->at(ijet),
			   els_eta->at(signal_elecs[cit]),
			   jets_AK5PFclean_phi->at(ijet),
			   els_phi->at(signal_elecs[cit]));	
	if(tmpdR < 0.3) isElectron = true;
      }
      
      bool isMuon = false;
      for(uint cit = 0; cit < good_muons_jet_cleaning.size();cit++) {
	double tmpdR = dR( jets_AK5PFclean_eta->at(ijet),
			   mus_eta->at(good_muons_jet_cleaning[cit]),
			   jets_AK5PFclean_phi->at(ijet),
			   mus_phi->at(good_muons_jet_cleaning[cit]));
	
	if(tmpdR < 0.1) isMuon = true;
      }
      
      if(!isMuon && !isElectron) {	    
	if(jets_AK5PFclean_pt->at(ijet) > JetPTThresholdHT) htPF += jets_AK5PFclean_pt->at(ijet);
	if(jets_AK5PFclean_pt->at(ijet) > JetPTThresholdNJ) good_jets.push_back(ijet);
      }
    } // Loop over jets	    

    // Muon selection
    vector<int> signal_muons;
    vector<int> veto_muons;
    for(uint cit = 0; cit < good_muons_jet_cleaning.size(); cit++) {	    
      bool close_to_jet = false;
      for(uint iit = 0; iit < good_jets.size(); iit++) {
	double tmpdR = dR( jets_AK5PFclean_eta->at(good_jets[iit]),
			   mus_eta->at(good_muons_jet_cleaning[cit]),
			   jets_AK5PFclean_phi->at(good_jets[iit]),
			   mus_phi->at(good_muons_jet_cleaning[cit]));
	if(tmpdR < 0.3) close_to_jet = true;
      }
      if(!close_to_jet) signal_muons.push_back(good_muons_jet_cleaning.at(cit));
    }
    // Veto muons
    for(uint cit = 0; cit < veto_muons_jet_cleaning.size(); cit++) {	    
      bool close_to_jet = false;
      
      for(uint iit = 0; iit < good_jets.size(); iit++) {
	double tmpdR = dR( jets_AK5PFclean_eta->at(good_jets[iit]),
			   mus_eta->at(veto_muons_jet_cleaning[cit]),
			   jets_AK5PFclean_phi->at(good_jets[iit]),
			   mus_phi->at(veto_muons_jet_cleaning[cit]));
	if( tmpdR < 0.3 ) {close_to_jet = true;}
      }      
      if( !close_to_jet ) veto_muons.push_back(veto_muons_jet_cleaning[cit]);
    }
//      cout<<entry<<": "<<good_jets.size()<<" jets, "<<good_muons_jet_cleaning.size()<<" signal cleaning muons, "
//  	<<veto_muons_jet_cleaning.size()<<" veto cleaning muons, "<<signal_muons.size()<<" signal muons, "
//  	<<veto_muons.size()<<" veto muons, "<<signal_elecs.size()<<" signal elecs, "
//  	<<veto_elecs.size()<<" veto elecs. "<<els_pt->size()<<endl<<"======"<<endl;

    // Baseline selection
    if(good_jets.size() < 3) continue;
    if (!((signal_muons.size() == 0 && veto_muons.size() == 0 && signal_elecs.size() == 1 && veto_elecs.size() == 1) || 
	  (signal_muons.size() == 1 && veto_muons.size() == 1 && signal_elecs.size() == 0 && veto_elecs.size() == 0)))
      continue; 
    double lep_pt = 0.0;
    if (signal_muons.size() == 1 && signal_elecs.size() == 0) lep_pt = mus_pt->at(signal_muons.at(0));
    if (signal_muons.size() == 0 && signal_elecs.size() == 1) lep_pt = els_pt->at(signal_elecs.at(0));

    // Looping over triggers
    for(int ieff(0); ieff < NTrigEfficiencies; ieff++){
      for(int ind(0); ind<2; ind++){
	TrigEffDecision[ieff][ind] = 0;
	trigEffName = "HLT_"; trigEffName += TriggerName[ieff][ind]; trigEffName += "_v";
	for(unsigned int tri(0); tri < trigger_decision->size(); tri++){
	  trigname = trigger_name->at(tri);
	  //cout<<trigname<<": decision "<<trigger_decision->at(tri)<<", prescale "<<trigger_prescalevalue->at(tri)<<endl;
	  if(trigname.BeginsWith(trigEffName)) {
	    TrigEffDecision[ieff][ind]++;
	    if(trigger_decision->at(tri)==1) {TrigEffDecision[ieff][ind]++;
// 	      if(ind==1)cout<<"Event "<<entry<<": "<<good_jets.size()<<" jets, "<<good_muons_jet_cleaning.size()<<" signal cleaning muons, "
// 		  <<veto_muons_jet_cleaning.size()<<" veto cleaning muons, "<<signal_muons.size()<<" signal muons, "
// 		  <<veto_muons.size()<<" veto muons, "<<signal_elecs.size()<<" signal elecs, "
// 		  <<veto_elecs.size()<<" veto elecs. "<<els_pt->size()<<endl<<"======"<<endl;
}
	    continue;
	  }
	} // Loop over triggers
      } 

      if(TrigEffDecision[ieff][0]==2 && TrigEffDecision[ieff][1]>=1){
	hTrigEff[ieff][1]->Fill(htPF);
	if(TrigEffDecision[ieff][1]==2) hTrigEff[ieff][0]->Fill(htPF);
      }
    } // Loop over trigger efficiencies
  } // Loop over events

  for(int ieff(0); ieff < NTrigEfficiencies; ieff++) 
    DivideHistosEff(hTrigEff[ieff][0], hTrigEff[ieff][1], hTrigEff[ieff][2]);
  outFile.Write();
  outFile.Close();
  cout<<"Finished saving file "<<outFilename<<endl;
}

bool trigger_handler::passedMuonSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  //float d0PV = mus_tk_d0dum->at(imu)-pv_x->at(0)*sin(mus_tk_phi->at(imu))+pv_y->at(0)*cos(mus_tk_phi->at(imu));
  double sumEt = mus_pfIsolationR03_sumNeutralHadronEt->at(imu) + mus_pfIsolationR03_sumPhotonEt->at(imu) 
    - 0.5*mus_pfIsolationR03_sumPUPt->at(imu);
  if(sumEt<0.0) sumEt=0.0;
  float relIso = (mus_pfIsolationR03_sumChargedHadronPt->at(imu) + sumEt)/mus_pt->at(imu);
  int pfIdx=-1;
  
//   cout<<"Muon "<<imu<<" => global "<<mus_isGlobalMuon->at(imu)<<", isPFMuon "<< mus_isPFMuon->at(imu)
//        <<", GlobalMuonPromptTight "<< mus_id_GlobalMuonPromptTight->at(imu)
//        <<", LayersWithMeasurement "<< mus_tk_LayersWithMeasurement->at(imu)<<", numvalPixelhits "<< mus_tk_numvalPixelhits->at(imu)
//        <<", numberOfMatchedStations "<<mus_numberOfMatchedStations->at(imu) <<", dB "<< mus_dB->at(imu)
//        <<", pt "<< mus_pt->at(imu)<<", MuonPTThreshold "<< MuonPTThreshold
//        <<", eta "<< mus_eta->at(imu)<<", relIso "<< relIso
//        <<", hasPFMatch "<< hasPFMatch(imu, particleId::muon, pfIdx)<<", tk_vx "<< mus_tk_vx->at(imu)<<endl;
  return (mus_isGlobalMuon->at(imu) > 0
	  && mus_isPFMuon->at(imu) > 0
	  && mus_id_GlobalMuonPromptTight->at(imu)> 0 
	  && mus_tk_LayersWithMeasurement->at(imu) > 5
	  && mus_tk_numvalPixelhits->at(imu) > 0
	  && mus_numberOfMatchedStations->at(imu) > 1
	  && fabs(mus_dB->at(imu)) < 0.02
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5
	  && mus_pt->at(imu) >= MuonPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.4
	  && relIso < 0.12
	  && hasPFMatch(imu, particleId::muon, pfIdx)); 
  
}

bool trigger_handler::passedMuonVetoSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  //float d0PV = mus_tk_d0dum->at(imu)-pv_x->at(0)*sin(mus_tk_phi->at(imu))+pv_y->at(0)*cos(mus_tk_phi->at(imu));
  double sumEt = mus_pfIsolationR03_sumNeutralHadronEt->at(imu) + mus_pfIsolationR03_sumPhotonEt->at(imu) 
    - 0.5*mus_pfIsolationR03_sumPUPt->at(imu);
  if(sumEt<0.0) sumEt=0.0;
  float relIso = (mus_pfIsolationR03_sumChargedHadronPt->at(imu) + sumEt)/mus_pt->at(imu);
  int pfIdx=-1; 
  
  return ((mus_isGlobalMuon->at(imu) >0 || mus_isTrackerMuon->at(imu) >0)
	  && mus_isPFMuon->at(imu) > 0
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5 
	  && mus_pt->at(imu) >= MuonVetoPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.5
	  && relIso < 0.2
	  && hasPFMatch(imu, particleId::muon, pfIdx));      
}

bool trigger_handler::passedElectronSelection(uint iel){
  if(iel >= els_pt->size()) return false;

//   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
//       <<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  double sumEt = els_PFphotonIsoR03->at(iel) + els_PFneutralHadronIsoR03->at(iel) 
    - rho_kt6PFJetsForIsolation2011 * GetEffectiveArea(els_scEta->at(iel), IsMC());
  if(sumEt<0.0) sumEt=0;
  double relIso = (els_PFchargedHadronIsoR03->at(iel) + sumEt)/els_pt->at(iel);
  int pfIdx=-1;
  
//   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
//       <<", relIso "<<relIso<<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && relIso < 0.15
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

bool trigger_handler::passedElectronVetoSelection(uint iel){
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


float trigger_handler::GetEffectiveArea(float SCEta, bool isMC){
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

bool trigger_handler::hasPFMatch(int index, particleId::leptonType type, int &pfIdx){
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

double trigger_handler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

int trigger_handler::GetNumGoodJets(double ptThresh) const{
  int numGoodJets(0);
  for(uint ijet(0); ijet<jets_AK5PFclean_pt->size(); ++ijet){
    if(isGoodJet(ijet, ptThresh, 2.4)) ++numGoodJets;
  }
  return numGoodJets;
}

bool trigger_handler::isGoodJet(const unsigned int ijet, const double ptThresh, const double etaThresh) const{
  if(jets_AK5PFclean_pt->size()<=ijet) return false;
  if(!passedPFJetSelection(ijet)) return false;
  if(jets_AK5PFclean_pt->at(ijet)<ptThresh || fabs(jets_AK5PFclean_eta->at(ijet))>etaThresh) return false;
  return true;
}

bool trigger_handler::passedPFJetSelection(const unsigned int ijet) const{
  // Why AK5PFclean instead of AK5PF? Why not corrFactorRaw?
  const double jetenergy = jets_AK5PFclean_energy->at(ijet);// * jets_AK5PFclean_corrFactorRaw->at(ijet);
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

bool trigger_handler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
}

bool trigger_handler::PassesJSONCut() const {
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

bool trigger_handler::PassesMETCleaningCut() const{
  //   for(unsigned int jet(0); jet<jets_AK5PFclean_pt->size(); ++jet){
  //     if(isProblemJet(jet)) return false;
  //   }
  //   if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; 
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
  //     && greedymuonfilter_decision 
  //     && PassesBadJetFilter()
  //     && inconsistentPFmuonfilter_decision 
  //     && (ecallaserfilter_decision || sampleName.find("_v66")!=std::string::npos)
  //     && GetPBNR()>=1;
}

bool trigger_handler::IsMC(){
  return (sampleName.find("Run201") == string::npos);
}

trigger_handler::trigger_handler(const std::string &fileName, const bool isList, const bool fastMode):
  cfA(fileName, isList){
  if (fastMode) { // turn off unnecessary branches
    chainA.SetBranchStatus("triggerobject_*",0);
    chainA.SetBranchStatus("standalone_t*",0);
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
    chainB.SetBranchStatus("pfmets*",0);
    chainB.SetBranchStatus("mets_AK5_et",0);
    chainB.SetBranchStatus("Nmc*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("Nmc_*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("jets_AK5PF_*",0);
    chainB.SetBranchStatus("Njets_AK5PF",0);
    chainB.SetBranchStatus("jets_AK5PF_*",0);
//     chainB.SetBranchStatus("jets_AK5PFclean_*",0);
//     chainB.SetBranchStatus("Njets_AK5PFclean",0);
//     chainB.SetBranchStatus("jets_AK5PFclean_*",0);

//     chainA.SetBranchStatus("pdfweights_*",0);
//     chainB.SetBranchStatus("Npf_photons",0);
//     chainB.SetBranchStatus("pf_photons_*",0);
  }
}

