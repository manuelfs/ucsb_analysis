#ifndef H_UCSB_UTILS
#define H_UCSB_UTILS

#include "TString.h"

double deltaphi(double phi1, double phi2);
float dR(float eta1, float phi1, float eta2, float phi2);
TString RoundNumber(double num, int decimals, double denom=1);

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
