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
  float ht;
  float met;
  float metsig;
  float onmet;
  float ntrupv;
  int nel;
  int nvel;
  int nmu;
  int nvmu;
  int njets;
  vector<int> trigger;

  void Fill();
  void Write();
  int GetEntries();
};

#endif
