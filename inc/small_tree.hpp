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
  vector<int> mus_tru;
  vector<int> els_tru;
  float weight;
  float wpu;
  float wlumi;
  float ht;
  float met;
  float metsig;
  float onmet;
  float ntrupv;
  vector<double> jets_pt;
  vector<double> jets_eta;
  vector<double> jets_phi;
  vector<double> mus_pt;
  vector<double> mus_eta;
  vector<double> mus_phi;
  vector<double> mus_reliso;
  vector<double> els_pt;
  vector<double> els_eta;
  vector<double> els_phi;
  vector<double> els_reliso;

  void Fill();
  void Write();
  int GetEntries();
};

#endif
