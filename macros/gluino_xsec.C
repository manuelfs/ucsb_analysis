#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include <fstream>
#include <iostream>
#include <vector>
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraphErrors.h"

using namespace std;
using std::cout;
using std::endl;

void gluino_xsec(TString particle="squark", float minx=300, float maxx=3000){
  styles style("Standard"); style.setDefaultStyle();
  TCanvas can;

  ifstream filexsec("txt/"+particle+"_xsec.txt");
  vector<float> mass, emass, xsec, exsec;
  TString dummy, smass, sxsec, sexsec;
  for(int ind(0); ind < 9; ind++) filexsec >> dummy;
  while(filexsec){
    filexsec >> dummy >> smass >> dummy >> dummy >> sxsec >> dummy >> sexsec >> dummy;
    if(smass.Atof()<=0) break;
    mass.push_back(smass.Atof());
    emass.push_back(0);
    xsec.push_back(sxsec.Atof());
    exsec.push_back(sexsec.Atof()*xsec[xsec.size()-1]/100.);
  }

  double legX = 0.65, legY = 0.93;
  double legW = 0.12, legH = 0.074*2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TF1 expfit("expfit","[0]* pow(x, -[1]*exp(x*[2]))",minx,maxx);
  TF1 polyfit("polyfit","[0]*pow(x, -[1])",minx,maxx);
  polyfit.SetParameter(0, 3e13);
  polyfit.SetParameter(1, 7);
  polyfit.SetLineWidth(4);
  polyfit.SetLineColor(4);
  expfit.SetParameter(0, 3e13);
  expfit.SetParameter(1, 7);
  expfit.SetParameter(2, 0);
  expfit.SetLineWidth(4);
  TGraphErrors graph(mass.size(), &mass[0], &xsec[0], &emass[0], &exsec[0]);
  graph.SetTitle("");
  graph.Draw("ALP");
  graph.Fit(&expfit,"M Q N","",minx,maxx);
  graph.Fit(&polyfit,"M Q N","",minx,maxx);
  polyfit.Draw("same");
  expfit.Draw("same");
  graph.GetXaxis()->SetTitle(particle+" mass (GeV)");
  graph.GetYaxis()->SetTitle("Cross section (pb)"); 

  TString leglabel = "m_{"+particle+"}^{-"+RoundNumber(polyfit.GetParameter(1),1)+"}";
  leg.AddEntry(&polyfit, leglabel);
  leglabel = "m_{"+particle+"}^{-"+RoundNumber(expfit.GetParameter(1),1)+"} e^{" 
    +RoundNumber(expfit.GetParameter(2),6)+"m_{"+particle+"}}";
  leg.AddEntry(&expfit, leglabel);
  leg.Draw();

  cout<<"m^-"<<RoundNumber(polyfit.GetParameter(1),1)<<" or m^-"<<RoundNumber(expfit.GetParameter(1),1)
      <<" e^("<<RoundNumber(expfit.GetParameter(2),6)<<"m)"<<endl;
  can.SetLogy(1);
  can.SaveAs("plots/xsec/"+particle+"_xsec_"+RoundNumber(minx,0)+"_"+RoundNumber(maxx,0)+".png");
}
