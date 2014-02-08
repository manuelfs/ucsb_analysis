#ifndef H_TRIGGER_HANDLER
#define H_TRIGGER_HANDLER

#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <stdint.h>
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "cfa.hpp"

using namespace std;

class trigger_handler : public cfA{
public:
  explicit trigger_handler(const std::string &fileName, const bool isList, const bool fastMode);

  void CalTrigEfficiency(int Nentries, string outFilename);
  bool PassesMETCleaningCut() const;
  bool PassesPVCut() const;
  bool PassesJSONCut() const;
};

#endif
