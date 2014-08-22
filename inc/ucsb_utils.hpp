#ifndef H_UCSB_UTILS
#define H_UCSB_UTILS

#include "TString.h"
#include "TH1F.h"

typedef std::pair<int,double> int_double;
typedef std::pair<double,double> double_double;
const double PI = 3.14159265;

bool eigen2x2(float matrix[2][2], float &eig1, float &eig2);
bool id_big2small(const int_double& left, const int_double& right);
bool dd_big2small(const double_double& left, const double_double& right);
bool dd_small2big(const double_double& left, const double_double& right);
void DivideHistosEff(TH1F* hNum, TH1F* hDen, TH1F* hEff);
double deltaphi(double phi1, double phi2);
float dR(float eta1, float phi1, float eta2, float phi2);
TString RoundNumber(double num, int decimals, double denom=1.);
TString ParseSampleName(TString file, TString model, TString &energy, float &xsec);

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
