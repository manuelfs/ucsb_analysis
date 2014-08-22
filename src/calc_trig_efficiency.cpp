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
  bool iscfA(true), isfast(true), printTriggers(false);
  int c(0), Nentries(0), trigentry(0);
  while((c=getopt(argc, argv, "n:i:m:cft"))!=-1){
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
    case 'c':
      iscfA=false;
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

  std::string outFilename("");
  if(iscfA){
    if(printTriggers) outFilename="out/Triggers_"+inFilename+"_"+masspoint+".txt";
    else outFilename="out/"+inFilename+"_"+masspoint+".root";
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_*"+masspoint+"*.root";
  }else{
    std::string baseName(inFilename);
    size_t pos(baseName.find(".root"));
    if(pos!=std::string::npos){
      baseName.erase(pos);
    }
    pos=baseName.rfind("/");
    if(pos!=std::string::npos){
      if(pos!=baseName.size()-1){
	baseName.erase(0,pos+1);
      }else{
	baseName.append("file_name_ended_with_slash");
      }
    }
    if(printTriggers) outFilename="out/Triggers_"+baseName+".txt";
    else outFilename="out/"+baseName+".root";
    //cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
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
