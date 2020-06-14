//my
#include "src/anaL2.hh"

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
    Int_t runID    = atoi(argv[1]);
    TString inRootFile  = argv[2];
    TString outRootFile = argv[3];
    cout<<"--> Histograming <--"<<endl
	<<"runID       : "<<runID<<endl
	<<"inRootFile  : "<<inRootFile<<endl
	<<"outRootFile : "<<outRootFile<<endl;
    anaL2 ana(inRootFile);
    ana.Loop(outRootFile,runID);
  }
  else if(argc == 5){
    Int_t runID    = atoi(argv[1]);
    TString inRootFile  = argv[2];
    TString inTSFile    = argv[3];
    TString outRootFile = argv[4];
    cout<<"--> Histograming <--"<<endl
	<<"runID       : "<<runID<<endl
	<<"inRootFile  : "<<inRootFile<<endl
	<<"inTSFile    : "<<inTSFile<<endl
	<<"outRootFile : "<<outRootFile<<endl;
    anaL2 ana(inRootFile);
    ana.LoopTS(outRootFile,inTSFile);
  }
  else{
    cout<<" Parameter calculation from the WF "<<endl
	<<" [1] - runID                     "<<endl
      	<<" [2] - input root file with WF parameters"<<endl
      	<<" [3] - output root file with histograms"<<endl;
    cout<<" Parameter calculation from the WF + info from TS"<<endl
	<<" [1] - runID                     "<<endl
      	<<" [2] - input root file with WF parameters"<<endl
      	<<" [3] - input TXT file with translation stage position"<<endl
      	<<" [4] - output root file with histograms"<<endl;
  }
  return 0;
}
