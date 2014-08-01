#include "cfa.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "TChain.h"
#include "TBranch.h"

cfa::cfa(const std::string& fileIn):
  chainA("eventA"),
  chainB("eventB"),
  sampleName(fileIn),
  totalEntries(0),
  cfaVersion(-1),
  trigger_decision(0),
  trigger_name(0),
  trigger_prescalevalue(0),
  standalone_triggerobject_pt(0),
  standalone_triggerobject_px(0),
  standalone_triggerobject_py(0),
  standalone_triggerobject_pz(0),
  standalone_triggerobject_et(0),
  standalone_triggerobject_energy(0),
  standalone_triggerobject_phi(0),
  standalone_triggerobject_eta(0),
  standalone_triggerobject_collectionname(0),
  PU_zpositions(0),
  PU_sumpT_lowpT(0),
  PU_sumpT_highpT(0),
  PU_ntrks_lowpT(0),
  PU_ntrks_highpT(0),
  PU_NumInteractions(0),
  PU_bunchCrossing(0),
  PU_TrueNumInteractions(0),
  trackingfailurefilter_decision(0),
  goodVerticesfilter_decision(0),
  cschalofilter_decision(0),
  trkPOGfilter_decision(0),
  trkPOG_logErrorTooManyClustersfilter_decision(0),
  EcalDeadCellTriggerPrimitivefilter_decision(0),
  ecallaserfilter_decision(0),
  trkPOG_manystripclus53Xfilter_decision(0),
  eebadscfilter_decision(0),
  METFiltersfilter_decision(0),
  HBHENoisefilter_decision(0),
  trkPOG_toomanystripclus53Xfilter_decision(0),
  hcallaserfilter_decision(0),
  els_isPF(0),
  mus_isPF(0),
  jets_AK4_maxpt_id(0),
  jets_AK4_mu_ind(0),
  jets_AK4_el_ind(0),
  taus_el_ind(0),
  taus_mu_ind(0),
  els_jet_ind(0),
  mus_jet_ind(0),
  b_trigger_decision(),
  b_trigger_name(),
  b_trigger_prescalevalue(),
  b_standalone_triggerobject_pt(),
  b_standalone_triggerobject_px(),
  b_standalone_triggerobject_py(),
  b_standalone_triggerobject_pz(),
  b_standalone_triggerobject_et(),
  b_standalone_triggerobject_energy(),
  b_standalone_triggerobject_phi(),
  b_standalone_triggerobject_eta(),
  b_standalone_triggerobject_collectionname(),
  b_PU_zpositions(),
  b_PU_sumpT_lowpT(),
  b_PU_sumpT_highpT(),
  b_PU_ntrks_lowpT(),
  b_PU_ntrks_highpT(),
  b_PU_NumInteractions(),
  b_PU_bunchCrossing(),
  b_PU_TrueNumInteractions(),
  b_trackingfailurefilter_decision(),
  b_goodVerticesfilter_decision(),
  b_cschalofilter_decision(),
  b_trkPOGfilter_decision(),
  b_trkPOG_logErrorTooManyClustersfilter_decision(),
  b_EcalDeadCellTriggerPrimitivefilter_decision(),
  b_ecallaserfilter_decision(),
  b_trkPOG_manystripclus53Xfilter_decision(),
  b_eebadscfilter_decision(),
  b_METFiltersfilter_decision(),
  b_HBHENoisefilter_decision(),
  b_trkPOG_toomanystripclus53Xfilter_decision(),
  b_hcallaserfilter_decision(),
  b_els_isPF(),
  b_mus_isPF(),
  b_jets_AK4_maxpt_id(),
  b_jets_AK4_mu_ind(),
  b_jets_AK4_el_ind(),
  b_taus_el_ind(),
  b_taus_mu_ind(),
  b_els_jet_ind(),
  b_mus_jet_ind(),
  NbeamSpot(0),
  beamSpot_x(0),
  beamSpot_y(0),
  beamSpot_z(0),
  beamSpot_x0Error(0),
  beamSpot_y0Error(0),
  beamSpot_z0Error(0),
  beamSpot_sigmaZ(0),
  beamSpot_sigmaZ0Error(0),
  beamSpot_dxdz(0),
  beamSpot_dxdzError(0),
  beamSpot_dydz(0),
  beamSpot_dydzError(0),
  beamSpot_beamWidthX(0),
  beamSpot_beamWidthY(0),
  beamSpot_beamWidthXError(0),
  beamSpot_beamWidthYError(0),
  Nels(0),
  els_energy(0),
  els_et(0),
  els_eta(0),
  els_phi(0),
  els_pt(0),
  els_px(0),
  els_py(0),
  els_pz(0),
  els_status(0),
  els_theta(0),
  els_pfIsolationR03_sumChargedHadronPt(0),
  els_pfIsolationR03_sumNeutralHadronEt(0),
  els_pfIsolationR03_sumPhotonEt(0),
  els_pfIsolationR03_sumPUPt(0),
  els_full5x5_sigmaIetaIeta(0),
  els_gen_id(0),
  els_gen_phi(0),
  els_gen_pt(0),
  els_gen_pz(0),
  els_gen_px(0),
  els_gen_py(0),
  els_gen_eta(0),
  els_gen_theta(0),
  els_gen_et(0),
  els_gen_mother_id(0),
  els_gen_mother_phi(0),
  els_gen_mother_pt(0),
  els_gen_mother_pz(0),
  els_gen_mother_px(0),
  els_gen_mother_py(0),
  els_gen_mother_eta(0),
  els_gen_mother_theta(0),
  els_gen_mother_et(0),
  els_tightId(0),
  els_looseId(0),
  els_robustTightId(0),
  els_robustLooseId(0),
  els_robustHighEnergyId(0),
  els_simpleEleId95relIso(0),
  els_simpleEleId90relIso(0),
  els_simpleEleId85relIso(0),
  els_simpleEleId80relIso(0),
  els_simpleEleId70relIso(0),
  els_simpleEleId60relIso(0),
  els_simpleEleId95cIso(0),
  els_simpleEleId90cIso(0),
  els_simpleEleId85cIso(0),
  els_simpleEleId80cIso(0),
  els_simpleEleId70cIso(0),
  els_simpleEleId60cIso(0),
  els_cIso(0),
  els_tIso(0),
  els_ecalIso(0),
  els_hcalIso(0),
  els_chi2(0),
  els_charge(0),
  els_caloEnergy(0),
  els_hadOverEm(0),
  els_hcalOverEcalBc(0),
  els_eOverPIn(0),
  els_eSeedOverPOut(0),
  els_sigmaEtaEta(0),
  els_sigmaIEtaIEta(0),
  els_scEnergy(0),
  els_scRawEnergy(0),
  els_scSeedEnergy(0),
  els_scEta(0),
  els_scPhi(0),
  els_scEtaWidth(0),
  els_scPhiWidth(0),
  els_scE1x5(0),
  els_scE2x5Max(0),
  els_scE5x5(0),
  els_isEB(0),
  els_isEE(0),
  els_dEtaIn(0),
  els_dPhiIn(0),
  els_dEtaOut(0),
  els_dPhiOut(0),
  els_numvalhits(0),
  els_numlosthits(0),
  els_basicClustersSize(0),
  els_tk_pz(0),
  els_tk_pt(0),
  els_tk_phi(0),
  els_tk_eta(0),
  els_d0dum(0),
  els_dz(0),
  els_vx(0),
  els_vy(0),
  els_vz(0),
  els_ndof(0),
  els_ptError(0),
  els_d0dumError(0),
  els_dzError(0),
  els_etaError(0),
  els_phiError(0),
  els_tk_charge(0),
  els_core_ecalDrivenSeed(0),
  els_n_inner_layer(0),
  els_n_outer_layer(0),
  els_ctf_tk_id(0),
  els_ctf_tk_charge(0),
  els_ctf_tk_eta(0),
  els_ctf_tk_phi(0),
  els_fbrem(0),
  els_shFracInnerHits(0),
  els_dr03EcalRecHitSumEt(0),
  els_dr03HcalTowerSumEt(0),
  els_dr03HcalDepth1TowerSumEt(0),
  els_dr03HcalDepth2TowerSumEt(0),
  els_dr03TkSumPt(0),
  els_dr04EcalRecHitSumEt(0),
  els_dr04HcalTowerSumEt(0),
  els_dr04HcalDepth1TowerSumEt(0),
  els_dr04HcalDepth2TowerSumEt(0),
  els_dr04TkSumPt(0),
  els_cpx(0),
  els_cpy(0),
  els_cpz(0),
  els_vpx(0),
  els_vpy(0),
  els_vpz(0),
  els_cx(0),
  els_cy(0),
  els_cz(0),
  els_PATpassConversionVeto(0),
  Njets_AK4(0),
  jets_AK4_status(0),
  jets_AK4_phi(0),
  jets_AK4_pt(0),
  jets_AK4_pz(0),
  jets_AK4_px(0),
  jets_AK4_py(0),
  jets_AK4_eta(0),
  jets_AK4_theta(0),
  jets_AK4_et(0),
  jets_AK4_energy(0),
  jets_AK4_parton_Id(0),
  jets_AK4_parton_motherId(0),
  jets_AK4_parton_pt(0),
  jets_AK4_parton_phi(0),
  jets_AK4_parton_eta(0),
  jets_AK4_parton_Energy(0),
  jets_AK4_parton_mass(0),
  jets_AK4_gen_et(0),
  jets_AK4_gen_pt(0),
  jets_AK4_gen_eta(0),
  jets_AK4_gen_phi(0),
  jets_AK4_gen_mass(0),
  jets_AK4_gen_Energy(0),
  jets_AK4_gen_Id(0),
  jets_AK4_gen_motherID(0),
  jets_AK4_gen_threeCharge(0),
  jets_AK4_partonFlavour(0),
  jets_AK4_btag_TC_highPur(0),
  jets_AK4_btag_TC_highEff(0),
  jets_AK4_btag_jetProb(0),
  jets_AK4_btag_jetBProb(0),
  jets_AK4_btag_softEle(0),
  jets_AK4_btag_softMuon(0),
  jets_AK4_btag_secVertexHighPur(0),
  jets_AK4_btag_secVertexHighEff(0),
  jets_AK4_btag_secVertexCombined(0),
  jets_AK4_jetCharge(0),
  jets_AK4_chgEmE(0),
  jets_AK4_chgHadE(0),
  jets_AK4_photonEnergy(0),
  jets_AK4_chgMuE(0),
  jets_AK4_chg_Mult(0),
  jets_AK4_neutralEmE(0),
  jets_AK4_neutralHadE(0),
  jets_AK4_neutral_Mult(0),
  jets_AK4_mu_Mult(0),
  jets_AK4_emf(0),
  jets_AK4_ehf(0),
  jets_AK4_n60(0),
  jets_AK4_n90(0),
  jets_AK4_etaetaMoment(0),
  jets_AK4_etaphiMoment(0),
  jets_AK4_phiphiMoment(0),
  jets_AK4_n90Hits(0),
  jets_AK4_fHPD(0),
  jets_AK4_fRBX(0),
  jets_AK4_hitsInN90(0),
  jets_AK4_nECALTowers(0),
  jets_AK4_nHCALTowers(0),
  jets_AK4_fSubDetector1(0),
  jets_AK4_fSubDetector2(0),
  jets_AK4_fSubDetector3(0),
  jets_AK4_fSubDetector4(0),
  jets_AK4_area(0),
  jets_AK4_corrFactorRaw(0),
  jets_AK4_rawPt(0),
  jets_AK4_mass(0),
  Nmc_doc(0),
  mc_doc_id(0),
  mc_doc_pt(0),
  mc_doc_px(0),
  mc_doc_py(0),
  mc_doc_pz(0),
  mc_doc_eta(0),
  mc_doc_phi(0),
  mc_doc_theta(0),
  mc_doc_energy(0),
  mc_doc_status(0),
  mc_doc_charge(0),
  mc_doc_mother_id(0),
  mc_doc_grandmother_id(0),
  mc_doc_ggrandmother_id(0),
  mc_doc_mother_pt(0),
  mc_doc_vertex_x(0),
  mc_doc_vertex_y(0),
  mc_doc_vertex_z(0),
  mc_doc_mass(0),
  mc_doc_numOfDaughters(0),
  mc_doc_numOfMothers(0),
  Nmc_electrons(0),
  mc_electrons_id(0),
  mc_electrons_pt(0),
  mc_electrons_px(0),
  mc_electrons_py(0),
  mc_electrons_pz(0),
  mc_electrons_eta(0),
  mc_electrons_phi(0),
  mc_electrons_theta(0),
  mc_electrons_status(0),
  mc_electrons_energy(0),
  mc_electrons_charge(0),
  mc_electrons_mother_id(0),
  mc_electrons_mother_pt(0),
  mc_electrons_grandmother_id(0),
  mc_electrons_ggrandmother_id(0),
  mc_electrons_vertex_x(0),
  mc_electrons_vertex_y(0),
  mc_electrons_vertex_z(0),
  mc_electrons_mass(0),
  mc_electrons_numOfDaughters(0),
  Nmc_final(0),
  mc_final_id(0),
  mc_final_pt(0),
  mc_final_px(0),
  mc_final_py(0),
  mc_final_pz(0),
  mc_final_eta(0),
  mc_final_phi(0),
  mc_final_theta(0),
  mc_final_energy(0),
  mc_final_status(0),
  mc_final_charge(0),
  mc_final_mother_id(0),
  mc_final_grandmother_id(0),
  mc_final_ggrandmother_id(0),
  mc_final_mother_pt(0),
  mc_final_vertex_x(0),
  mc_final_vertex_y(0),
  mc_final_vertex_z(0),
  mc_final_mass(0),
  mc_final_numOfDaughters(0),
  mc_final_numOfMothers(0),
  Nmc_jets(0),
  mc_jets_phi(0),
  mc_jets_pt(0),
  mc_jets_pz(0),
  mc_jets_px(0),
  mc_jets_py(0),
  mc_jets_eta(0),
  mc_jets_theta(0),
  mc_jets_et(0),
  mc_jets_energy(0),
  mc_jets_emEnergy(0),
  mc_jets_hadEnergy(0),
  mc_jets_invisibleEnergy(0),
  mc_jets_auxiliaryEnergy(0),
  mc_jets_etaetaMoment(0),
  mc_jets_etaphiMoment(0),
  mc_jets_phiphiMoment(0),
  mc_jets_mass(0),
  Nmc_mus(0),
  mc_mus_id(0),
  mc_mus_pt(0),
  mc_mus_px(0),
  mc_mus_py(0),
  mc_mus_pz(0),
  mc_mus_eta(0),
  mc_mus_phi(0),
  mc_mus_theta(0),
  mc_mus_status(0),
  mc_mus_energy(0),
  mc_mus_charge(0),
  mc_mus_mother_id(0),
  mc_mus_mother_pt(0),
  mc_mus_grandmother_id(0),
  mc_mus_ggrandmother_id(0),
  mc_mus_vertex_x(0),
  mc_mus_vertex_y(0),
  mc_mus_vertex_z(0),
  mc_mus_mass(0),
  mc_mus_numOfDaughters(0),
  Nmc_nues(0),
  mc_nues_id(0),
  mc_nues_pt(0),
  mc_nues_px(0),
  mc_nues_py(0),
  mc_nues_pz(0),
  mc_nues_eta(0),
  mc_nues_phi(0),
  mc_nues_theta(0),
  mc_nues_status(0),
  mc_nues_energy(0),
  mc_nues_charge(0),
  mc_nues_mother_id(0),
  mc_nues_mother_pt(0),
  mc_nues_grandmother_id(0),
  mc_nues_ggrandmother_id(0),
  mc_nues_vertex_x(0),
  mc_nues_vertex_y(0),
  mc_nues_vertex_z(0),
  mc_nues_mass(0),
  mc_nues_numOfDaughters(0),
  Nmc_numus(0),
  mc_numus_id(0),
  mc_numus_pt(0),
  mc_numus_px(0),
  mc_numus_py(0),
  mc_numus_pz(0),
  mc_numus_eta(0),
  mc_numus_phi(0),
  mc_numus_theta(0),
  mc_numus_status(0),
  mc_numus_energy(0),
  mc_numus_charge(0),
  mc_numus_mother_id(0),
  mc_numus_mother_pt(0),
  mc_numus_grandmother_id(0),
  mc_numus_ggrandmother_id(0),
  mc_numus_vertex_x(0),
  mc_numus_vertex_y(0),
  mc_numus_vertex_z(0),
  mc_numus_mass(0),
  mc_numus_numOfDaughters(0),
  Nmc_nutaus(0),
  mc_nutaus_id(0),
  mc_nutaus_pt(0),
  mc_nutaus_px(0),
  mc_nutaus_py(0),
  mc_nutaus_pz(0),
  mc_nutaus_eta(0),
  mc_nutaus_phi(0),
  mc_nutaus_theta(0),
  mc_nutaus_status(0),
  mc_nutaus_energy(0),
  mc_nutaus_charge(0),
  mc_nutaus_mother_id(0),
  mc_nutaus_mother_pt(0),
  mc_nutaus_grandmother_id(0),
  mc_nutaus_ggrandmother_id(0),
  mc_nutaus_vertex_x(0),
  mc_nutaus_vertex_y(0),
  mc_nutaus_vertex_z(0),
  mc_nutaus_mass(0),
  mc_nutaus_numOfDaughters(0),
  Nmc_photons(0),
  mc_photons_id(0),
  mc_photons_pt(0),
  mc_photons_px(0),
  mc_photons_py(0),
  mc_photons_pz(0),
  mc_photons_eta(0),
  mc_photons_phi(0),
  mc_photons_theta(0),
  mc_photons_status(0),
  mc_photons_energy(0),
  mc_photons_charge(0),
  mc_photons_mother_id(0),
  mc_photons_mother_pt(0),
  mc_photons_grandmother_id(0),
  mc_photons_ggrandmother_id(0),
  mc_photons_vertex_x(0),
  mc_photons_vertex_y(0),
  mc_photons_vertex_z(0),
  mc_photons_mass(0),
  mc_photons_numOfDaughters(0),
  Nmc_taus(0),
  mc_taus_id(0),
  mc_taus_pt(0),
  mc_taus_px(0),
  mc_taus_py(0),
  mc_taus_pz(0),
  mc_taus_eta(0),
  mc_taus_phi(0),
  mc_taus_theta(0),
  mc_taus_status(0),
  mc_taus_energy(0),
  mc_taus_charge(0),
  mc_taus_mother_id(0),
  mc_taus_mother_pt(0),
  mc_taus_grandmother_id(0),
  mc_taus_ggrandmother_id(0),
  mc_taus_vertex_x(0),
  mc_taus_vertex_y(0),
  mc_taus_vertex_z(0),
  mc_taus_mass(0),
  mc_taus_numOfDaughters(0),
  Nmets(0),
  mets_et(0),
  mets_phi(0),
  mets_ex(0),
  mets_ey(0),
  mets_gen_et(0),
  mets_gen_phi(0),
  mets_sign(0),
  mets_sumEt(0),
  mets_unCPhi(0),
  mets_unCPt(0),
  Nmus(0),
  mus_energy(0),
  mus_et(0),
  mus_eta(0),
  mus_phi(0),
  mus_pt(0),
  mus_px(0),
  mus_py(0),
  mus_pz(0),
  mus_status(0),
  mus_theta(0),
  mus_gen_id(0),
  mus_gen_phi(0),
  mus_gen_pt(0),
  mus_gen_pz(0),
  mus_gen_px(0),
  mus_gen_py(0),
  mus_gen_eta(0),
  mus_gen_theta(0),
  mus_gen_et(0),
  mus_gen_mother_id(0),
  mus_gen_mother_phi(0),
  mus_gen_mother_pt(0),
  mus_gen_mother_pz(0),
  mus_gen_mother_px(0),
  mus_gen_mother_py(0),
  mus_gen_mother_eta(0),
  mus_gen_mother_theta(0),
  mus_gen_mother_et(0),
  mus_tkHits(0),
  mus_cIso(0),
  mus_tIso(0),
  mus_ecalIso(0),
  mus_hcalIso(0),
  mus_ecalvetoDep(0),
  mus_hcalvetoDep(0),
  mus_calEnergyEm(0),
  mus_calEnergyHad(0),
  mus_calEnergyHo(0),
  mus_calEnergyEmS9(0),
  mus_calEnergyHadS9(0),
  mus_calEnergyHoS9(0),
  mus_iso03_emVetoEt(0),
  mus_iso03_hadVetoEt(0),
  mus_iso03_sumPt(0),
  mus_iso03_emEt(0),
  mus_iso03_hadEt(0),
  mus_iso03_hoEt(0),
  mus_iso03_nTracks(0),
  mus_iso05_sumPt(0),
  mus_iso05_emEt(0),
  mus_iso05_hadEt(0),
  mus_iso05_hoEt(0),
  mus_iso05_nTracks(0),
  mus_pfIsolationR03_sumChargedHadronPt(0),
  mus_pfIsolationR03_sumChargedParticlePt(0),
  mus_pfIsolationR03_sumNeutralHadronEt(0),
  mus_pfIsolationR03_sumNeutralHadronEtHighThreshold(0),
  mus_pfIsolationR03_sumPhotonEt(0),
  mus_pfIsolationR03_sumPhotonEtHighThreshold(0),
  mus_pfIsolationR03_sumPUPt(0),
  mus_pfIsolationR04_sumChargedHadronPt(0),
  mus_pfIsolationR04_sumChargedParticlePt(0),
  mus_pfIsolationR04_sumNeutralHadronEt(0),
  mus_pfIsolationR04_sumNeutralHadronEtHighThreshold(0),
  mus_pfIsolationR04_sumPhotonEt(0),
  mus_pfIsolationR04_sumPhotonEtHighThreshold(0),
  mus_pfIsolationR04_sumPUPt(0),
  mus_charge(0),
  mus_cm_chi2(0),
  mus_cm_ndof(0),
  mus_cm_chg(0),
  mus_cm_pt(0),
  mus_cm_px(0),
  mus_cm_py(0),
  mus_cm_pz(0),
  mus_cm_eta(0),
  mus_cm_phi(0),
  mus_cm_theta(0),
  mus_cm_d0dum(0),
  mus_cm_dz(0),
  mus_cm_vx(0),
  mus_cm_vy(0),
  mus_cm_vz(0),
  mus_cm_numvalhits(0),
  mus_cm_numlosthits(0),
  mus_cm_numvalMuonhits(0),
  mus_cm_d0dumErr(0),
  mus_cm_dzErr(0),
  mus_cm_ptErr(0),
  mus_cm_etaErr(0),
  mus_cm_phiErr(0),
  mus_tk_id(0),
  mus_tk_chi2(0),
  mus_tk_ndof(0),
  mus_tk_chg(0),
  mus_tk_pt(0),
  mus_tk_px(0),
  mus_tk_py(0),
  mus_tk_pz(0),
  mus_tk_eta(0),
  mus_tk_phi(0),
  mus_tk_theta(0),
  mus_tk_d0dum(0),
  mus_tk_dz(0),
  mus_tk_vx(0),
  mus_tk_vy(0),
  mus_tk_vz(0),
  mus_tk_numvalhits(0),
  mus_tk_numlosthits(0),
  mus_tk_d0dumErr(0),
  mus_tk_dzErr(0),
  mus_tk_ptErr(0),
  mus_tk_etaErr(0),
  mus_tk_phiErr(0),
  mus_tk_numvalPixelhits(0),
  mus_tk_numpixelWthMeasr(0),
  mus_stamu_chi2(0),
  mus_stamu_ndof(0),
  mus_stamu_chg(0),
  mus_stamu_pt(0),
  mus_stamu_px(0),
  mus_stamu_py(0),
  mus_stamu_pz(0),
  mus_stamu_eta(0),
  mus_stamu_phi(0),
  mus_stamu_theta(0),
  mus_stamu_d0dum(0),
  mus_stamu_dz(0),
  mus_stamu_vx(0),
  mus_stamu_vy(0),
  mus_stamu_vz(0),
  mus_stamu_numvalhits(0),
  mus_stamu_numlosthits(0),
  mus_stamu_d0dumErr(0),
  mus_stamu_dzErr(0),
  mus_stamu_ptErr(0),
  mus_stamu_etaErr(0),
  mus_stamu_phiErr(0),
  mus_num_matches(0),
  mus_isPFMuon(0),
  mus_isTrackerMuon(0),
  mus_isStandAloneMuon(0),
  mus_isCaloMuon(0),
  mus_isGlobalMuon(0),
  mus_isElectron(0),
  mus_isConvertedPhoton(0),
  mus_isPhoton(0),
  mus_id_All(0),
  mus_id_AllGlobalMuons(0),
  mus_id_AllStandAloneMuons(0),
  mus_id_AllTrackerMuons(0),
  mus_id_TrackerMuonArbitrated(0),
  mus_id_AllArbitrated(0),
  mus_id_GlobalMuonPromptTight(0),
  mus_id_TMLastStationLoose(0),
  mus_id_TMLastStationTight(0),
  mus_id_TM2DCompatibilityLoose(0),
  mus_id_TM2DCompatibilityTight(0),
  mus_id_TMOneStationLoose(0),
  mus_id_TMOneStationTight(0),
  mus_id_TMLastStationOptimizedLowPtLoose(0),
  mus_id_TMLastStationOptimizedLowPtTight(0),
  mus_tk_LayersWithMeasurement(0),
  mus_tk_PixelLayersWithMeasurement(0),
  mus_tk_ValidStripLayersWithMonoAndStereoHit(0),
  mus_tk_LayersWithoutMeasurement(0),
  mus_tk_ExpectedHitsInner(0),
  mus_tk_ExpectedHitsOuter(0),
  mus_cm_LayersWithMeasurement(0),
  mus_cm_PixelLayersWithMeasurement(0),
  mus_cm_ValidStripLayersWithMonoAndStereoHit(0),
  mus_cm_LayersWithoutMeasurement(0),
  mus_cm_ExpectedHitsInner(0),
  mus_cm_ExpectedHitsOuter(0),
  mus_picky_LayersWithMeasurement(0),
  mus_picky_PixelLayersWithMeasurement(0),
  mus_picky_ValidStripLayersWithMonoAndStereoHit(0),
  mus_picky_LayersWithoutMeasurement(0),
  mus_picky_ExpectedHitsInner(0),
  mus_picky_ExpectedHitsOuter(0),
  mus_tpfms_LayersWithMeasurement(0),
  mus_tpfms_PixelLayersWithMeasurement(0),
  mus_tpfms_ValidStripLayersWithMonoAndStereoHit(0),
  mus_tpfms_LayersWithoutMeasurement(0),
  mus_tpfms_ExpectedHitsInner(0),
  mus_tpfms_ExpectedHitsOuter(0),
  mus_picky_id(0),
  mus_picky_chi2(0),
  mus_picky_ndof(0),
  mus_picky_chg(0),
  mus_picky_pt(0),
  mus_picky_px(0),
  mus_picky_py(0),
  mus_picky_pz(0),
  mus_picky_eta(0),
  mus_picky_phi(0),
  mus_picky_theta(0),
  mus_picky_d0dum(0),
  mus_picky_dz(0),
  mus_picky_vx(0),
  mus_picky_vy(0),
  mus_picky_vz(0),
  mus_picky_numvalhits(0),
  mus_picky_numlosthits(0),
  mus_picky_d0dumErr(0),
  mus_picky_dzErr(0),
  mus_picky_ptErr(0),
  mus_picky_etaErr(0),
  mus_picky_phiErr(0),
  mus_picky_numvalPixelhits(0),
  mus_tpfms_id(0),
  mus_tpfms_chi2(0),
  mus_tpfms_ndof(0),
  mus_tpfms_chg(0),
  mus_tpfms_pt(0),
  mus_tpfms_px(0),
  mus_tpfms_py(0),
  mus_tpfms_pz(0),
  mus_tpfms_eta(0),
  mus_tpfms_phi(0),
  mus_tpfms_theta(0),
  mus_tpfms_d0dum(0),
  mus_tpfms_dz(0),
  mus_tpfms_vx(0),
  mus_tpfms_vy(0),
  mus_tpfms_vz(0),
  mus_tpfms_numvalhits(0),
  mus_tpfms_numlosthits(0),
  mus_tpfms_d0dumErr(0),
  mus_tpfms_dzErr(0),
  mus_tpfms_ptErr(0),
  mus_tpfms_etaErr(0),
  mus_tpfms_phiErr(0),
  mus_tpfms_numvalPixelhits(0),
  mus_dB(0),
  mus_numberOfMatchedStations(0),
  Npfcand(0),
  pfcand_pdgId(0),
  pfcand_pt(0),
  pfcand_pz(0),
  pfcand_px(0),
  pfcand_py(0),
  pfcand_eta(0),
  pfcand_phi(0),
  pfcand_theta(0),
  pfcand_energy(0),
  pfcand_charge(0),
  Nphotons(0),
  photons_energy(0),
  photons_et(0),
  photons_eta(0),
  photons_phi(0),
  photons_pt(0),
  photons_px(0),
  photons_py(0),
  photons_pz(0),
  photons_status(0),
  photons_theta(0),
  photons_hadOverEM(0),
  photons_hadTowOverEM(0),
  photons_scEnergy(0),
  photons_scRawEnergy(0),
  photons_scEta(0),
  photons_scPhi(0),
  photons_scEtaWidth(0),
  photons_scPhiWidth(0),
  photons_tIso(0),
  photons_ecalIso(0),
  photons_hcalIso(0),
  photons_isoEcalRecHitDR04(0),
  photons_isoHcalRecHitDR04(0),
  photons_isoSolidTrkConeDR04(0),
  photons_isoHollowTrkConeDR04(0),
  photons_nTrkSolidConeDR04(0),
  photons_nTrkHollowConeDR04(0),
  photons_isoEcalRecHitDR03(0),
  photons_isoHcalRecHitDR03(0),
  photons_isoSolidTrkConeDR03(0),
  photons_isoHollowTrkConeDR03(0),
  photons_nTrkSolidConeDR03(0),
  photons_nTrkHollowConeDR03(0),
  photons_isAlsoElectron(0),
  photons_hasPixelSeed(0),
  photons_isConverted(0),
  photons_isEBGap(0),
  photons_isEEGap(0),
  photons_isEBEEGap(0),
  photons_isEBPho(0),
  photons_isEEPho(0),
  photons_isLoosePhoton(0),
  photons_isTightPhoton(0),
  photons_maxEnergyXtal(0),
  photons_e1x5(0),
  photons_e2x5(0),
  photons_e3x3(0),
  photons_e5x5(0),
  photons_sigmaEtaEta(0),
  photons_sigmaIetaIeta(0),
  photons_r9(0),
  photons_gen_et(0),
  photons_gen_eta(0),
  photons_gen_phi(0),
  photons_gen_id(0),
  Npv(0),
  pv_x(0),
  pv_y(0),
  pv_z(0),
  pv_xErr(0),
  pv_yErr(0),
  pv_zErr(0),
  pv_chi2(0),
  pv_ndof(0),
  pv_isFake(0),
  pv_isValid(0),
  pv_tracksSize(0),
  Ntaus(0),
  taus_status(0),
  taus_phi(0),
  taus_pt(0),
  taus_pz(0),
  taus_px(0),
  taus_py(0),
  taus_eta(0),
  taus_theta(0),
  taus_et(0),
  taus_energy(0),
  taus_charge(0),
  taus_emf(0),
  taus_hcalTotOverPLead(0),
  taus_hcalMaxOverPLead(0),
  taus_hcal3x3OverPLead(0),
  taus_ecalStripSumEOverPLead(0),
  taus_elecPreIdOutput(0),
  taus_elecPreIdDecision(0),
  taus_leadPFChargedHadrCand_pt(0),
  taus_leadPFChargedHadrCand_charge(0),
  taus_leadPFChargedHadrCand_eta(0),
  taus_leadPFChargedHadrCand_ECAL_eta(0),
  taus_leadPFChargedHadrCand_phi(0),
  taus_isoPFGammaCandsEtSum(0),
  taus_isoPFChargedHadrCandsPtSum(0),
  taus_leadingTrackFinding(0),
  taus_leadingTrackPtCut(0),
  taus_trackIsolation(0),
  taus_ecalIsolation(0),
  taus_byIsolation(0),
  taus_againstElectron(0),
  taus_againstMuon(0),
  taus_taNC_quarter(0),
  taus_taNC_one(0),
  taus_taNC_half(0),
  taus_taNC_tenth(0),
  taus_taNC(0),
  taus_byIsoUsingLeadingPi(0),
  taus_tkIsoUsingLeadingPi(0),
  taus_ecalIsoUsingLeadingPi(0),
  taus_againstElectronLoose(0),
  taus_againstElectronMedium(0),
  taus_againstElectronTight(0),
  taus_againstElectronMVA(0),
  taus_againstMuonLoose(0),
  taus_againstMuonMedium(0),
  taus_againstMuonTight(0),
  taus_decayModeFinding(0),
  taus_byVLooseIsolation(0),
  taus_byLooseIsolation(0),
  taus_byMediumIsolation(0),
  taus_byTightIsolation(0),
  taus_byVLooseIsolationDeltaBetaCorr(0),
  taus_byLooseIsolationDeltaBetaCorr(0),
  taus_byMediumIsolationDeltaBetaCorr(0),
  taus_byTightIsolationDeltaBetaCorr(0),
  taus_signalPFChargedHadrCandsSize(0),
  taus_muDecision(0),
  taus_Nprongs(0),
  run(0),
  event(0),
  lumiblock(0),
  experimentType(0),
  bunchCrossing(0),
  orbitNumber(0),
  weight(0),
  model_params(0),
  b_NbeamSpot(),
  b_beamSpot_x(),
  b_beamSpot_y(),
  b_beamSpot_z(),
  b_beamSpot_x0Error(),
  b_beamSpot_y0Error(),
  b_beamSpot_z0Error(),
  b_beamSpot_sigmaZ(),
  b_beamSpot_sigmaZ0Error(),
  b_beamSpot_dxdz(),
  b_beamSpot_dxdzError(),
  b_beamSpot_dydz(),
  b_beamSpot_dydzError(),
  b_beamSpot_beamWidthX(),
  b_beamSpot_beamWidthY(),
  b_beamSpot_beamWidthXError(),
  b_beamSpot_beamWidthYError(),
  b_Nels(),
  b_els_energy(),
  b_els_et(),
  b_els_eta(),
  b_els_phi(),
  b_els_pt(),
  b_els_px(),
  b_els_py(),
  b_els_pz(),
  b_els_status(),
  b_els_theta(),
  b_els_pfIsolationR03_sumChargedHadronPt(),
  b_els_pfIsolationR03_sumNeutralHadronEt(),
  b_els_pfIsolationR03_sumPhotonEt(),
  b_els_pfIsolationR03_sumPUPt(),
  b_els_full5x5_sigmaIetaIeta(),
  b_els_gen_id(),
  b_els_gen_phi(),
  b_els_gen_pt(),
  b_els_gen_pz(),
  b_els_gen_px(),
  b_els_gen_py(),
  b_els_gen_eta(),
  b_els_gen_theta(),
  b_els_gen_et(),
  b_els_gen_mother_id(),
  b_els_gen_mother_phi(),
  b_els_gen_mother_pt(),
  b_els_gen_mother_pz(),
  b_els_gen_mother_px(),
  b_els_gen_mother_py(),
  b_els_gen_mother_eta(),
  b_els_gen_mother_theta(),
  b_els_gen_mother_et(),
  b_els_tightId(),
  b_els_looseId(),
  b_els_robustTightId(),
  b_els_robustLooseId(),
  b_els_robustHighEnergyId(),
  b_els_simpleEleId95relIso(),
  b_els_simpleEleId90relIso(),
  b_els_simpleEleId85relIso(),
  b_els_simpleEleId80relIso(),
  b_els_simpleEleId70relIso(),
  b_els_simpleEleId60relIso(),
  b_els_simpleEleId95cIso(),
  b_els_simpleEleId90cIso(),
  b_els_simpleEleId85cIso(),
  b_els_simpleEleId80cIso(),
  b_els_simpleEleId70cIso(),
  b_els_simpleEleId60cIso(),
  b_els_cIso(),
  b_els_tIso(),
  b_els_ecalIso(),
  b_els_hcalIso(),
  b_els_chi2(),
  b_els_charge(),
  b_els_caloEnergy(),
  b_els_hadOverEm(),
  b_els_hcalOverEcalBc(),
  b_els_eOverPIn(),
  b_els_eSeedOverPOut(),
  b_els_sigmaEtaEta(),
  b_els_sigmaIEtaIEta(),
  b_els_scEnergy(),
  b_els_scRawEnergy(),
  b_els_scSeedEnergy(),
  b_els_scEta(),
  b_els_scPhi(),
  b_els_scEtaWidth(),
  b_els_scPhiWidth(),
  b_els_scE1x5(),
  b_els_scE2x5Max(),
  b_els_scE5x5(),
  b_els_isEB(),
  b_els_isEE(),
  b_els_dEtaIn(),
  b_els_dPhiIn(),
  b_els_dEtaOut(),
  b_els_dPhiOut(),
  b_els_numvalhits(),
  b_els_numlosthits(),
  b_els_basicClustersSize(),
  b_els_tk_pz(),
  b_els_tk_pt(),
  b_els_tk_phi(),
  b_els_tk_eta(),
  b_els_d0dum(),
  b_els_dz(),
  b_els_vx(),
  b_els_vy(),
  b_els_vz(),
  b_els_ndof(),
  b_els_ptError(),
  b_els_d0dumError(),
  b_els_dzError(),
  b_els_etaError(),
  b_els_phiError(),
  b_els_tk_charge(),
  b_els_core_ecalDrivenSeed(),
  b_els_n_inner_layer(),
  b_els_n_outer_layer(),
  b_els_ctf_tk_id(),
  b_els_ctf_tk_charge(),
  b_els_ctf_tk_eta(),
  b_els_ctf_tk_phi(),
  b_els_fbrem(),
  b_els_shFracInnerHits(),
  b_els_dr03EcalRecHitSumEt(),
  b_els_dr03HcalTowerSumEt(),
  b_els_dr03HcalDepth1TowerSumEt(),
  b_els_dr03HcalDepth2TowerSumEt(),
  b_els_dr03TkSumPt(),
  b_els_dr04EcalRecHitSumEt(),
  b_els_dr04HcalTowerSumEt(),
  b_els_dr04HcalDepth1TowerSumEt(),
  b_els_dr04HcalDepth2TowerSumEt(),
  b_els_dr04TkSumPt(),
  b_els_cpx(),
  b_els_cpy(),
  b_els_cpz(),
  b_els_vpx(),
  b_els_vpy(),
  b_els_vpz(),
  b_els_cx(),
  b_els_cy(),
  b_els_cz(),
  b_els_PATpassConversionVeto(),
  b_Njets_AK4(),
  b_jets_AK4_status(),
  b_jets_AK4_phi(),
  b_jets_AK4_pt(),
  b_jets_AK4_pz(),
  b_jets_AK4_px(),
  b_jets_AK4_py(),
  b_jets_AK4_eta(),
  b_jets_AK4_theta(),
  b_jets_AK4_et(),
  b_jets_AK4_energy(),
  b_jets_AK4_parton_Id(),
  b_jets_AK4_parton_motherId(),
  b_jets_AK4_parton_pt(),
  b_jets_AK4_parton_phi(),
  b_jets_AK4_parton_eta(),
  b_jets_AK4_parton_Energy(),
  b_jets_AK4_parton_mass(),
  b_jets_AK4_gen_et(),
  b_jets_AK4_gen_pt(),
  b_jets_AK4_gen_eta(),
  b_jets_AK4_gen_phi(),
  b_jets_AK4_gen_mass(),
  b_jets_AK4_gen_Energy(),
  b_jets_AK4_gen_Id(),
  b_jets_AK4_gen_motherID(),
  b_jets_AK4_gen_threeCharge(),
  b_jets_AK4_partonFlavour(),
  b_jets_AK4_btag_TC_highPur(),
  b_jets_AK4_btag_TC_highEff(),
  b_jets_AK4_btag_jetProb(),
  b_jets_AK4_btag_jetBProb(),
  b_jets_AK4_btag_softEle(),
  b_jets_AK4_btag_softMuon(),
  b_jets_AK4_btag_secVertexHighPur(),
  b_jets_AK4_btag_secVertexHighEff(),
  b_jets_AK4_btag_secVertexCombined(),
  b_jets_AK4_jetCharge(),
  b_jets_AK4_chgEmE(),
  b_jets_AK4_chgHadE(),
  b_jets_AK4_photonEnergy(),
  b_jets_AK4_chgMuE(),
  b_jets_AK4_chg_Mult(),
  b_jets_AK4_neutralEmE(),
  b_jets_AK4_neutralHadE(),
  b_jets_AK4_neutral_Mult(),
  b_jets_AK4_mu_Mult(),
  b_jets_AK4_emf(),
  b_jets_AK4_ehf(),
  b_jets_AK4_n60(),
  b_jets_AK4_n90(),
  b_jets_AK4_etaetaMoment(),
  b_jets_AK4_etaphiMoment(),
  b_jets_AK4_phiphiMoment(),
  b_jets_AK4_n90Hits(),
  b_jets_AK4_fHPD(),
  b_jets_AK4_fRBX(),
  b_jets_AK4_hitsInN90(),
  b_jets_AK4_nECALTowers(),
  b_jets_AK4_nHCALTowers(),
  b_jets_AK4_fSubDetector1(),
  b_jets_AK4_fSubDetector2(),
  b_jets_AK4_fSubDetector3(),
  b_jets_AK4_fSubDetector4(),
  b_jets_AK4_area(),
  b_jets_AK4_corrFactorRaw(),
  b_jets_AK4_rawPt(),
  b_jets_AK4_mass(),
  b_Nmc_doc(),
  b_mc_doc_id(),
  b_mc_doc_pt(),
  b_mc_doc_px(),
  b_mc_doc_py(),
  b_mc_doc_pz(),
  b_mc_doc_eta(),
  b_mc_doc_phi(),
  b_mc_doc_theta(),
  b_mc_doc_energy(),
  b_mc_doc_status(),
  b_mc_doc_charge(),
  b_mc_doc_mother_id(),
  b_mc_doc_grandmother_id(),
  b_mc_doc_ggrandmother_id(),
  b_mc_doc_mother_pt(),
  b_mc_doc_vertex_x(),
  b_mc_doc_vertex_y(),
  b_mc_doc_vertex_z(),
  b_mc_doc_mass(),
  b_mc_doc_numOfDaughters(),
  b_mc_doc_numOfMothers(),
  b_Nmc_electrons(),
  b_mc_electrons_id(),
  b_mc_electrons_pt(),
  b_mc_electrons_px(),
  b_mc_electrons_py(),
  b_mc_electrons_pz(),
  b_mc_electrons_eta(),
  b_mc_electrons_phi(),
  b_mc_electrons_theta(),
  b_mc_electrons_status(),
  b_mc_electrons_energy(),
  b_mc_electrons_charge(),
  b_mc_electrons_mother_id(),
  b_mc_electrons_mother_pt(),
  b_mc_electrons_grandmother_id(),
  b_mc_electrons_ggrandmother_id(),
  b_mc_electrons_vertex_x(),
  b_mc_electrons_vertex_y(),
  b_mc_electrons_vertex_z(),
  b_mc_electrons_mass(),
  b_mc_electrons_numOfDaughters(),
  b_Nmc_final(),
  b_mc_final_id(),
  b_mc_final_pt(),
  b_mc_final_px(),
  b_mc_final_py(),
  b_mc_final_pz(),
  b_mc_final_eta(),
  b_mc_final_phi(),
  b_mc_final_theta(),
  b_mc_final_energy(),
  b_mc_final_status(),
  b_mc_final_charge(),
  b_mc_final_mother_id(),
  b_mc_final_grandmother_id(),
  b_mc_final_ggrandmother_id(),
  b_mc_final_mother_pt(),
  b_mc_final_vertex_x(),
  b_mc_final_vertex_y(),
  b_mc_final_vertex_z(),
  b_mc_final_mass(),
  b_mc_final_numOfDaughters(),
  b_mc_final_numOfMothers(),
  b_Nmc_jets(),
  b_mc_jets_phi(),
  b_mc_jets_pt(),
  b_mc_jets_pz(),
  b_mc_jets_px(),
  b_mc_jets_py(),
  b_mc_jets_eta(),
  b_mc_jets_theta(),
  b_mc_jets_et(),
  b_mc_jets_energy(),
  b_mc_jets_emEnergy(),
  b_mc_jets_hadEnergy(),
  b_mc_jets_invisibleEnergy(),
  b_mc_jets_auxiliaryEnergy(),
  b_mc_jets_etaetaMoment(),
  b_mc_jets_etaphiMoment(),
  b_mc_jets_phiphiMoment(),
  b_mc_jets_mass(),
  b_Nmc_mus(),
  b_mc_mus_id(),
  b_mc_mus_pt(),
  b_mc_mus_px(),
  b_mc_mus_py(),
  b_mc_mus_pz(),
  b_mc_mus_eta(),
  b_mc_mus_phi(),
  b_mc_mus_theta(),
  b_mc_mus_status(),
  b_mc_mus_energy(),
  b_mc_mus_charge(),
  b_mc_mus_mother_id(),
  b_mc_mus_mother_pt(),
  b_mc_mus_grandmother_id(),
  b_mc_mus_ggrandmother_id(),
  b_mc_mus_vertex_x(),
  b_mc_mus_vertex_y(),
  b_mc_mus_vertex_z(),
  b_mc_mus_mass(),
  b_mc_mus_numOfDaughters(),
  b_Nmc_nues(),
  b_mc_nues_id(),
  b_mc_nues_pt(),
  b_mc_nues_px(),
  b_mc_nues_py(),
  b_mc_nues_pz(),
  b_mc_nues_eta(),
  b_mc_nues_phi(),
  b_mc_nues_theta(),
  b_mc_nues_status(),
  b_mc_nues_energy(),
  b_mc_nues_charge(),
  b_mc_nues_mother_id(),
  b_mc_nues_mother_pt(),
  b_mc_nues_grandmother_id(),
  b_mc_nues_ggrandmother_id(),
  b_mc_nues_vertex_x(),
  b_mc_nues_vertex_y(),
  b_mc_nues_vertex_z(),
  b_mc_nues_mass(),
  b_mc_nues_numOfDaughters(),
  b_Nmc_numus(),
  b_mc_numus_id(),
  b_mc_numus_pt(),
  b_mc_numus_px(),
  b_mc_numus_py(),
  b_mc_numus_pz(),
  b_mc_numus_eta(),
  b_mc_numus_phi(),
  b_mc_numus_theta(),
  b_mc_numus_status(),
  b_mc_numus_energy(),
  b_mc_numus_charge(),
  b_mc_numus_mother_id(),
  b_mc_numus_mother_pt(),
  b_mc_numus_grandmother_id(),
  b_mc_numus_ggrandmother_id(),
  b_mc_numus_vertex_x(),
  b_mc_numus_vertex_y(),
  b_mc_numus_vertex_z(),
  b_mc_numus_mass(),
  b_mc_numus_numOfDaughters(),
  b_Nmc_nutaus(),
  b_mc_nutaus_id(),
  b_mc_nutaus_pt(),
  b_mc_nutaus_px(),
  b_mc_nutaus_py(),
  b_mc_nutaus_pz(),
  b_mc_nutaus_eta(),
  b_mc_nutaus_phi(),
  b_mc_nutaus_theta(),
  b_mc_nutaus_status(),
  b_mc_nutaus_energy(),
  b_mc_nutaus_charge(),
  b_mc_nutaus_mother_id(),
  b_mc_nutaus_mother_pt(),
  b_mc_nutaus_grandmother_id(),
  b_mc_nutaus_ggrandmother_id(),
  b_mc_nutaus_vertex_x(),
  b_mc_nutaus_vertex_y(),
  b_mc_nutaus_vertex_z(),
  b_mc_nutaus_mass(),
  b_mc_nutaus_numOfDaughters(),
  b_Nmc_photons(),
  b_mc_photons_id(),
  b_mc_photons_pt(),
  b_mc_photons_px(),
  b_mc_photons_py(),
  b_mc_photons_pz(),
  b_mc_photons_eta(),
  b_mc_photons_phi(),
  b_mc_photons_theta(),
  b_mc_photons_status(),
  b_mc_photons_energy(),
  b_mc_photons_charge(),
  b_mc_photons_mother_id(),
  b_mc_photons_mother_pt(),
  b_mc_photons_grandmother_id(),
  b_mc_photons_ggrandmother_id(),
  b_mc_photons_vertex_x(),
  b_mc_photons_vertex_y(),
  b_mc_photons_vertex_z(),
  b_mc_photons_mass(),
  b_mc_photons_numOfDaughters(),
  b_Nmc_taus(),
  b_mc_taus_id(),
  b_mc_taus_pt(),
  b_mc_taus_px(),
  b_mc_taus_py(),
  b_mc_taus_pz(),
  b_mc_taus_eta(),
  b_mc_taus_phi(),
  b_mc_taus_theta(),
  b_mc_taus_status(),
  b_mc_taus_energy(),
  b_mc_taus_charge(),
  b_mc_taus_mother_id(),
  b_mc_taus_mother_pt(),
  b_mc_taus_grandmother_id(),
  b_mc_taus_ggrandmother_id(),
  b_mc_taus_vertex_x(),
  b_mc_taus_vertex_y(),
  b_mc_taus_vertex_z(),
  b_mc_taus_mass(),
  b_mc_taus_numOfDaughters(),
  b_Nmets(),
  b_mets_et(),
  b_mets_phi(),
  b_mets_ex(),
  b_mets_ey(),
  b_mets_gen_et(),
  b_mets_gen_phi(),
  b_mets_sign(),
  b_mets_sumEt(),
  b_mets_unCPhi(),
  b_mets_unCPt(),
  b_Nmus(),
  b_mus_energy(),
  b_mus_et(),
  b_mus_eta(),
  b_mus_phi(),
  b_mus_pt(),
  b_mus_px(),
  b_mus_py(),
  b_mus_pz(),
  b_mus_status(),
  b_mus_theta(),
  b_mus_gen_id(),
  b_mus_gen_phi(),
  b_mus_gen_pt(),
  b_mus_gen_pz(),
  b_mus_gen_px(),
  b_mus_gen_py(),
  b_mus_gen_eta(),
  b_mus_gen_theta(),
  b_mus_gen_et(),
  b_mus_gen_mother_id(),
  b_mus_gen_mother_phi(),
  b_mus_gen_mother_pt(),
  b_mus_gen_mother_pz(),
  b_mus_gen_mother_px(),
  b_mus_gen_mother_py(),
  b_mus_gen_mother_eta(),
  b_mus_gen_mother_theta(),
  b_mus_gen_mother_et(),
  b_mus_tkHits(),
  b_mus_cIso(),
  b_mus_tIso(),
  b_mus_ecalIso(),
  b_mus_hcalIso(),
  b_mus_ecalvetoDep(),
  b_mus_hcalvetoDep(),
  b_mus_calEnergyEm(),
  b_mus_calEnergyHad(),
  b_mus_calEnergyHo(),
  b_mus_calEnergyEmS9(),
  b_mus_calEnergyHadS9(),
  b_mus_calEnergyHoS9(),
  b_mus_iso03_emVetoEt(),
  b_mus_iso03_hadVetoEt(),
  b_mus_iso03_sumPt(),
  b_mus_iso03_emEt(),
  b_mus_iso03_hadEt(),
  b_mus_iso03_hoEt(),
  b_mus_iso03_nTracks(),
  b_mus_iso05_sumPt(),
  b_mus_iso05_emEt(),
  b_mus_iso05_hadEt(),
  b_mus_iso05_hoEt(),
  b_mus_iso05_nTracks(),
  b_mus_pfIsolationR03_sumChargedHadronPt(),
  b_mus_pfIsolationR03_sumChargedParticlePt(),
  b_mus_pfIsolationR03_sumNeutralHadronEt(),
  b_mus_pfIsolationR03_sumNeutralHadronEtHighThreshold(),
  b_mus_pfIsolationR03_sumPhotonEt(),
  b_mus_pfIsolationR03_sumPhotonEtHighThreshold(),
  b_mus_pfIsolationR03_sumPUPt(),
  b_mus_pfIsolationR04_sumChargedHadronPt(),
  b_mus_pfIsolationR04_sumChargedParticlePt(),
  b_mus_pfIsolationR04_sumNeutralHadronEt(),
  b_mus_pfIsolationR04_sumNeutralHadronEtHighThreshold(),
  b_mus_pfIsolationR04_sumPhotonEt(),
  b_mus_pfIsolationR04_sumPhotonEtHighThreshold(),
  b_mus_pfIsolationR04_sumPUPt(),
  b_mus_charge(),
  b_mus_cm_chi2(),
  b_mus_cm_ndof(),
  b_mus_cm_chg(),
  b_mus_cm_pt(),
  b_mus_cm_px(),
  b_mus_cm_py(),
  b_mus_cm_pz(),
  b_mus_cm_eta(),
  b_mus_cm_phi(),
  b_mus_cm_theta(),
  b_mus_cm_d0dum(),
  b_mus_cm_dz(),
  b_mus_cm_vx(),
  b_mus_cm_vy(),
  b_mus_cm_vz(),
  b_mus_cm_numvalhits(),
  b_mus_cm_numlosthits(),
  b_mus_cm_numvalMuonhits(),
  b_mus_cm_d0dumErr(),
  b_mus_cm_dzErr(),
  b_mus_cm_ptErr(),
  b_mus_cm_etaErr(),
  b_mus_cm_phiErr(),
  b_mus_tk_id(),
  b_mus_tk_chi2(),
  b_mus_tk_ndof(),
  b_mus_tk_chg(),
  b_mus_tk_pt(),
  b_mus_tk_px(),
  b_mus_tk_py(),
  b_mus_tk_pz(),
  b_mus_tk_eta(),
  b_mus_tk_phi(),
  b_mus_tk_theta(),
  b_mus_tk_d0dum(),
  b_mus_tk_dz(),
  b_mus_tk_vx(),
  b_mus_tk_vy(),
  b_mus_tk_vz(),
  b_mus_tk_numvalhits(),
  b_mus_tk_numlosthits(),
  b_mus_tk_d0dumErr(),
  b_mus_tk_dzErr(),
  b_mus_tk_ptErr(),
  b_mus_tk_etaErr(),
  b_mus_tk_phiErr(),
  b_mus_tk_numvalPixelhits(),
  b_mus_tk_numpixelWthMeasr(),
  b_mus_stamu_chi2(),
  b_mus_stamu_ndof(),
  b_mus_stamu_chg(),
  b_mus_stamu_pt(),
  b_mus_stamu_px(),
  b_mus_stamu_py(),
  b_mus_stamu_pz(),
  b_mus_stamu_eta(),
  b_mus_stamu_phi(),
  b_mus_stamu_theta(),
  b_mus_stamu_d0dum(),
  b_mus_stamu_dz(),
  b_mus_stamu_vx(),
  b_mus_stamu_vy(),
  b_mus_stamu_vz(),
  b_mus_stamu_numvalhits(),
  b_mus_stamu_numlosthits(),
  b_mus_stamu_d0dumErr(),
  b_mus_stamu_dzErr(),
  b_mus_stamu_ptErr(),
  b_mus_stamu_etaErr(),
  b_mus_stamu_phiErr(),
  b_mus_num_matches(),
  b_mus_isPFMuon(),
  b_mus_isTrackerMuon(),
  b_mus_isStandAloneMuon(),
  b_mus_isCaloMuon(),
  b_mus_isGlobalMuon(),
  b_mus_isElectron(),
  b_mus_isConvertedPhoton(),
  b_mus_isPhoton(),
  b_mus_id_All(),
  b_mus_id_AllGlobalMuons(),
  b_mus_id_AllStandAloneMuons(),
  b_mus_id_AllTrackerMuons(),
  b_mus_id_TrackerMuonArbitrated(),
  b_mus_id_AllArbitrated(),
  b_mus_id_GlobalMuonPromptTight(),
  b_mus_id_TMLastStationLoose(),
  b_mus_id_TMLastStationTight(),
  b_mus_id_TM2DCompatibilityLoose(),
  b_mus_id_TM2DCompatibilityTight(),
  b_mus_id_TMOneStationLoose(),
  b_mus_id_TMOneStationTight(),
  b_mus_id_TMLastStationOptimizedLowPtLoose(),
  b_mus_id_TMLastStationOptimizedLowPtTight(),
  b_mus_tk_LayersWithMeasurement(),
  b_mus_tk_PixelLayersWithMeasurement(),
  b_mus_tk_ValidStripLayersWithMonoAndStereoHit(),
  b_mus_tk_LayersWithoutMeasurement(),
  b_mus_tk_ExpectedHitsInner(),
  b_mus_tk_ExpectedHitsOuter(),
  b_mus_cm_LayersWithMeasurement(),
  b_mus_cm_PixelLayersWithMeasurement(),
  b_mus_cm_ValidStripLayersWithMonoAndStereoHit(),
  b_mus_cm_LayersWithoutMeasurement(),
  b_mus_cm_ExpectedHitsInner(),
  b_mus_cm_ExpectedHitsOuter(),
  b_mus_picky_LayersWithMeasurement(),
  b_mus_picky_PixelLayersWithMeasurement(),
  b_mus_picky_ValidStripLayersWithMonoAndStereoHit(),
  b_mus_picky_LayersWithoutMeasurement(),
  b_mus_picky_ExpectedHitsInner(),
  b_mus_picky_ExpectedHitsOuter(),
  b_mus_tpfms_LayersWithMeasurement(),
  b_mus_tpfms_PixelLayersWithMeasurement(),
  b_mus_tpfms_ValidStripLayersWithMonoAndStereoHit(),
  b_mus_tpfms_LayersWithoutMeasurement(),
  b_mus_tpfms_ExpectedHitsInner(),
  b_mus_tpfms_ExpectedHitsOuter(),
  b_mus_picky_id(),
  b_mus_picky_chi2(),
  b_mus_picky_ndof(),
  b_mus_picky_chg(),
  b_mus_picky_pt(),
  b_mus_picky_px(),
  b_mus_picky_py(),
  b_mus_picky_pz(),
  b_mus_picky_eta(),
  b_mus_picky_phi(),
  b_mus_picky_theta(),
  b_mus_picky_d0dum(),
  b_mus_picky_dz(),
  b_mus_picky_vx(),
  b_mus_picky_vy(),
  b_mus_picky_vz(),
  b_mus_picky_numvalhits(),
  b_mus_picky_numlosthits(),
  b_mus_picky_d0dumErr(),
  b_mus_picky_dzErr(),
  b_mus_picky_ptErr(),
  b_mus_picky_etaErr(),
  b_mus_picky_phiErr(),
  b_mus_picky_numvalPixelhits(),
  b_mus_tpfms_id(),
  b_mus_tpfms_chi2(),
  b_mus_tpfms_ndof(),
  b_mus_tpfms_chg(),
  b_mus_tpfms_pt(),
  b_mus_tpfms_px(),
  b_mus_tpfms_py(),
  b_mus_tpfms_pz(),
  b_mus_tpfms_eta(),
  b_mus_tpfms_phi(),
  b_mus_tpfms_theta(),
  b_mus_tpfms_d0dum(),
  b_mus_tpfms_dz(),
  b_mus_tpfms_vx(),
  b_mus_tpfms_vy(),
  b_mus_tpfms_vz(),
  b_mus_tpfms_numvalhits(),
  b_mus_tpfms_numlosthits(),
  b_mus_tpfms_d0dumErr(),
  b_mus_tpfms_dzErr(),
  b_mus_tpfms_ptErr(),
  b_mus_tpfms_etaErr(),
  b_mus_tpfms_phiErr(),
  b_mus_tpfms_numvalPixelhits(),
  b_mus_dB(),
  b_mus_numberOfMatchedStations(),
  b_Npfcand(),
  b_pfcand_pdgId(),
  b_pfcand_pt(),
  b_pfcand_pz(),
  b_pfcand_px(),
  b_pfcand_py(),
  b_pfcand_eta(),
  b_pfcand_phi(),
  b_pfcand_theta(),
  b_pfcand_energy(),
  b_pfcand_charge(),
  b_Nphotons(),
  b_photons_energy(),
  b_photons_et(),
  b_photons_eta(),
  b_photons_phi(),
  b_photons_pt(),
  b_photons_px(),
  b_photons_py(),
  b_photons_pz(),
  b_photons_status(),
  b_photons_theta(),
  b_photons_hadOverEM(),
  b_photons_hadTowOverEM(),
  b_photons_scEnergy(),
  b_photons_scRawEnergy(),
  b_photons_scEta(),
  b_photons_scPhi(),
  b_photons_scEtaWidth(),
  b_photons_scPhiWidth(),
  b_photons_tIso(),
  b_photons_ecalIso(),
  b_photons_hcalIso(),
  b_photons_isoEcalRecHitDR04(),
  b_photons_isoHcalRecHitDR04(),
  b_photons_isoSolidTrkConeDR04(),
  b_photons_isoHollowTrkConeDR04(),
  b_photons_nTrkSolidConeDR04(),
  b_photons_nTrkHollowConeDR04(),
  b_photons_isoEcalRecHitDR03(),
  b_photons_isoHcalRecHitDR03(),
  b_photons_isoSolidTrkConeDR03(),
  b_photons_isoHollowTrkConeDR03(),
  b_photons_nTrkSolidConeDR03(),
  b_photons_nTrkHollowConeDR03(),
  b_photons_isAlsoElectron(),
  b_photons_hasPixelSeed(),
  b_photons_isConverted(),
  b_photons_isEBGap(),
  b_photons_isEEGap(),
  b_photons_isEBEEGap(),
  b_photons_isEBPho(),
  b_photons_isEEPho(),
  b_photons_isLoosePhoton(),
  b_photons_isTightPhoton(),
  b_photons_maxEnergyXtal(),
  b_photons_e1x5(),
  b_photons_e2x5(),
  b_photons_e3x3(),
  b_photons_e5x5(),
  b_photons_sigmaEtaEta(),
  b_photons_sigmaIetaIeta(),
  b_photons_r9(),
  b_photons_gen_et(),
  b_photons_gen_eta(),
  b_photons_gen_phi(),
  b_photons_gen_id(),
  b_Npv(),
  b_pv_x(),
  b_pv_y(),
  b_pv_z(),
  b_pv_xErr(),
  b_pv_yErr(),
  b_pv_zErr(),
  b_pv_chi2(),
  b_pv_ndof(),
  b_pv_isFake(),
  b_pv_isValid(),
  b_pv_tracksSize(),
  b_Ntaus(),
  b_taus_status(),
  b_taus_phi(),
  b_taus_pt(),
  b_taus_pz(),
  b_taus_px(),
  b_taus_py(),
  b_taus_eta(),
  b_taus_theta(),
  b_taus_et(),
  b_taus_energy(),
  b_taus_charge(),
  b_taus_emf(),
  b_taus_hcalTotOverPLead(),
  b_taus_hcalMaxOverPLead(),
  b_taus_hcal3x3OverPLead(),
  b_taus_ecalStripSumEOverPLead(),
  b_taus_elecPreIdOutput(),
  b_taus_elecPreIdDecision(),
  b_taus_leadPFChargedHadrCand_pt(),
  b_taus_leadPFChargedHadrCand_charge(),
  b_taus_leadPFChargedHadrCand_eta(),
  b_taus_leadPFChargedHadrCand_ECAL_eta(),
  b_taus_leadPFChargedHadrCand_phi(),
  b_taus_isoPFGammaCandsEtSum(),
  b_taus_isoPFChargedHadrCandsPtSum(),
  b_taus_leadingTrackFinding(),
  b_taus_leadingTrackPtCut(),
  b_taus_trackIsolation(),
  b_taus_ecalIsolation(),
  b_taus_byIsolation(),
  b_taus_againstElectron(),
  b_taus_againstMuon(),
  b_taus_taNC_quarter(),
  b_taus_taNC_one(),
  b_taus_taNC_half(),
  b_taus_taNC_tenth(),
  b_taus_taNC(),
  b_taus_byIsoUsingLeadingPi(),
  b_taus_tkIsoUsingLeadingPi(),
  b_taus_ecalIsoUsingLeadingPi(),
  b_taus_againstElectronLoose(),
  b_taus_againstElectronMedium(),
  b_taus_againstElectronTight(),
  b_taus_againstElectronMVA(),
  b_taus_againstMuonLoose(),
  b_taus_againstMuonMedium(),
  b_taus_againstMuonTight(),
  b_taus_decayModeFinding(),
  b_taus_byVLooseIsolation(),
  b_taus_byLooseIsolation(),
  b_taus_byMediumIsolation(),
  b_taus_byTightIsolation(),
  b_taus_byVLooseIsolationDeltaBetaCorr(),
  b_taus_byLooseIsolationDeltaBetaCorr(),
  b_taus_byMediumIsolationDeltaBetaCorr(),
  b_taus_byTightIsolationDeltaBetaCorr(),
  b_taus_signalPFChargedHadrCandsSize(),
  b_taus_muDecision(),
  b_taus_Nprongs(),
  b_run(),
  b_event(),
  b_lumiblock(),
  b_experimentType(),
  b_bunchCrossing(),
  b_orbitNumber(),
  b_weight(),
  b_model_params(){
  GetVersion();
  AddFiles(fileIn);
  PrepareNewChains();
}

void cfa::GetVersion(){
  size_t pos(sampleName.rfind("_v"));
  if(pos!=std::string::npos && pos<sampleName.size()-2){
    std::istringstream iss(sampleName.substr(pos+2));
    iss >> cfaVersion;
    if(iss.fail() || iss.bad()){
      cfaVersion=-1;
    }
  }
}

void cfa::PrepareNewChains(){
  InitializeA();
  InitializeB();
  CalcTotalEntries();
}

void cfa::AddFiles(const std::string& fileIn){
  chainA.Add((fileIn+"/configurableAnalysis/eventA").c_str());
  chainB.Add((fileIn+"/configurableAnalysis/eventB").c_str());
}

void cfa::SetFile(const std::string& fileIn){
  chainA.Reset(); chainB.Reset();
  AddFiles(fileIn);
}

int cfa::GetEntry(const unsigned int entryIn){
  return chainA.GetEntry(entryIn)+chainB.GetEntry(entryIn);
}

void cfa::CalcTotalEntries(){
  const int nEntriesA(chainA.GetEntries()), nEntriesB(chainB.GetEntries());
  if (nEntriesA!=nEntriesB){
    totalEntries=-1;
  }else{
    totalEntries=nEntriesA;
  }
}

TChain* cfa::GetChainA(){
  return &chainA;
}

TChain* cfa::GetChainB(){
  return &chainB;
}

std::string cfa::GetSampleName() const{
  return sampleName;
}

int cfa::GetTotalEntries() const{
  return totalEntries;
}

void cfa::InitializeA(){
  trigger_decision=0;
  trigger_name=0;
  trigger_prescalevalue=0;
  standalone_triggerobject_pt=0;
  standalone_triggerobject_px=0;
  standalone_triggerobject_py=0;
  standalone_triggerobject_pz=0;
  standalone_triggerobject_et=0;
  standalone_triggerobject_energy=0;
  standalone_triggerobject_phi=0;
  standalone_triggerobject_eta=0;
  standalone_triggerobject_collectionname=0;
  PU_zpositions=0;
  PU_sumpT_lowpT=0;
  PU_sumpT_highpT=0;
  PU_ntrks_lowpT=0;
  PU_ntrks_highpT=0;
  PU_NumInteractions=0;
  PU_bunchCrossing=0;
  PU_TrueNumInteractions=0;
  trackingfailurefilter_decision=0;
  goodVerticesfilter_decision=0;
  cschalofilter_decision=0;
  trkPOGfilter_decision=0;
  trkPOG_logErrorTooManyClustersfilter_decision=0;
  EcalDeadCellTriggerPrimitivefilter_decision=0;
  ecallaserfilter_decision=0;
  trkPOG_manystripclus53Xfilter_decision=0;
  eebadscfilter_decision=0;
  METFiltersfilter_decision=0;
  HBHENoisefilter_decision=0;
  trkPOG_toomanystripclus53Xfilter_decision=0;
  hcallaserfilter_decision=0;
  els_isPF=0;
  mus_isPF=0;
  jets_AK4_maxpt_id=0;
  jets_AK4_mu_ind=0;
  jets_AK4_el_ind=0;
  taus_el_ind=0;
  taus_mu_ind=0;
  els_jet_ind=0;
  mus_jet_ind=0;
  chainA.SetBranchAddress("trigger_decision", &trigger_decision, &b_trigger_decision);
  chainA.SetBranchAddress("trigger_name", &trigger_name, &b_trigger_name);
  chainA.SetBranchAddress("trigger_prescalevalue", &trigger_prescalevalue, &b_trigger_prescalevalue);
  chainA.SetBranchAddress("standalone_triggerobject_pt", &standalone_triggerobject_pt, &b_standalone_triggerobject_pt);
  chainA.SetBranchAddress("standalone_triggerobject_px", &standalone_triggerobject_px, &b_standalone_triggerobject_px);
  chainA.SetBranchAddress("standalone_triggerobject_py", &standalone_triggerobject_py, &b_standalone_triggerobject_py);
  chainA.SetBranchAddress("standalone_triggerobject_pz", &standalone_triggerobject_pz, &b_standalone_triggerobject_pz);
  chainA.SetBranchAddress("standalone_triggerobject_et", &standalone_triggerobject_et, &b_standalone_triggerobject_et);
  chainA.SetBranchAddress("standalone_triggerobject_energy", &standalone_triggerobject_energy, &b_standalone_triggerobject_energy);
  chainA.SetBranchAddress("standalone_triggerobject_phi", &standalone_triggerobject_phi, &b_standalone_triggerobject_phi);
  chainA.SetBranchAddress("standalone_triggerobject_eta", &standalone_triggerobject_eta, &b_standalone_triggerobject_eta);
  chainA.SetBranchAddress("standalone_triggerobject_collectionname", &standalone_triggerobject_collectionname, &b_standalone_triggerobject_collectionname);
  chainA.SetBranchAddress("PU_zpositions", &PU_zpositions, &b_PU_zpositions);
  chainA.SetBranchAddress("PU_sumpT_lowpT", &PU_sumpT_lowpT, &b_PU_sumpT_lowpT);
  chainA.SetBranchAddress("PU_sumpT_highpT", &PU_sumpT_highpT, &b_PU_sumpT_highpT);
  chainA.SetBranchAddress("PU_ntrks_lowpT", &PU_ntrks_lowpT, &b_PU_ntrks_lowpT);
  chainA.SetBranchAddress("PU_ntrks_highpT", &PU_ntrks_highpT, &b_PU_ntrks_highpT);
  chainA.SetBranchAddress("PU_NumInteractions", &PU_NumInteractions, &b_PU_NumInteractions);
  chainA.SetBranchAddress("PU_bunchCrossing", &PU_bunchCrossing, &b_PU_bunchCrossing);
  chainA.SetBranchAddress("PU_TrueNumInteractions", &PU_TrueNumInteractions, &b_PU_TrueNumInteractions);
  chainA.SetBranchAddress("trackingfailurefilter_decision", &trackingfailurefilter_decision, &b_trackingfailurefilter_decision);
  chainA.SetBranchAddress("goodVerticesfilter_decision", &goodVerticesfilter_decision, &b_goodVerticesfilter_decision);
  chainA.SetBranchAddress("cschalofilter_decision", &cschalofilter_decision, &b_cschalofilter_decision);
  chainA.SetBranchAddress("trkPOGfilter_decision", &trkPOGfilter_decision, &b_trkPOGfilter_decision);
  chainA.SetBranchAddress("trkPOG_logErrorTooManyClustersfilter_decision", &trkPOG_logErrorTooManyClustersfilter_decision, &b_trkPOG_logErrorTooManyClustersfilter_decision);
  chainA.SetBranchAddress("EcalDeadCellTriggerPrimitivefilter_decision", &EcalDeadCellTriggerPrimitivefilter_decision, &b_EcalDeadCellTriggerPrimitivefilter_decision);
  chainA.SetBranchAddress("ecallaserfilter_decision", &ecallaserfilter_decision, &b_ecallaserfilter_decision);
  chainA.SetBranchAddress("trkPOG_manystripclus53Xfilter_decision", &trkPOG_manystripclus53Xfilter_decision, &b_trkPOG_manystripclus53Xfilter_decision);
  chainA.SetBranchAddress("eebadscfilter_decision", &eebadscfilter_decision, &b_eebadscfilter_decision);
  chainA.SetBranchAddress("METFiltersfilter_decision", &METFiltersfilter_decision, &b_METFiltersfilter_decision);
  chainA.SetBranchAddress("HBHENoisefilter_decision", &HBHENoisefilter_decision, &b_HBHENoisefilter_decision);
  chainA.SetBranchAddress("trkPOG_toomanystripclus53Xfilter_decision", &trkPOG_toomanystripclus53Xfilter_decision, &b_trkPOG_toomanystripclus53Xfilter_decision);
  chainA.SetBranchAddress("hcallaserfilter_decision", &hcallaserfilter_decision, &b_hcallaserfilter_decision);
  chainA.SetBranchAddress("els_isPF", &els_isPF, &b_els_isPF);
  chainA.SetBranchAddress("mus_isPF", &mus_isPF, &b_mus_isPF);
  chainA.SetBranchAddress("jets_AK4_maxpt_id", &jets_AK4_maxpt_id, &b_jets_AK4_maxpt_id);
  chainA.SetBranchAddress("jets_AK4_mu_ind", &jets_AK4_mu_ind, &b_jets_AK4_mu_ind);
  chainA.SetBranchAddress("jets_AK4_el_ind", &jets_AK4_el_ind, &b_jets_AK4_el_ind);
  chainA.SetBranchAddress("taus_el_ind", &taus_el_ind, &b_taus_el_ind);
  chainA.SetBranchAddress("taus_mu_ind", &taus_mu_ind, &b_taus_mu_ind);
  chainA.SetBranchAddress("els_jet_ind", &els_jet_ind, &b_els_jet_ind);
  chainA.SetBranchAddress("mus_jet_ind", &mus_jet_ind, &b_mus_jet_ind);
}

void cfa::InitializeB(){
  NbeamSpot=0;
  beamSpot_x=0;
  beamSpot_y=0;
  beamSpot_z=0;
  beamSpot_x0Error=0;
  beamSpot_y0Error=0;
  beamSpot_z0Error=0;
  beamSpot_sigmaZ=0;
  beamSpot_sigmaZ0Error=0;
  beamSpot_dxdz=0;
  beamSpot_dxdzError=0;
  beamSpot_dydz=0;
  beamSpot_dydzError=0;
  beamSpot_beamWidthX=0;
  beamSpot_beamWidthY=0;
  beamSpot_beamWidthXError=0;
  beamSpot_beamWidthYError=0;
  Nels=0;
  els_energy=0;
  els_et=0;
  els_eta=0;
  els_phi=0;
  els_pt=0;
  els_px=0;
  els_py=0;
  els_pz=0;
  els_status=0;
  els_theta=0;
  els_pfIsolationR03_sumChargedHadronPt=0;
  els_pfIsolationR03_sumNeutralHadronEt=0;
  els_pfIsolationR03_sumPhotonEt=0;
  els_pfIsolationR03_sumPUPt=0;
  els_full5x5_sigmaIetaIeta=0;
  els_gen_id=0;
  els_gen_phi=0;
  els_gen_pt=0;
  els_gen_pz=0;
  els_gen_px=0;
  els_gen_py=0;
  els_gen_eta=0;
  els_gen_theta=0;
  els_gen_et=0;
  els_gen_mother_id=0;
  els_gen_mother_phi=0;
  els_gen_mother_pt=0;
  els_gen_mother_pz=0;
  els_gen_mother_px=0;
  els_gen_mother_py=0;
  els_gen_mother_eta=0;
  els_gen_mother_theta=0;
  els_gen_mother_et=0;
  els_tightId=0;
  els_looseId=0;
  els_robustTightId=0;
  els_robustLooseId=0;
  els_robustHighEnergyId=0;
  els_simpleEleId95relIso=0;
  els_simpleEleId90relIso=0;
  els_simpleEleId85relIso=0;
  els_simpleEleId80relIso=0;
  els_simpleEleId70relIso=0;
  els_simpleEleId60relIso=0;
  els_simpleEleId95cIso=0;
  els_simpleEleId90cIso=0;
  els_simpleEleId85cIso=0;
  els_simpleEleId80cIso=0;
  els_simpleEleId70cIso=0;
  els_simpleEleId60cIso=0;
  els_cIso=0;
  els_tIso=0;
  els_ecalIso=0;
  els_hcalIso=0;
  els_chi2=0;
  els_charge=0;
  els_caloEnergy=0;
  els_hadOverEm=0;
  els_hcalOverEcalBc=0;
  els_eOverPIn=0;
  els_eSeedOverPOut=0;
  els_sigmaEtaEta=0;
  els_sigmaIEtaIEta=0;
  els_scEnergy=0;
  els_scRawEnergy=0;
  els_scSeedEnergy=0;
  els_scEta=0;
  els_scPhi=0;
  els_scEtaWidth=0;
  els_scPhiWidth=0;
  els_scE1x5=0;
  els_scE2x5Max=0;
  els_scE5x5=0;
  els_isEB=0;
  els_isEE=0;
  els_dEtaIn=0;
  els_dPhiIn=0;
  els_dEtaOut=0;
  els_dPhiOut=0;
  els_numvalhits=0;
  els_numlosthits=0;
  els_basicClustersSize=0;
  els_tk_pz=0;
  els_tk_pt=0;
  els_tk_phi=0;
  els_tk_eta=0;
  els_d0dum=0;
  els_dz=0;
  els_vx=0;
  els_vy=0;
  els_vz=0;
  els_ndof=0;
  els_ptError=0;
  els_d0dumError=0;
  els_dzError=0;
  els_etaError=0;
  els_phiError=0;
  els_tk_charge=0;
  els_core_ecalDrivenSeed=0;
  els_n_inner_layer=0;
  els_n_outer_layer=0;
  els_ctf_tk_id=0;
  els_ctf_tk_charge=0;
  els_ctf_tk_eta=0;
  els_ctf_tk_phi=0;
  els_fbrem=0;
  els_shFracInnerHits=0;
  els_dr03EcalRecHitSumEt=0;
  els_dr03HcalTowerSumEt=0;
  els_dr03HcalDepth1TowerSumEt=0;
  els_dr03HcalDepth2TowerSumEt=0;
  els_dr03TkSumPt=0;
  els_dr04EcalRecHitSumEt=0;
  els_dr04HcalTowerSumEt=0;
  els_dr04HcalDepth1TowerSumEt=0;
  els_dr04HcalDepth2TowerSumEt=0;
  els_dr04TkSumPt=0;
  els_cpx=0;
  els_cpy=0;
  els_cpz=0;
  els_vpx=0;
  els_vpy=0;
  els_vpz=0;
  els_cx=0;
  els_cy=0;
  els_cz=0;
  els_PATpassConversionVeto=0;
  Njets_AK4=0;
  jets_AK4_status=0;
  jets_AK4_phi=0;
  jets_AK4_pt=0;
  jets_AK4_pz=0;
  jets_AK4_px=0;
  jets_AK4_py=0;
  jets_AK4_eta=0;
  jets_AK4_theta=0;
  jets_AK4_et=0;
  jets_AK4_energy=0;
  jets_AK4_parton_Id=0;
  jets_AK4_parton_motherId=0;
  jets_AK4_parton_pt=0;
  jets_AK4_parton_phi=0;
  jets_AK4_parton_eta=0;
  jets_AK4_parton_Energy=0;
  jets_AK4_parton_mass=0;
  jets_AK4_gen_et=0;
  jets_AK4_gen_pt=0;
  jets_AK4_gen_eta=0;
  jets_AK4_gen_phi=0;
  jets_AK4_gen_mass=0;
  jets_AK4_gen_Energy=0;
  jets_AK4_gen_Id=0;
  jets_AK4_gen_motherID=0;
  jets_AK4_gen_threeCharge=0;
  jets_AK4_partonFlavour=0;
  jets_AK4_btag_TC_highPur=0;
  jets_AK4_btag_TC_highEff=0;
  jets_AK4_btag_jetProb=0;
  jets_AK4_btag_jetBProb=0;
  jets_AK4_btag_softEle=0;
  jets_AK4_btag_softMuon=0;
  jets_AK4_btag_secVertexHighPur=0;
  jets_AK4_btag_secVertexHighEff=0;
  jets_AK4_btag_secVertexCombined=0;
  jets_AK4_jetCharge=0;
  jets_AK4_chgEmE=0;
  jets_AK4_chgHadE=0;
  jets_AK4_photonEnergy=0;
  jets_AK4_chgMuE=0;
  jets_AK4_chg_Mult=0;
  jets_AK4_neutralEmE=0;
  jets_AK4_neutralHadE=0;
  jets_AK4_neutral_Mult=0;
  jets_AK4_mu_Mult=0;
  jets_AK4_emf=0;
  jets_AK4_ehf=0;
  jets_AK4_n60=0;
  jets_AK4_n90=0;
  jets_AK4_etaetaMoment=0;
  jets_AK4_etaphiMoment=0;
  jets_AK4_phiphiMoment=0;
  jets_AK4_n90Hits=0;
  jets_AK4_fHPD=0;
  jets_AK4_fRBX=0;
  jets_AK4_hitsInN90=0;
  jets_AK4_nECALTowers=0;
  jets_AK4_nHCALTowers=0;
  jets_AK4_fSubDetector1=0;
  jets_AK4_fSubDetector2=0;
  jets_AK4_fSubDetector3=0;
  jets_AK4_fSubDetector4=0;
  jets_AK4_area=0;
  jets_AK4_corrFactorRaw=0;
  jets_AK4_rawPt=0;
  jets_AK4_mass=0;
  Nmc_doc=0;
  mc_doc_id=0;
  mc_doc_pt=0;
  mc_doc_px=0;
  mc_doc_py=0;
  mc_doc_pz=0;
  mc_doc_eta=0;
  mc_doc_phi=0;
  mc_doc_theta=0;
  mc_doc_energy=0;
  mc_doc_status=0;
  mc_doc_charge=0;
  mc_doc_mother_id=0;
  mc_doc_grandmother_id=0;
  mc_doc_ggrandmother_id=0;
  mc_doc_mother_pt=0;
  mc_doc_vertex_x=0;
  mc_doc_vertex_y=0;
  mc_doc_vertex_z=0;
  mc_doc_mass=0;
  mc_doc_numOfDaughters=0;
  mc_doc_numOfMothers=0;
  Nmc_electrons=0;
  mc_electrons_id=0;
  mc_electrons_pt=0;
  mc_electrons_px=0;
  mc_electrons_py=0;
  mc_electrons_pz=0;
  mc_electrons_eta=0;
  mc_electrons_phi=0;
  mc_electrons_theta=0;
  mc_electrons_status=0;
  mc_electrons_energy=0;
  mc_electrons_charge=0;
  mc_electrons_mother_id=0;
  mc_electrons_mother_pt=0;
  mc_electrons_grandmother_id=0;
  mc_electrons_ggrandmother_id=0;
  mc_electrons_vertex_x=0;
  mc_electrons_vertex_y=0;
  mc_electrons_vertex_z=0;
  mc_electrons_mass=0;
  mc_electrons_numOfDaughters=0;
  Nmc_final=0;
  mc_final_id=0;
  mc_final_pt=0;
  mc_final_px=0;
  mc_final_py=0;
  mc_final_pz=0;
  mc_final_eta=0;
  mc_final_phi=0;
  mc_final_theta=0;
  mc_final_energy=0;
  mc_final_status=0;
  mc_final_charge=0;
  mc_final_mother_id=0;
  mc_final_grandmother_id=0;
  mc_final_ggrandmother_id=0;
  mc_final_mother_pt=0;
  mc_final_vertex_x=0;
  mc_final_vertex_y=0;
  mc_final_vertex_z=0;
  mc_final_mass=0;
  mc_final_numOfDaughters=0;
  mc_final_numOfMothers=0;
  Nmc_jets=0;
  mc_jets_phi=0;
  mc_jets_pt=0;
  mc_jets_pz=0;
  mc_jets_px=0;
  mc_jets_py=0;
  mc_jets_eta=0;
  mc_jets_theta=0;
  mc_jets_et=0;
  mc_jets_energy=0;
  mc_jets_emEnergy=0;
  mc_jets_hadEnergy=0;
  mc_jets_invisibleEnergy=0;
  mc_jets_auxiliaryEnergy=0;
  mc_jets_etaetaMoment=0;
  mc_jets_etaphiMoment=0;
  mc_jets_phiphiMoment=0;
  mc_jets_mass=0;
  Nmc_mus=0;
  mc_mus_id=0;
  mc_mus_pt=0;
  mc_mus_px=0;
  mc_mus_py=0;
  mc_mus_pz=0;
  mc_mus_eta=0;
  mc_mus_phi=0;
  mc_mus_theta=0;
  mc_mus_status=0;
  mc_mus_energy=0;
  mc_mus_charge=0;
  mc_mus_mother_id=0;
  mc_mus_mother_pt=0;
  mc_mus_grandmother_id=0;
  mc_mus_ggrandmother_id=0;
  mc_mus_vertex_x=0;
  mc_mus_vertex_y=0;
  mc_mus_vertex_z=0;
  mc_mus_mass=0;
  mc_mus_numOfDaughters=0;
  Nmc_nues=0;
  mc_nues_id=0;
  mc_nues_pt=0;
  mc_nues_px=0;
  mc_nues_py=0;
  mc_nues_pz=0;
  mc_nues_eta=0;
  mc_nues_phi=0;
  mc_nues_theta=0;
  mc_nues_status=0;
  mc_nues_energy=0;
  mc_nues_charge=0;
  mc_nues_mother_id=0;
  mc_nues_mother_pt=0;
  mc_nues_grandmother_id=0;
  mc_nues_ggrandmother_id=0;
  mc_nues_vertex_x=0;
  mc_nues_vertex_y=0;
  mc_nues_vertex_z=0;
  mc_nues_mass=0;
  mc_nues_numOfDaughters=0;
  Nmc_numus=0;
  mc_numus_id=0;
  mc_numus_pt=0;
  mc_numus_px=0;
  mc_numus_py=0;
  mc_numus_pz=0;
  mc_numus_eta=0;
  mc_numus_phi=0;
  mc_numus_theta=0;
  mc_numus_status=0;
  mc_numus_energy=0;
  mc_numus_charge=0;
  mc_numus_mother_id=0;
  mc_numus_mother_pt=0;
  mc_numus_grandmother_id=0;
  mc_numus_ggrandmother_id=0;
  mc_numus_vertex_x=0;
  mc_numus_vertex_y=0;
  mc_numus_vertex_z=0;
  mc_numus_mass=0;
  mc_numus_numOfDaughters=0;
  Nmc_nutaus=0;
  mc_nutaus_id=0;
  mc_nutaus_pt=0;
  mc_nutaus_px=0;
  mc_nutaus_py=0;
  mc_nutaus_pz=0;
  mc_nutaus_eta=0;
  mc_nutaus_phi=0;
  mc_nutaus_theta=0;
  mc_nutaus_status=0;
  mc_nutaus_energy=0;
  mc_nutaus_charge=0;
  mc_nutaus_mother_id=0;
  mc_nutaus_mother_pt=0;
  mc_nutaus_grandmother_id=0;
  mc_nutaus_ggrandmother_id=0;
  mc_nutaus_vertex_x=0;
  mc_nutaus_vertex_y=0;
  mc_nutaus_vertex_z=0;
  mc_nutaus_mass=0;
  mc_nutaus_numOfDaughters=0;
  Nmc_photons=0;
  mc_photons_id=0;
  mc_photons_pt=0;
  mc_photons_px=0;
  mc_photons_py=0;
  mc_photons_pz=0;
  mc_photons_eta=0;
  mc_photons_phi=0;
  mc_photons_theta=0;
  mc_photons_status=0;
  mc_photons_energy=0;
  mc_photons_charge=0;
  mc_photons_mother_id=0;
  mc_photons_mother_pt=0;
  mc_photons_grandmother_id=0;
  mc_photons_ggrandmother_id=0;
  mc_photons_vertex_x=0;
  mc_photons_vertex_y=0;
  mc_photons_vertex_z=0;
  mc_photons_mass=0;
  mc_photons_numOfDaughters=0;
  Nmc_taus=0;
  mc_taus_id=0;
  mc_taus_pt=0;
  mc_taus_px=0;
  mc_taus_py=0;
  mc_taus_pz=0;
  mc_taus_eta=0;
  mc_taus_phi=0;
  mc_taus_theta=0;
  mc_taus_status=0;
  mc_taus_energy=0;
  mc_taus_charge=0;
  mc_taus_mother_id=0;
  mc_taus_mother_pt=0;
  mc_taus_grandmother_id=0;
  mc_taus_ggrandmother_id=0;
  mc_taus_vertex_x=0;
  mc_taus_vertex_y=0;
  mc_taus_vertex_z=0;
  mc_taus_mass=0;
  mc_taus_numOfDaughters=0;
  Nmets=0;
  mets_et=0;
  mets_phi=0;
  mets_ex=0;
  mets_ey=0;
  mets_gen_et=0;
  mets_gen_phi=0;
  mets_sign=0;
  mets_sumEt=0;
  mets_unCPhi=0;
  mets_unCPt=0;
  Nmus=0;
  mus_energy=0;
  mus_et=0;
  mus_eta=0;
  mus_phi=0;
  mus_pt=0;
  mus_px=0;
  mus_py=0;
  mus_pz=0;
  mus_status=0;
  mus_theta=0;
  mus_gen_id=0;
  mus_gen_phi=0;
  mus_gen_pt=0;
  mus_gen_pz=0;
  mus_gen_px=0;
  mus_gen_py=0;
  mus_gen_eta=0;
  mus_gen_theta=0;
  mus_gen_et=0;
  mus_gen_mother_id=0;
  mus_gen_mother_phi=0;
  mus_gen_mother_pt=0;
  mus_gen_mother_pz=0;
  mus_gen_mother_px=0;
  mus_gen_mother_py=0;
  mus_gen_mother_eta=0;
  mus_gen_mother_theta=0;
  mus_gen_mother_et=0;
  mus_tkHits=0;
  mus_cIso=0;
  mus_tIso=0;
  mus_ecalIso=0;
  mus_hcalIso=0;
  mus_ecalvetoDep=0;
  mus_hcalvetoDep=0;
  mus_calEnergyEm=0;
  mus_calEnergyHad=0;
  mus_calEnergyHo=0;
  mus_calEnergyEmS9=0;
  mus_calEnergyHadS9=0;
  mus_calEnergyHoS9=0;
  mus_iso03_emVetoEt=0;
  mus_iso03_hadVetoEt=0;
  mus_iso03_sumPt=0;
  mus_iso03_emEt=0;
  mus_iso03_hadEt=0;
  mus_iso03_hoEt=0;
  mus_iso03_nTracks=0;
  mus_iso05_sumPt=0;
  mus_iso05_emEt=0;
  mus_iso05_hadEt=0;
  mus_iso05_hoEt=0;
  mus_iso05_nTracks=0;
  mus_pfIsolationR03_sumChargedHadronPt=0;
  mus_pfIsolationR03_sumChargedParticlePt=0;
  mus_pfIsolationR03_sumNeutralHadronEt=0;
  mus_pfIsolationR03_sumNeutralHadronEtHighThreshold=0;
  mus_pfIsolationR03_sumPhotonEt=0;
  mus_pfIsolationR03_sumPhotonEtHighThreshold=0;
  mus_pfIsolationR03_sumPUPt=0;
  mus_pfIsolationR04_sumChargedHadronPt=0;
  mus_pfIsolationR04_sumChargedParticlePt=0;
  mus_pfIsolationR04_sumNeutralHadronEt=0;
  mus_pfIsolationR04_sumNeutralHadronEtHighThreshold=0;
  mus_pfIsolationR04_sumPhotonEt=0;
  mus_pfIsolationR04_sumPhotonEtHighThreshold=0;
  mus_pfIsolationR04_sumPUPt=0;
  mus_charge=0;
  mus_cm_chi2=0;
  mus_cm_ndof=0;
  mus_cm_chg=0;
  mus_cm_pt=0;
  mus_cm_px=0;
  mus_cm_py=0;
  mus_cm_pz=0;
  mus_cm_eta=0;
  mus_cm_phi=0;
  mus_cm_theta=0;
  mus_cm_d0dum=0;
  mus_cm_dz=0;
  mus_cm_vx=0;
  mus_cm_vy=0;
  mus_cm_vz=0;
  mus_cm_numvalhits=0;
  mus_cm_numlosthits=0;
  mus_cm_numvalMuonhits=0;
  mus_cm_d0dumErr=0;
  mus_cm_dzErr=0;
  mus_cm_ptErr=0;
  mus_cm_etaErr=0;
  mus_cm_phiErr=0;
  mus_tk_id=0;
  mus_tk_chi2=0;
  mus_tk_ndof=0;
  mus_tk_chg=0;
  mus_tk_pt=0;
  mus_tk_px=0;
  mus_tk_py=0;
  mus_tk_pz=0;
  mus_tk_eta=0;
  mus_tk_phi=0;
  mus_tk_theta=0;
  mus_tk_d0dum=0;
  mus_tk_dz=0;
  mus_tk_vx=0;
  mus_tk_vy=0;
  mus_tk_vz=0;
  mus_tk_numvalhits=0;
  mus_tk_numlosthits=0;
  mus_tk_d0dumErr=0;
  mus_tk_dzErr=0;
  mus_tk_ptErr=0;
  mus_tk_etaErr=0;
  mus_tk_phiErr=0;
  mus_tk_numvalPixelhits=0;
  mus_tk_numpixelWthMeasr=0;
  mus_stamu_chi2=0;
  mus_stamu_ndof=0;
  mus_stamu_chg=0;
  mus_stamu_pt=0;
  mus_stamu_px=0;
  mus_stamu_py=0;
  mus_stamu_pz=0;
  mus_stamu_eta=0;
  mus_stamu_phi=0;
  mus_stamu_theta=0;
  mus_stamu_d0dum=0;
  mus_stamu_dz=0;
  mus_stamu_vx=0;
  mus_stamu_vy=0;
  mus_stamu_vz=0;
  mus_stamu_numvalhits=0;
  mus_stamu_numlosthits=0;
  mus_stamu_d0dumErr=0;
  mus_stamu_dzErr=0;
  mus_stamu_ptErr=0;
  mus_stamu_etaErr=0;
  mus_stamu_phiErr=0;
  mus_num_matches=0;
  mus_isPFMuon=0;
  mus_isTrackerMuon=0;
  mus_isStandAloneMuon=0;
  mus_isCaloMuon=0;
  mus_isGlobalMuon=0;
  mus_isElectron=0;
  mus_isConvertedPhoton=0;
  mus_isPhoton=0;
  mus_id_All=0;
  mus_id_AllGlobalMuons=0;
  mus_id_AllStandAloneMuons=0;
  mus_id_AllTrackerMuons=0;
  mus_id_TrackerMuonArbitrated=0;
  mus_id_AllArbitrated=0;
  mus_id_GlobalMuonPromptTight=0;
  mus_id_TMLastStationLoose=0;
  mus_id_TMLastStationTight=0;
  mus_id_TM2DCompatibilityLoose=0;
  mus_id_TM2DCompatibilityTight=0;
  mus_id_TMOneStationLoose=0;
  mus_id_TMOneStationTight=0;
  mus_id_TMLastStationOptimizedLowPtLoose=0;
  mus_id_TMLastStationOptimizedLowPtTight=0;
  mus_tk_LayersWithMeasurement=0;
  mus_tk_PixelLayersWithMeasurement=0;
  mus_tk_ValidStripLayersWithMonoAndStereoHit=0;
  mus_tk_LayersWithoutMeasurement=0;
  mus_tk_ExpectedHitsInner=0;
  mus_tk_ExpectedHitsOuter=0;
  mus_cm_LayersWithMeasurement=0;
  mus_cm_PixelLayersWithMeasurement=0;
  mus_cm_ValidStripLayersWithMonoAndStereoHit=0;
  mus_cm_LayersWithoutMeasurement=0;
  mus_cm_ExpectedHitsInner=0;
  mus_cm_ExpectedHitsOuter=0;
  mus_picky_LayersWithMeasurement=0;
  mus_picky_PixelLayersWithMeasurement=0;
  mus_picky_ValidStripLayersWithMonoAndStereoHit=0;
  mus_picky_LayersWithoutMeasurement=0;
  mus_picky_ExpectedHitsInner=0;
  mus_picky_ExpectedHitsOuter=0;
  mus_tpfms_LayersWithMeasurement=0;
  mus_tpfms_PixelLayersWithMeasurement=0;
  mus_tpfms_ValidStripLayersWithMonoAndStereoHit=0;
  mus_tpfms_LayersWithoutMeasurement=0;
  mus_tpfms_ExpectedHitsInner=0;
  mus_tpfms_ExpectedHitsOuter=0;
  mus_picky_id=0;
  mus_picky_chi2=0;
  mus_picky_ndof=0;
  mus_picky_chg=0;
  mus_picky_pt=0;
  mus_picky_px=0;
  mus_picky_py=0;
  mus_picky_pz=0;
  mus_picky_eta=0;
  mus_picky_phi=0;
  mus_picky_theta=0;
  mus_picky_d0dum=0;
  mus_picky_dz=0;
  mus_picky_vx=0;
  mus_picky_vy=0;
  mus_picky_vz=0;
  mus_picky_numvalhits=0;
  mus_picky_numlosthits=0;
  mus_picky_d0dumErr=0;
  mus_picky_dzErr=0;
  mus_picky_ptErr=0;
  mus_picky_etaErr=0;
  mus_picky_phiErr=0;
  mus_picky_numvalPixelhits=0;
  mus_tpfms_id=0;
  mus_tpfms_chi2=0;
  mus_tpfms_ndof=0;
  mus_tpfms_chg=0;
  mus_tpfms_pt=0;
  mus_tpfms_px=0;
  mus_tpfms_py=0;
  mus_tpfms_pz=0;
  mus_tpfms_eta=0;
  mus_tpfms_phi=0;
  mus_tpfms_theta=0;
  mus_tpfms_d0dum=0;
  mus_tpfms_dz=0;
  mus_tpfms_vx=0;
  mus_tpfms_vy=0;
  mus_tpfms_vz=0;
  mus_tpfms_numvalhits=0;
  mus_tpfms_numlosthits=0;
  mus_tpfms_d0dumErr=0;
  mus_tpfms_dzErr=0;
  mus_tpfms_ptErr=0;
  mus_tpfms_etaErr=0;
  mus_tpfms_phiErr=0;
  mus_tpfms_numvalPixelhits=0;
  mus_dB=0;
  mus_numberOfMatchedStations=0;
  Npfcand=0;
  pfcand_pdgId=0;
  pfcand_pt=0;
  pfcand_pz=0;
  pfcand_px=0;
  pfcand_py=0;
  pfcand_eta=0;
  pfcand_phi=0;
  pfcand_theta=0;
  pfcand_energy=0;
  pfcand_charge=0;
  Nphotons=0;
  photons_energy=0;
  photons_et=0;
  photons_eta=0;
  photons_phi=0;
  photons_pt=0;
  photons_px=0;
  photons_py=0;
  photons_pz=0;
  photons_status=0;
  photons_theta=0;
  photons_hadOverEM=0;
  photons_hadTowOverEM=0;
  photons_scEnergy=0;
  photons_scRawEnergy=0;
  photons_scEta=0;
  photons_scPhi=0;
  photons_scEtaWidth=0;
  photons_scPhiWidth=0;
  photons_tIso=0;
  photons_ecalIso=0;
  photons_hcalIso=0;
  photons_isoEcalRecHitDR04=0;
  photons_isoHcalRecHitDR04=0;
  photons_isoSolidTrkConeDR04=0;
  photons_isoHollowTrkConeDR04=0;
  photons_nTrkSolidConeDR04=0;
  photons_nTrkHollowConeDR04=0;
  photons_isoEcalRecHitDR03=0;
  photons_isoHcalRecHitDR03=0;
  photons_isoSolidTrkConeDR03=0;
  photons_isoHollowTrkConeDR03=0;
  photons_nTrkSolidConeDR03=0;
  photons_nTrkHollowConeDR03=0;
  photons_isAlsoElectron=0;
  photons_hasPixelSeed=0;
  photons_isConverted=0;
  photons_isEBGap=0;
  photons_isEEGap=0;
  photons_isEBEEGap=0;
  photons_isEBPho=0;
  photons_isEEPho=0;
  photons_isLoosePhoton=0;
  photons_isTightPhoton=0;
  photons_maxEnergyXtal=0;
  photons_e1x5=0;
  photons_e2x5=0;
  photons_e3x3=0;
  photons_e5x5=0;
  photons_sigmaEtaEta=0;
  photons_sigmaIetaIeta=0;
  photons_r9=0;
  photons_gen_et=0;
  photons_gen_eta=0;
  photons_gen_phi=0;
  photons_gen_id=0;
  Npv=0;
  pv_x=0;
  pv_y=0;
  pv_z=0;
  pv_xErr=0;
  pv_yErr=0;
  pv_zErr=0;
  pv_chi2=0;
  pv_ndof=0;
  pv_isFake=0;
  pv_isValid=0;
  pv_tracksSize=0;
  Ntaus=0;
  taus_status=0;
  taus_phi=0;
  taus_pt=0;
  taus_pz=0;
  taus_px=0;
  taus_py=0;
  taus_eta=0;
  taus_theta=0;
  taus_et=0;
  taus_energy=0;
  taus_charge=0;
  taus_emf=0;
  taus_hcalTotOverPLead=0;
  taus_hcalMaxOverPLead=0;
  taus_hcal3x3OverPLead=0;
  taus_ecalStripSumEOverPLead=0;
  taus_elecPreIdOutput=0;
  taus_elecPreIdDecision=0;
  taus_leadPFChargedHadrCand_pt=0;
  taus_leadPFChargedHadrCand_charge=0;
  taus_leadPFChargedHadrCand_eta=0;
  taus_leadPFChargedHadrCand_ECAL_eta=0;
  taus_leadPFChargedHadrCand_phi=0;
  taus_isoPFGammaCandsEtSum=0;
  taus_isoPFChargedHadrCandsPtSum=0;
  taus_leadingTrackFinding=0;
  taus_leadingTrackPtCut=0;
  taus_trackIsolation=0;
  taus_ecalIsolation=0;
  taus_byIsolation=0;
  taus_againstElectron=0;
  taus_againstMuon=0;
  taus_taNC_quarter=0;
  taus_taNC_one=0;
  taus_taNC_half=0;
  taus_taNC_tenth=0;
  taus_taNC=0;
  taus_byIsoUsingLeadingPi=0;
  taus_tkIsoUsingLeadingPi=0;
  taus_ecalIsoUsingLeadingPi=0;
  taus_againstElectronLoose=0;
  taus_againstElectronMedium=0;
  taus_againstElectronTight=0;
  taus_againstElectronMVA=0;
  taus_againstMuonLoose=0;
  taus_againstMuonMedium=0;
  taus_againstMuonTight=0;
  taus_decayModeFinding=0;
  taus_byVLooseIsolation=0;
  taus_byLooseIsolation=0;
  taus_byMediumIsolation=0;
  taus_byTightIsolation=0;
  taus_byVLooseIsolationDeltaBetaCorr=0;
  taus_byLooseIsolationDeltaBetaCorr=0;
  taus_byMediumIsolationDeltaBetaCorr=0;
  taus_byTightIsolationDeltaBetaCorr=0;
  taus_signalPFChargedHadrCandsSize=0;
  taus_muDecision=0;
  taus_Nprongs=0;
  run=0;
  event=0;
  lumiblock=0;
  experimentType=0;
  bunchCrossing=0;
  orbitNumber=0;
  weight=0;
  model_params=0;
  chainB.SetBranchAddress("NbeamSpot", &NbeamSpot, &b_NbeamSpot);
  chainB.SetBranchAddress("beamSpot_x", &beamSpot_x, &b_beamSpot_x);
  chainB.SetBranchAddress("beamSpot_y", &beamSpot_y, &b_beamSpot_y);
  chainB.SetBranchAddress("beamSpot_z", &beamSpot_z, &b_beamSpot_z);
  chainB.SetBranchAddress("beamSpot_x0Error", &beamSpot_x0Error, &b_beamSpot_x0Error);
  chainB.SetBranchAddress("beamSpot_y0Error", &beamSpot_y0Error, &b_beamSpot_y0Error);
  chainB.SetBranchAddress("beamSpot_z0Error", &beamSpot_z0Error, &b_beamSpot_z0Error);
  chainB.SetBranchAddress("beamSpot_sigmaZ", &beamSpot_sigmaZ, &b_beamSpot_sigmaZ);
  chainB.SetBranchAddress("beamSpot_sigmaZ0Error", &beamSpot_sigmaZ0Error, &b_beamSpot_sigmaZ0Error);
  chainB.SetBranchAddress("beamSpot_dxdz", &beamSpot_dxdz, &b_beamSpot_dxdz);
  chainB.SetBranchAddress("beamSpot_dxdzError", &beamSpot_dxdzError, &b_beamSpot_dxdzError);
  chainB.SetBranchAddress("beamSpot_dydz", &beamSpot_dydz, &b_beamSpot_dydz);
  chainB.SetBranchAddress("beamSpot_dydzError", &beamSpot_dydzError, &b_beamSpot_dydzError);
  chainB.SetBranchAddress("beamSpot_beamWidthX", &beamSpot_beamWidthX, &b_beamSpot_beamWidthX);
  chainB.SetBranchAddress("beamSpot_beamWidthY", &beamSpot_beamWidthY, &b_beamSpot_beamWidthY);
  chainB.SetBranchAddress("beamSpot_beamWidthXError", &beamSpot_beamWidthXError, &b_beamSpot_beamWidthXError);
  chainB.SetBranchAddress("beamSpot_beamWidthYError", &beamSpot_beamWidthYError, &b_beamSpot_beamWidthYError);
  chainB.SetBranchAddress("Nels", &Nels, &b_Nels);
  chainB.SetBranchAddress("els_energy", &els_energy, &b_els_energy);
  chainB.SetBranchAddress("els_et", &els_et, &b_els_et);
  chainB.SetBranchAddress("els_eta", &els_eta, &b_els_eta);
  chainB.SetBranchAddress("els_phi", &els_phi, &b_els_phi);
  chainB.SetBranchAddress("els_pt", &els_pt, &b_els_pt);
  chainB.SetBranchAddress("els_px", &els_px, &b_els_px);
  chainB.SetBranchAddress("els_py", &els_py, &b_els_py);
  chainB.SetBranchAddress("els_pz", &els_pz, &b_els_pz);
  chainB.SetBranchAddress("els_status", &els_status, &b_els_status);
  chainB.SetBranchAddress("els_theta", &els_theta, &b_els_theta);
  chainB.SetBranchAddress("els_pfIsolationR03_sumChargedHadronPt", &els_pfIsolationR03_sumChargedHadronPt, &b_els_pfIsolationR03_sumChargedHadronPt);
  chainB.SetBranchAddress("els_pfIsolationR03_sumNeutralHadronEt", &els_pfIsolationR03_sumNeutralHadronEt, &b_els_pfIsolationR03_sumNeutralHadronEt);
  chainB.SetBranchAddress("els_pfIsolationR03_sumPhotonEt", &els_pfIsolationR03_sumPhotonEt, &b_els_pfIsolationR03_sumPhotonEt);
  chainB.SetBranchAddress("els_pfIsolationR03_sumPUPt", &els_pfIsolationR03_sumPUPt, &b_els_pfIsolationR03_sumPUPt);
  chainB.SetBranchAddress("els_full5x5_sigmaIetaIeta", &els_full5x5_sigmaIetaIeta, &b_els_full5x5_sigmaIetaIeta);
  chainB.SetBranchAddress("els_gen_id", &els_gen_id, &b_els_gen_id);
  chainB.SetBranchAddress("els_gen_phi", &els_gen_phi, &b_els_gen_phi);
  chainB.SetBranchAddress("els_gen_pt", &els_gen_pt, &b_els_gen_pt);
  chainB.SetBranchAddress("els_gen_pz", &els_gen_pz, &b_els_gen_pz);
  chainB.SetBranchAddress("els_gen_px", &els_gen_px, &b_els_gen_px);
  chainB.SetBranchAddress("els_gen_py", &els_gen_py, &b_els_gen_py);
  chainB.SetBranchAddress("els_gen_eta", &els_gen_eta, &b_els_gen_eta);
  chainB.SetBranchAddress("els_gen_theta", &els_gen_theta, &b_els_gen_theta);
  chainB.SetBranchAddress("els_gen_et", &els_gen_et, &b_els_gen_et);
  chainB.SetBranchAddress("els_gen_mother_id", &els_gen_mother_id, &b_els_gen_mother_id);
  chainB.SetBranchAddress("els_gen_mother_phi", &els_gen_mother_phi, &b_els_gen_mother_phi);
  chainB.SetBranchAddress("els_gen_mother_pt", &els_gen_mother_pt, &b_els_gen_mother_pt);
  chainB.SetBranchAddress("els_gen_mother_pz", &els_gen_mother_pz, &b_els_gen_mother_pz);
  chainB.SetBranchAddress("els_gen_mother_px", &els_gen_mother_px, &b_els_gen_mother_px);
  chainB.SetBranchAddress("els_gen_mother_py", &els_gen_mother_py, &b_els_gen_mother_py);
  chainB.SetBranchAddress("els_gen_mother_eta", &els_gen_mother_eta, &b_els_gen_mother_eta);
  chainB.SetBranchAddress("els_gen_mother_theta", &els_gen_mother_theta, &b_els_gen_mother_theta);
  chainB.SetBranchAddress("els_gen_mother_et", &els_gen_mother_et, &b_els_gen_mother_et);
  chainB.SetBranchAddress("els_tightId", &els_tightId, &b_els_tightId);
  chainB.SetBranchAddress("els_looseId", &els_looseId, &b_els_looseId);
  chainB.SetBranchAddress("els_robustTightId", &els_robustTightId, &b_els_robustTightId);
  chainB.SetBranchAddress("els_robustLooseId", &els_robustLooseId, &b_els_robustLooseId);
  chainB.SetBranchAddress("els_robustHighEnergyId", &els_robustHighEnergyId, &b_els_robustHighEnergyId);
  chainB.SetBranchAddress("els_simpleEleId95relIso", &els_simpleEleId95relIso, &b_els_simpleEleId95relIso);
  chainB.SetBranchAddress("els_simpleEleId90relIso", &els_simpleEleId90relIso, &b_els_simpleEleId90relIso);
  chainB.SetBranchAddress("els_simpleEleId85relIso", &els_simpleEleId85relIso, &b_els_simpleEleId85relIso);
  chainB.SetBranchAddress("els_simpleEleId80relIso", &els_simpleEleId80relIso, &b_els_simpleEleId80relIso);
  chainB.SetBranchAddress("els_simpleEleId70relIso", &els_simpleEleId70relIso, &b_els_simpleEleId70relIso);
  chainB.SetBranchAddress("els_simpleEleId60relIso", &els_simpleEleId60relIso, &b_els_simpleEleId60relIso);
  chainB.SetBranchAddress("els_simpleEleId95cIso", &els_simpleEleId95cIso, &b_els_simpleEleId95cIso);
  chainB.SetBranchAddress("els_simpleEleId90cIso", &els_simpleEleId90cIso, &b_els_simpleEleId90cIso);
  chainB.SetBranchAddress("els_simpleEleId85cIso", &els_simpleEleId85cIso, &b_els_simpleEleId85cIso);
  chainB.SetBranchAddress("els_simpleEleId80cIso", &els_simpleEleId80cIso, &b_els_simpleEleId80cIso);
  chainB.SetBranchAddress("els_simpleEleId70cIso", &els_simpleEleId70cIso, &b_els_simpleEleId70cIso);
  chainB.SetBranchAddress("els_simpleEleId60cIso", &els_simpleEleId60cIso, &b_els_simpleEleId60cIso);
  chainB.SetBranchAddress("els_cIso", &els_cIso, &b_els_cIso);
  chainB.SetBranchAddress("els_tIso", &els_tIso, &b_els_tIso);
  chainB.SetBranchAddress("els_ecalIso", &els_ecalIso, &b_els_ecalIso);
  chainB.SetBranchAddress("els_hcalIso", &els_hcalIso, &b_els_hcalIso);
  chainB.SetBranchAddress("els_chi2", &els_chi2, &b_els_chi2);
  chainB.SetBranchAddress("els_charge", &els_charge, &b_els_charge);
  chainB.SetBranchAddress("els_caloEnergy", &els_caloEnergy, &b_els_caloEnergy);
  chainB.SetBranchAddress("els_hadOverEm", &els_hadOverEm, &b_els_hadOverEm);
  chainB.SetBranchAddress("els_hcalOverEcalBc", &els_hcalOverEcalBc, &b_els_hcalOverEcalBc);
  chainB.SetBranchAddress("els_eOverPIn", &els_eOverPIn, &b_els_eOverPIn);
  chainB.SetBranchAddress("els_eSeedOverPOut", &els_eSeedOverPOut, &b_els_eSeedOverPOut);
  chainB.SetBranchAddress("els_sigmaEtaEta", &els_sigmaEtaEta, &b_els_sigmaEtaEta);
  chainB.SetBranchAddress("els_sigmaIEtaIEta", &els_sigmaIEtaIEta, &b_els_sigmaIEtaIEta);
  chainB.SetBranchAddress("els_scEnergy", &els_scEnergy, &b_els_scEnergy);
  chainB.SetBranchAddress("els_scRawEnergy", &els_scRawEnergy, &b_els_scRawEnergy);
  chainB.SetBranchAddress("els_scSeedEnergy", &els_scSeedEnergy, &b_els_scSeedEnergy);
  chainB.SetBranchAddress("els_scEta", &els_scEta, &b_els_scEta);
  chainB.SetBranchAddress("els_scPhi", &els_scPhi, &b_els_scPhi);
  chainB.SetBranchAddress("els_scEtaWidth", &els_scEtaWidth, &b_els_scEtaWidth);
  chainB.SetBranchAddress("els_scPhiWidth", &els_scPhiWidth, &b_els_scPhiWidth);
  chainB.SetBranchAddress("els_scE1x5", &els_scE1x5, &b_els_scE1x5);
  chainB.SetBranchAddress("els_scE2x5Max", &els_scE2x5Max, &b_els_scE2x5Max);
  chainB.SetBranchAddress("els_scE5x5", &els_scE5x5, &b_els_scE5x5);
  chainB.SetBranchAddress("els_isEB", &els_isEB, &b_els_isEB);
  chainB.SetBranchAddress("els_isEE", &els_isEE, &b_els_isEE);
  chainB.SetBranchAddress("els_dEtaIn", &els_dEtaIn, &b_els_dEtaIn);
  chainB.SetBranchAddress("els_dPhiIn", &els_dPhiIn, &b_els_dPhiIn);
  chainB.SetBranchAddress("els_dEtaOut", &els_dEtaOut, &b_els_dEtaOut);
  chainB.SetBranchAddress("els_dPhiOut", &els_dPhiOut, &b_els_dPhiOut);
  chainB.SetBranchAddress("els_numvalhits", &els_numvalhits, &b_els_numvalhits);
  chainB.SetBranchAddress("els_numlosthits", &els_numlosthits, &b_els_numlosthits);
  chainB.SetBranchAddress("els_basicClustersSize", &els_basicClustersSize, &b_els_basicClustersSize);
  chainB.SetBranchAddress("els_tk_pz", &els_tk_pz, &b_els_tk_pz);
  chainB.SetBranchAddress("els_tk_pt", &els_tk_pt, &b_els_tk_pt);
  chainB.SetBranchAddress("els_tk_phi", &els_tk_phi, &b_els_tk_phi);
  chainB.SetBranchAddress("els_tk_eta", &els_tk_eta, &b_els_tk_eta);
  chainB.SetBranchAddress("els_d0dum", &els_d0dum, &b_els_d0dum);
  chainB.SetBranchAddress("els_dz", &els_dz, &b_els_dz);
  chainB.SetBranchAddress("els_vx", &els_vx, &b_els_vx);
  chainB.SetBranchAddress("els_vy", &els_vy, &b_els_vy);
  chainB.SetBranchAddress("els_vz", &els_vz, &b_els_vz);
  chainB.SetBranchAddress("els_ndof", &els_ndof, &b_els_ndof);
  chainB.SetBranchAddress("els_ptError", &els_ptError, &b_els_ptError);
  chainB.SetBranchAddress("els_d0dumError", &els_d0dumError, &b_els_d0dumError);
  chainB.SetBranchAddress("els_dzError", &els_dzError, &b_els_dzError);
  chainB.SetBranchAddress("els_etaError", &els_etaError, &b_els_etaError);
  chainB.SetBranchAddress("els_phiError", &els_phiError, &b_els_phiError);
  chainB.SetBranchAddress("els_tk_charge", &els_tk_charge, &b_els_tk_charge);
  chainB.SetBranchAddress("els_core_ecalDrivenSeed", &els_core_ecalDrivenSeed, &b_els_core_ecalDrivenSeed);
  chainB.SetBranchAddress("els_n_inner_layer", &els_n_inner_layer, &b_els_n_inner_layer);
  chainB.SetBranchAddress("els_n_outer_layer", &els_n_outer_layer, &b_els_n_outer_layer);
  chainB.SetBranchAddress("els_ctf_tk_id", &els_ctf_tk_id, &b_els_ctf_tk_id);
  chainB.SetBranchAddress("els_ctf_tk_charge", &els_ctf_tk_charge, &b_els_ctf_tk_charge);
  chainB.SetBranchAddress("els_ctf_tk_eta", &els_ctf_tk_eta, &b_els_ctf_tk_eta);
  chainB.SetBranchAddress("els_ctf_tk_phi", &els_ctf_tk_phi, &b_els_ctf_tk_phi);
  chainB.SetBranchAddress("els_fbrem", &els_fbrem, &b_els_fbrem);
  chainB.SetBranchAddress("els_shFracInnerHits", &els_shFracInnerHits, &b_els_shFracInnerHits);
  chainB.SetBranchAddress("els_dr03EcalRecHitSumEt", &els_dr03EcalRecHitSumEt, &b_els_dr03EcalRecHitSumEt);
  chainB.SetBranchAddress("els_dr03HcalTowerSumEt", &els_dr03HcalTowerSumEt, &b_els_dr03HcalTowerSumEt);
  chainB.SetBranchAddress("els_dr03HcalDepth1TowerSumEt", &els_dr03HcalDepth1TowerSumEt, &b_els_dr03HcalDepth1TowerSumEt);
  chainB.SetBranchAddress("els_dr03HcalDepth2TowerSumEt", &els_dr03HcalDepth2TowerSumEt, &b_els_dr03HcalDepth2TowerSumEt);
  chainB.SetBranchAddress("els_dr03TkSumPt", &els_dr03TkSumPt, &b_els_dr03TkSumPt);
  chainB.SetBranchAddress("els_dr04EcalRecHitSumEt", &els_dr04EcalRecHitSumEt, &b_els_dr04EcalRecHitSumEt);
  chainB.SetBranchAddress("els_dr04HcalTowerSumEt", &els_dr04HcalTowerSumEt, &b_els_dr04HcalTowerSumEt);
  chainB.SetBranchAddress("els_dr04HcalDepth1TowerSumEt", &els_dr04HcalDepth1TowerSumEt, &b_els_dr04HcalDepth1TowerSumEt);
  chainB.SetBranchAddress("els_dr04HcalDepth2TowerSumEt", &els_dr04HcalDepth2TowerSumEt, &b_els_dr04HcalDepth2TowerSumEt);
  chainB.SetBranchAddress("els_dr04TkSumPt", &els_dr04TkSumPt, &b_els_dr04TkSumPt);
  chainB.SetBranchAddress("els_cpx", &els_cpx, &b_els_cpx);
  chainB.SetBranchAddress("els_cpy", &els_cpy, &b_els_cpy);
  chainB.SetBranchAddress("els_cpz", &els_cpz, &b_els_cpz);
  chainB.SetBranchAddress("els_vpx", &els_vpx, &b_els_vpx);
  chainB.SetBranchAddress("els_vpy", &els_vpy, &b_els_vpy);
  chainB.SetBranchAddress("els_vpz", &els_vpz, &b_els_vpz);
  chainB.SetBranchAddress("els_cx", &els_cx, &b_els_cx);
  chainB.SetBranchAddress("els_cy", &els_cy, &b_els_cy);
  chainB.SetBranchAddress("els_cz", &els_cz, &b_els_cz);
  chainB.SetBranchAddress("els_PATpassConversionVeto", &els_PATpassConversionVeto, &b_els_PATpassConversionVeto);
  chainB.SetBranchAddress("Njets_AK4", &Njets_AK4, &b_Njets_AK4);
  chainB.SetBranchAddress("jets_AK4_status", &jets_AK4_status, &b_jets_AK4_status);
  chainB.SetBranchAddress("jets_AK4_phi", &jets_AK4_phi, &b_jets_AK4_phi);
  chainB.SetBranchAddress("jets_AK4_pt", &jets_AK4_pt, &b_jets_AK4_pt);
  chainB.SetBranchAddress("jets_AK4_pz", &jets_AK4_pz, &b_jets_AK4_pz);
  chainB.SetBranchAddress("jets_AK4_px", &jets_AK4_px, &b_jets_AK4_px);
  chainB.SetBranchAddress("jets_AK4_py", &jets_AK4_py, &b_jets_AK4_py);
  chainB.SetBranchAddress("jets_AK4_eta", &jets_AK4_eta, &b_jets_AK4_eta);
  chainB.SetBranchAddress("jets_AK4_theta", &jets_AK4_theta, &b_jets_AK4_theta);
  chainB.SetBranchAddress("jets_AK4_et", &jets_AK4_et, &b_jets_AK4_et);
  chainB.SetBranchAddress("jets_AK4_energy", &jets_AK4_energy, &b_jets_AK4_energy);
  chainB.SetBranchAddress("jets_AK4_parton_Id", &jets_AK4_parton_Id, &b_jets_AK4_parton_Id);
  chainB.SetBranchAddress("jets_AK4_parton_motherId", &jets_AK4_parton_motherId, &b_jets_AK4_parton_motherId);
  chainB.SetBranchAddress("jets_AK4_parton_pt", &jets_AK4_parton_pt, &b_jets_AK4_parton_pt);
  chainB.SetBranchAddress("jets_AK4_parton_phi", &jets_AK4_parton_phi, &b_jets_AK4_parton_phi);
  chainB.SetBranchAddress("jets_AK4_parton_eta", &jets_AK4_parton_eta, &b_jets_AK4_parton_eta);
  chainB.SetBranchAddress("jets_AK4_parton_Energy", &jets_AK4_parton_Energy, &b_jets_AK4_parton_Energy);
  chainB.SetBranchAddress("jets_AK4_parton_mass", &jets_AK4_parton_mass, &b_jets_AK4_parton_mass);
  chainB.SetBranchAddress("jets_AK4_gen_et", &jets_AK4_gen_et, &b_jets_AK4_gen_et);
  chainB.SetBranchAddress("jets_AK4_gen_pt", &jets_AK4_gen_pt, &b_jets_AK4_gen_pt);
  chainB.SetBranchAddress("jets_AK4_gen_eta", &jets_AK4_gen_eta, &b_jets_AK4_gen_eta);
  chainB.SetBranchAddress("jets_AK4_gen_phi", &jets_AK4_gen_phi, &b_jets_AK4_gen_phi);
  chainB.SetBranchAddress("jets_AK4_gen_mass", &jets_AK4_gen_mass, &b_jets_AK4_gen_mass);
  chainB.SetBranchAddress("jets_AK4_gen_Energy", &jets_AK4_gen_Energy, &b_jets_AK4_gen_Energy);
  chainB.SetBranchAddress("jets_AK4_gen_Id", &jets_AK4_gen_Id, &b_jets_AK4_gen_Id);
  chainB.SetBranchAddress("jets_AK4_gen_motherID", &jets_AK4_gen_motherID, &b_jets_AK4_gen_motherID);
  chainB.SetBranchAddress("jets_AK4_gen_threeCharge", &jets_AK4_gen_threeCharge, &b_jets_AK4_gen_threeCharge);
  chainB.SetBranchAddress("jets_AK4_partonFlavour", &jets_AK4_partonFlavour, &b_jets_AK4_partonFlavour);
  chainB.SetBranchAddress("jets_AK4_btag_TC_highPur", &jets_AK4_btag_TC_highPur, &b_jets_AK4_btag_TC_highPur);
  chainB.SetBranchAddress("jets_AK4_btag_TC_highEff", &jets_AK4_btag_TC_highEff, &b_jets_AK4_btag_TC_highEff);
  chainB.SetBranchAddress("jets_AK4_btag_jetProb", &jets_AK4_btag_jetProb, &b_jets_AK4_btag_jetProb);
  chainB.SetBranchAddress("jets_AK4_btag_jetBProb", &jets_AK4_btag_jetBProb, &b_jets_AK4_btag_jetBProb);
  chainB.SetBranchAddress("jets_AK4_btag_softEle", &jets_AK4_btag_softEle, &b_jets_AK4_btag_softEle);
  chainB.SetBranchAddress("jets_AK4_btag_softMuon", &jets_AK4_btag_softMuon, &b_jets_AK4_btag_softMuon);
  chainB.SetBranchAddress("jets_AK4_btag_secVertexHighPur", &jets_AK4_btag_secVertexHighPur, &b_jets_AK4_btag_secVertexHighPur);
  chainB.SetBranchAddress("jets_AK4_btag_secVertexHighEff", &jets_AK4_btag_secVertexHighEff, &b_jets_AK4_btag_secVertexHighEff);
  chainB.SetBranchAddress("jets_AK4_btag_secVertexCombined", &jets_AK4_btag_secVertexCombined, &b_jets_AK4_btag_secVertexCombined);
  chainB.SetBranchAddress("jets_AK4_jetCharge", &jets_AK4_jetCharge, &b_jets_AK4_jetCharge);
  chainB.SetBranchAddress("jets_AK4_chgEmE", &jets_AK4_chgEmE, &b_jets_AK4_chgEmE);
  chainB.SetBranchAddress("jets_AK4_chgHadE", &jets_AK4_chgHadE, &b_jets_AK4_chgHadE);
  chainB.SetBranchAddress("jets_AK4_photonEnergy", &jets_AK4_photonEnergy, &b_jets_AK4_photonEnergy);
  chainB.SetBranchAddress("jets_AK4_chgMuE", &jets_AK4_chgMuE, &b_jets_AK4_chgMuE);
  chainB.SetBranchAddress("jets_AK4_chg_Mult", &jets_AK4_chg_Mult, &b_jets_AK4_chg_Mult);
  chainB.SetBranchAddress("jets_AK4_neutralEmE", &jets_AK4_neutralEmE, &b_jets_AK4_neutralEmE);
  chainB.SetBranchAddress("jets_AK4_neutralHadE", &jets_AK4_neutralHadE, &b_jets_AK4_neutralHadE);
  chainB.SetBranchAddress("jets_AK4_neutral_Mult", &jets_AK4_neutral_Mult, &b_jets_AK4_neutral_Mult);
  chainB.SetBranchAddress("jets_AK4_mu_Mult", &jets_AK4_mu_Mult, &b_jets_AK4_mu_Mult);
  chainB.SetBranchAddress("jets_AK4_emf", &jets_AK4_emf, &b_jets_AK4_emf);
  chainB.SetBranchAddress("jets_AK4_ehf", &jets_AK4_ehf, &b_jets_AK4_ehf);
  chainB.SetBranchAddress("jets_AK4_n60", &jets_AK4_n60, &b_jets_AK4_n60);
  chainB.SetBranchAddress("jets_AK4_n90", &jets_AK4_n90, &b_jets_AK4_n90);
  chainB.SetBranchAddress("jets_AK4_etaetaMoment", &jets_AK4_etaetaMoment, &b_jets_AK4_etaetaMoment);
  chainB.SetBranchAddress("jets_AK4_etaphiMoment", &jets_AK4_etaphiMoment, &b_jets_AK4_etaphiMoment);
  chainB.SetBranchAddress("jets_AK4_phiphiMoment", &jets_AK4_phiphiMoment, &b_jets_AK4_phiphiMoment);
  chainB.SetBranchAddress("jets_AK4_n90Hits", &jets_AK4_n90Hits, &b_jets_AK4_n90Hits);
  chainB.SetBranchAddress("jets_AK4_fHPD", &jets_AK4_fHPD, &b_jets_AK4_fHPD);
  chainB.SetBranchAddress("jets_AK4_fRBX", &jets_AK4_fRBX, &b_jets_AK4_fRBX);
  chainB.SetBranchAddress("jets_AK4_hitsInN90", &jets_AK4_hitsInN90, &b_jets_AK4_hitsInN90);
  chainB.SetBranchAddress("jets_AK4_nECALTowers", &jets_AK4_nECALTowers, &b_jets_AK4_nECALTowers);
  chainB.SetBranchAddress("jets_AK4_nHCALTowers", &jets_AK4_nHCALTowers, &b_jets_AK4_nHCALTowers);
  chainB.SetBranchAddress("jets_AK4_fSubDetector1", &jets_AK4_fSubDetector1, &b_jets_AK4_fSubDetector1);
  chainB.SetBranchAddress("jets_AK4_fSubDetector2", &jets_AK4_fSubDetector2, &b_jets_AK4_fSubDetector2);
  chainB.SetBranchAddress("jets_AK4_fSubDetector3", &jets_AK4_fSubDetector3, &b_jets_AK4_fSubDetector3);
  chainB.SetBranchAddress("jets_AK4_fSubDetector4", &jets_AK4_fSubDetector4, &b_jets_AK4_fSubDetector4);
  chainB.SetBranchAddress("jets_AK4_area", &jets_AK4_area, &b_jets_AK4_area);
  chainB.SetBranchAddress("jets_AK4_corrFactorRaw", &jets_AK4_corrFactorRaw, &b_jets_AK4_corrFactorRaw);
  chainB.SetBranchAddress("jets_AK4_rawPt", &jets_AK4_rawPt, &b_jets_AK4_rawPt);
  chainB.SetBranchAddress("jets_AK4_mass", &jets_AK4_mass, &b_jets_AK4_mass);
  chainB.SetBranchAddress("Nmc_doc", &Nmc_doc, &b_Nmc_doc);
  chainB.SetBranchAddress("mc_doc_id", &mc_doc_id, &b_mc_doc_id);
  chainB.SetBranchAddress("mc_doc_pt", &mc_doc_pt, &b_mc_doc_pt);
  chainB.SetBranchAddress("mc_doc_px", &mc_doc_px, &b_mc_doc_px);
  chainB.SetBranchAddress("mc_doc_py", &mc_doc_py, &b_mc_doc_py);
  chainB.SetBranchAddress("mc_doc_pz", &mc_doc_pz, &b_mc_doc_pz);
  chainB.SetBranchAddress("mc_doc_eta", &mc_doc_eta, &b_mc_doc_eta);
  chainB.SetBranchAddress("mc_doc_phi", &mc_doc_phi, &b_mc_doc_phi);
  chainB.SetBranchAddress("mc_doc_theta", &mc_doc_theta, &b_mc_doc_theta);
  chainB.SetBranchAddress("mc_doc_energy", &mc_doc_energy, &b_mc_doc_energy);
  chainB.SetBranchAddress("mc_doc_status", &mc_doc_status, &b_mc_doc_status);
  chainB.SetBranchAddress("mc_doc_charge", &mc_doc_charge, &b_mc_doc_charge);
  chainB.SetBranchAddress("mc_doc_mother_id", &mc_doc_mother_id, &b_mc_doc_mother_id);
  chainB.SetBranchAddress("mc_doc_grandmother_id", &mc_doc_grandmother_id, &b_mc_doc_grandmother_id);
  chainB.SetBranchAddress("mc_doc_ggrandmother_id", &mc_doc_ggrandmother_id, &b_mc_doc_ggrandmother_id);
  chainB.SetBranchAddress("mc_doc_mother_pt", &mc_doc_mother_pt, &b_mc_doc_mother_pt);
  chainB.SetBranchAddress("mc_doc_vertex_x", &mc_doc_vertex_x, &b_mc_doc_vertex_x);
  chainB.SetBranchAddress("mc_doc_vertex_y", &mc_doc_vertex_y, &b_mc_doc_vertex_y);
  chainB.SetBranchAddress("mc_doc_vertex_z", &mc_doc_vertex_z, &b_mc_doc_vertex_z);
  chainB.SetBranchAddress("mc_doc_mass", &mc_doc_mass, &b_mc_doc_mass);
  chainB.SetBranchAddress("mc_doc_numOfDaughters", &mc_doc_numOfDaughters, &b_mc_doc_numOfDaughters);
  chainB.SetBranchAddress("mc_doc_numOfMothers", &mc_doc_numOfMothers, &b_mc_doc_numOfMothers);
  chainB.SetBranchAddress("Nmc_electrons", &Nmc_electrons, &b_Nmc_electrons);
  chainB.SetBranchAddress("mc_electrons_id", &mc_electrons_id, &b_mc_electrons_id);
  chainB.SetBranchAddress("mc_electrons_pt", &mc_electrons_pt, &b_mc_electrons_pt);
  chainB.SetBranchAddress("mc_electrons_px", &mc_electrons_px, &b_mc_electrons_px);
  chainB.SetBranchAddress("mc_electrons_py", &mc_electrons_py, &b_mc_electrons_py);
  chainB.SetBranchAddress("mc_electrons_pz", &mc_electrons_pz, &b_mc_electrons_pz);
  chainB.SetBranchAddress("mc_electrons_eta", &mc_electrons_eta, &b_mc_electrons_eta);
  chainB.SetBranchAddress("mc_electrons_phi", &mc_electrons_phi, &b_mc_electrons_phi);
  chainB.SetBranchAddress("mc_electrons_theta", &mc_electrons_theta, &b_mc_electrons_theta);
  chainB.SetBranchAddress("mc_electrons_status", &mc_electrons_status, &b_mc_electrons_status);
  chainB.SetBranchAddress("mc_electrons_energy", &mc_electrons_energy, &b_mc_electrons_energy);
  chainB.SetBranchAddress("mc_electrons_charge", &mc_electrons_charge, &b_mc_electrons_charge);
  chainB.SetBranchAddress("mc_electrons_mother_id", &mc_electrons_mother_id, &b_mc_electrons_mother_id);
  chainB.SetBranchAddress("mc_electrons_mother_pt", &mc_electrons_mother_pt, &b_mc_electrons_mother_pt);
  chainB.SetBranchAddress("mc_electrons_grandmother_id", &mc_electrons_grandmother_id, &b_mc_electrons_grandmother_id);
  chainB.SetBranchAddress("mc_electrons_ggrandmother_id", &mc_electrons_ggrandmother_id, &b_mc_electrons_ggrandmother_id);
  chainB.SetBranchAddress("mc_electrons_vertex_x", &mc_electrons_vertex_x, &b_mc_electrons_vertex_x);
  chainB.SetBranchAddress("mc_electrons_vertex_y", &mc_electrons_vertex_y, &b_mc_electrons_vertex_y);
  chainB.SetBranchAddress("mc_electrons_vertex_z", &mc_electrons_vertex_z, &b_mc_electrons_vertex_z);
  chainB.SetBranchAddress("mc_electrons_mass", &mc_electrons_mass, &b_mc_electrons_mass);
  chainB.SetBranchAddress("mc_electrons_numOfDaughters", &mc_electrons_numOfDaughters, &b_mc_electrons_numOfDaughters);
  chainB.SetBranchAddress("Nmc_final", &Nmc_final, &b_Nmc_final);
  chainB.SetBranchAddress("mc_final_id", &mc_final_id, &b_mc_final_id);
  chainB.SetBranchAddress("mc_final_pt", &mc_final_pt, &b_mc_final_pt);
  chainB.SetBranchAddress("mc_final_px", &mc_final_px, &b_mc_final_px);
  chainB.SetBranchAddress("mc_final_py", &mc_final_py, &b_mc_final_py);
  chainB.SetBranchAddress("mc_final_pz", &mc_final_pz, &b_mc_final_pz);
  chainB.SetBranchAddress("mc_final_eta", &mc_final_eta, &b_mc_final_eta);
  chainB.SetBranchAddress("mc_final_phi", &mc_final_phi, &b_mc_final_phi);
  chainB.SetBranchAddress("mc_final_theta", &mc_final_theta, &b_mc_final_theta);
  chainB.SetBranchAddress("mc_final_energy", &mc_final_energy, &b_mc_final_energy);
  chainB.SetBranchAddress("mc_final_status", &mc_final_status, &b_mc_final_status);
  chainB.SetBranchAddress("mc_final_charge", &mc_final_charge, &b_mc_final_charge);
  chainB.SetBranchAddress("mc_final_mother_id", &mc_final_mother_id, &b_mc_final_mother_id);
  chainB.SetBranchAddress("mc_final_grandmother_id", &mc_final_grandmother_id, &b_mc_final_grandmother_id);
  chainB.SetBranchAddress("mc_final_ggrandmother_id", &mc_final_ggrandmother_id, &b_mc_final_ggrandmother_id);
  chainB.SetBranchAddress("mc_final_mother_pt", &mc_final_mother_pt, &b_mc_final_mother_pt);
  chainB.SetBranchAddress("mc_final_vertex_x", &mc_final_vertex_x, &b_mc_final_vertex_x);
  chainB.SetBranchAddress("mc_final_vertex_y", &mc_final_vertex_y, &b_mc_final_vertex_y);
  chainB.SetBranchAddress("mc_final_vertex_z", &mc_final_vertex_z, &b_mc_final_vertex_z);
  chainB.SetBranchAddress("mc_final_mass", &mc_final_mass, &b_mc_final_mass);
  chainB.SetBranchAddress("mc_final_numOfDaughters", &mc_final_numOfDaughters, &b_mc_final_numOfDaughters);
  chainB.SetBranchAddress("mc_final_numOfMothers", &mc_final_numOfMothers, &b_mc_final_numOfMothers);
  chainB.SetBranchAddress("Nmc_jets", &Nmc_jets, &b_Nmc_jets);
  chainB.SetBranchAddress("mc_jets_phi", &mc_jets_phi, &b_mc_jets_phi);
  chainB.SetBranchAddress("mc_jets_pt", &mc_jets_pt, &b_mc_jets_pt);
  chainB.SetBranchAddress("mc_jets_pz", &mc_jets_pz, &b_mc_jets_pz);
  chainB.SetBranchAddress("mc_jets_px", &mc_jets_px, &b_mc_jets_px);
  chainB.SetBranchAddress("mc_jets_py", &mc_jets_py, &b_mc_jets_py);
  chainB.SetBranchAddress("mc_jets_eta", &mc_jets_eta, &b_mc_jets_eta);
  chainB.SetBranchAddress("mc_jets_theta", &mc_jets_theta, &b_mc_jets_theta);
  chainB.SetBranchAddress("mc_jets_et", &mc_jets_et, &b_mc_jets_et);
  chainB.SetBranchAddress("mc_jets_energy", &mc_jets_energy, &b_mc_jets_energy);
  chainB.SetBranchAddress("mc_jets_emEnergy", &mc_jets_emEnergy, &b_mc_jets_emEnergy);
  chainB.SetBranchAddress("mc_jets_hadEnergy", &mc_jets_hadEnergy, &b_mc_jets_hadEnergy);
  chainB.SetBranchAddress("mc_jets_invisibleEnergy", &mc_jets_invisibleEnergy, &b_mc_jets_invisibleEnergy);
  chainB.SetBranchAddress("mc_jets_auxiliaryEnergy", &mc_jets_auxiliaryEnergy, &b_mc_jets_auxiliaryEnergy);
  chainB.SetBranchAddress("mc_jets_etaetaMoment", &mc_jets_etaetaMoment, &b_mc_jets_etaetaMoment);
  chainB.SetBranchAddress("mc_jets_etaphiMoment", &mc_jets_etaphiMoment, &b_mc_jets_etaphiMoment);
  chainB.SetBranchAddress("mc_jets_phiphiMoment", &mc_jets_phiphiMoment, &b_mc_jets_phiphiMoment);
  chainB.SetBranchAddress("mc_jets_mass", &mc_jets_mass, &b_mc_jets_mass);
  chainB.SetBranchAddress("Nmc_mus", &Nmc_mus, &b_Nmc_mus);
  chainB.SetBranchAddress("mc_mus_id", &mc_mus_id, &b_mc_mus_id);
  chainB.SetBranchAddress("mc_mus_pt", &mc_mus_pt, &b_mc_mus_pt);
  chainB.SetBranchAddress("mc_mus_px", &mc_mus_px, &b_mc_mus_px);
  chainB.SetBranchAddress("mc_mus_py", &mc_mus_py, &b_mc_mus_py);
  chainB.SetBranchAddress("mc_mus_pz", &mc_mus_pz, &b_mc_mus_pz);
  chainB.SetBranchAddress("mc_mus_eta", &mc_mus_eta, &b_mc_mus_eta);
  chainB.SetBranchAddress("mc_mus_phi", &mc_mus_phi, &b_mc_mus_phi);
  chainB.SetBranchAddress("mc_mus_theta", &mc_mus_theta, &b_mc_mus_theta);
  chainB.SetBranchAddress("mc_mus_status", &mc_mus_status, &b_mc_mus_status);
  chainB.SetBranchAddress("mc_mus_energy", &mc_mus_energy, &b_mc_mus_energy);
  chainB.SetBranchAddress("mc_mus_charge", &mc_mus_charge, &b_mc_mus_charge);
  chainB.SetBranchAddress("mc_mus_mother_id", &mc_mus_mother_id, &b_mc_mus_mother_id);
  chainB.SetBranchAddress("mc_mus_mother_pt", &mc_mus_mother_pt, &b_mc_mus_mother_pt);
  chainB.SetBranchAddress("mc_mus_grandmother_id", &mc_mus_grandmother_id, &b_mc_mus_grandmother_id);
  chainB.SetBranchAddress("mc_mus_ggrandmother_id", &mc_mus_ggrandmother_id, &b_mc_mus_ggrandmother_id);
  chainB.SetBranchAddress("mc_mus_vertex_x", &mc_mus_vertex_x, &b_mc_mus_vertex_x);
  chainB.SetBranchAddress("mc_mus_vertex_y", &mc_mus_vertex_y, &b_mc_mus_vertex_y);
  chainB.SetBranchAddress("mc_mus_vertex_z", &mc_mus_vertex_z, &b_mc_mus_vertex_z);
  chainB.SetBranchAddress("mc_mus_mass", &mc_mus_mass, &b_mc_mus_mass);
  chainB.SetBranchAddress("mc_mus_numOfDaughters", &mc_mus_numOfDaughters, &b_mc_mus_numOfDaughters);
  chainB.SetBranchAddress("Nmc_nues", &Nmc_nues, &b_Nmc_nues);
  chainB.SetBranchAddress("mc_nues_id", &mc_nues_id, &b_mc_nues_id);
  chainB.SetBranchAddress("mc_nues_pt", &mc_nues_pt, &b_mc_nues_pt);
  chainB.SetBranchAddress("mc_nues_px", &mc_nues_px, &b_mc_nues_px);
  chainB.SetBranchAddress("mc_nues_py", &mc_nues_py, &b_mc_nues_py);
  chainB.SetBranchAddress("mc_nues_pz", &mc_nues_pz, &b_mc_nues_pz);
  chainB.SetBranchAddress("mc_nues_eta", &mc_nues_eta, &b_mc_nues_eta);
  chainB.SetBranchAddress("mc_nues_phi", &mc_nues_phi, &b_mc_nues_phi);
  chainB.SetBranchAddress("mc_nues_theta", &mc_nues_theta, &b_mc_nues_theta);
  chainB.SetBranchAddress("mc_nues_status", &mc_nues_status, &b_mc_nues_status);
  chainB.SetBranchAddress("mc_nues_energy", &mc_nues_energy, &b_mc_nues_energy);
  chainB.SetBranchAddress("mc_nues_charge", &mc_nues_charge, &b_mc_nues_charge);
  chainB.SetBranchAddress("mc_nues_mother_id", &mc_nues_mother_id, &b_mc_nues_mother_id);
  chainB.SetBranchAddress("mc_nues_mother_pt", &mc_nues_mother_pt, &b_mc_nues_mother_pt);
  chainB.SetBranchAddress("mc_nues_grandmother_id", &mc_nues_grandmother_id, &b_mc_nues_grandmother_id);
  chainB.SetBranchAddress("mc_nues_ggrandmother_id", &mc_nues_ggrandmother_id, &b_mc_nues_ggrandmother_id);
  chainB.SetBranchAddress("mc_nues_vertex_x", &mc_nues_vertex_x, &b_mc_nues_vertex_x);
  chainB.SetBranchAddress("mc_nues_vertex_y", &mc_nues_vertex_y, &b_mc_nues_vertex_y);
  chainB.SetBranchAddress("mc_nues_vertex_z", &mc_nues_vertex_z, &b_mc_nues_vertex_z);
  chainB.SetBranchAddress("mc_nues_mass", &mc_nues_mass, &b_mc_nues_mass);
  chainB.SetBranchAddress("mc_nues_numOfDaughters", &mc_nues_numOfDaughters, &b_mc_nues_numOfDaughters);
  chainB.SetBranchAddress("Nmc_numus", &Nmc_numus, &b_Nmc_numus);
  chainB.SetBranchAddress("mc_numus_id", &mc_numus_id, &b_mc_numus_id);
  chainB.SetBranchAddress("mc_numus_pt", &mc_numus_pt, &b_mc_numus_pt);
  chainB.SetBranchAddress("mc_numus_px", &mc_numus_px, &b_mc_numus_px);
  chainB.SetBranchAddress("mc_numus_py", &mc_numus_py, &b_mc_numus_py);
  chainB.SetBranchAddress("mc_numus_pz", &mc_numus_pz, &b_mc_numus_pz);
  chainB.SetBranchAddress("mc_numus_eta", &mc_numus_eta, &b_mc_numus_eta);
  chainB.SetBranchAddress("mc_numus_phi", &mc_numus_phi, &b_mc_numus_phi);
  chainB.SetBranchAddress("mc_numus_theta", &mc_numus_theta, &b_mc_numus_theta);
  chainB.SetBranchAddress("mc_numus_status", &mc_numus_status, &b_mc_numus_status);
  chainB.SetBranchAddress("mc_numus_energy", &mc_numus_energy, &b_mc_numus_energy);
  chainB.SetBranchAddress("mc_numus_charge", &mc_numus_charge, &b_mc_numus_charge);
  chainB.SetBranchAddress("mc_numus_mother_id", &mc_numus_mother_id, &b_mc_numus_mother_id);
  chainB.SetBranchAddress("mc_numus_mother_pt", &mc_numus_mother_pt, &b_mc_numus_mother_pt);
  chainB.SetBranchAddress("mc_numus_grandmother_id", &mc_numus_grandmother_id, &b_mc_numus_grandmother_id);
  chainB.SetBranchAddress("mc_numus_ggrandmother_id", &mc_numus_ggrandmother_id, &b_mc_numus_ggrandmother_id);
  chainB.SetBranchAddress("mc_numus_vertex_x", &mc_numus_vertex_x, &b_mc_numus_vertex_x);
  chainB.SetBranchAddress("mc_numus_vertex_y", &mc_numus_vertex_y, &b_mc_numus_vertex_y);
  chainB.SetBranchAddress("mc_numus_vertex_z", &mc_numus_vertex_z, &b_mc_numus_vertex_z);
  chainB.SetBranchAddress("mc_numus_mass", &mc_numus_mass, &b_mc_numus_mass);
  chainB.SetBranchAddress("mc_numus_numOfDaughters", &mc_numus_numOfDaughters, &b_mc_numus_numOfDaughters);
  chainB.SetBranchAddress("Nmc_nutaus", &Nmc_nutaus, &b_Nmc_nutaus);
  chainB.SetBranchAddress("mc_nutaus_id", &mc_nutaus_id, &b_mc_nutaus_id);
  chainB.SetBranchAddress("mc_nutaus_pt", &mc_nutaus_pt, &b_mc_nutaus_pt);
  chainB.SetBranchAddress("mc_nutaus_px", &mc_nutaus_px, &b_mc_nutaus_px);
  chainB.SetBranchAddress("mc_nutaus_py", &mc_nutaus_py, &b_mc_nutaus_py);
  chainB.SetBranchAddress("mc_nutaus_pz", &mc_nutaus_pz, &b_mc_nutaus_pz);
  chainB.SetBranchAddress("mc_nutaus_eta", &mc_nutaus_eta, &b_mc_nutaus_eta);
  chainB.SetBranchAddress("mc_nutaus_phi", &mc_nutaus_phi, &b_mc_nutaus_phi);
  chainB.SetBranchAddress("mc_nutaus_theta", &mc_nutaus_theta, &b_mc_nutaus_theta);
  chainB.SetBranchAddress("mc_nutaus_status", &mc_nutaus_status, &b_mc_nutaus_status);
  chainB.SetBranchAddress("mc_nutaus_energy", &mc_nutaus_energy, &b_mc_nutaus_energy);
  chainB.SetBranchAddress("mc_nutaus_charge", &mc_nutaus_charge, &b_mc_nutaus_charge);
  chainB.SetBranchAddress("mc_nutaus_mother_id", &mc_nutaus_mother_id, &b_mc_nutaus_mother_id);
  chainB.SetBranchAddress("mc_nutaus_mother_pt", &mc_nutaus_mother_pt, &b_mc_nutaus_mother_pt);
  chainB.SetBranchAddress("mc_nutaus_grandmother_id", &mc_nutaus_grandmother_id, &b_mc_nutaus_grandmother_id);
  chainB.SetBranchAddress("mc_nutaus_ggrandmother_id", &mc_nutaus_ggrandmother_id, &b_mc_nutaus_ggrandmother_id);
  chainB.SetBranchAddress("mc_nutaus_vertex_x", &mc_nutaus_vertex_x, &b_mc_nutaus_vertex_x);
  chainB.SetBranchAddress("mc_nutaus_vertex_y", &mc_nutaus_vertex_y, &b_mc_nutaus_vertex_y);
  chainB.SetBranchAddress("mc_nutaus_vertex_z", &mc_nutaus_vertex_z, &b_mc_nutaus_vertex_z);
  chainB.SetBranchAddress("mc_nutaus_mass", &mc_nutaus_mass, &b_mc_nutaus_mass);
  chainB.SetBranchAddress("mc_nutaus_numOfDaughters", &mc_nutaus_numOfDaughters, &b_mc_nutaus_numOfDaughters);
  chainB.SetBranchAddress("Nmc_photons", &Nmc_photons, &b_Nmc_photons);
  chainB.SetBranchAddress("mc_photons_id", &mc_photons_id, &b_mc_photons_id);
  chainB.SetBranchAddress("mc_photons_pt", &mc_photons_pt, &b_mc_photons_pt);
  chainB.SetBranchAddress("mc_photons_px", &mc_photons_px, &b_mc_photons_px);
  chainB.SetBranchAddress("mc_photons_py", &mc_photons_py, &b_mc_photons_py);
  chainB.SetBranchAddress("mc_photons_pz", &mc_photons_pz, &b_mc_photons_pz);
  chainB.SetBranchAddress("mc_photons_eta", &mc_photons_eta, &b_mc_photons_eta);
  chainB.SetBranchAddress("mc_photons_phi", &mc_photons_phi, &b_mc_photons_phi);
  chainB.SetBranchAddress("mc_photons_theta", &mc_photons_theta, &b_mc_photons_theta);
  chainB.SetBranchAddress("mc_photons_status", &mc_photons_status, &b_mc_photons_status);
  chainB.SetBranchAddress("mc_photons_energy", &mc_photons_energy, &b_mc_photons_energy);
  chainB.SetBranchAddress("mc_photons_charge", &mc_photons_charge, &b_mc_photons_charge);
  chainB.SetBranchAddress("mc_photons_mother_id", &mc_photons_mother_id, &b_mc_photons_mother_id);
  chainB.SetBranchAddress("mc_photons_mother_pt", &mc_photons_mother_pt, &b_mc_photons_mother_pt);
  chainB.SetBranchAddress("mc_photons_grandmother_id", &mc_photons_grandmother_id, &b_mc_photons_grandmother_id);
  chainB.SetBranchAddress("mc_photons_ggrandmother_id", &mc_photons_ggrandmother_id, &b_mc_photons_ggrandmother_id);
  chainB.SetBranchAddress("mc_photons_vertex_x", &mc_photons_vertex_x, &b_mc_photons_vertex_x);
  chainB.SetBranchAddress("mc_photons_vertex_y", &mc_photons_vertex_y, &b_mc_photons_vertex_y);
  chainB.SetBranchAddress("mc_photons_vertex_z", &mc_photons_vertex_z, &b_mc_photons_vertex_z);
  chainB.SetBranchAddress("mc_photons_mass", &mc_photons_mass, &b_mc_photons_mass);
  chainB.SetBranchAddress("mc_photons_numOfDaughters", &mc_photons_numOfDaughters, &b_mc_photons_numOfDaughters);
  chainB.SetBranchAddress("Nmc_taus", &Nmc_taus, &b_Nmc_taus);
  chainB.SetBranchAddress("mc_taus_id", &mc_taus_id, &b_mc_taus_id);
  chainB.SetBranchAddress("mc_taus_pt", &mc_taus_pt, &b_mc_taus_pt);
  chainB.SetBranchAddress("mc_taus_px", &mc_taus_px, &b_mc_taus_px);
  chainB.SetBranchAddress("mc_taus_py", &mc_taus_py, &b_mc_taus_py);
  chainB.SetBranchAddress("mc_taus_pz", &mc_taus_pz, &b_mc_taus_pz);
  chainB.SetBranchAddress("mc_taus_eta", &mc_taus_eta, &b_mc_taus_eta);
  chainB.SetBranchAddress("mc_taus_phi", &mc_taus_phi, &b_mc_taus_phi);
  chainB.SetBranchAddress("mc_taus_theta", &mc_taus_theta, &b_mc_taus_theta);
  chainB.SetBranchAddress("mc_taus_status", &mc_taus_status, &b_mc_taus_status);
  chainB.SetBranchAddress("mc_taus_energy", &mc_taus_energy, &b_mc_taus_energy);
  chainB.SetBranchAddress("mc_taus_charge", &mc_taus_charge, &b_mc_taus_charge);
  chainB.SetBranchAddress("mc_taus_mother_id", &mc_taus_mother_id, &b_mc_taus_mother_id);
  chainB.SetBranchAddress("mc_taus_mother_pt", &mc_taus_mother_pt, &b_mc_taus_mother_pt);
  chainB.SetBranchAddress("mc_taus_grandmother_id", &mc_taus_grandmother_id, &b_mc_taus_grandmother_id);
  chainB.SetBranchAddress("mc_taus_ggrandmother_id", &mc_taus_ggrandmother_id, &b_mc_taus_ggrandmother_id);
  chainB.SetBranchAddress("mc_taus_vertex_x", &mc_taus_vertex_x, &b_mc_taus_vertex_x);
  chainB.SetBranchAddress("mc_taus_vertex_y", &mc_taus_vertex_y, &b_mc_taus_vertex_y);
  chainB.SetBranchAddress("mc_taus_vertex_z", &mc_taus_vertex_z, &b_mc_taus_vertex_z);
  chainB.SetBranchAddress("mc_taus_mass", &mc_taus_mass, &b_mc_taus_mass);
  chainB.SetBranchAddress("mc_taus_numOfDaughters", &mc_taus_numOfDaughters, &b_mc_taus_numOfDaughters);
  chainB.SetBranchAddress("Nmets", &Nmets, &b_Nmets);
  chainB.SetBranchAddress("mets_et", &mets_et, &b_mets_et);
  chainB.SetBranchAddress("mets_phi", &mets_phi, &b_mets_phi);
  chainB.SetBranchAddress("mets_ex", &mets_ex, &b_mets_ex);
  chainB.SetBranchAddress("mets_ey", &mets_ey, &b_mets_ey);
  chainB.SetBranchAddress("mets_gen_et", &mets_gen_et, &b_mets_gen_et);
  chainB.SetBranchAddress("mets_gen_phi", &mets_gen_phi, &b_mets_gen_phi);
  chainB.SetBranchAddress("mets_sign", &mets_sign, &b_mets_sign);
  chainB.SetBranchAddress("mets_sumEt", &mets_sumEt, &b_mets_sumEt);
  chainB.SetBranchAddress("mets_unCPhi", &mets_unCPhi, &b_mets_unCPhi);
  chainB.SetBranchAddress("mets_unCPt", &mets_unCPt, &b_mets_unCPt);
  chainB.SetBranchAddress("Nmus", &Nmus, &b_Nmus);
  chainB.SetBranchAddress("mus_energy", &mus_energy, &b_mus_energy);
  chainB.SetBranchAddress("mus_et", &mus_et, &b_mus_et);
  chainB.SetBranchAddress("mus_eta", &mus_eta, &b_mus_eta);
  chainB.SetBranchAddress("mus_phi", &mus_phi, &b_mus_phi);
  chainB.SetBranchAddress("mus_pt", &mus_pt, &b_mus_pt);
  chainB.SetBranchAddress("mus_px", &mus_px, &b_mus_px);
  chainB.SetBranchAddress("mus_py", &mus_py, &b_mus_py);
  chainB.SetBranchAddress("mus_pz", &mus_pz, &b_mus_pz);
  chainB.SetBranchAddress("mus_status", &mus_status, &b_mus_status);
  chainB.SetBranchAddress("mus_theta", &mus_theta, &b_mus_theta);
  chainB.SetBranchAddress("mus_gen_id", &mus_gen_id, &b_mus_gen_id);
  chainB.SetBranchAddress("mus_gen_phi", &mus_gen_phi, &b_mus_gen_phi);
  chainB.SetBranchAddress("mus_gen_pt", &mus_gen_pt, &b_mus_gen_pt);
  chainB.SetBranchAddress("mus_gen_pz", &mus_gen_pz, &b_mus_gen_pz);
  chainB.SetBranchAddress("mus_gen_px", &mus_gen_px, &b_mus_gen_px);
  chainB.SetBranchAddress("mus_gen_py", &mus_gen_py, &b_mus_gen_py);
  chainB.SetBranchAddress("mus_gen_eta", &mus_gen_eta, &b_mus_gen_eta);
  chainB.SetBranchAddress("mus_gen_theta", &mus_gen_theta, &b_mus_gen_theta);
  chainB.SetBranchAddress("mus_gen_et", &mus_gen_et, &b_mus_gen_et);
  chainB.SetBranchAddress("mus_gen_mother_id", &mus_gen_mother_id, &b_mus_gen_mother_id);
  chainB.SetBranchAddress("mus_gen_mother_phi", &mus_gen_mother_phi, &b_mus_gen_mother_phi);
  chainB.SetBranchAddress("mus_gen_mother_pt", &mus_gen_mother_pt, &b_mus_gen_mother_pt);
  chainB.SetBranchAddress("mus_gen_mother_pz", &mus_gen_mother_pz, &b_mus_gen_mother_pz);
  chainB.SetBranchAddress("mus_gen_mother_px", &mus_gen_mother_px, &b_mus_gen_mother_px);
  chainB.SetBranchAddress("mus_gen_mother_py", &mus_gen_mother_py, &b_mus_gen_mother_py);
  chainB.SetBranchAddress("mus_gen_mother_eta", &mus_gen_mother_eta, &b_mus_gen_mother_eta);
  chainB.SetBranchAddress("mus_gen_mother_theta", &mus_gen_mother_theta, &b_mus_gen_mother_theta);
  chainB.SetBranchAddress("mus_gen_mother_et", &mus_gen_mother_et, &b_mus_gen_mother_et);
  chainB.SetBranchAddress("mus_tkHits", &mus_tkHits, &b_mus_tkHits);
  chainB.SetBranchAddress("mus_cIso", &mus_cIso, &b_mus_cIso);
  chainB.SetBranchAddress("mus_tIso", &mus_tIso, &b_mus_tIso);
  chainB.SetBranchAddress("mus_ecalIso", &mus_ecalIso, &b_mus_ecalIso);
  chainB.SetBranchAddress("mus_hcalIso", &mus_hcalIso, &b_mus_hcalIso);
  chainB.SetBranchAddress("mus_ecalvetoDep", &mus_ecalvetoDep, &b_mus_ecalvetoDep);
  chainB.SetBranchAddress("mus_hcalvetoDep", &mus_hcalvetoDep, &b_mus_hcalvetoDep);
  chainB.SetBranchAddress("mus_calEnergyEm", &mus_calEnergyEm, &b_mus_calEnergyEm);
  chainB.SetBranchAddress("mus_calEnergyHad", &mus_calEnergyHad, &b_mus_calEnergyHad);
  chainB.SetBranchAddress("mus_calEnergyHo", &mus_calEnergyHo, &b_mus_calEnergyHo);
  chainB.SetBranchAddress("mus_calEnergyEmS9", &mus_calEnergyEmS9, &b_mus_calEnergyEmS9);
  chainB.SetBranchAddress("mus_calEnergyHadS9", &mus_calEnergyHadS9, &b_mus_calEnergyHadS9);
  chainB.SetBranchAddress("mus_calEnergyHoS9", &mus_calEnergyHoS9, &b_mus_calEnergyHoS9);
  chainB.SetBranchAddress("mus_iso03_emVetoEt", &mus_iso03_emVetoEt, &b_mus_iso03_emVetoEt);
  chainB.SetBranchAddress("mus_iso03_hadVetoEt", &mus_iso03_hadVetoEt, &b_mus_iso03_hadVetoEt);
  chainB.SetBranchAddress("mus_iso03_sumPt", &mus_iso03_sumPt, &b_mus_iso03_sumPt);
  chainB.SetBranchAddress("mus_iso03_emEt", &mus_iso03_emEt, &b_mus_iso03_emEt);
  chainB.SetBranchAddress("mus_iso03_hadEt", &mus_iso03_hadEt, &b_mus_iso03_hadEt);
  chainB.SetBranchAddress("mus_iso03_hoEt", &mus_iso03_hoEt, &b_mus_iso03_hoEt);
  chainB.SetBranchAddress("mus_iso03_nTracks", &mus_iso03_nTracks, &b_mus_iso03_nTracks);
  chainB.SetBranchAddress("mus_iso05_sumPt", &mus_iso05_sumPt, &b_mus_iso05_sumPt);
  chainB.SetBranchAddress("mus_iso05_emEt", &mus_iso05_emEt, &b_mus_iso05_emEt);
  chainB.SetBranchAddress("mus_iso05_hadEt", &mus_iso05_hadEt, &b_mus_iso05_hadEt);
  chainB.SetBranchAddress("mus_iso05_hoEt", &mus_iso05_hoEt, &b_mus_iso05_hoEt);
  chainB.SetBranchAddress("mus_iso05_nTracks", &mus_iso05_nTracks, &b_mus_iso05_nTracks);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumChargedHadronPt", &mus_pfIsolationR03_sumChargedHadronPt, &b_mus_pfIsolationR03_sumChargedHadronPt);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumChargedParticlePt", &mus_pfIsolationR03_sumChargedParticlePt, &b_mus_pfIsolationR03_sumChargedParticlePt);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumNeutralHadronEt", &mus_pfIsolationR03_sumNeutralHadronEt, &b_mus_pfIsolationR03_sumNeutralHadronEt);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumNeutralHadronEtHighThreshold", &mus_pfIsolationR03_sumNeutralHadronEtHighThreshold, &b_mus_pfIsolationR03_sumNeutralHadronEtHighThreshold);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumPhotonEt", &mus_pfIsolationR03_sumPhotonEt, &b_mus_pfIsolationR03_sumPhotonEt);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumPhotonEtHighThreshold", &mus_pfIsolationR03_sumPhotonEtHighThreshold, &b_mus_pfIsolationR03_sumPhotonEtHighThreshold);
  chainB.SetBranchAddress("mus_pfIsolationR03_sumPUPt", &mus_pfIsolationR03_sumPUPt, &b_mus_pfIsolationR03_sumPUPt);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumChargedHadronPt", &mus_pfIsolationR04_sumChargedHadronPt, &b_mus_pfIsolationR04_sumChargedHadronPt);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumChargedParticlePt", &mus_pfIsolationR04_sumChargedParticlePt, &b_mus_pfIsolationR04_sumChargedParticlePt);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumNeutralHadronEt", &mus_pfIsolationR04_sumNeutralHadronEt, &b_mus_pfIsolationR04_sumNeutralHadronEt);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumNeutralHadronEtHighThreshold", &mus_pfIsolationR04_sumNeutralHadronEtHighThreshold, &b_mus_pfIsolationR04_sumNeutralHadronEtHighThreshold);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumPhotonEt", &mus_pfIsolationR04_sumPhotonEt, &b_mus_pfIsolationR04_sumPhotonEt);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumPhotonEtHighThreshold", &mus_pfIsolationR04_sumPhotonEtHighThreshold, &b_mus_pfIsolationR04_sumPhotonEtHighThreshold);
  chainB.SetBranchAddress("mus_pfIsolationR04_sumPUPt", &mus_pfIsolationR04_sumPUPt, &b_mus_pfIsolationR04_sumPUPt);
  chainB.SetBranchAddress("mus_charge", &mus_charge, &b_mus_charge);
  chainB.SetBranchAddress("mus_cm_chi2", &mus_cm_chi2, &b_mus_cm_chi2);
  chainB.SetBranchAddress("mus_cm_ndof", &mus_cm_ndof, &b_mus_cm_ndof);
  chainB.SetBranchAddress("mus_cm_chg", &mus_cm_chg, &b_mus_cm_chg);
  chainB.SetBranchAddress("mus_cm_pt", &mus_cm_pt, &b_mus_cm_pt);
  chainB.SetBranchAddress("mus_cm_px", &mus_cm_px, &b_mus_cm_px);
  chainB.SetBranchAddress("mus_cm_py", &mus_cm_py, &b_mus_cm_py);
  chainB.SetBranchAddress("mus_cm_pz", &mus_cm_pz, &b_mus_cm_pz);
  chainB.SetBranchAddress("mus_cm_eta", &mus_cm_eta, &b_mus_cm_eta);
  chainB.SetBranchAddress("mus_cm_phi", &mus_cm_phi, &b_mus_cm_phi);
  chainB.SetBranchAddress("mus_cm_theta", &mus_cm_theta, &b_mus_cm_theta);
  chainB.SetBranchAddress("mus_cm_d0dum", &mus_cm_d0dum, &b_mus_cm_d0dum);
  chainB.SetBranchAddress("mus_cm_dz", &mus_cm_dz, &b_mus_cm_dz);
  chainB.SetBranchAddress("mus_cm_vx", &mus_cm_vx, &b_mus_cm_vx);
  chainB.SetBranchAddress("mus_cm_vy", &mus_cm_vy, &b_mus_cm_vy);
  chainB.SetBranchAddress("mus_cm_vz", &mus_cm_vz, &b_mus_cm_vz);
  chainB.SetBranchAddress("mus_cm_numvalhits", &mus_cm_numvalhits, &b_mus_cm_numvalhits);
  chainB.SetBranchAddress("mus_cm_numlosthits", &mus_cm_numlosthits, &b_mus_cm_numlosthits);
  chainB.SetBranchAddress("mus_cm_numvalMuonhits", &mus_cm_numvalMuonhits, &b_mus_cm_numvalMuonhits);
  chainB.SetBranchAddress("mus_cm_d0dumErr", &mus_cm_d0dumErr, &b_mus_cm_d0dumErr);
  chainB.SetBranchAddress("mus_cm_dzErr", &mus_cm_dzErr, &b_mus_cm_dzErr);
  chainB.SetBranchAddress("mus_cm_ptErr", &mus_cm_ptErr, &b_mus_cm_ptErr);
  chainB.SetBranchAddress("mus_cm_etaErr", &mus_cm_etaErr, &b_mus_cm_etaErr);
  chainB.SetBranchAddress("mus_cm_phiErr", &mus_cm_phiErr, &b_mus_cm_phiErr);
  chainB.SetBranchAddress("mus_tk_id", &mus_tk_id, &b_mus_tk_id);
  chainB.SetBranchAddress("mus_tk_chi2", &mus_tk_chi2, &b_mus_tk_chi2);
  chainB.SetBranchAddress("mus_tk_ndof", &mus_tk_ndof, &b_mus_tk_ndof);
  chainB.SetBranchAddress("mus_tk_chg", &mus_tk_chg, &b_mus_tk_chg);
  chainB.SetBranchAddress("mus_tk_pt", &mus_tk_pt, &b_mus_tk_pt);
  chainB.SetBranchAddress("mus_tk_px", &mus_tk_px, &b_mus_tk_px);
  chainB.SetBranchAddress("mus_tk_py", &mus_tk_py, &b_mus_tk_py);
  chainB.SetBranchAddress("mus_tk_pz", &mus_tk_pz, &b_mus_tk_pz);
  chainB.SetBranchAddress("mus_tk_eta", &mus_tk_eta, &b_mus_tk_eta);
  chainB.SetBranchAddress("mus_tk_phi", &mus_tk_phi, &b_mus_tk_phi);
  chainB.SetBranchAddress("mus_tk_theta", &mus_tk_theta, &b_mus_tk_theta);
  chainB.SetBranchAddress("mus_tk_d0dum", &mus_tk_d0dum, &b_mus_tk_d0dum);
  chainB.SetBranchAddress("mus_tk_dz", &mus_tk_dz, &b_mus_tk_dz);
  chainB.SetBranchAddress("mus_tk_vx", &mus_tk_vx, &b_mus_tk_vx);
  chainB.SetBranchAddress("mus_tk_vy", &mus_tk_vy, &b_mus_tk_vy);
  chainB.SetBranchAddress("mus_tk_vz", &mus_tk_vz, &b_mus_tk_vz);
  chainB.SetBranchAddress("mus_tk_numvalhits", &mus_tk_numvalhits, &b_mus_tk_numvalhits);
  chainB.SetBranchAddress("mus_tk_numlosthits", &mus_tk_numlosthits, &b_mus_tk_numlosthits);
  chainB.SetBranchAddress("mus_tk_d0dumErr", &mus_tk_d0dumErr, &b_mus_tk_d0dumErr);
  chainB.SetBranchAddress("mus_tk_dzErr", &mus_tk_dzErr, &b_mus_tk_dzErr);
  chainB.SetBranchAddress("mus_tk_ptErr", &mus_tk_ptErr, &b_mus_tk_ptErr);
  chainB.SetBranchAddress("mus_tk_etaErr", &mus_tk_etaErr, &b_mus_tk_etaErr);
  chainB.SetBranchAddress("mus_tk_phiErr", &mus_tk_phiErr, &b_mus_tk_phiErr);
  chainB.SetBranchAddress("mus_tk_numvalPixelhits", &mus_tk_numvalPixelhits, &b_mus_tk_numvalPixelhits);
  chainB.SetBranchAddress("mus_tk_numpixelWthMeasr", &mus_tk_numpixelWthMeasr, &b_mus_tk_numpixelWthMeasr);
  chainB.SetBranchAddress("mus_stamu_chi2", &mus_stamu_chi2, &b_mus_stamu_chi2);
  chainB.SetBranchAddress("mus_stamu_ndof", &mus_stamu_ndof, &b_mus_stamu_ndof);
  chainB.SetBranchAddress("mus_stamu_chg", &mus_stamu_chg, &b_mus_stamu_chg);
  chainB.SetBranchAddress("mus_stamu_pt", &mus_stamu_pt, &b_mus_stamu_pt);
  chainB.SetBranchAddress("mus_stamu_px", &mus_stamu_px, &b_mus_stamu_px);
  chainB.SetBranchAddress("mus_stamu_py", &mus_stamu_py, &b_mus_stamu_py);
  chainB.SetBranchAddress("mus_stamu_pz", &mus_stamu_pz, &b_mus_stamu_pz);
  chainB.SetBranchAddress("mus_stamu_eta", &mus_stamu_eta, &b_mus_stamu_eta);
  chainB.SetBranchAddress("mus_stamu_phi", &mus_stamu_phi, &b_mus_stamu_phi);
  chainB.SetBranchAddress("mus_stamu_theta", &mus_stamu_theta, &b_mus_stamu_theta);
  chainB.SetBranchAddress("mus_stamu_d0dum", &mus_stamu_d0dum, &b_mus_stamu_d0dum);
  chainB.SetBranchAddress("mus_stamu_dz", &mus_stamu_dz, &b_mus_stamu_dz);
  chainB.SetBranchAddress("mus_stamu_vx", &mus_stamu_vx, &b_mus_stamu_vx);
  chainB.SetBranchAddress("mus_stamu_vy", &mus_stamu_vy, &b_mus_stamu_vy);
  chainB.SetBranchAddress("mus_stamu_vz", &mus_stamu_vz, &b_mus_stamu_vz);
  chainB.SetBranchAddress("mus_stamu_numvalhits", &mus_stamu_numvalhits, &b_mus_stamu_numvalhits);
  chainB.SetBranchAddress("mus_stamu_numlosthits", &mus_stamu_numlosthits, &b_mus_stamu_numlosthits);
  chainB.SetBranchAddress("mus_stamu_d0dumErr", &mus_stamu_d0dumErr, &b_mus_stamu_d0dumErr);
  chainB.SetBranchAddress("mus_stamu_dzErr", &mus_stamu_dzErr, &b_mus_stamu_dzErr);
  chainB.SetBranchAddress("mus_stamu_ptErr", &mus_stamu_ptErr, &b_mus_stamu_ptErr);
  chainB.SetBranchAddress("mus_stamu_etaErr", &mus_stamu_etaErr, &b_mus_stamu_etaErr);
  chainB.SetBranchAddress("mus_stamu_phiErr", &mus_stamu_phiErr, &b_mus_stamu_phiErr);
  chainB.SetBranchAddress("mus_num_matches", &mus_num_matches, &b_mus_num_matches);
  chainB.SetBranchAddress("mus_isPFMuon", &mus_isPFMuon, &b_mus_isPFMuon);
  chainB.SetBranchAddress("mus_isTrackerMuon", &mus_isTrackerMuon, &b_mus_isTrackerMuon);
  chainB.SetBranchAddress("mus_isStandAloneMuon", &mus_isStandAloneMuon, &b_mus_isStandAloneMuon);
  chainB.SetBranchAddress("mus_isCaloMuon", &mus_isCaloMuon, &b_mus_isCaloMuon);
  chainB.SetBranchAddress("mus_isGlobalMuon", &mus_isGlobalMuon, &b_mus_isGlobalMuon);
  chainB.SetBranchAddress("mus_isElectron", &mus_isElectron, &b_mus_isElectron);
  chainB.SetBranchAddress("mus_isConvertedPhoton", &mus_isConvertedPhoton, &b_mus_isConvertedPhoton);
  chainB.SetBranchAddress("mus_isPhoton", &mus_isPhoton, &b_mus_isPhoton);
  chainB.SetBranchAddress("mus_id_All", &mus_id_All, &b_mus_id_All);
  chainB.SetBranchAddress("mus_id_AllGlobalMuons", &mus_id_AllGlobalMuons, &b_mus_id_AllGlobalMuons);
  chainB.SetBranchAddress("mus_id_AllStandAloneMuons", &mus_id_AllStandAloneMuons, &b_mus_id_AllStandAloneMuons);
  chainB.SetBranchAddress("mus_id_AllTrackerMuons", &mus_id_AllTrackerMuons, &b_mus_id_AllTrackerMuons);
  chainB.SetBranchAddress("mus_id_TrackerMuonArbitrated", &mus_id_TrackerMuonArbitrated, &b_mus_id_TrackerMuonArbitrated);
  chainB.SetBranchAddress("mus_id_AllArbitrated", &mus_id_AllArbitrated, &b_mus_id_AllArbitrated);
  chainB.SetBranchAddress("mus_id_GlobalMuonPromptTight", &mus_id_GlobalMuonPromptTight, &b_mus_id_GlobalMuonPromptTight);
  chainB.SetBranchAddress("mus_id_TMLastStationLoose", &mus_id_TMLastStationLoose, &b_mus_id_TMLastStationLoose);
  chainB.SetBranchAddress("mus_id_TMLastStationTight", &mus_id_TMLastStationTight, &b_mus_id_TMLastStationTight);
  chainB.SetBranchAddress("mus_id_TM2DCompatibilityLoose", &mus_id_TM2DCompatibilityLoose, &b_mus_id_TM2DCompatibilityLoose);
  chainB.SetBranchAddress("mus_id_TM2DCompatibilityTight", &mus_id_TM2DCompatibilityTight, &b_mus_id_TM2DCompatibilityTight);
  chainB.SetBranchAddress("mus_id_TMOneStationLoose", &mus_id_TMOneStationLoose, &b_mus_id_TMOneStationLoose);
  chainB.SetBranchAddress("mus_id_TMOneStationTight", &mus_id_TMOneStationTight, &b_mus_id_TMOneStationTight);
  chainB.SetBranchAddress("mus_id_TMLastStationOptimizedLowPtLoose", &mus_id_TMLastStationOptimizedLowPtLoose, &b_mus_id_TMLastStationOptimizedLowPtLoose);
  chainB.SetBranchAddress("mus_id_TMLastStationOptimizedLowPtTight", &mus_id_TMLastStationOptimizedLowPtTight, &b_mus_id_TMLastStationOptimizedLowPtTight);
  chainB.SetBranchAddress("mus_tk_LayersWithMeasurement", &mus_tk_LayersWithMeasurement, &b_mus_tk_LayersWithMeasurement);
  chainB.SetBranchAddress("mus_tk_PixelLayersWithMeasurement", &mus_tk_PixelLayersWithMeasurement, &b_mus_tk_PixelLayersWithMeasurement);
  chainB.SetBranchAddress("mus_tk_ValidStripLayersWithMonoAndStereoHit", &mus_tk_ValidStripLayersWithMonoAndStereoHit, &b_mus_tk_ValidStripLayersWithMonoAndStereoHit);
  chainB.SetBranchAddress("mus_tk_LayersWithoutMeasurement", &mus_tk_LayersWithoutMeasurement, &b_mus_tk_LayersWithoutMeasurement);
  chainB.SetBranchAddress("mus_tk_ExpectedHitsInner", &mus_tk_ExpectedHitsInner, &b_mus_tk_ExpectedHitsInner);
  chainB.SetBranchAddress("mus_tk_ExpectedHitsOuter", &mus_tk_ExpectedHitsOuter, &b_mus_tk_ExpectedHitsOuter);
  chainB.SetBranchAddress("mus_cm_LayersWithMeasurement", &mus_cm_LayersWithMeasurement, &b_mus_cm_LayersWithMeasurement);
  chainB.SetBranchAddress("mus_cm_PixelLayersWithMeasurement", &mus_cm_PixelLayersWithMeasurement, &b_mus_cm_PixelLayersWithMeasurement);
  chainB.SetBranchAddress("mus_cm_ValidStripLayersWithMonoAndStereoHit", &mus_cm_ValidStripLayersWithMonoAndStereoHit, &b_mus_cm_ValidStripLayersWithMonoAndStereoHit);
  chainB.SetBranchAddress("mus_cm_LayersWithoutMeasurement", &mus_cm_LayersWithoutMeasurement, &b_mus_cm_LayersWithoutMeasurement);
  chainB.SetBranchAddress("mus_cm_ExpectedHitsInner", &mus_cm_ExpectedHitsInner, &b_mus_cm_ExpectedHitsInner);
  chainB.SetBranchAddress("mus_cm_ExpectedHitsOuter", &mus_cm_ExpectedHitsOuter, &b_mus_cm_ExpectedHitsOuter);
  chainB.SetBranchAddress("mus_picky_LayersWithMeasurement", &mus_picky_LayersWithMeasurement, &b_mus_picky_LayersWithMeasurement);
  chainB.SetBranchAddress("mus_picky_PixelLayersWithMeasurement", &mus_picky_PixelLayersWithMeasurement, &b_mus_picky_PixelLayersWithMeasurement);
  chainB.SetBranchAddress("mus_picky_ValidStripLayersWithMonoAndStereoHit", &mus_picky_ValidStripLayersWithMonoAndStereoHit, &b_mus_picky_ValidStripLayersWithMonoAndStereoHit);
  chainB.SetBranchAddress("mus_picky_LayersWithoutMeasurement", &mus_picky_LayersWithoutMeasurement, &b_mus_picky_LayersWithoutMeasurement);
  chainB.SetBranchAddress("mus_picky_ExpectedHitsInner", &mus_picky_ExpectedHitsInner, &b_mus_picky_ExpectedHitsInner);
  chainB.SetBranchAddress("mus_picky_ExpectedHitsOuter", &mus_picky_ExpectedHitsOuter, &b_mus_picky_ExpectedHitsOuter);
  chainB.SetBranchAddress("mus_tpfms_LayersWithMeasurement", &mus_tpfms_LayersWithMeasurement, &b_mus_tpfms_LayersWithMeasurement);
  chainB.SetBranchAddress("mus_tpfms_PixelLayersWithMeasurement", &mus_tpfms_PixelLayersWithMeasurement, &b_mus_tpfms_PixelLayersWithMeasurement);
  chainB.SetBranchAddress("mus_tpfms_ValidStripLayersWithMonoAndStereoHit", &mus_tpfms_ValidStripLayersWithMonoAndStereoHit, &b_mus_tpfms_ValidStripLayersWithMonoAndStereoHit);
  chainB.SetBranchAddress("mus_tpfms_LayersWithoutMeasurement", &mus_tpfms_LayersWithoutMeasurement, &b_mus_tpfms_LayersWithoutMeasurement);
  chainB.SetBranchAddress("mus_tpfms_ExpectedHitsInner", &mus_tpfms_ExpectedHitsInner, &b_mus_tpfms_ExpectedHitsInner);
  chainB.SetBranchAddress("mus_tpfms_ExpectedHitsOuter", &mus_tpfms_ExpectedHitsOuter, &b_mus_tpfms_ExpectedHitsOuter);
  chainB.SetBranchAddress("mus_picky_id", &mus_picky_id, &b_mus_picky_id);
  chainB.SetBranchAddress("mus_picky_chi2", &mus_picky_chi2, &b_mus_picky_chi2);
  chainB.SetBranchAddress("mus_picky_ndof", &mus_picky_ndof, &b_mus_picky_ndof);
  chainB.SetBranchAddress("mus_picky_chg", &mus_picky_chg, &b_mus_picky_chg);
  chainB.SetBranchAddress("mus_picky_pt", &mus_picky_pt, &b_mus_picky_pt);
  chainB.SetBranchAddress("mus_picky_px", &mus_picky_px, &b_mus_picky_px);
  chainB.SetBranchAddress("mus_picky_py", &mus_picky_py, &b_mus_picky_py);
  chainB.SetBranchAddress("mus_picky_pz", &mus_picky_pz, &b_mus_picky_pz);
  chainB.SetBranchAddress("mus_picky_eta", &mus_picky_eta, &b_mus_picky_eta);
  chainB.SetBranchAddress("mus_picky_phi", &mus_picky_phi, &b_mus_picky_phi);
  chainB.SetBranchAddress("mus_picky_theta", &mus_picky_theta, &b_mus_picky_theta);
  chainB.SetBranchAddress("mus_picky_d0dum", &mus_picky_d0dum, &b_mus_picky_d0dum);
  chainB.SetBranchAddress("mus_picky_dz", &mus_picky_dz, &b_mus_picky_dz);
  chainB.SetBranchAddress("mus_picky_vx", &mus_picky_vx, &b_mus_picky_vx);
  chainB.SetBranchAddress("mus_picky_vy", &mus_picky_vy, &b_mus_picky_vy);
  chainB.SetBranchAddress("mus_picky_vz", &mus_picky_vz, &b_mus_picky_vz);
  chainB.SetBranchAddress("mus_picky_numvalhits", &mus_picky_numvalhits, &b_mus_picky_numvalhits);
  chainB.SetBranchAddress("mus_picky_numlosthits", &mus_picky_numlosthits, &b_mus_picky_numlosthits);
  chainB.SetBranchAddress("mus_picky_d0dumErr", &mus_picky_d0dumErr, &b_mus_picky_d0dumErr);
  chainB.SetBranchAddress("mus_picky_dzErr", &mus_picky_dzErr, &b_mus_picky_dzErr);
  chainB.SetBranchAddress("mus_picky_ptErr", &mus_picky_ptErr, &b_mus_picky_ptErr);
  chainB.SetBranchAddress("mus_picky_etaErr", &mus_picky_etaErr, &b_mus_picky_etaErr);
  chainB.SetBranchAddress("mus_picky_phiErr", &mus_picky_phiErr, &b_mus_picky_phiErr);
  chainB.SetBranchAddress("mus_picky_numvalPixelhits", &mus_picky_numvalPixelhits, &b_mus_picky_numvalPixelhits);
  chainB.SetBranchAddress("mus_tpfms_id", &mus_tpfms_id, &b_mus_tpfms_id);
  chainB.SetBranchAddress("mus_tpfms_chi2", &mus_tpfms_chi2, &b_mus_tpfms_chi2);
  chainB.SetBranchAddress("mus_tpfms_ndof", &mus_tpfms_ndof, &b_mus_tpfms_ndof);
  chainB.SetBranchAddress("mus_tpfms_chg", &mus_tpfms_chg, &b_mus_tpfms_chg);
  chainB.SetBranchAddress("mus_tpfms_pt", &mus_tpfms_pt, &b_mus_tpfms_pt);
  chainB.SetBranchAddress("mus_tpfms_px", &mus_tpfms_px, &b_mus_tpfms_px);
  chainB.SetBranchAddress("mus_tpfms_py", &mus_tpfms_py, &b_mus_tpfms_py);
  chainB.SetBranchAddress("mus_tpfms_pz", &mus_tpfms_pz, &b_mus_tpfms_pz);
  chainB.SetBranchAddress("mus_tpfms_eta", &mus_tpfms_eta, &b_mus_tpfms_eta);
  chainB.SetBranchAddress("mus_tpfms_phi", &mus_tpfms_phi, &b_mus_tpfms_phi);
  chainB.SetBranchAddress("mus_tpfms_theta", &mus_tpfms_theta, &b_mus_tpfms_theta);
  chainB.SetBranchAddress("mus_tpfms_d0dum", &mus_tpfms_d0dum, &b_mus_tpfms_d0dum);
  chainB.SetBranchAddress("mus_tpfms_dz", &mus_tpfms_dz, &b_mus_tpfms_dz);
  chainB.SetBranchAddress("mus_tpfms_vx", &mus_tpfms_vx, &b_mus_tpfms_vx);
  chainB.SetBranchAddress("mus_tpfms_vy", &mus_tpfms_vy, &b_mus_tpfms_vy);
  chainB.SetBranchAddress("mus_tpfms_vz", &mus_tpfms_vz, &b_mus_tpfms_vz);
  chainB.SetBranchAddress("mus_tpfms_numvalhits", &mus_tpfms_numvalhits, &b_mus_tpfms_numvalhits);
  chainB.SetBranchAddress("mus_tpfms_numlosthits", &mus_tpfms_numlosthits, &b_mus_tpfms_numlosthits);
  chainB.SetBranchAddress("mus_tpfms_d0dumErr", &mus_tpfms_d0dumErr, &b_mus_tpfms_d0dumErr);
  chainB.SetBranchAddress("mus_tpfms_dzErr", &mus_tpfms_dzErr, &b_mus_tpfms_dzErr);
  chainB.SetBranchAddress("mus_tpfms_ptErr", &mus_tpfms_ptErr, &b_mus_tpfms_ptErr);
  chainB.SetBranchAddress("mus_tpfms_etaErr", &mus_tpfms_etaErr, &b_mus_tpfms_etaErr);
  chainB.SetBranchAddress("mus_tpfms_phiErr", &mus_tpfms_phiErr, &b_mus_tpfms_phiErr);
  chainB.SetBranchAddress("mus_tpfms_numvalPixelhits", &mus_tpfms_numvalPixelhits, &b_mus_tpfms_numvalPixelhits);
  chainB.SetBranchAddress("mus_dB", &mus_dB, &b_mus_dB);
  chainB.SetBranchAddress("mus_numberOfMatchedStations", &mus_numberOfMatchedStations, &b_mus_numberOfMatchedStations);
  chainB.SetBranchAddress("Npfcand", &Npfcand, &b_Npfcand);
  chainB.SetBranchAddress("pfcand_pdgId", &pfcand_pdgId, &b_pfcand_pdgId);
  chainB.SetBranchAddress("pfcand_pt", &pfcand_pt, &b_pfcand_pt);
  chainB.SetBranchAddress("pfcand_pz", &pfcand_pz, &b_pfcand_pz);
  chainB.SetBranchAddress("pfcand_px", &pfcand_px, &b_pfcand_px);
  chainB.SetBranchAddress("pfcand_py", &pfcand_py, &b_pfcand_py);
  chainB.SetBranchAddress("pfcand_eta", &pfcand_eta, &b_pfcand_eta);
  chainB.SetBranchAddress("pfcand_phi", &pfcand_phi, &b_pfcand_phi);
  chainB.SetBranchAddress("pfcand_theta", &pfcand_theta, &b_pfcand_theta);
  chainB.SetBranchAddress("pfcand_energy", &pfcand_energy, &b_pfcand_energy);
  chainB.SetBranchAddress("pfcand_charge", &pfcand_charge, &b_pfcand_charge);
  chainB.SetBranchAddress("Nphotons", &Nphotons, &b_Nphotons);
  chainB.SetBranchAddress("photons_energy", &photons_energy, &b_photons_energy);
  chainB.SetBranchAddress("photons_et", &photons_et, &b_photons_et);
  chainB.SetBranchAddress("photons_eta", &photons_eta, &b_photons_eta);
  chainB.SetBranchAddress("photons_phi", &photons_phi, &b_photons_phi);
  chainB.SetBranchAddress("photons_pt", &photons_pt, &b_photons_pt);
  chainB.SetBranchAddress("photons_px", &photons_px, &b_photons_px);
  chainB.SetBranchAddress("photons_py", &photons_py, &b_photons_py);
  chainB.SetBranchAddress("photons_pz", &photons_pz, &b_photons_pz);
  chainB.SetBranchAddress("photons_status", &photons_status, &b_photons_status);
  chainB.SetBranchAddress("photons_theta", &photons_theta, &b_photons_theta);
  chainB.SetBranchAddress("photons_hadOverEM", &photons_hadOverEM, &b_photons_hadOverEM);
  chainB.SetBranchAddress("photons_hadTowOverEM", &photons_hadTowOverEM, &b_photons_hadTowOverEM);
  chainB.SetBranchAddress("photons_scEnergy", &photons_scEnergy, &b_photons_scEnergy);
  chainB.SetBranchAddress("photons_scRawEnergy", &photons_scRawEnergy, &b_photons_scRawEnergy);
  chainB.SetBranchAddress("photons_scEta", &photons_scEta, &b_photons_scEta);
  chainB.SetBranchAddress("photons_scPhi", &photons_scPhi, &b_photons_scPhi);
  chainB.SetBranchAddress("photons_scEtaWidth", &photons_scEtaWidth, &b_photons_scEtaWidth);
  chainB.SetBranchAddress("photons_scPhiWidth", &photons_scPhiWidth, &b_photons_scPhiWidth);
  chainB.SetBranchAddress("photons_tIso", &photons_tIso, &b_photons_tIso);
  chainB.SetBranchAddress("photons_ecalIso", &photons_ecalIso, &b_photons_ecalIso);
  chainB.SetBranchAddress("photons_hcalIso", &photons_hcalIso, &b_photons_hcalIso);
  chainB.SetBranchAddress("photons_isoEcalRecHitDR04", &photons_isoEcalRecHitDR04, &b_photons_isoEcalRecHitDR04);
  chainB.SetBranchAddress("photons_isoHcalRecHitDR04", &photons_isoHcalRecHitDR04, &b_photons_isoHcalRecHitDR04);
  chainB.SetBranchAddress("photons_isoSolidTrkConeDR04", &photons_isoSolidTrkConeDR04, &b_photons_isoSolidTrkConeDR04);
  chainB.SetBranchAddress("photons_isoHollowTrkConeDR04", &photons_isoHollowTrkConeDR04, &b_photons_isoHollowTrkConeDR04);
  chainB.SetBranchAddress("photons_nTrkSolidConeDR04", &photons_nTrkSolidConeDR04, &b_photons_nTrkSolidConeDR04);
  chainB.SetBranchAddress("photons_nTrkHollowConeDR04", &photons_nTrkHollowConeDR04, &b_photons_nTrkHollowConeDR04);
  chainB.SetBranchAddress("photons_isoEcalRecHitDR03", &photons_isoEcalRecHitDR03, &b_photons_isoEcalRecHitDR03);
  chainB.SetBranchAddress("photons_isoHcalRecHitDR03", &photons_isoHcalRecHitDR03, &b_photons_isoHcalRecHitDR03);
  chainB.SetBranchAddress("photons_isoSolidTrkConeDR03", &photons_isoSolidTrkConeDR03, &b_photons_isoSolidTrkConeDR03);
  chainB.SetBranchAddress("photons_isoHollowTrkConeDR03", &photons_isoHollowTrkConeDR03, &b_photons_isoHollowTrkConeDR03);
  chainB.SetBranchAddress("photons_nTrkSolidConeDR03", &photons_nTrkSolidConeDR03, &b_photons_nTrkSolidConeDR03);
  chainB.SetBranchAddress("photons_nTrkHollowConeDR03", &photons_nTrkHollowConeDR03, &b_photons_nTrkHollowConeDR03);
  chainB.SetBranchAddress("photons_isAlsoElectron", &photons_isAlsoElectron, &b_photons_isAlsoElectron);
  chainB.SetBranchAddress("photons_hasPixelSeed", &photons_hasPixelSeed, &b_photons_hasPixelSeed);
  chainB.SetBranchAddress("photons_isConverted", &photons_isConverted, &b_photons_isConverted);
  chainB.SetBranchAddress("photons_isEBGap", &photons_isEBGap, &b_photons_isEBGap);
  chainB.SetBranchAddress("photons_isEEGap", &photons_isEEGap, &b_photons_isEEGap);
  chainB.SetBranchAddress("photons_isEBEEGap", &photons_isEBEEGap, &b_photons_isEBEEGap);
  chainB.SetBranchAddress("photons_isEBPho", &photons_isEBPho, &b_photons_isEBPho);
  chainB.SetBranchAddress("photons_isEEPho", &photons_isEEPho, &b_photons_isEEPho);
  chainB.SetBranchAddress("photons_isLoosePhoton", &photons_isLoosePhoton, &b_photons_isLoosePhoton);
  chainB.SetBranchAddress("photons_isTightPhoton", &photons_isTightPhoton, &b_photons_isTightPhoton);
  chainB.SetBranchAddress("photons_maxEnergyXtal", &photons_maxEnergyXtal, &b_photons_maxEnergyXtal);
  chainB.SetBranchAddress("photons_e1x5", &photons_e1x5, &b_photons_e1x5);
  chainB.SetBranchAddress("photons_e2x5", &photons_e2x5, &b_photons_e2x5);
  chainB.SetBranchAddress("photons_e3x3", &photons_e3x3, &b_photons_e3x3);
  chainB.SetBranchAddress("photons_e5x5", &photons_e5x5, &b_photons_e5x5);
  chainB.SetBranchAddress("photons_sigmaEtaEta", &photons_sigmaEtaEta, &b_photons_sigmaEtaEta);
  chainB.SetBranchAddress("photons_sigmaIetaIeta", &photons_sigmaIetaIeta, &b_photons_sigmaIetaIeta);
  chainB.SetBranchAddress("photons_r9", &photons_r9, &b_photons_r9);
  chainB.SetBranchAddress("photons_gen_et", &photons_gen_et, &b_photons_gen_et);
  chainB.SetBranchAddress("photons_gen_eta", &photons_gen_eta, &b_photons_gen_eta);
  chainB.SetBranchAddress("photons_gen_phi", &photons_gen_phi, &b_photons_gen_phi);
  chainB.SetBranchAddress("photons_gen_id", &photons_gen_id, &b_photons_gen_id);
  chainB.SetBranchAddress("Npv", &Npv, &b_Npv);
  chainB.SetBranchAddress("pv_x", &pv_x, &b_pv_x);
  chainB.SetBranchAddress("pv_y", &pv_y, &b_pv_y);
  chainB.SetBranchAddress("pv_z", &pv_z, &b_pv_z);
  chainB.SetBranchAddress("pv_xErr", &pv_xErr, &b_pv_xErr);
  chainB.SetBranchAddress("pv_yErr", &pv_yErr, &b_pv_yErr);
  chainB.SetBranchAddress("pv_zErr", &pv_zErr, &b_pv_zErr);
  chainB.SetBranchAddress("pv_chi2", &pv_chi2, &b_pv_chi2);
  chainB.SetBranchAddress("pv_ndof", &pv_ndof, &b_pv_ndof);
  chainB.SetBranchAddress("pv_isFake", &pv_isFake, &b_pv_isFake);
  chainB.SetBranchAddress("pv_isValid", &pv_isValid, &b_pv_isValid);
  chainB.SetBranchAddress("pv_tracksSize", &pv_tracksSize, &b_pv_tracksSize);
  chainB.SetBranchAddress("Ntaus", &Ntaus, &b_Ntaus);
  chainB.SetBranchAddress("taus_status", &taus_status, &b_taus_status);
  chainB.SetBranchAddress("taus_phi", &taus_phi, &b_taus_phi);
  chainB.SetBranchAddress("taus_pt", &taus_pt, &b_taus_pt);
  chainB.SetBranchAddress("taus_pz", &taus_pz, &b_taus_pz);
  chainB.SetBranchAddress("taus_px", &taus_px, &b_taus_px);
  chainB.SetBranchAddress("taus_py", &taus_py, &b_taus_py);
  chainB.SetBranchAddress("taus_eta", &taus_eta, &b_taus_eta);
  chainB.SetBranchAddress("taus_theta", &taus_theta, &b_taus_theta);
  chainB.SetBranchAddress("taus_et", &taus_et, &b_taus_et);
  chainB.SetBranchAddress("taus_energy", &taus_energy, &b_taus_energy);
  chainB.SetBranchAddress("taus_charge", &taus_charge, &b_taus_charge);
  chainB.SetBranchAddress("taus_emf", &taus_emf, &b_taus_emf);
  chainB.SetBranchAddress("taus_hcalTotOverPLead", &taus_hcalTotOverPLead, &b_taus_hcalTotOverPLead);
  chainB.SetBranchAddress("taus_hcalMaxOverPLead", &taus_hcalMaxOverPLead, &b_taus_hcalMaxOverPLead);
  chainB.SetBranchAddress("taus_hcal3x3OverPLead", &taus_hcal3x3OverPLead, &b_taus_hcal3x3OverPLead);
  chainB.SetBranchAddress("taus_ecalStripSumEOverPLead", &taus_ecalStripSumEOverPLead, &b_taus_ecalStripSumEOverPLead);
  chainB.SetBranchAddress("taus_elecPreIdOutput", &taus_elecPreIdOutput, &b_taus_elecPreIdOutput);
  chainB.SetBranchAddress("taus_elecPreIdDecision", &taus_elecPreIdDecision, &b_taus_elecPreIdDecision);
  chainB.SetBranchAddress("taus_leadPFChargedHadrCand_pt", &taus_leadPFChargedHadrCand_pt, &b_taus_leadPFChargedHadrCand_pt);
  chainB.SetBranchAddress("taus_leadPFChargedHadrCand_charge", &taus_leadPFChargedHadrCand_charge, &b_taus_leadPFChargedHadrCand_charge);
  chainB.SetBranchAddress("taus_leadPFChargedHadrCand_eta", &taus_leadPFChargedHadrCand_eta, &b_taus_leadPFChargedHadrCand_eta);
  chainB.SetBranchAddress("taus_leadPFChargedHadrCand_ECAL_eta", &taus_leadPFChargedHadrCand_ECAL_eta, &b_taus_leadPFChargedHadrCand_ECAL_eta);
  chainB.SetBranchAddress("taus_leadPFChargedHadrCand_phi", &taus_leadPFChargedHadrCand_phi, &b_taus_leadPFChargedHadrCand_phi);
  chainB.SetBranchAddress("taus_isoPFGammaCandsEtSum", &taus_isoPFGammaCandsEtSum, &b_taus_isoPFGammaCandsEtSum);
  chainB.SetBranchAddress("taus_isoPFChargedHadrCandsPtSum", &taus_isoPFChargedHadrCandsPtSum, &b_taus_isoPFChargedHadrCandsPtSum);
  chainB.SetBranchAddress("taus_leadingTrackFinding", &taus_leadingTrackFinding, &b_taus_leadingTrackFinding);
  chainB.SetBranchAddress("taus_leadingTrackPtCut", &taus_leadingTrackPtCut, &b_taus_leadingTrackPtCut);
  chainB.SetBranchAddress("taus_trackIsolation", &taus_trackIsolation, &b_taus_trackIsolation);
  chainB.SetBranchAddress("taus_ecalIsolation", &taus_ecalIsolation, &b_taus_ecalIsolation);
  chainB.SetBranchAddress("taus_byIsolation", &taus_byIsolation, &b_taus_byIsolation);
  chainB.SetBranchAddress("taus_againstElectron", &taus_againstElectron, &b_taus_againstElectron);
  chainB.SetBranchAddress("taus_againstMuon", &taus_againstMuon, &b_taus_againstMuon);
  chainB.SetBranchAddress("taus_taNC_quarter", &taus_taNC_quarter, &b_taus_taNC_quarter);
  chainB.SetBranchAddress("taus_taNC_one", &taus_taNC_one, &b_taus_taNC_one);
  chainB.SetBranchAddress("taus_taNC_half", &taus_taNC_half, &b_taus_taNC_half);
  chainB.SetBranchAddress("taus_taNC_tenth", &taus_taNC_tenth, &b_taus_taNC_tenth);
  chainB.SetBranchAddress("taus_taNC", &taus_taNC, &b_taus_taNC);
  chainB.SetBranchAddress("taus_byIsoUsingLeadingPi", &taus_byIsoUsingLeadingPi, &b_taus_byIsoUsingLeadingPi);
  chainB.SetBranchAddress("taus_tkIsoUsingLeadingPi", &taus_tkIsoUsingLeadingPi, &b_taus_tkIsoUsingLeadingPi);
  chainB.SetBranchAddress("taus_ecalIsoUsingLeadingPi", &taus_ecalIsoUsingLeadingPi, &b_taus_ecalIsoUsingLeadingPi);
  chainB.SetBranchAddress("taus_againstElectronLoose", &taus_againstElectronLoose, &b_taus_againstElectronLoose);
  chainB.SetBranchAddress("taus_againstElectronMedium", &taus_againstElectronMedium, &b_taus_againstElectronMedium);
  chainB.SetBranchAddress("taus_againstElectronTight", &taus_againstElectronTight, &b_taus_againstElectronTight);
  chainB.SetBranchAddress("taus_againstElectronMVA", &taus_againstElectronMVA, &b_taus_againstElectronMVA);
  chainB.SetBranchAddress("taus_againstMuonLoose", &taus_againstMuonLoose, &b_taus_againstMuonLoose);
  chainB.SetBranchAddress("taus_againstMuonMedium", &taus_againstMuonMedium, &b_taus_againstMuonMedium);
  chainB.SetBranchAddress("taus_againstMuonTight", &taus_againstMuonTight, &b_taus_againstMuonTight);
  chainB.SetBranchAddress("taus_decayModeFinding", &taus_decayModeFinding, &b_taus_decayModeFinding);
  chainB.SetBranchAddress("taus_byVLooseIsolation", &taus_byVLooseIsolation, &b_taus_byVLooseIsolation);
  chainB.SetBranchAddress("taus_byLooseIsolation", &taus_byLooseIsolation, &b_taus_byLooseIsolation);
  chainB.SetBranchAddress("taus_byMediumIsolation", &taus_byMediumIsolation, &b_taus_byMediumIsolation);
  chainB.SetBranchAddress("taus_byTightIsolation", &taus_byTightIsolation, &b_taus_byTightIsolation);
  chainB.SetBranchAddress("taus_byVLooseIsolationDeltaBetaCorr", &taus_byVLooseIsolationDeltaBetaCorr, &b_taus_byVLooseIsolationDeltaBetaCorr);
  chainB.SetBranchAddress("taus_byLooseIsolationDeltaBetaCorr", &taus_byLooseIsolationDeltaBetaCorr, &b_taus_byLooseIsolationDeltaBetaCorr);
  chainB.SetBranchAddress("taus_byMediumIsolationDeltaBetaCorr", &taus_byMediumIsolationDeltaBetaCorr, &b_taus_byMediumIsolationDeltaBetaCorr);
  chainB.SetBranchAddress("taus_byTightIsolationDeltaBetaCorr", &taus_byTightIsolationDeltaBetaCorr, &b_taus_byTightIsolationDeltaBetaCorr);
  chainB.SetBranchAddress("taus_signalPFChargedHadrCandsSize", &taus_signalPFChargedHadrCandsSize, &b_taus_signalPFChargedHadrCandsSize);
  chainB.SetBranchAddress("taus_muDecision", &taus_muDecision, &b_taus_muDecision);
  chainB.SetBranchAddress("taus_Nprongs", &taus_Nprongs, &b_taus_Nprongs);
  chainB.SetBranchAddress("run", &run, &b_run);
  chainB.SetBranchAddress("event", &event, &b_event);
  chainB.SetBranchAddress("lumiblock", &lumiblock, &b_lumiblock);
  chainB.SetBranchAddress("experimentType", &experimentType, &b_experimentType);
  chainB.SetBranchAddress("bunchCrossing", &bunchCrossing, &b_bunchCrossing);
  chainB.SetBranchAddress("orbitNumber", &orbitNumber, &b_orbitNumber);
  chainB.SetBranchAddress("weight", &weight, &b_weight);
  chainB.SetBranchAddress("model_params", &model_params, &b_model_params);
}

