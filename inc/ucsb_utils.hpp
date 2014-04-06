#ifndef H_UCSB_UTILS
#define H_UCSB_UTILS

#include "TString.h"
#include "TH1F.h"

typedef std::pair<int,double> int_double;

bool eigen2x2(float matrix[2][2], float &eig1, float &eig2);
bool comp_pair(const int_double& left, const int_double& right);
void DivideHistosEff(TH1F* hNum, TH1F* hDen, TH1F* hEff);
double deltaphi(double phi1, double phi2);
float dR(float eta1, float phi1, float eta2, float phi2);
TString RoundNumber(double num, int decimals, double denom=1.);
TString ParseSampleName(TString file, TString &energy);
TString GetSampleEnergy(TString file, TString &sampleSimple);

namespace particleId {
  enum leptonType { 
    X=0,
    electron=2,
    muon=3,
    electronVeto=4,
    muonVeto=5
  };
}

#endif
