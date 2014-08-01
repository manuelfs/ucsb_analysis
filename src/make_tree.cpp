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
  bool isfast(true);
  int c(0), Nentries(0);
  while((c=getopt(argc, argv, "n:i:m:f"))!=-1){
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
    }
  }

  size_t pos(inFilename.find(".root"));
  if(pos==std::string::npos){
    inFilename = inFilename + "*.root";
  }
  TString outFilename(inFilename);
  outFilename.Remove(0,outFilename.Last('/')+1);
  outFilename = "out/small_"+outFilename;

  cout<<"Opening "<<inFilename<<endl;
  ra4_handler tHandler(inFilename, isfast); 
  
  if(Nentries > tHandler.GetTotalEntries() || Nentries < 0) Nentries = tHandler.GetTotalEntries();

  time(&curTime);
  cout<<"Getting started takes "<<difftime(curTime,startTime)<<" seconds. "
      <<"Making reduced tree with "<<Nentries<<" entries out of "<<tHandler.GetTotalEntries()<<endl;
  tHandler.ReduceTree(Nentries, outFilename);

  time(&curTime);
  cout<<Nentries<<" events took "<<difftime(curTime,startTime)<<" seconds"<<endl;

  return 0;
}
