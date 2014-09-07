#include "pdtlund.hpp"
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
const double CSVCuts[] = {0.244, 0.679, 0.898};
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

#define NTrigEfficiencies 14
#define NTrigReduced 12

void ra4_handler::ReduceTree(int Nentries, TString outFilename){
  // for(int entry(0); entry < Nentries; entry++){
  //   GetEntry(entry);
  //   for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
  //     if(abs(mc_doc_mother_id->at(imc))==24){// && mc_doc_status->at(imc)==23){
  //     cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
  // 	  <<", \tGMom ID "<<(int)mc_doc_grandmother_id->at(imc)
  // 	  <<", \tGGMom ID "<<(int)mc_doc_ggrandmother_id->at(imc)
  // 	  <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
  // 	  <<",   \tN moms "<<mc_doc_numOfMothers->at(imc)
  // 	  <<",   \tstatus "<<mc_doc_status->at(imc)
  // 	  <<",   \tpT "<<mc_doc_pt->at(imc)
  // 	  <<",   \teta "<<mc_doc_eta->at(imc)
  // 	  <<",   \tphi "<<mc_doc_phi->at(imc)<<endl;
  //     }
  //   }
  //   cout<<endl<<endl;
  // }
  // return;

  TFile outFile(outFilename, "recreate");
  outFile.cd();

  // Reduced tree
  small_tree tree;

  // Setting up desired triggers
  vector <string> triggername;
  TString trigname, trigEffName, word, model;
  TString energy, SampleName;
  string TriggerName[] = 
    {"Mu5", "Mu24", "Mu40",									// 0-2
     "Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL", "Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL",	// 3-4
     "PFNoPUHT350", "PFNoPUHT650", "PFNoPUHT750",						// 5-7
     "PFMET150", "PFMET180",									// 8-9
     "PFNoPUHT350_Mu15_PFMET4", "PFNoPUHT400_Mu5_PFMET4"};					// 10-11
 
  int TriggerIndex[NTrigReduced], AllTriggers(0);
  float xsec(0);
  GetEntry(0);
  model = model_params->c_str();
  SampleName = ParseSampleName(outFilename, model, energy, xsec);

  for(int ieff(0); ieff < NTrigReduced; ieff++){
    TriggerIndex[ieff] = -1; 
    triggername.push_back(TriggerName[ieff]);
    tree.v_trigger.push_back(-1);
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

  const float luminosity = 19600;
  Float_t pt_thresh[] = {30, 40, 50, 60, 70, 80};
  int nthresh = 6;
  tree.v_njets.resize(nthresh);
  tree.v_nbl.resize(nthresh);tree.v_nbm.resize(nthresh);tree.v_nbt.resize(nthresh);
  double deltaR, lepmax_pt, lepmax_px, lepmax_py;
  int mcID, mcmomID, lepID;
  float spher[2][2], spher_jets[2][2], spher_nolin[2][2];
  float pt(-99.), px(-99.), py(-99.), eig1(-99.), eig2(-99.);

  vector<float> wpu_min, wpu_max, wpu;
  vector<int_double> csv_sorted;
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

    ////////////////   Triggers   ////////////////
    AllTriggers = 0;
    for(int ieff(0); ieff < NTrigReduced; ieff++){
      if(TriggerIndex[ieff] >= 0){
	tree.v_trigger[ieff] = static_cast<int> (trigger_decision->at(TriggerIndex[ieff]));
	AllTriggers += tree.v_trigger[ieff];
      } else tree.v_trigger[ieff] = -1;
    }
    //if(tree.v_trigger[0]==0 && tree.v_trigger[3]==0) continue;
    // if(AllTriggers == 0) continue; // No desired triggers passed

    ////////////////   Leptons   ////////////////
    vector<int> signal_electrons = GetElectrons();
    vector<int> veto_electrons = GetElectrons(false);
    vector<int> signal_muons = GetMuons();
    vector<int> veto_muons = GetMuons(false);
    vector<int> good_jets = GetJets(signal_electrons, signal_muons, veto_electrons, veto_muons, tree.ht);
    tree.nel  = signal_electrons.size();
    tree.nvel = veto_electrons.size();
    tree.nmu  = signal_muons.size();
    tree.nvmu = veto_muons.size();
    if(tree.nel+tree.nmu == tree.nvel+tree.nvmu) tree.nlep = tree.nel+tree.nmu;
    else tree.nlep = -99;

    ////////////////   Jets   ////////////////
    tree.v_jets_pt.resize(0);
    tree.v_jets_eta.resize(0);
    tree.v_jets_phi.resize(0);
    tree.v_jets_csv.resize(0);
    csv_sorted.resize(0);
    spher[0][0] = 0; spher[0][1] = 0; spher[1][0] = 0; spher[1][1] = 0; 
    spher_nolin[0][0] = 0; spher_nolin[0][1] = 0; spher_nolin[1][0] = 0; spher_nolin[1][1] = 0; 
    spher_jets[0][0] = 0; spher_jets[0][1] = 0; spher_jets[1][0] = 0; spher_jets[1][1] = 0; 
    for(int ith(0); ith < nthresh; ith++) {
      tree.v_njets[ith] = 0;
      tree.v_nbl[ith] = 0;
      tree.v_nbm[ith] = 0;
      tree.v_nbt[ith] = 0;
    }
    for(uint ijet = 0; ijet<jets_AK4_pt->size(); ijet++) {
      if(!isGoodJet(ijet, 30)) continue;
      pt = jets_AK4_pt->at(ijet);
      px = jets_AK4_px->at(ijet);
      py = jets_AK4_py->at(ijet);
      tree.v_jets_pt.push_back(pt);
      tree.v_jets_eta.push_back(jets_AK4_eta->at(ijet));
      tree.v_jets_phi.push_back(jets_AK4_phi->at(ijet));
      double csv = jets_AK4_btag_secVertexCombined->at(ijet);
      tree.v_jets_csv.push_back(csv);
      csv_sorted.push_back(make_pair(tree.v_jets_csv.size()-1, csv));
      // Transverse sphericity matrix (not including 1/sum(pt), which cancels in the ratio of eig)
      spher_nolin[0][0] += px*px; spher_nolin[0][1] += px*py; 
      spher_nolin[1][0] += px*py; spher_nolin[1][1] += py*py; 
      // Linearized transverse sphericity matrix
      spher[0][0] += px*px/pt; spher[0][1] += px*py/pt; 
      spher[1][0] += px*py/pt; spher[1][1] += py*py/pt; 
      // Linearized transverse sphericity matrix with only jets
      spher_jets[0][0] += px*px/pt; spher_jets[0][1] += px*py/pt; 
      spher_jets[1][0] += px*py/pt; spher_jets[1][1] += py*py/pt; 

//       cout<<ijet<<": csv "<<csv<<", eta "<<jets_AK4_eta->at(ijet)<<", phi "<<jets_AK4_phi->at(ijet)<<endl;
      for(int ith(0); ith < nthresh; ith++) {
	if(jets_AK4_pt->at(ijet) >= pt_thresh[ith]) {
	  tree.v_njets[ith]++;
	  if(csv >= CSVCuts[0]) tree.v_nbl[ith]++;
	  if(csv >= CSVCuts[1]) tree.v_nbm[ith]++;
	  if(csv >= CSVCuts[2]) tree.v_nbt[ith]++;
	}

      } // Loop over pT thresholds
    } // Loop over all jets

    if(eigen2x2(spher_jets, eig1, eig2)) tree.spher_jets = 2*eig2/(eig1+eig2);
    else tree.spher_jets = -999.;
    
    if(tree.v_jets_csv.size() >= 2){
      sort(csv_sorted.begin(), csv_sorted.end(), id_big2small);
      tree.dr_bb = dR(tree.v_jets_eta[csv_sorted[0].first], tree.v_jets_eta[csv_sorted[1].first], 
		      tree.v_jets_phi[csv_sorted[0].first], tree.v_jets_phi[csv_sorted[1].first]);
//       cout<<"Calculated dr_bb = "<<tree.dr_bb<<endl;
//       cout<<csv_sorted[0].first<<": csv "<<csv_sorted[0].second<<", eta "<<tree.v_jets_eta[csv_sorted[0].first]
// 	  <<", phi "<<tree.v_jets_phi[csv_sorted[0].first]<<endl;
//       cout<<csv_sorted[1].first<<": csv "<<csv_sorted[1].second<<", eta "<<tree.v_jets_eta[csv_sorted[1].first]
// 	  <<", phi "<<tree.v_jets_phi[csv_sorted[1].first]<<endl<<endl;
    } else tree.dr_bb = -999.;
    
    
    ////////////////   Leptons   ////////////////
    lepmax_pt=0; lepmax_px=0; lepmax_py=0;
    tree.v_lep_pt.resize(0);
    tree.v_lep_eta.resize(0);
    tree.v_lep_phi.resize(0);
    tree.v_lep_reliso.resize(0);
    tree.v_lep_id.resize(0);
    tree.v_lep_tru_id.resize(0);
    tree.v_lep_tru_momid.resize(0);
    tree.v_lep_tru_dr.resize(0);
    for(uint index=0; index<els_pt->size(); index++){
      if(!passedBaseElectronSelection(index)) continue;

      pt = els_pt->at(index);
      px = els_px->at(index);
      py = els_py->at(index);
      if(els_charge->at(index)>0) lepID = pdtlund::e_minus;
      else lepID = pdtlund::e_plus;
      mcID = GetTrueElectron((int)index, mcmomID, deltaR);
      tree.v_lep_pt.push_back(pt);
      tree.v_lep_eta.push_back(els_eta->at(index));
      tree.v_lep_phi.push_back(els_phi->at(index));
      tree.v_lep_reliso.push_back(GetElectronIsolation(index));
      tree.v_lep_id.push_back(lepID);
      tree.v_lep_tru_id.push_back(mcID);
      tree.v_lep_tru_momid.push_back(mcmomID);
      tree.v_lep_tru_dr.push_back(deltaR);

      if(els_pt->at(index) > lepmax_pt){
	lepmax_pt=els_pt->at(index); 
	lepmax_px=els_px->at(index); 
	lepmax_py=els_py->at(index);
      }
      // Transverse sphericity matrix (not including 1/sum(pt), which cancels in the ratio of eig)
      spher_nolin[0][0] += px*px; spher_nolin[0][1] += px*py; 
      spher_nolin[1][0] += px*py; spher_nolin[1][1] += py*py; 
      // Linearized transverse sphericity matrix
      spher[0][0] += px*px/pt; spher[0][1] += px*py/pt; 
      spher[1][0] += px*py/pt; spher[1][1] += py*py/pt; 
//       cout<<"Rec el eta "<<tree.v_lep_eta[tree.v_lep_pt.size()-1]<<", phi "<<tree.v_lep_phi[tree.v_lep_pt.size()-1]
//   	  <<", tru id "<<tree.v_lep_tru_id[tree.v_lep_pt.size()-1]<<", tru momid "<<tree.v_lep_tru_momid[tree.v_lep_pt.size()-1]
// 	  <<", dR "<<deltaR<<endl;
    }
    for(uint index=0; index<mus_pt->size(); index++){
      if(!passedBaseMuonSelection(index)) continue;

      if(mus_charge->at(index)>0) lepID = pdtlund::mu_minus;
      else lepID = pdtlund::mu_plus;
      mcID = GetTrueMuon((int)index, mcmomID, deltaR);
      tree.v_lep_pt.push_back(mus_pt->at(index));
      tree.v_lep_eta.push_back(mus_eta->at(index));
      tree.v_lep_phi.push_back(mus_phi->at(index));
      tree.v_lep_reliso.push_back(GetMuonIsolation(index));
      tree.v_lep_id.push_back(lepID);
      tree.v_lep_tru_id.push_back(mcID);
      tree.v_lep_tru_momid.push_back(mcmomID);
      tree.v_lep_tru_dr.push_back(deltaR);

      if(mus_pt->at(index) > lepmax_pt){
	lepmax_pt=mus_pt->at(index); 
	lepmax_px=mus_px->at(index); 
	lepmax_py=mus_py->at(index);
      }
      // Transverse sphericity matrix (not including 1/sum(pt), which cancels in the ratio of eig)
      spher_nolin[0][0] += px*px; spher_nolin[0][1] += px*py; 
      spher_nolin[1][0] += px*py; spher_nolin[1][1] += py*py; 
      // Linearized transverse sphericity matrix
      spher[0][0] += px*px/pt; spher[0][1] += px*py/pt; 
      spher[1][0] += px*py/pt; spher[1][1] += py*py/pt; 
//          cout<<"Rec mu pT "<<tree.v_lep_pt[tree.v_lep_pt.size()-1]<<", eta "<<tree.v_lep_eta[tree.v_lep_pt.size()-1]
//  	  <<", phi "<<tree.v_lep_phi[tree.v_lep_pt.size()-1]
//    	  <<", tru id "<<tree.v_lep_tru_id[tree.v_lep_pt.size()-1]<<", tru momid "<<tree.v_lep_tru_momid[tree.v_lep_pt.size()-1]
//    	  <<", dR "<<deltaR<<endl;
   }

//        if(tree.v_lep_pt.size()>0){
//           for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
//  	   //if(abs(mc_doc_id->at(imc)) == pdtlund::mu_minus)
//    	   cout<<imc<<": ID "<<(int)mc_doc_id->at(imc)<<",   \tMom ID "<<(int)mc_doc_mother_id->at(imc)
//  	       <<", \tGMom ID "<<(int)mc_doc_grandmother_id->at(imc)
//  	       <<", \tGGMom ID "<<(int)mc_doc_ggrandmother_id->at(imc)
//  	       <<", \tN daughters "<<mc_doc_numOfDaughters->at(imc)
//  	       <<",   \tN moms "<<mc_doc_numOfMothers->at(imc)
//    	       <<",   \tpT "<<mc_doc_pt->at(imc)
//    	       <<",   \teta "<<mc_doc_eta->at(imc)
//    	       <<",   \tphi "<<mc_doc_phi->at(imc)<<endl;
//           }
//           cout<<endl;

//           for(unsigned int imc = 0; imc < mc_mus_id->size(); imc++){
//  	   //if(abs(mc_mus_id->at(imc)) == pdtlund::mu_minus)
//    	   cout<<imc<<": ID "<<(int)mc_mus_id->at(imc)<<",   \tMom ID "<<(int)mc_mus_mother_id->at(imc)
//  	       <<", \tGMom ID "<<(int)mc_mus_grandmother_id->at(imc)
//  	       <<", \tGGMom ID "<<(int)mc_mus_ggrandmother_id->at(imc)
//  	       <<", \tN daughters "<<mc_mus_numOfDaughters->at(imc)
//    	       <<",   \tpT "<<mc_mus_pt->at(imc)
//    	       <<",   \teta "<<mc_mus_eta->at(imc)
//    	       <<",   \tphi "<<mc_mus_phi->at(imc)<<endl;
//           }
//           cout<<endl;
//        }

    if(eigen2x2(spher, eig1, eig2)) tree.spher = 2*eig2/(eig1+eig2);
    else tree.spher = -999.;
    if(eigen2x2(spher_nolin, eig1, eig2)) tree.spher_nolin = 2*eig2/(eig1+eig2);
    else tree.spher_nolin = -999.;


    ////////////////   METS   ////////////////
    tree.met = mets_et->at(0);
    tree.met_phi = mets_phi->at(0);
    tree.metsig = -1; // Undefined in new cfA

    // True MET and HT
    float metx(0), mety(0);
    for(unsigned int imc = 0; imc < mc_final_id->size(); imc++){
      int id = static_cast<int>(abs(mc_final_id->at(imc)));
      if(id==12 || id==14 || id==16 || id==39 || id>1e6&&mc_final_charge->at(imc)==0) {
	metx += mc_final_px->at(imc);
	mety += mc_final_py->at(imc);
      }
    }
    tree.genmet = sqrt(pow(metx,2)+pow(mety,2));
    tree.genht = 0;
    for(unsigned int imc = 0; imc < mc_jets_pt->size(); imc++)
      if(mc_jets_pt->at(imc)>40 && mc_jets_eta->at(imc)<3) tree.genht += mc_jets_pt->at(imc);

    // True leptons
    tree.genmus_pt->resize(0); tree.genels_pt->resize(0);
    tree.genmus_eta->resize(0); tree.genels_eta->resize(0);
    tree.genmus_phi->resize(0); tree.genels_phi->resize(0);
    for(unsigned int imc = 0; imc < mc_doc_id->size(); imc++){
      int id = static_cast<int>(abs(mc_doc_id->at(imc)));
      int momid = static_cast<int>(abs(mc_doc_mother_id->at(imc)));
      pt = mc_doc_pt->at(imc);
      if(momid == 24){
	if(id==11) {
	  tree.genels_pt->push_back(pt);
	  tree.genels_eta->push_back(mc_doc_eta->at(imc));
	  tree.genels_phi->push_back(mc_doc_phi->at(imc));
	}
	if(id==13) {
	  tree.genmus_pt->push_back(pt);
	  tree.genmus_eta->push_back(mc_doc_eta->at(imc));
	  tree.genmus_phi->push_back(mc_doc_phi->at(imc));
	}
      }
    }
   

    ////////////////   Online objects   ////////////////
    // Setting up online MET
    int index_onmet(-1);
    bool has_caloiso(false), has_ak5nopu(false), has_isomu(false);
    float sumjetspu(0), sumjets(0), objpt, objeta; // sumjetsl1(0), sumjetscalo(0), 
    tree.onelpt = -999; tree.onmupt = -999;
    for(unsigned int tri(0); tri < standalone_triggerobject_pt->size(); tri++){
      trigname = standalone_triggerobject_collectionname->at(tri); 
      objpt = standalone_triggerobject_pt->at(tri);
      objeta = standalone_triggerobject_eta->at(tri);
      // if(trigname.Contains("hltPFJetsMatchedToCaloJets50") && objpt>40 && objeta<3) sumjetscalo += objpt;
      // if(trigname.Contains("hltPFJetsL1Matched") && objpt>40 && objeta<3) sumjetsl1 += objpt;
      if(trigname.Contains("hltAK5PFJetL1FastL2L3Corrected::") && objpt>40 && objeta<3) sumjets += objpt;
      if(trigname.Contains("hltAK5PFJetL1FastL2L3CorrectedNoPU") && objpt>40 && objeta<3) {
	has_ak5nopu = true;
	sumjetspu += objpt;
      }
      if(index_onmet<0 && trigname.BeginsWith("hltPFMETProducer")) index_onmet = tri;
      if(trigname.Contains("CaloIdTTrkIdVLCaloIsoVLTrkIsoVL")) has_caloiso = true;
      if(trigname.Contains("IsoMu")) has_isomu = true;
      if(trigname.BeginsWith("hltL3MuonCandidates") && objeta<2.5) 
	if(objpt > tree.onmupt) tree.onmupt = objpt;

      if((trigname.Contains("3HitElectron") && trigname.Contains("Pixel")))
	if(objpt > tree.onelpt) tree.onelpt = objpt;
    }

    // Debug to find online seeds
    // if(!has_isomu && tree.v_trigger[1]==1){
    //   cout<<endl<<entry<<" Mu40 decision "<<tree.v_trigger[1]<<". mus ";
    //   for(uint index=0; index<mus_pt->size(); index++)
    // 	cout<<"("<<mus_pt->at(index)<<", "<<mus_eta->at(index)<<", "<<mus_phi->at(index)<<") ";
    //   cout<<endl;
    
    //   for(unsigned int tri(0); tri < standalone_triggerobject_pt->size(); tri++){
    // 	trigname = standalone_triggerobject_collectionname->at(tri); 
    // 	objpt = standalone_triggerobject_pt->at(tri);
    // 	objeta = standalone_triggerobject_eta->at(tri);
    // 	if(trigname.Contains("Mu")){
    // 	  cout<<trigname<<" -> ("<<objpt<<", "<<objeta
    // 	      <<", "<<standalone_triggerobject_phi->at(tri)<<") "<<endl;
    // 	}
    //   }
    // }

    if(index_onmet >= 0) tree.onmet = standalone_triggerobject_et->at(index_onmet);
    else tree.onmet = -tree.met;//-999;
    if(tree.onelpt>8 && tree.v_trigger[3]==0) tree.onelpt *= -1;
    if(tree.onmupt>5 && tree.v_trigger[0]==0) tree.onmupt *= -1;
    // cout<<"Sum jets "<<sumjets<<endl;
    // cout<<"Sum jets No PU "<<sumjetspu<<endl;
    // cout<<"Sum jets calo "<<sumjetscalo<<endl;
    // cout<<"Sum jets L1 "<<sumjetsl1<<endl;
    if(has_ak5nopu) tree.onht = sumjetspu;
    else tree.onht = sumjets;
    
    // Finding mT and deltaPhi with respect to highest pT lepton
    tree.mt = -999.; tree.dphi_wlep = -999.;
    if(lepmax_pt > 0){
      double lepmax_phi = atan2(lepmax_py, lepmax_px);
      double Wx = mets_ex->at(0) + lepmax_px;
      double Wy = mets_ey->at(0) + lepmax_py;
      tree.dphi_wlep = abs(atan2(Wy,Wx)-lepmax_phi);
      if(tree.dphi_wlep > PI) tree.dphi_wlep = 2*PI-tree.dphi_wlep;
      tree.mt = sqrt(2*lepmax_pt* tree.met*(1-cos(tree.met_phi-lepmax_phi)));
    }


    ////////////////   Pile-up   ////////////////
    for(unsigned int bc(0); bc<PU_bunchCrossing->size(); ++bc){
      if(PU_bunchCrossing->at(bc)==0){
	tree.ntrupv = PU_NumInteractions->at(bc);
	tree.ntrupv_mean = PU_TrueNumInteractions->at(bc);
	break;
      }
    }
    tree.npv = Npv;
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
    } 
    tree.wlumi = xsec*luminosity / static_cast<double>(Nentries);
    tree.wl1ht200 = (0.5*TMath::Erf((1.35121e-02)*(tree.genht-(3.02695e+02)))+0.5);
    tree.weight = tree.wpu*tree.wlumi*tree.wl1ht200;

    
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


void ra4_handler::CalTrigEfficiency(int Nentries, TString outFilename){

  styles style; style.setDefaultStyle();
  TFile outFile(outFilename, "recreate");
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
    if(mets_et->size()>0) HtMet[1] = mets_et->at(0);
    vector<int> signal_electrons = GetElectrons();
    vector<int> veto_electrons = GetElectrons(false);
    vector<int> signal_muons = GetMuons();
    vector<int> veto_muons = GetMuons(false);
    vector<int> good_jets = GetJets(signal_electrons, signal_muons, veto_electrons, veto_muons, HtMet[0]);

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

void ra4_handler::PrintAllTriggers(TString outName, int entry){
  GetEntry(entry);
  ofstream outTrigger(outName);
  for(unsigned int tri(0); tri < trigger_decision->size(); tri++)
    outTrigger<<trigger_name->at(tri)<<": decision "<<trigger_decision->at(tri)<<", prescale "<<trigger_prescalevalue->at(tri)<<endl;
  outTrigger<<endl<<endl<<"============== Standalone objects  ============="<<endl;
  for(unsigned int tri(0); tri < standalone_triggerobject_pt->size(); tri++){
    TString trigname = standalone_triggerobject_collectionname->at(tri); 
    outTrigger<<trigname<<" -> ("<<standalone_triggerobject_pt->at(tri)
	      <<", "<<standalone_triggerobject_eta->at(tri)
	      <<", "<<standalone_triggerobject_phi->at(tri)<<") ";
    outTrigger<<" \tels: ";
    for(uint index=0; index<els_pt->size(); index++)
      outTrigger<<"("<<els_pt->at(index)<<", "<<els_eta->at(index)<<", "<<els_phi->at(index)<<") ";

    outTrigger<<endl;
  }

  cout<<"Printed list of triggers in "<<outName<<endl;
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

int ra4_handler::GetTrueMuon(int index, int &momID, double &closest_dR){
  if(index < 0 || index >= (int)mus_eta->size()) return -1;

  int closest_imc = -1, idLepton = 0; 
  double dR = 9999.; closest_dR = 9999.;
  double MCEta, MCPhi;
  double RecEta = mus_eta->at(index), RecPhi = mus_phi->at(index);
  for(unsigned int imc=0; imc < mc_mus_id->size(); imc++){
    MCEta = mc_mus_eta->at(imc); MCPhi = mc_mus_phi->at(imc);
    dR = sqrt(pow(RecEta-MCEta,2) + pow(RecPhi-MCPhi,2));
    if(dR < closest_dR) {
      closest_dR = dR;
      closest_imc = imc;
    }
  }
  if(closest_imc >= 0){
    idLepton = static_cast<int>(mc_mus_id->at(closest_imc));
    momID = static_cast<int>(mc_mus_mother_id->at(closest_imc));
    if(idLepton == momID) momID = static_cast<int>(mc_mus_ggrandmother_id->at(closest_imc));
  } else {
    closest_imc = GetTrueParticle(RecEta, RecPhi, closest_dR);
    if(closest_imc >= 0){
      momID = static_cast<int>(mc_doc_mother_id->at(closest_imc));
      idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
    } else {
      momID = 0;
      idLepton = 0;
    }
  }
  return idLepton;
}

int ra4_handler::GetTrueElectron(int index, int &momID, double &closest_dR){
  if(index < 0 || index >= (int)els_eta->size()) return -1;

  int closest_imc = -1, idLepton = 0; 
  double dR = 9999.; closest_dR = 9999.;
  double MCEta, MCPhi;
  double RecEta = els_eta->at(index), RecPhi = els_phi->at(index);
  for(unsigned int imc=0; imc < mc_electrons_id->size(); imc++){
    MCEta = mc_electrons_eta->at(imc); MCPhi = mc_electrons_phi->at(imc);
    dR = sqrt(pow(RecEta-MCEta,2) + pow(RecPhi-MCPhi,2));
    if(dR < closest_dR) {
      closest_dR = dR;
      closest_imc = imc;
    }
  }
  if(closest_imc >= 0){
    idLepton = static_cast<int>(mc_electrons_id->at(closest_imc));
    momID = static_cast<int>(mc_electrons_mother_id->at(closest_imc));
    if(idLepton == momID) momID = static_cast<int>(mc_electrons_ggrandmother_id->at(closest_imc));
  } else {
    closest_imc = GetTrueParticle(RecEta, RecPhi, closest_dR);
    if(closest_imc >= 0){
      momID = static_cast<int>(mc_doc_mother_id->at(closest_imc));
      idLepton = static_cast<int>(mc_doc_id->at(closest_imc));
    } else {
      momID = 0;
      idLepton = 0;
    }
  }
  return idLepton;
}

int ra4_handler::GetTrueParticle(double RecEta, double RecPhi, double &closest_dR){
  int closest_imc = -1; 
  double dR = 9999.; closest_dR = 9999.;
  double MCEta, MCPhi;
  for(unsigned int imc=0; imc < mc_doc_id->size(); imc++){
    MCEta = mc_doc_eta->at(imc); MCPhi = mc_doc_phi->at(imc);
    dR = sqrt(pow(RecEta-MCEta,2) + pow(RecPhi-MCPhi,2));
    if(dR < closest_dR) {
      closest_dR = dR;
      closest_imc = imc;
    }
  }
  return closest_imc;
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
  
  //   cout<<"Muon "<<imu<<" => global "<<mus_isGlobalMuon->at(imu)<<", isPFMuon "<< mus_isPF->at(imu)
  //        <<", GlobalMuonPromptTight "<< mus_id_GlobalMuonPromptTight->at(imu)
  //        <<", LayersWithMeasurement "<< mus_tk_LayersWithMeasurement->at(imu)<<", numvalPixelhits "<< mus_tk_numvalPixelhits->at(imu)
  //        <<", numberOfMatchedStations "<<mus_numberOfMatchedStations->at(imu) <<", dB "<< mus_dB->at(imu)
  //        <<", pt "<< mus_pt->at(imu)<<", MuonPTThreshold "<< MuonPTThreshold
  //        <<", eta "<< mus_eta->at(imu)
  //        <<", tk_vx "<< mus_tk_vx->at(imu)<<endl;
  return (mus_isGlobalMuon->at(imu) > 0
	  && mus_isPF->at(imu)
	  && mus_id_GlobalMuonPromptTight->at(imu)> 0 
	  && mus_tk_LayersWithMeasurement->at(imu) > 5
	  && mus_tk_numvalPixelhits->at(imu) > 0
	  && mus_numberOfMatchedStations->at(imu) > 1
	  //&& fabs(mus_dB->at(imu)) < 0.02
	  && fabs(d0PV) < 0.02
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5
	  && mus_pt->at(imu) >= MuonPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.4);
}

bool ra4_handler::passedMuonSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  float relIso = GetMuonIsolation(imu);  
  return (passedBaseMuonSelection(imu) && relIso < 0.12); 
}

bool ra4_handler::passedMuonVetoSelection(uint imu){
  if(imu >= mus_pt->size()) return false;

  float relIso = GetMuonIsolation(imu);
  
  return ((mus_isGlobalMuon->at(imu) >0 || mus_isTrackerMuon->at(imu) >0)
	  && mus_isPF->at(imu) 
	  && fabs(getDZ(mus_tk_vx->at(imu), mus_tk_vy->at(imu), mus_tk_vz->at(imu), mus_tk_px->at(imu), 
			mus_tk_py->at(imu), mus_tk_pz->at(imu), 0)) < 0.5 
	  && mus_pt->at(imu) >= MuonVetoPTThreshold
	  && fabs(mus_eta->at(imu)) <= 2.5
	  && relIso < 0.2);
}

float ra4_handler::GetElectronIsolation(uint iel){
  float absiso = els_pfIsolationR03_sumChargedHadronPt->at(iel) + std::max(0.0 , els_pfIsolationR03_sumNeutralHadronEt->at(iel) + els_pfIsolationR03_sumPhotonEt->at(iel) - 0.5 * els_pfIsolationR03_sumPUPt->at(iel) );
  return absiso/els_pt->at(iel);
}

bool ra4_handler::passedBaseElectronSelection(uint iel){
  if(iel >= els_pt->size()) return false;

  //   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
  //       <<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  float d0PV = els_d0dum->at(iel)-pv_x->at(0)*sin(els_tk_phi->at(iel))+pv_y->at(0)*cos(els_tk_phi->at(iel));
  
  //   cout<<"pt "<<els_pt->at(iel)<<", thresh "<<ElectronPTThreshold<<", eta "<<els_scEta->at(iel)
  //       <<", conversion "<<els_hasMatchedConversion->at(iel)<<endl;
  return (els_pt->at(iel) > ElectronPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  //&& !els_hasMatchedConversion->at(iel)
	  && els_n_inner_layer->at(iel) <= 1
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.1
	  && fabs(1./els_caloEnergy->at(iel) - els_eOverPIn->at(iel)/els_caloEnergy->at(iel)) < 0.05 
	  && fabs(d0PV) < 0.02 
	  && ((els_isEB->at(iel) // Endcap selection
	       && fabs(els_dEtaIn->at(iel)) < 0.004
	       && fabs(els_dPhiIn->at(iel)) < 0.06
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.12 ) ||
	      (els_isEE->at(iel)  // Barrel selection
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
  double relIso = GetElectronIsolation(iel);
  
  return (els_pt->at(iel) > ElectronVetoPTThreshold
	  && fabs(els_scEta->at(iel)) < 2.5
	  && relIso < 0.15
	  && fabs(getDZ(els_vx->at(iel), els_vy->at(iel), els_vz->at(iel), cos(els_tk_phi->at(iel))*els_tk_pt->at(iel), 
			sin(els_tk_phi->at(iel))*els_tk_pt->at(iel), els_tk_pz->at(iel), 0)) < 0.2
	  && fabs(d0PV) < 0.04 
	  && ((els_isEB->at(iel) // Endcap selection
	       && fabs(els_dEtaIn->at(iel)) < 0.007
	       && fabs(els_dPhiIn->at(iel)) < 0.8
	       && els_sigmaIEtaIEta->at(iel) < 0.01
	       && els_hadOverEm->at(iel) < 0.15) ||
	      (els_isEE->at(iel)  // Barrel selection
	       && fabs(els_dEtaIn->at(iel)) < 0.01
	       && fabs(els_dPhiIn->at(iel)) < 0.7
	       && els_sigmaIEtaIEta->at(iel) < 0.03))
	  );  
}


double ra4_handler::getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex){
  return vz - pv_z->at(firstGoodVertex) -((vx-pv_x->at(firstGoodVertex))*px+(vy-pv_y->at(firstGoodVertex))*py)*pz/(px*px+py*py); 
}

vector<int> ra4_handler::GetJets(vector<int> SigEl, vector<int> SigMu, vector<int> VetoEl, vector<int> VetoMu, float &HT){
  vector<int> jets;
  // vector<bool> jet_is_lepton(jets_AK4_pt->size(), false);
  HT = SigEl.size()+VetoEl.size()+SigMu.size()+VetoMu.size(); // To avoid warnings
  HT = 0;
  // // Finding jets that contain good leptons
  // for(uint index = 0; index < SigEl.size(); index++) {
  //   int ijet = els_jet_ind->at(SigEl[index]);
  //   if(ijet >= 0) {jet_is_lepton[ijet] = true;
  //   }
  // }
  // for(uint index = 0; index < VetoEl.size(); index++) {
  //   int ijet = els_jet_ind->at(VetoEl[index]);
  //   if(ijet >= 0) jet_is_lepton[ijet] = true;
  // }

  // for(uint index = 0; index < SigMu.size(); index++) {
  //   int ijet = mus_jet_ind->at(SigMu[index]);
  //   if(ijet >= 0) {jet_is_lepton[ijet] = true;
  //   }
  // }
  // for(uint index = 0; index < VetoMu.size(); index++) {
  //   int ijet = mus_jet_ind->at(VetoMu[index]);
  //   if(ijet >= 0) jet_is_lepton[ijet] = true;
  //}

  // Tau/photon cleaning, and calculation of HT
  for(uint ijet = 0; ijet<jets_AK4_pt->size(); ijet++) {
    //if(!isGoodJet(ijet) || jet_is_lepton[ijet]) continue;
    if(!isGoodJet(ijet) ) continue;

    // double tmpdR, partp, jetp = sqrt(pow(jets_AK4_px->at(ijet),2)+pow(jets_AK4_py->at(ijet),2)+pow(jets_AK4_pz->at(ijet),2));
    // bool useJet = true;
    // Tau cleaning: jet rejected if withing deltaR = 0.4 of tau, and momentum at least 60% from tau
    // for(uint index = 0; index < taus_pt->size(); index++) {
    //   tmpdR = dR(jets_AK4_eta->at(ijet), taus_eta->at(index), jets_AK4_phi->at(ijet), taus_phi->at(index));	
    //   partp = sqrt(pow(taus_px->at(index),2)+pow(taus_py->at(index),2)+pow(taus_pz->at(index),2));
    //   if(tmpdR < 0.4 && partp/jetp >= 0.6){useJet = false; break;}
    // }
    // if(!useJet) continue;

    // // Photon cleaning: jet rejected if withing deltaR = 0.4 of photon, and momentum at least 60% from photon
    // for(uint index = 0; index < photons_pt->size(); index++) {
    //   tmpdR = dR(jets_AK4_eta->at(ijet), photons_eta->at(index), jets_AK4_phi->at(ijet), photons_phi->at(index));	
    //   partp = sqrt(pow(photons_px->at(index),2)+pow(photons_py->at(index),2)+pow(photons_pz->at(index),2));
    //   if(tmpdR < 0.4 && partp/jetp >= 0.6){useJet = false; break;}
    // }
    // if(!useJet) continue;

    if(jets_AK4_pt->at(ijet) > JetPTThresholdHT) HT += jets_AK4_pt->at(ijet);
    if(jets_AK4_pt->at(ijet) > JetPTThresholdNJ) jets.push_back(ijet);
  } // Loop over jets
  return jets;
}

int ra4_handler::GetNumGoodJets(double ptThresh) const{
  int numGoodJets(0);
  for(uint ijet(0); ijet<jets_AK4_pt->size(); ++ijet){
    if(isGoodJet(ijet, ptThresh, 2.4)) ++numGoodJets;
  }
  return numGoodJets;
}

bool ra4_handler::isGoodJet(const unsigned int ijet, const double ptThresh, const double etaThresh) const{
  if(jets_AK4_pt->size()<=ijet) return false;
  if(!passedPFJetSelection(ijet)) return false;
  if(jets_AK4_pt->at(ijet)<ptThresh || fabs(jets_AK4_eta->at(ijet))>etaThresh) return false;
  return true;
}

bool ra4_handler::passedPFJetSelection(const unsigned int ijet) const{
  double rawRatio =(jets_AK4_rawPt->at(ijet)/jets_AK4_pt->at(ijet)); // Same as jets_AK4_corrFactorRaw
  const double jetenergy = jets_AK4_energy->at(ijet) * rawRatio;
  double NEF = -999., CEF = -999., NHF=-999., CHF=-999.;
  double chgMult=jets_AK4_chg_Mult->at(ijet);
  double numConst=jets_AK4_mu_Mult->at(ijet)+jets_AK4_neutral_Mult->at(ijet)+jets_AK4_chg_Mult->at(ijet);
   
  if(jetenergy > 0){
    NEF = jets_AK4_neutralEmE->at(ijet)/jetenergy;
    CEF = jets_AK4_chgEmE->at(ijet)/jetenergy;
    NHF = jets_AK4_neutralHadE->at(ijet)/jetenergy;
    CHF = jets_AK4_chgHadE->at(ijet)/jetenergy;   
  }
   
  //    cout<<"Jet "<<ijet<<" => NEF "<<NEF<<", CEF "<< CEF
  //        <<", NHF "<<NHF <<",  CHF "<< CHF<<", chgMult "<< chgMult
  //        <<", pt "<<jets_AK4_pt->at(ijet)<<", eta "<<jets_AK4_eta->at(ijet)
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
    //&& hbhefilter_decision
    && hcallaserfilter_decision 
    //&& ecalTPfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision; 
  //&& scrapingVeto_decision;
}

bool ra4_handler::IsMC(){
  return (sampleName.find("Run201") == string::npos);
}

ra4_handler::ra4_handler(const std::string &fileName, const bool fastMode):
  cfa(fileName){
  if (fastMode) { // turn off unnecessary branches
    //chainA.SetBranchStatus("standalone_t*",0);
    //chainB.SetBranchStatus("photons_*",0);
    chainB.SetBranchStatus("Nmets*",0);
    //chainB.SetBranchStatus("taus*",0);
    chainB.SetBranchStatus("pfcand*",0);
    //chainB.SetBranchStatus("mc_final*",0);
  }
}

