// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#ifndef H_SMALL_TREE
#define H_SMALL_TREE

#include <vector>
#include "TTree.h"

using namespace std;

class small_tree{
public:
  small_tree();

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

  inline void Fill(){tree.Fill();}

  inline void Write(){tree.Write();}

  inline void GetEntries(){tree.GetEntries();}
};

#endif
