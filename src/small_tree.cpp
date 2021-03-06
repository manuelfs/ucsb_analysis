// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#include "small_tree.hpp"
#include <iostream>

using namespace std;
using std::cout;
using std::endl;

small_tree::small_tree():
  tree("tree", "tree"){
  tree.Branch("genmus_pt", &genmus_pt);
  genmus_pt = &v_genmus_pt;
  tree.Branch("genmus_eta", &genmus_eta);
  genmus_eta = &v_genmus_eta;
  tree.Branch("genmus_phi", &genmus_phi);
  genmus_phi = &v_genmus_phi;
  tree.Branch("genels_pt", &genels_pt);
  genels_pt = &v_genels_pt;
  tree.Branch("genels_eta", &genels_eta);
  genels_eta = &v_genels_eta;
  tree.Branch("genels_phi", &genels_phi);
  genels_phi = &v_genels_phi;
  tree.Branch("nlep", &nlep);
  tree.Branch("nel", &nel);
  tree.Branch("nvel", &nvel);
  tree.Branch("nmu", &nmu);
  tree.Branch("nvmu", &nvmu);
  tree.Branch("npv", &npv);
  tree.Branch("ntrupv", &ntrupv);
  tree.Branch("trigger", &trigger);
  trigger = &v_trigger;
  tree.Branch("njets", &njets);
  njets = &v_njets;
  tree.Branch("nbt", &nbt);
  nbt = &v_nbt;
  tree.Branch("nbm", &nbm);
  nbm = &v_nbm;
  tree.Branch("nbl", &nbl);
  nbl = &v_nbl;
  tree.Branch("lep_id", &lep_id);
  lep_id = &v_lep_id;
  tree.Branch("lep_tru_id", &lep_tru_id);
  lep_tru_id = &v_lep_tru_id;
  tree.Branch("lep_tru_momid", &lep_tru_momid);
  lep_tru_momid = &v_lep_tru_momid;
  tree.Branch("weight", &weight);
  tree.Branch("wpu", &wpu);
  tree.Branch("wlumi", &wlumi);
  tree.Branch("wl1ht200", &wl1ht200);
  tree.Branch("ht", &ht);
  tree.Branch("mt", &mt);
  tree.Branch("met", &met);
  tree.Branch("genmet", &genmet);
  tree.Branch("genht", &genht);
  tree.Branch("met_phi", &met_phi);
  tree.Branch("dphi_wlep", &dphi_wlep);
  tree.Branch("metsig", &metsig);
  tree.Branch("onmet", &onmet);
  tree.Branch("onht", &onht);
  tree.Branch("onmupt", &onmupt);
  tree.Branch("onelpt", &onelpt);
  tree.Branch("dr_bb", &dr_bb);
  tree.Branch("spher", &spher);
  tree.Branch("spher_jets", &spher_jets);
  tree.Branch("spher_nolin", &spher_nolin);
  tree.Branch("ntrupv_mean", &ntrupv_mean);
  tree.Branch("jets_pt", &jets_pt);
  jets_pt = &v_jets_pt;
  tree.Branch("jets_eta", &jets_eta);
  jets_eta = &v_jets_eta;
  tree.Branch("jets_phi", &jets_phi);
  jets_phi = &v_jets_phi;
  tree.Branch("jets_csv", &jets_csv);
  jets_csv = &v_jets_csv;
  tree.Branch("lep_pt", &lep_pt);
  lep_pt = &v_lep_pt;
  tree.Branch("lep_gen_pt", &lep_gen_pt);
  lep_gen_pt = &v_lep_gen_pt;
  tree.Branch("lep_eta", &lep_eta);
  lep_eta = &v_lep_eta;
  tree.Branch("lep_phi", &lep_phi);
  lep_phi = &v_lep_phi;
  tree.Branch("lep_reliso", &lep_reliso);
  lep_reliso = &v_lep_reliso;
  tree.Branch("lep_tru_dr", &lep_tru_dr);
  lep_tru_dr = &v_lep_tru_dr;
  isReadOnly = false;
}

small_tree::small_tree(TString filename):
  chain("tree"){
  chain.Add(filename);
  chain.SetBranchAddress("genmus_pt", &genmus_pt);
  chain.SetBranchAddress("genmus_eta", &genmus_eta);
  chain.SetBranchAddress("genmus_phi", &genmus_phi);
  chain.SetBranchAddress("genels_pt", &genels_pt);
  chain.SetBranchAddress("genels_eta", &genels_eta);
  chain.SetBranchAddress("genels_phi", &genels_phi);
  chain.SetBranchAddress("nlep", &nlep);
  chain.SetBranchAddress("nel", &nel);
  chain.SetBranchAddress("nvel", &nvel);
  chain.SetBranchAddress("nmu", &nmu);
  chain.SetBranchAddress("nvmu", &nvmu);
  chain.SetBranchAddress("npv", &npv);
  chain.SetBranchAddress("ntrupv", &ntrupv);
  chain.SetBranchAddress("trigger", &trigger);
  chain.SetBranchAddress("njets", &njets);
  chain.SetBranchAddress("nbt", &nbt);
  chain.SetBranchAddress("nbm", &nbm);
  chain.SetBranchAddress("nbl", &nbl);
  chain.SetBranchAddress("lep_id", &lep_id);
  chain.SetBranchAddress("lep_tru_id", &lep_tru_id);
  chain.SetBranchAddress("lep_tru_momid", &lep_tru_momid);
  chain.SetBranchAddress("weight", &weight);
  chain.SetBranchAddress("wpu", &wpu);
  chain.SetBranchAddress("wlumi", &wlumi);
  chain.SetBranchAddress("wl1ht200", &wl1ht200);
  chain.SetBranchAddress("ht", &ht);
  chain.SetBranchAddress("mt", &mt);
  chain.SetBranchAddress("met", &met);
  chain.SetBranchAddress("genmet", &genmet);
  chain.SetBranchAddress("genht", &genht);
  chain.SetBranchAddress("met_phi", &met_phi);
  chain.SetBranchAddress("dphi_wlep", &dphi_wlep);
  chain.SetBranchAddress("metsig", &metsig);
  chain.SetBranchAddress("onmet", &onmet);
  chain.SetBranchAddress("onht", &onht);
  chain.SetBranchAddress("onmupt", &onmupt);
  chain.SetBranchAddress("onelpt", &onelpt);
  chain.SetBranchAddress("dr_bb", &dr_bb);
  chain.SetBranchAddress("spher", &spher);
  chain.SetBranchAddress("spher_jets", &spher_jets);
  chain.SetBranchAddress("spher_nolin", &spher_nolin);
  chain.SetBranchAddress("ntrupv_mean", &ntrupv_mean);
  chain.SetBranchAddress("jets_pt", &jets_pt);
  chain.SetBranchAddress("jets_eta", &jets_eta);
  chain.SetBranchAddress("jets_phi", &jets_phi);
  chain.SetBranchAddress("jets_csv", &jets_csv);
  chain.SetBranchAddress("lep_pt", &lep_pt);
  chain.SetBranchAddress("lep_gen_pt", &lep_gen_pt);
  chain.SetBranchAddress("lep_eta", &lep_eta);
  chain.SetBranchAddress("lep_phi", &lep_phi);
  chain.SetBranchAddress("lep_reliso", &lep_reliso);
  chain.SetBranchAddress("lep_tru_dr", &lep_tru_dr);
  isReadOnly = true;
}

void small_tree::Fill(){
  if(isReadOnly) cout<<"Tree is read only"<<endl;
  else tree.Fill();
}

void small_tree::Write(){
  if(isReadOnly) chain.Write();
  else tree.Write();
}

int small_tree::GetEntries(){
  if(isReadOnly) return chain.GetEntries();
  else return tree.GetEntries();
}

