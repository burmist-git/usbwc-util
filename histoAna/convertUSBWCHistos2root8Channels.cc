////////////////////////////////////////////////////////////////////
//                Date: Fri Oct 23 15:44:00 CEST 2015             //
//              Author: Sebastien Dubos                           //
// Program description: Convertion of histograms (txt raw data)   //
//                      into the root format.                     //
//                      Initial txt data file produced            //
//                      by the single board 8 channel             //
//                      USB-WaveCatcher.                          //
////////////////////////////////////////////////////////////////////


/////////////////
//=== FILE SAVED WITH SOFTWARE VERSION: V2.4.18 ===
//=== CHARGE HISTO === 
//== CHANNEL : 0  ==
//== Nb Of Entries in Histogram : 44520  ==
//== X AXIS : Charge in pico-Coulombs [400 values] ==
//-9987.375 -9962.125 -9936.875 -9911.625 -9886.375 -9861.125
//...
//== Y AXIS : Charge Distribution [400 values] ==
//0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//...
//== Mean Charge =   0.596 [pico-Coulombs] ==
//== Charge Sigma  =  12.256 [pico-Coulombs rms] ==
//== CHANNEL : 1  ==
//== Nb Of Entries in Histogram : 44520  ==
//== X AXIS : Charge in pico-Coulombs [400 values] ==
//-9987.375 -9962.125 -9936.875 -9911.625 -9886.375
//...
     

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
#include <sstream> // For "istringstream" function

using namespace std;

void convBunchDataUSBHistos2root(TString inputDataFile, TString outputrootFile);

int main(int argc, char *argv[]){  
  TString inputDataFile;
  TString outputrootFile;
  if(argc == 4 && atoi(argv[1])==0){
    inputDataFile = argv[2];
    outputrootFile = argv[3];
    cout<<"In data file : "<<inputDataFile<<endl
        <<"Out root file     : "<<outputrootFile<<endl;
    convBunchDataUSBHistos2root(inputDataFile, outputrootFile);
  }
  else{
    cout<<" ERROR --->  in input arguments "<<endl
        <<"  runID [1] = 0  "<<endl
        <<"        [2] - in data file"<<endl
        <<"        [3] - out root file"<<endl;
  }  
  return 0;
}

void convBunchDataUSBHistos2root(TString inputDataFile, TString outputrootFile){


  const Int_t nChannels = 8;
  const string SoftwareVersion="V2.4.18";
  
  
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
  
  
  // Starts to read data file
  ifstream indata;
  indata.open(inputDataFile.Data()); 
  assert(indata.is_open());
  cout<<" ---> Conversion of "<<inputDataFile<<endl;
  string mot;
  string str_nbBins;
  Int_t nbBins;
  Int_t i = 0;
  Int_t j = 0;
  Int_t Channel = 999;
  Long_t nbEntries = 999;
  Double_t MeanCharge_pC = 999.0;
  Double_t ChargeSigma_pC = 999.0;
  Double_t binWidth = 999.0;
  Double_t Xmin_LowEdge = 999.0;
  Double_t Xmax_UpperEdge = 999.0;
  tree->Branch("Channel",&Channel, "Channel/I");
  tree->Branch("nbEntries",&nbEntries, "nbEntries/L");
  tree->Branch("nbBins",&nbBins, "nbBins/I");
  tree->Branch("MeanCharge_pC",&MeanCharge_pC, "MeanCharge_pC/D");
  tree->Branch("ChargeSigma_pC",&ChargeSigma_pC, "ChargeSigma/D");
  TString HistogramName = "ChargeHisto_Channel_XX";



  
  
  while (indata >> mot ){
	  
	  	  // Check software version
	  	  if(mot == "VERSION:"){
	  		  indata >> mot;
	  		  cout<<" - Software version: "<<mot<<endl;
	  		  if(mot != SoftwareVersion){
	  			  cout << " WARNING: Check software version !" <<endl;
	  			  assert(0);
	  		  } 
	  	  }
	  	  
	  	  // Read channel data
	  	  if(mot == "CHANNEL"){
	  		  indata >> mot;
	  		  indata >> Channel;
	  		  cout << "-> CHANNEL : " << Channel << endl;
	  		  
	  		  // Read file-header only
	  		  for(j=0; j<18;j++){ 
	  			  indata >> mot;
	  			  
	  			  // Find number of entries
	  			  if(j==8){
	  				  istringstream (mot) >> nbEntries;
	  				  cout << "- Nb. of entries in histogram : " << nbEntries << endl;
	  			  }
	  			  
	  			  // Find number of bins to read
	  			  if(j==17){
	  				  str_nbBins=mot; // [XXX
	  				  str_nbBins=str_nbBins.substr(1,string::npos); // XXX
	  				  istringstream (str_nbBins) >> nbBins;
	  				  cout << "- Nb. of bins in histogram : " << nbBins << endl;
	  			  }
	  		  }
	  		  
	  		  // Create tables
	  		  Double_t X_axis[nbBins];
	  		  Long_t Y_axis[nbBins];
				  for(j = 0;j<nbBins;j++){
					  X_axis[j] = -999.0;
					  Y_axis[j] = -999;
				  }
	  		  
	  		  // Fill tables and variables
	  		  indata >> mot >> mot;
	  		  	  for(i=0;i<nbBins;i++){
	  		  		  indata >> X_axis[i];
	  		  	  }
	  		  	  for(j=0;j<9;j++){
	  		  		  indata >> mot;
	  		  	  }
	  		  	  for(i=0;i<nbBins;i++){
	  		  		  indata >> Y_axis[i];
	  		  	  }
	  		  indata >> mot >> mot >> mot >> mot;
	  		  indata >> MeanCharge_pC;
	  		  indata >> mot >> mot >> mot >> mot >> mot >> mot;
	  		  indata >> ChargeSigma_pC;
	  		  cout << "- Mean charge (pC) : " << MeanCharge_pC << " +/- " << ChargeSigma_pC << endl;
	  		  tree->Fill();
	  		  
	  		  // Histogram
	  		  binWidth = (X_axis[nbBins-1] - X_axis[0])/(nbBins-1);
	  		  Xmin_LowEdge = X_axis[0] - binWidth/2.0;
	  		  Xmax_UpperEdge = X_axis[nbBins-1] + binWidth/2.0;
	  		  HistogramName = Form("ChargeHisto_Channel_%d",Channel) ; 
	  		  TH1D *Histogram = new TH1D(HistogramName,HistogramName,nbBins,Xmin_LowEdge,Xmax_UpperEdge);  
	  		  for(i = 0;i<nbBins;i++){
	  			  Histogram->SetBinContent(i,Y_axis[i]);
	  		  }
	  	  }
  }
  indata.close();
  




  
  /////////////////////////////
  
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
  
}
