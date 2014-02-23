#include <iostream>
#include <fstream>
#include <vector>
#include "TString.h"

 
using namespace std;

int main(){
  TString name;
  // Add all small_tree variables here
  vector<TString> variables;
  variables.push_back("float ht");
  variables.push_back("float met");
  variables.push_back("float metsig");
  variables.push_back("float onmet");
  variables.push_back("float ntrupv");  // Thinking about casting it into an int
  variables.push_back("int nel");
  variables.push_back("int nvel");
  variables.push_back("int nmu");
  variables.push_back("int nvmu");
  variables.push_back("int njets");
  variables.push_back("vector<int> trigger");

  vector<TString> functions;
  functions.push_back("Fill");
  functions.push_back("Write");
  functions.push_back("GetEntries");

  std::ofstream cppFile("src/small_tree.cpp"), hppFile("inc/small_tree.hpp");
  
  //////////  hpp file  ///////////////////
  hppFile << "// small_tree: class that contains reduced cfA ntuples\n";
  hppFile << "// File generated with generate_small_tree.exe\n\n";
  hppFile << "#ifndef H_SMALL_TREE\n";
  hppFile << "#define H_SMALL_TREE\n\n";
  hppFile << "#include <vector>\n";
  hppFile << "#include \"TTree.h\"\n\n";
  hppFile << "using namespace std;\n\n";

  hppFile << "class small_tree{\n";
  hppFile << "public:\n";
  hppFile << "  small_tree();\n\n";
  hppFile << "  TTree tree;\n";
  for(unsigned int var(0); var<variables.size(); var++){
    hppFile << "  "<<variables[var]<<";\n";
  }
  for(unsigned int fun(0); fun<functions.size(); fun++){
    hppFile << "  inline void "<<functions[fun]<<"(){tree."<<functions[fun]<<"();}\n";
  }
  
  hppFile << "};\n\n";
  hppFile << "#endif" << endl;

  //////////  cpp file  ///////////////////
  cppFile << "// small_tree: class that contains reduced cfA ntuples\n";
  cppFile << "// File generated with generate_small_tree.exe\n\n";
  cppFile << "#include \"small_tree.hpp\"\n";
  cppFile << "#include <vector>\n\n";

  cppFile << "small_tree::small_tree():\n";
  cppFile << "  tree(\"tree\", \"tree\"){\n";
  for(unsigned int var(0); var<variables.size(); var++){
    name = variables[var];
    name.Remove(0, name.Last(' ')+1);
    cppFile << "  tree.Branch(\""<<name<<"\", &"<<name<<");\n";
  }
  cppFile << "}\n\n";

  cout<<"Written src/small_tree.cpp and inc/small_tree.hpp"<<endl;
  
  cppFile.close();
  hppFile.close();
  return 0;
}
