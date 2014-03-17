// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#ifndef H_SMALL_TREE
#define H_SMALL_TREE

#include <vector>
#include "TChain.h"
#include "TTree.h"

using namespace std;

class small_tree{
public:
  small_tree(); // Constructor to create tree
  small_tree(TString filename); // Constructor to read tree

  bool isReadOnly;
  TChain chain;
  TTree tree;
  int nel;
  int nvel;
  int nmu;
  int nvmu;
  vector<int> trigger;
  vector<int> njets;
  vector<int> nbt;
  vector<int> nbm;
  vector<int> nbl;
  vector<int> lep_id;
  vector<int> lep_tru_id;
  vector<int> lep_tru_momid;
  float weight;
  float wpu;
  float wlumi;
  float ht;
  float mt;
  float met;
  float met_phi;
  float metlep_dphi;
  float metsig;
  float onmet;
  float ntrupv;
  vector<double> jets_pt;
  vector<double> jets_eta;
  vector<double> jets_phi;
  vector<double> jets_csv;
  vector<double> lep_pt;
  vector<double> lep_eta;
  vector<double> lep_phi;
  vector<double> lep_reliso;
  vector<double> lep_tru_dr;

  void Fill();
  void Write();
  int GetEntries();
};

#endif
