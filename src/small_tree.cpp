// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#include "small_tree.hpp"
#include <iostream>

using namespace std;
using std::cout;
using std::endl;

small_tree::small_tree():
  tree("tree", "tree"){
  tree.Branch("weight", &weight);
  tree.Branch("wpu", &wpu);
  tree.Branch("ht", &ht);
  tree.Branch("met", &met);
  tree.Branch("metsig", &metsig);
  tree.Branch("onmet", &onmet);
  tree.Branch("ntrupv", &ntrupv);
  tree.Branch("nel", &nel);
  tree.Branch("nvel", &nvel);
  tree.Branch("nmu", &nmu);
  tree.Branch("nvmu", &nvmu);
  tree.Branch("njets", &njets);
  tree.Branch("trigger", &trigger);
  isReadOnly = false;
}

small_tree::small_tree(TString filename):
  chain("tree"){
  chain.Add(filename);
  chain.SetBranchAddress("weight", &weight);
  chain.SetBranchAddress("wpu", &wpu);
  chain.SetBranchAddress("ht", &ht);
  chain.SetBranchAddress("met", &met);
  chain.SetBranchAddress("metsig", &metsig);
  chain.SetBranchAddress("onmet", &onmet);
  chain.SetBranchAddress("ntrupv", &ntrupv);
  chain.SetBranchAddress("nel", &nel);
  chain.SetBranchAddress("nvel", &nvel);
  chain.SetBranchAddress("nmu", &nmu);
  chain.SetBranchAddress("nvmu", &nvmu);
  chain.SetBranchAddress("njets", &njets);
  chain.SetBranchAddress("trigger", &trigger);
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

