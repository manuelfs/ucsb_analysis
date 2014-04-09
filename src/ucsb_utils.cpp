//----------------------------------------------------------------------------
// ucsb_utils - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef INT_ROOT
   #include "ucsb_utils.hpp"
#endif

#include <iostream>
#include "TMath.h"
#include "TString.h"
#include "TH1F.h"

using namespace std;

bool eigen2x2(float matrix[2][2], float &eig1, float &eig2){
  float root = pow(matrix[0][0],2) + pow(matrix[1][1],2)-2*matrix[0][0]*matrix[1][1]+4*matrix[0][1]*matrix[1][0];
  if(root<0) return false;
  
  eig1 = (matrix[0][0]+matrix[1][1]+sqrt(root))/2.;
  eig2 = (matrix[0][0]+matrix[1][1]-sqrt(root))/2.;
  return true;
}

bool id_big2small(const int_double& left, const int_double& right){ 
  return left.second > right.second; 
}  

bool dd_small2big(const double_double& left, const double_double& right){ 
  return left.first < right.first; 
}  

bool dd_big2small(const double_double& left, const double_double& right){ 
  return left.first > right.first; 
}  

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
  return sqrt(pow(eta1-eta2, 2) + pow(deltaphi(phi1,phi2), 2)) ;
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

TString ParseSampleName(TString file, TString &energy){
  TString sample = file;
  energy = file;
  if(sample.Contains("2012")){
    sample.Remove(0, sample.First("2012"));
    sample.Remove(sample.Index("-"), sample.Sizeof());
    sample.Insert(0,"Data ");
    energy = "8";
  } else {
    energy.Remove(energy.Index("TeV"), energy.Sizeof());
    energy.Remove(0, energy.Last('_')+1);
  }
  if(sample.Contains("TT")) sample = "t#bar{t}";
  if(sample.Contains("SMS")) sample = "T1ttt";

  return sample;
}

TString GetSampleEnergy(TString file, TString &sampleSimple){
  TString energy;
  TString sampleName = ParseSampleName(file, energy);
  sampleSimple = sampleName; sampleSimple += "_"; sampleSimple += energy; sampleSimple += "TeV_";
  sampleSimple.ReplaceAll(" ", "_");
  sampleName += " @ "; sampleName += energy; sampleName += " TeV";
  return sampleName;
}
