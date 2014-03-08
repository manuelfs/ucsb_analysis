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
  float weight;
  float wpu;
  float wlumi;
  float ht;
  float met;
  float metsig;
  float onmet;
  float ntrupv;
  vector<float> jets_pt;
  vector<float> jets_eta;
  vector<float> jets_phi;
  vector<float> mus_pt;
  vector<float> mus_eta;
  vector<float> mus_phi;
  vector<float> mus_reliso;
  vector<float> els_pt;
  vector<float> els_eta;
  vector<float> els_phi;
  vector<float> els_reliso;

  void Fill();
  void Write();
  int GetEntries();
};

#endif
