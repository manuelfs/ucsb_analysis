#ifndef H_RA4_HANDLER
#define H_RA4_HANDLER

#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <stdint.h>
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "cfa.hpp"
#include "ucsb_utils.hpp"

#define Def_ptThreshold 40
#define Def_etaThreshold 2.4

using namespace std;

class ra4_handler : public cfA{
public:
  explicit ra4_handler(const std::string &fileName, const bool isList, const bool fastMode);

  // General
  void ReduceTree(int Nentries, string outFilename);
  void CalTrigEfficiency(int Nentries, string outFilename);
  double getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex);
  bool IsMC();
  void PrintAllTriggers(string outFilename);

  // Cleaning
  bool PassesMETCleaningCut() const;
  bool PassesPVCut() const;
  bool PassesJSONCut() const;

  // Jets
  static const int JetPTThresholdNJ = 40;
  static const int JetPTThresholdHT = 40;
  int GetNumGoodJets(double ptThresh=Def_ptThreshold) const;
  bool isGoodJet(const unsigned int ijet, const double ptThresh=Def_ptThreshold, const double etaThresh=Def_etaThreshold) const;
  bool passedPFJetSelection(const unsigned int ijet) const;
  vector<int> GetJets(vector<int> electrons, vector<int> muons, float &HT);

  // Leptons
  static const int MuonPTThreshold         = 20; // statics cannot be floats
  static const int MuonVetoPTThreshold     = 15;
  static const int ElectronPTThreshold     = 20;
  static const int ElectronVetoPTThreshold = 15;
  bool hasPFMatch(int index, particleId::leptonType type, int &pfIdx);
  bool passedMuonSelection(uint imu);
  bool passedMuonVetoSelection(uint imu);
  bool passedElectronSelection(uint iel);
  bool passedElectronVetoSelection(uint iel);
  float GetEffectiveArea(float SCEta, bool isMC);
  vector<int> GetElectrons(bool doSignal = true);
  vector<int> GetMuons(bool doSignal = true);
  float GetElectronIsolation(uint iel);
  float GetMuonIsolation(uint imu);
  bool passedBaseElectronSelection(uint iel);
  bool passedBaseMuonSelection(uint iel);
  int GetTrueElectron(int imu, double &closest_dR);
  int GetTrueMuon(int imu, double &closest_dR);

};

#endif
