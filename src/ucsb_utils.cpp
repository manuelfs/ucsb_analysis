#include "TMath.h"
#include "TString.h"

double deltaphi(double phi1, double phi2){
  double result = fabs(phi1-phi2);
  if (result>TMath::Pi()) result = 2*TMath::Pi() - result;
  return result;
}

float dR(float eta1, float phi1, float eta2, float phi2) {
  return sqrt(pow(eta1-eta2, 2) + pow(fabs(deltaphi(phi1,phi2)), 2)) ;
}

TString RoundNumber(double num, int decimals, double denom){
  if(denom==0) return " - ";
  double neg = 1; if(num*denom<0) neg = -1;
  num /= neg*denom; num += 0.5*pow(10.,-decimals);
  int num_int = (int)num;
  int num_dec = (int)((1+num-num_int)*pow(10.,decimals));
  TString s_dec = ""; s_dec += num_dec; s_dec.Remove(0,1);
  TString result=""; 
  if(neg<0) result+="-";
  result+= num_int;
  if(decimals>0) {
    result+="."; result+=s_dec;
  }
  
  TString afterdot = result;
  afterdot.Remove(0,afterdot.First(".")+1);
  for(int i=0; i<decimals-afterdot.Length(); i++)
    result += "0";
  return result;
}

