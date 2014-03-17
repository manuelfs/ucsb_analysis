// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#include "small_tree.hpp"
#include <iostream>

using namespace std;
using std::cout;
using std::endl;

small_tree::small_tree():
  tree("tree", "tree"){
  tree.Branch("nel", &nel);
  tree.Branch("nvel", &nvel);
  tree.Branch("nmu", &nmu);
  tree.Branch("nvmu", &nvmu);
  tree.Branch("trigger", &trigger);
  tree.Branch("njets", &njets);
  tree.Branch("nbt", &nbt);
  tree.Branch("nbm", &nbm);
  tree.Branch("nbl", &nbl);
  tree.Branch("lep_id", &lep_id);
  tree.Branch("lep_tru_id", &lep_tru_id);
  tree.Branch("lep_tru_momid", &lep_tru_momid);
  tree.Branch("weight", &weight);
  tree.Branch("wpu", &wpu);
  tree.Branch("wlumi", &wlumi);
  tree.Branch("ht", &ht);
  tree.Branch("mt", &mt);
  tree.Branch("met", &met);
  tree.Branch("met_phi", &met_phi);
  tree.Branch("metlep_dphi", &metlep_dphi);
  tree.Branch("metsig", &metsig);
  tree.Branch("onmet", &onmet);
  tree.Branch("ntrupv", &ntrupv);
  tree.Branch("jets_pt", &jets_pt);
  tree.Branch("jets_eta", &jets_eta);
  tree.Branch("jets_phi", &jets_phi);
  tree.Branch("jets_csv", &jets_csv);
  tree.Branch("lep_pt", &lep_pt);
  tree.Branch("lep_eta", &lep_eta);
  tree.Branch("lep_phi", &lep_phi);
  tree.Branch("lep_reliso", &lep_reliso);
  tree.Branch("lep_tru_dr", &lep_tru_dr);
  isReadOnly = false;
}

small_tree::small_tree(TString filename):
  chain("tree"){
  chain.Add(filename);
  chain.SetBranchAddress("nel", &nel);
  chain.SetBranchAddress("nvel", &nvel);
  chain.SetBranchAddress("nmu", &nmu);
  chain.SetBranchAddress("nvmu", &nvmu);
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
  chain.SetBranchAddress("ht", &ht);
  chain.SetBranchAddress("mt", &mt);
  chain.SetBranchAddress("met", &met);
  chain.SetBranchAddress("met_phi", &met_phi);
  chain.SetBranchAddress("metlep_dphi", &metlep_dphi);
  chain.SetBranchAddress("metsig", &metsig);
  chain.SetBranchAddress("onmet", &onmet);
  chain.SetBranchAddress("ntrupv", &ntrupv);
  chain.SetBranchAddress("jets_pt", &jets_pt);
  chain.SetBranchAddress("jets_eta", &jets_eta);
  chain.SetBranchAddress("jets_phi", &jets_phi);
  chain.SetBranchAddress("jets_csv", &jets_csv);
  chain.SetBranchAddress("lep_pt", &lep_pt);
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

