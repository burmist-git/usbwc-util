//my
#include "src/anaL1Bin.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){

  if(argc == 5 && atoi(argv[1])==0){
    TString rootFilesListF = argv[2];
    TString fileAnaSetups = argv[3];
    TString outRootFileF = argv[4];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"rootFilesListF : "<<rootFilesListF<<endl
	<<"fileAnaSetups  : "<<fileAnaSetups<<endl
	<<"outRootFileF   : "<<outRootFileF<<endl;
    anaL1Bin ana(rootFilesListF);
    ana.anaWFfillRootFile(fileAnaSetups,outRootFileF);
  }
  else{
    cout<<" Parameter calculation from the WF "<<endl
	<<" runID [1] = 0                     "<<endl
      	<<"       [2] - file with list of the root files with WF"<<endl
	<<"       [3] - file with setups for WF analisis"<<endl
	<<"       [4] - name of root file with parameters"<<endl;
  }

  return 0;
}
