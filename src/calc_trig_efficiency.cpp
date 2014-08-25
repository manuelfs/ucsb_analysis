#include <iostream>
#include <string>
#include <unistd.h>
#include "TString.h"
#include "ra4_handler.hpp"
#include <ctime>

 
using namespace std;

int main(int argc, char *argv[]){
  time_t startTime, curTime;
  time(&startTime);

  std::string inFilename("");
  std::string masspoint("");
  bool isfast(true), printTriggers(false);
  int c(0), Nentries(0), trigentry(0);
  while((c=getopt(argc, argv, "n:i:m:ft:"))!=-1){
    switch(c){
    case 'n':
      Nentries=atoi(optarg);
      break;
    case 'i':
      inFilename=optarg;
      break;
    case 'm':
      masspoint=optarg;
      break;
    case 'f':
      isfast=false;
      break;
    case 't':
      printTriggers=true;
      trigentry=atoi(optarg);
      break;
    }
  }

  size_t pos(inFilename.find(".root"));
  TString outFilename(inFilename);
  if(pos==std::string::npos){
    inFilename = inFilename + "/*.root";
    int len(outFilename.Sizeof());
    if(outFilename[len-2] == '/') outFilename.Remove(len-2, len-1);
    outFilename.Remove(0,outFilename.Last('/')+1);
    outFilename = "out/Triggers_"+outFilename+".txt";
  } else {
    outFilename.ReplaceAll("/configurableAnalysis","");
    outFilename.Remove(0,outFilename.Last('/')+1);
    outFilename = "out/Triggers_"+outFilename;
    outFilename.ReplaceAll(".root",".txt");
  }

  cout<<"Opening "<<inFilename<<endl;
  ra4_handler tHandler(inFilename, isfast); 
  
  if(Nentries > tHandler.GetTotalEntries() || Nentries < 0) Nentries = tHandler.GetTotalEntries();

  time(&curTime);
  cout<<"Getting started takes "<<difftime(curTime,startTime)<<" seconds. "
      <<"Calculating trigger efficiency with "<<Nentries<<" entries out of "<<tHandler.GetTotalEntries()<<endl;
  if(printTriggers) tHandler.PrintAllTriggers(outFilename, trigentry);
  else tHandler.CalTrigEfficiency(Nentries, outFilename);

  time(&curTime);
  cout<<Nentries<<" events took "<<difftime(curTime,startTime)<<" seconds"<<endl;

  return 0;
}
