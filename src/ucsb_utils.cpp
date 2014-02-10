#include <iostream>
#include "TMath.h"
#include "TString.h"
#include "TH1F.h"

using namespace std;

void DivideHistosEff(TH1F* hNum, TH1F* hDen, TH1F* hEff){
  float num, den, eff;
  for(int bin(1); bin <= hNum->GetNbinsX(); bin++){
    num = hNum->GetBinContent(bin);
    den = hDen->GetBinContent(bin);
    if(den){
      eff = num/den;
      if(eff<=1){
	hEff->SetBinContent(bin, eff);
	hEff->SetBinError(bin, sqrt(num)/den); // Errors from likelihood to be implemented
      } else cout<<"Ratio "<<eff<<" > 1, not an efficiency calculation"<<endl;
    } else {
      hEff->SetBinContent(bin, 0);
      hEff->SetBinError(bin, 0);
    }
  }
}

double deltaphi(double phi1, double phi2){
  double result = fabs(phi1-phi2);
  while (result>TMath::Pi()) result -= 2*TMath::Pi();
  while (result<=-TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

float dR(float eta1, float eta2, float phi1, float phi2) {
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

