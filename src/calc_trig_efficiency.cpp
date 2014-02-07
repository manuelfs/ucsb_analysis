#include <iostream>
#include <string>
#include <unistd.h>
#include "TString.h"
#include "trigger_handler.hpp"
#include <ctime>

 
using namespace std;

int main(int argc, char *argv[]){
  cout<<"Starting calc_trig_efficiency"<<endl;
  time_t startTime, curTime;
  time(&startTime);

  std::string inFilename("");
  bool iscfA(true), isfast(true);
  int c(0), Nentries(0);
  while((c=getopt(argc, argv, "n:i:cf"))!=-1){
    switch(c){
    case 'n':
      Nentries=atoi(optarg);
      break;
    case 'i':
      inFilename=optarg;
      break;
    case 'c':
      iscfA=false;
      break;
    case 'f':
      isfast=false;
      break;
    }
  }
  cout<<"Opening "<<inFilename<<endl;
  std::string outFilename("");
  if(iscfA){
    outFilename="out/"+inFilename+".root";
    inFilename="/net/cms2/cms2r0/cfA/"+inFilename+"/cfA_*.root";
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
    outFilename="out/"+baseName+".root";
    std::cout << inFilename << "\n" << baseName << "\n" << outFilename << "\n";
  }

  cout<<"Opening "<<inFilename<<endl;
  trigger_handler tHandler(inFilename, false, isfast); 
  
  if(Nentries > tHandler.GetTotalEntries()) Nentries = tHandler.GetTotalEntries();

  time(&curTime);
  cout<<"Getting started takes "<<difftime(curTime,startTime)<<" seconds"<<endl;
  tHandler.CalTrigEfficiency(Nentries);

  time(&curTime);
  cout<<Nentries<<" events took "<<difftime(curTime,startTime)<<" seconds"<<endl;
}
