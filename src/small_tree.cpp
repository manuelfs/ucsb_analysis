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
  tree.Branch("weight", &weight);
  tree.Branch("wpu", &wpu);
  tree.Branch("wlumi", &wlumi);
  tree.Branch("ht", &ht);
  tree.Branch("met", &met);
  tree.Branch("metsig", &metsig);
  tree.Branch("onmet", &onmet);
  tree.Branch("ntrupv", &ntrupv);
  tree.Branch("jets_pt", &jets_pt);
  tree.Branch("jets_eta", &jets_eta);
  tree.Branch("jets_phi", &jets_phi);
  tree.Branch("mus_pt", &mus_pt);
  tree.Branch("mus_eta", &mus_eta);
  tree.Branch("mus_phi", &mus_phi);
  tree.Branch("mus_reliso", &mus_reliso);
  tree.Branch("els_pt", &els_pt);
  tree.Branch("els_eta", &els_eta);
  tree.Branch("els_phi", &els_phi);
  tree.Branch("els_reliso", &els_reliso);
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
  chain.SetBranchAddress("weight", &weight);
  chain.SetBranchAddress("wpu", &wpu);
  chain.SetBranchAddress("wlumi", &wlumi);
  chain.SetBranchAddress("ht", &ht);
  chain.SetBranchAddress("met", &met);
  chain.SetBranchAddress("metsig", &metsig);
  chain.SetBranchAddress("onmet", &onmet);
  chain.SetBranchAddress("ntrupv", &ntrupv);
  chain.SetBranchAddress("jets_pt", &jets_pt);
  chain.SetBranchAddress("jets_eta", &jets_eta);
  chain.SetBranchAddress("jets_phi", &jets_phi);
  chain.SetBranchAddress("mus_pt", &mus_pt);
  chain.SetBranchAddress("mus_eta", &mus_eta);
  chain.SetBranchAddress("mus_phi", &mus_phi);
  chain.SetBranchAddress("mus_reliso", &mus_reliso);
  chain.SetBranchAddress("els_pt", &els_pt);
  chain.SetBranchAddress("els_eta", &els_eta);
  chain.SetBranchAddress("els_phi", &els_phi);
  chain.SetBranchAddress("els_reliso", &els_reliso);
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

