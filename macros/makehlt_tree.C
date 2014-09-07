// makehlt_tree.C: Generates small trees for the HLT study

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"
#include "TSystem.h"

using namespace std;
using std::cout;
using std::endl;

void makehlt_tree(TString filename, TString outfolder="root/hlt/sep4/"){
  TString cuts[] = {"onelpt>15", "onmupt>15"};
  TString tags[] = {"el15", "mu15"}, folder, sCut;

  TChain tree("tree"); TChain treeglobal("treeglobal");
  tree.Add(filename); treeglobal.Add(filename);
  vector<int> genlep_thresh, v_nori_genels, v_nori_genmus;
  vector<int> *nori_genels = &v_nori_genels, *nori_genmus = &v_nori_genmus;
  int noriginal, nori_genmu0, nori_genel0;
  treeglobal.SetBranchAddress("noriginal",&noriginal);
  treeglobal.GetEntry(0);

  genlep_thresh.push_back(0); genlep_thresh.push_back(10); 
  genlep_thresh.push_back(15); genlep_thresh.push_back(17); 
  genlep_thresh.push_back(20); genlep_thresh.push_back(25); 
  for(unsigned ind(0); ind<genlep_thresh.size(); ind++){
    sCut = "Max$(genels_pt)>"; sCut += genlep_thresh[ind];
    v_nori_genels.push_back(tree.GetEntries(sCut));
    sCut = "Max$(genmus_pt)>"; sCut += genlep_thresh[ind];
    v_nori_genmus.push_back(tree.GetEntries(sCut));
  }
  nori_genel0 = v_nori_genels[0]; nori_genmu0 = v_nori_genmus[0]; 

  filename.Remove(0,filename.Last('/')+1);
  filename.ReplaceAll(".root","");
  for(int cut(0); cut<2; cut++){
    folder = outfolder+"/"+tags[cut]+"/";
    gSystem->mkdir(folder, kTRUE);
    TTree *ctree = tree.CopyTree(cuts[cut]);
    folder += filename+"_"+tags[cut]+".root";
    ctree->SaveAs(folder);

    TFile file(folder,"UPDATE");
    file.cd();
    TTree ctreeglobal("treeglobal", "treeglobal");
    ctreeglobal.Branch("noriginal",&noriginal);
    ctreeglobal.Branch("nori_genels",&nori_genels);
    ctreeglobal.Branch("nori_genmus",&nori_genmus);
    ctreeglobal.Branch("nori_genel0",&nori_genel0);
    ctreeglobal.Branch("nori_genmu0",&nori_genmu0);
    ctreeglobal.Branch("genlep_thresh", &genlep_thresh);
    ctreeglobal.Fill();
    ctreeglobal.Write();
    file.Close();
  }


}

