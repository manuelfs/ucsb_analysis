// small_tree: class that contains reduced cfA ntuples
// File generated with generate_small_tree.exe

#include "small_tree.hpp"
#include <vector>

small_tree::small_tree():
  tree("tree", "tree"){
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
}

