////////////////////////////////////////////////////////////////////
//                Date: Thu Oct 16 14:36:45 CEST 2014             //
//               Author: Sebastien Dubos / Leonid Burmistrov      //
// Program description: Convertion of the txt data into the root  //
//                      format. Initial txt data file produced   //
//                      by the single board 8 channel             //
//                      USB-WaveCatcher.                          //
////////////////////////////////////////////////////////////////////


/////////////////
// === DATA FILE SAVED WITH SOFTWARE VERSION: V2.4.18 ===
// === WAVECATCHER SYSTEM OF TYPE 0 WITH 8 CHANNELS AND GAIN: 1.0 ===
// === Other Parameters *** ===
// === Rates [Hz] of: Ch0 Ch1 Ch2 Ch3 Ch4 Ch5 Ch6 Ch7 ===
// === Event 1 UnixTime = 1433447568.589 date = 2015.6.4 time = 21h.52m.48s.589ms ===
//   170890.58   146361.33   131704.84   152162.84        0.00        0.00        0.00        0.00 
// === Event 2 UnixTime = 1433447570.233 date = 2015.6.4 time = 21h.52m.50s.233ms ===
//   170890.58   146361.33   131704.84   152162.84        0.00        0.00        0.00        0.00 
// === Event 3 UnixTime = 1433447573.259 date = 2015.6.4 time = 21h.52m.53s.259ms ===
//        0.64        1.49        6.38        2.76        0.00        0.00        0.00        0.00 
// === Event 4 UnixTime = 1433447578.315 date = 2015.6.4 time = 21h.52m.58s.315ms ===
//        0.20        0.00        6.13        1.98        0.00        0.00        0.00        0.00 
// === Event 5 UnixTime = 1433447578.340 date = 2015.6.4 time = 21h.52m.58s.340ms ===
//        0.20        0.00        6.13        1.98        0.00        0.00        0.00        0.00
     

//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void convBunchDataUSBRates2root(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile);

int main(int argc, char *argv[]){  
  TString inputDataFileList;
  TString inputDataFileFolder;
  TString outputrootFile;
  if(argc == 5 && atoi(argv[1])==0){
    inputDataFileList = argv[2];
    inputDataFileFolder = argv[3];
    outputrootFile = argv[4];
    cout<<"In data file list : "<<inputDataFileList<<endl
        <<"In data path      : "<<inputDataFileFolder<<endl
        <<"Out root file     : "<<outputrootFile<<endl;
    convBunchDataUSBRates2root(inputDataFileList, inputDataFileFolder, outputrootFile);
  }
  else{
    cout<<" ERROR --->  in input arguments "<<endl
        <<"  runID [1] = 0  "<<endl
        <<"        [2] - in data file list"<<endl
        <<"        [3] - in data path"<<endl
        <<"        [4] - out root file"<<endl;
  }  
  return 0;
}

void convBunchDataUSBRates2root(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile){
  TString inputDataFile;

  const Int_t nChannels = 8;
  const string SoftwareVersion="V2.14.18";

  //USBWC data in one ev//
  Int_t eventID;
  Double_t UnixTime;
  Int_t nChusbwc = nChannels;
  Double_t Rates[nChannels], rates;

  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( outputrootFile, "RECREATE", "USBWC Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << outputrootFile << ": check that the path is correct!!!"
	 << endl;
    assert(0);
  }
  TTree *tree = new TTree("T", "USBWC Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  
  // Create new event
  TTree::SetBranchStyle(0);

  //Event USBWC
  tree->Branch("eventID",&eventID, "eventID/I");
  tree->Branch("UnixTime",&UnixTime, "UnixTime/D");
  tree->Branch("nChusbwc",&nChusbwc, "nChusbwc/I");
  tree->Branch("Rates", Rates, "Rates[nChusbwc]/D");
  
  ///////////////////////////////////////////////////////

  Int_t i = 0;
  Int_t j = 0;
  string mot;

  Int_t nEv = 0;
  Int_t nEv_Tot = 0;
  
  Double_t timeNpevEv = -999.0; //Unix time of previous event

  // Init event variables
  eventID=-999;
  UnixTime = -999.0;
  for(j = 0;j<nChannels;j++){
  Rates[j] = -999.0;
  }

  ifstream indata;
  ifstream indataList;

  indataList.open(inputDataFileList.Data());
  assert(indataList.is_open());
  while (indataList >> mot ){
    inputDataFile = inputDataFileFolder;
    inputDataFile += mot;
    indata.open(inputDataFile.Data()); 
    assert(indata.is_open());
    //cout<<indata.is_open()<<endl;
    cout<<" ---> Conversion of "<<inputDataFile<<endl;
    nEv = 0; // Event counter for a given file
    
    while (indata >> mot ){

      if(mot == "VERSION:"){
	indata >> mot;
	if(nEv_Tot == 0){
	  cout<<" - Software version: "<<mot<<endl;
	  if(mot != "V2.4.18"){
	    cout << " WARNING: Check software version !" <<endl;
	    assert(0);
	  }
	}
      }

      if(mot =="TYPE"){
	indata >> mot >> mot;
	indata >> mot;
	if(nEv_Tot == 0){
	  cout<<" - Number of channels: "<<mot<<endl;
	  if(mot != "8"){
	  cout << " ERROR: Channel number != 8"<<endl;
	  assert(0);
	  }
	}
      }

      if(mot == "GAIN:"){
	indata >> mot;
	if(nEv_Tot == 0){
	  cout << " - Gain: " << mot << endl;
	}
      }
      
      if(mot == "Event"){
	indata >> eventID;
	//cout<<"eventID = "<<eventID<<endl;
	indata >> mot;
	if(mot != "UnixTime") assert(0);
	indata >> mot;
	indata >> UnixTime;
	 if(timeNpevEv != -999.0 && (UnixTime - timeNpevEv)<0.0){
	   cout << "ERROR: " << "Events / files are not ordered in time !" << endl;
	   cout << " - " << "Current eventID : " << eventID << endl;
	   cout << " - " << "Current UnixTime : " << UnixTime << endl;
	   cout << " - " << "Previous Unixtime : " << timeNpevEv << endl;
	   assert(0);
	 }
	 timeNpevEv = UnixTime;
	//=== Event 1 UnixTime = 1413384064.877 date = 2014.10.15 time = 16h.41m.4s.877ms ===
	indata >> mot; if (mot != "date") assert(0);
	indata >> mot >> mot;
	indata >> mot; if (mot != "time") assert(0);
	indata >> mot >> mot >> mot;
	 for(i = 0;i<nChannels;i++){
	   indata >> rates;
	   Rates[i] = rates;
	 }
	nEv++;
	tree->Fill();
      }
     }
    nEv_Tot=nEv_Tot+nEv;
    indata.close();
  } // All files read (endwhile), for one folder
  cout << " - " << nEv_Tot << " events found and stored for this run folder" << endl;

  
  if(nEv_Tot != eventID){
    cout << "WARNING: " << "nEv_Tot found != last eventID read !"<< endl;
    cout << " - " << "nEv_Tot found = " << nEv_Tot << endl;
    cout << " - " << "last eventID recorded : " << eventID << endl;
  }
  indataList.close();
  
  /////////////////////////////
  
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
  
}
