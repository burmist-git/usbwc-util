//my
#include "src/anaMeasures.hh"

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

  if(argc == 4){
    Int_t runID = atoi(argv[1]);
    TString inrootFilesName  = argv[2];
    TString outrootFilesName = argv[3];
    cout<<"--> Histograming of measurements <--"<<endl
	<<"run ID           : "<<runID<<endl
	<<"inrootFilesName  : "<<inrootFilesName<<endl
	<<"outrootFilesName : "<<outrootFilesName<<endl;
    anaMeasures ana(inrootFilesName);
    if(runID == 0){
      ana.Loop0(outrootFilesName);
    }
    else{
      assert(0);
    }
  }
  else{
    cout<<" Parameter calculation from the WF "<<endl
	<<" runID [1] = runID          "<<endl
      	<<"       [2] - inrootFilesName"<<endl
	<<"       [3] - outrootFilesName"<<endl;
  }

  return 0;
}
