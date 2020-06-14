///////////////////////////////////////////////////////////////////////
//                Date: Wed Jan 21 13:27:00 CET 2015                 //
//               Autor: Leonid Burmistrov                            //
// Program description: Convertion of the binary data into the root  //
//                      format. Initioal binary data file produced   //
//                      by the single board 8 channel                //
//                      USB-WaveCatcher (software version 2.4.11)    //
///////////////////////////////////////////////////////////////////////

//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const Int_t nCh = 8;
const Int_t USBWCnSamplesPoint = 1024;
TString softwareVersion  = "V2.6.4";

void convBunchDataUSB2rootBin(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile);

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
    convBunchDataUSB2rootBin(inputDataFileList, inputDataFileFolder, outputrootFile);
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

void convBunchDataUSB2rootBin(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile){

  FILE *pFile;
  long totFileSizeByte;
  long currentPositionByte;
  long dataPositionByteZero;
  Int_t headerMaxSize = 300;
  char *buffer;
  char *buffertmp;
  char bufferdummy;

  TString inputDataFile;

  const Int_t nChannels     = nCh;
  const Int_t nSamplesPoint = USBWCnSamplesPoint;

  //USBWC data in one ev//
  int EventNumber;
  double EpochTime;
  unsigned int Year;
  unsigned int Month;
  unsigned int Day;
  unsigned int Hour;
  unsigned int Minute;
  unsigned int Second;
  unsigned int Millisecond;
  int channel;
  int EventIDsamIndex;
  int FirstCellToPlotsamIndex;
  float	MeasuredBaseline[nCh];
  float	AmplitudeValue[nCh];
  float	ComputedCharge[nCh];
  float	RiseTimeInstant[nCh];
  float	FallTimeInstant[nCh];
  float	RawTriggerRate[nCh];
  short amplValuesTmp;
  float floatR;

  Int_t EventNumber_usbwc;
  Float_t SamplingPeriod_usbwc;
  Double_t EpochTime_usbwc;
  Int_t Year_usbwc;
  Int_t Month_usbwc;
  Int_t Day_usbwc;
  Int_t Hour_usbwc;
  Int_t Minute_usbwc;
  Int_t Second_usbwc;
  Int_t Millisecond_usbwc;
  Int_t channel_usbwc;
  Int_t EventIDsamIndex_usbwc;
  Int_t FirstCellToPlotsamIndex_usbwc;
  Float_t MeasuredBaseline_usbwc[nCh];
  Float_t AmplitudeValue_usbwc[nCh];
  Float_t ComputedCharge_usbwc[nCh];
  Float_t RiseTimeInstant_usbwc[nCh];
  Float_t FallTimeInstant_usbwc[nCh];
  Float_t RawTriggerRate_usbwc[nCh];
  Short_t amplValues_usbwc[nCh][nSamplesPoint];

  TString softwareVersion_str;

  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( outputrootFile, "RECREATE", "USBWC Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << outputrootFile << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "USBWC Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);

  // for chekin the file
  Int_t nChannels_chek_ii = -999;
  Int_t nSamplingPoints_chek;

  TString chName_ss = "amplValues_usbwc[";
  chName_ss += nChannels;
  chName_ss += "][";
  chName_ss += nSamplesPoint;
  chName_ss +="]/S";
  //cout<<"chName_ss = "<<chName_ss<<endl;

  TString baseline_ss = "MeasuredBaseline_usbwc[";
  TString amplitude_ss = "AmplitudeValue_usbwc[";
  TString charge_ss = "ComputedCharge_usbwc[";
  TString leadingEdgeTime_ss = "RiseTimeInstant_usbwc[";
  TString trailingEdgeTime_ss = "RawTriggerRate_usbwc[";
  TString rateCounter_ss = "RawTriggerRate_usbwc[";

  baseline_ss+=nChannels; baseline_ss+="]/F";
  amplitude_ss+=nChannels; amplitude_ss+="]/F";
  charge_ss+=nChannels; charge_ss+="]/F";
  leadingEdgeTime_ss+=nChannels; leadingEdgeTime_ss+="]/F";
  trailingEdgeTime_ss+=nChannels; trailingEdgeTime_ss+="]/F";
  rateCounter_ss+=nChannels; rateCounter_ss+="]/F";

  //cout<<"vvv_"<<baseline_ss<<"_www"<<endl;
  //cout<<"vvv_"<<amplitude_ss<<"_www"<<endl;
  //cout<<"vvv_"<<charge_ss<<"_www"<<endl;
  //cout<<"vvv_"<<leadingEdgeTime_ss<<"_www"<<endl;
  //cout<<"vvv_"<<trailingEdgeTime_ss<<"_www"<<endl;

  //Event USBWC
  tree->Branch("EventNumber_usbwc",&EventNumber_usbwc, "EventNumber_usbwc/I");
  tree->Branch("SamplingPeriod_usbwc", &SamplingPeriod_usbwc,  "SamplingPeriod_usbwc/F");
  tree->Branch("EpochTime_usbwc",&EpochTime_usbwc, "EpochTime_usbwc/D");
  tree->Branch("Year_usbwc",&Year_usbwc, "Year_usbwc/I");
  tree->Branch("Month_usbwc",&Month_usbwc, "Month_usbwc/I");
  tree->Branch("Day_usbwc",&Day_usbwc, "Day_usbwc/I");
  tree->Branch("Hour_usbwc",&Hour_usbwc, "Hour_usbwc/I");
  tree->Branch("Minute_usbwc",&Minute_usbwc, "Minute_usbwc/I");
  tree->Branch("Second_usbwc",&Second_usbwc, "Second_usbwc/I");
  tree->Branch("Millisecond_usbwc",&Millisecond_usbwc, "Millisecond_usbwc/I");
  tree->Branch("channel_usbwc",&channel_usbwc, "channel_usbwc/I");
  tree->Branch("EventIDsamIndex_usbwc",&EventIDsamIndex_usbwc, "EventIDsamIndex_usbwc/I");
  tree->Branch("FirstCellToPlotsamIndex_usbwc",&FirstCellToPlotsamIndex_usbwc, "FirstCellToPlotsamIndex_usbwc/I");
  tree->Branch("MeasuredBaseline_usbwc", MeasuredBaseline_usbwc, baseline_ss.Data());
  tree->Branch("AmplitudeValue_usbwc", AmplitudeValue_usbwc, amplitude_ss.Data());
  tree->Branch("ComputedCharge_usbwc", ComputedCharge_usbwc, charge_ss.Data());
  tree->Branch("RiseTimeInstant_usbwc", RiseTimeInstant_usbwc, leadingEdgeTime_ss.Data());
  tree->Branch("FallTimeInstant_usbwc", FallTimeInstant_usbwc, trailingEdgeTime_ss.Data());
  tree->Branch("RawTriggerRate_usbwc", RawTriggerRate_usbwc, rateCounter_ss.Data());
  tree->Branch("amplValues_usbwc", amplValues_usbwc, chName_ss.Data());
  ///////////////////////////////////////////////////////

  Int_t i = 0;
  Int_t j = 0;
  Int_t k = 0;
  Int_t l = 0;

  string mot;
  string mot2;
  Double_t amplVal;
  Int_t chID = 0;

  Int_t nEv = 0;
  Int_t nEv_Tot = 0;
  
  Double_t timeNpevEv = -999.0; //Unix time of previous event

  // Init event variables
  EventNumber_usbwc = -999;
  SamplingPeriod_usbwc = -999;
  EpochTime_usbwc = -999;
  Year_usbwc = -999;
  Month_usbwc = -999;
  Day_usbwc = -999;
  Hour_usbwc = -999;
  Minute_usbwc = -999;
  Second_usbwc = -999;
  Millisecond_usbwc = -999;
  channel_usbwc = -999;
  EventIDsamIndex_usbwc = -999;
  FirstCellToPlotsamIndex_usbwc = -999;
  for(j = 0;j<nChannels;j++){
    MeasuredBaseline_usbwc[j] = -999;
    AmplitudeValue_usbwc[j] = -999;
    ComputedCharge_usbwc[j] = -999;
    RiseTimeInstant_usbwc[j] = -999;
    FallTimeInstant_usbwc[j] = -999;
    RawTriggerRate_usbwc[j] = -999;
    for(i = 0;i<nSamplesPoint;i++){
      amplValues_usbwc[j][i] = -999.0;
    }
  }

  ifstream indataReadHeaderOnly;
  ifstream indataList;
  Int_t ii = 0;
  string DATA_SAMPLES_N_P;
  Int_t N_CHANNELS_ACQUIRED;
  indataList.open(inputDataFileList.Data());
  assert(indataList.is_open());
  while (indataList >> mot ){
    inputDataFile = inputDataFileFolder;
    inputDataFile += mot;
    //cout<<indata.is_open()<<endl;
    cout<<" ---> Conversion of "<<inputDataFile<<endl;

    /////////////////////////////////////////////////
    ////////        READ HEADER FIRST        ////////
    //=== DATA FILE SAVED WITH SOFTWARE VERSION: V2.6.4 ===
    //=== WAVECATCHER SYSTEM OF TYPE 0 WITH 8 CHANNELS AND GAIN: 1.0 ===
    //=== Other Parameters *** ===
    //=== DATA SAMPLES [1024] in Volts == NB OF CHANNELS ACQUIRED: 8 == Sampling Period: 312.5 ps == INL Correction: 1 ===
    indataReadHeaderOnly.open(inputDataFile.Data());
    //while (indataReadHeaderOnly >> mot2 ){
    for(ii = 0;ii<100;ii++){
      indataReadHeaderOnly>>mot2;
      //cout<<ii<<" "<<mot2<<endl;
      if(mot2 == "VERSION:"){
	indataReadHeaderOnly>>softwareVersion_str;
      }
      else if(mot2 == "DATA"){
	indataReadHeaderOnly>>mot2;
	if(mot2 == "SAMPLES"){
	  indataReadHeaderOnly>>DATA_SAMPLES_N_P;
	  char *carr = const_cast<char*>(DATA_SAMPLES_N_P.c_str());
	  //cout<<"carr[1] "<<carr[1]<<endl
	  //  <<"carr[2] "<<carr[2]<<endl
	  //  <<"carr[3] "<<carr[3]<<endl
	  //  <<"carr[4] "<<carr[4]<<endl;
	  nSamplingPoints_chek = (carr[1]-'0')*1000 + (carr[2]-'0')*100 + (carr[3]-'0')*10 + (carr[4]-'0');
	  //cout<<"nSamplingPoints_chek "<<nSamplingPoints_chek<<endl;
	}
      }
      else if(mot2 == "CHANNELS"){
	indataReadHeaderOnly>>mot2;
	if(mot2 == "ACQUIRED:"){
	  indataReadHeaderOnly>>N_CHANNELS_ACQUIRED;
	}
      }
      else if(mot2 == "Sampling"){
	indataReadHeaderOnly>>mot2;
	if(mot2 == "Period:"){
	  indataReadHeaderOnly>>SamplingPeriod_usbwc;
	}
      }
      else if(mot2 == "Correction:"){
	break;
      }
    }
    //cout<<"softwareVersion_str  "<<softwareVersion_str<<endl
    //	<<"DATA_SAMPLES_N_P     "<<DATA_SAMPLES_N_P<<endl
    //	<<"N_CHANNELS_ACQUIRED  "<<N_CHANNELS_ACQUIRED<<endl
    //	<<"SamplingPeriod_usbwc "<<SamplingPeriod_usbwc<<endl;
    //cout<<"EXIT LOOP"<<endl;
    indataReadHeaderOnly.close();
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////
    ////////       FIND END OF HEADER !      ////////
    pFile = fopen(inputDataFile.Data(), "rb" );
    //buffertmp = (char*) malloc (sizeof(char)*);
    buffertmp = new char[headerMaxSize];
    //cout<<" ftell (pFile) "<<ftell(pFile)<<endl;
    fread (buffertmp,1,headerMaxSize,pFile);
    for(ii = 0;ii<headerMaxSize;ii++){
      //cout<<ii<<" "<<buffertmp[ii]<<endl;
      if(buffertmp[ii] == 'C')
	if(buffertmp[(ii+1)] == 'o')
	  if(buffertmp[(ii+2)] == 'r')
	    if(buffertmp[(ii+3)] == 'r')
	      if(buffertmp[(ii+4)] == 'e')
		if(buffertmp[(ii+5)] == 'c')
		  if(buffertmp[(ii+6)] == 't')
		    if(buffertmp[(ii+7)] == 'i')
		      if(buffertmp[(ii+8)] == 'o')
			if(buffertmp[(ii+9)] == 'n')
			  if(buffertmp[(ii+10)] == ':')
			    dataPositionByteZero = ii + 18;
      //i = 249   C
      //i = 250   o
      //i = 251   r
      //i = 252   r
      //i = 253   e
      //i = 254   c
      //i = 255   t
      //i = 256   i
      //i = 257   o
      //i = 258   n
      //i = 259   :
      //i = 260    
      //i = 261   1
      //i = 262    
      //i = 263   =
      //i = 264   =
      //i = 265   =
    }
    fclose(pFile);
    //assert(0);
    //dataPositionByteZero = 267;
    //cout<<"dataPositionByteZero = "<<dataPositionByteZero<<endl;
    /////////////////////////////////////////////////
    
    nEv = 0; // Event counter for a given file
    pFile = fopen(inputDataFile.Data(), "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); assert(0);}  
    //cout<<" ---> File to convert : "<<inputDataFile<<endl;
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    totFileSizeByte = ftell (pFile);
    rewind (pFile);
    //cout<<"totFileSizeByte = "<<totFileSizeByte<<endl;
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*totFileSizeByte);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    // copy the file into the buffer:
    //result = fread (buffer,1,lSize,pFile);
    fread (buffer,1,dataPositionByteZero,pFile);
    currentPositionByte = dataPositionByteZero;
    //for(i = 0;i<dataPositionByteZero;i++){
    //cout<<"i = "<<i<<"   "<<buffer[i]<<endl;
    //}
    if( softwareVersion_str != softwareVersion ){
      cout<<" ERROR --> Wrong software version"<<endl
    	  <<"           softwareVersion_str != softwareVersion"<<endl
    	  <<"           softwareVersion_str "<<softwareVersion_str<<endl
    	  <<"           softwareVersion   "<<softwareVersion<<endl;
      assert(0);
    }
    //-----------------------------------------------
    //----------------------------------------------- 
    //assert(0);
    if(N_CHANNELS_ACQUIRED != nChannels){
      cout<<" ERROR --> Wrong number of channles"<<endl
	  <<"           N_CHANNELS_ACQUIRED != nChannels"<<endl
	  <<"           N_CHANNELS_ACQUIRED  = "<<N_CHANNELS_ACQUIRED<<endl
	  <<"           nChannels            = "<<nChannels<<endl;
      assert(0);
    }
    if( nSamplingPoints_chek  != USBWCnSamplesPoint){
      cout<<" ERROR --> Wrong number of sumpling points"<<endl
	  <<"           nSamplingPoints_chek  != USBWCnSamplesPoint"<<endl
	  <<"           nSamplingPoints_chek   = "<<nSamplingPoints_chek<<endl
	  <<"           USBWCnSamplesPoint = "<<USBWCnSamplesPoint<<endl;
      assert(0);
    } 
    //cout<<"assert(0) here"<<endl;
    //assert(0);
    while(currentPositionByte<totFileSizeByte){
    //for(k = 0;k<100;k++){
      currentPositionByte += fread (&EventNumber,1,4,pFile);
      currentPositionByte += fread (&EpochTime,1,8,pFile);
      currentPositionByte += fread (&Year,1,4,pFile);
      currentPositionByte += fread (&Month,1,4,pFile);
      currentPositionByte += fread (&Day,1,4,pFile);
      currentPositionByte += fread (&Hour,1,4,pFile);
      currentPositionByte += fread (&Minute,1,4,pFile);
      currentPositionByte += fread (&Second,1,4,pFile);
      currentPositionByte += fread (&Millisecond,1,4,pFile);    
      EventNumber_usbwc = EventNumber;
      EpochTime_usbwc = EpochTime;
      Year_usbwc = Year;
      Month_usbwc = Month;
      Day_usbwc = Day;
      Hour_usbwc = Hour;
      Minute_usbwc = Minute;
      Second_usbwc = Second;
      Millisecond_usbwc = Millisecond;
      //cout<<"EventNumber_usbwc = "<<EventNumber_usbwc<<endl;
      for(i = 0;i<nCh;i++){
	currentPositionByte += fread (&channel,1,4,pFile);
	currentPositionByte += fread (&EventIDsamIndex,1,4,pFile);
	currentPositionByte += fread (&FirstCellToPlotsamIndex,1,4,pFile);
	currentPositionByte += fread (&floatR,1,4,pFile); MeasuredBaseline[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); AmplitudeValue[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); ComputedCharge[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); RiseTimeInstant[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); FallTimeInstant[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); RawTriggerRate[i] = floatR;
	channel_usbwc = channel;
	EventIDsamIndex_usbwc = EventIDsamIndex;
	FirstCellToPlotsamIndex_usbwc = FirstCellToPlotsamIndex;
	MeasuredBaseline_usbwc[i] = MeasuredBaseline[i];
	AmplitudeValue_usbwc[i]   = AmplitudeValue[i];
	ComputedCharge_usbwc[i]   = ComputedCharge[i];
	RiseTimeInstant_usbwc[i]  = RiseTimeInstant[i];
	FallTimeInstant_usbwc[i]  = FallTimeInstant[i];
	RawTriggerRate_usbwc[i]   = RawTriggerRate[i];
	//cout<<"channel_usbwc           = "<<channel_usbwc<<endl;
	//cout<<"RawTriggerRate_usbwc[i] = "<<RawTriggerRate_usbwc[i]<<endl;
	for(j = 0;j<USBWCnSamplesPoint;j++){
	  currentPositionByte += fread (&amplValuesTmp,1,2,pFile);
	  amplValues_usbwc[i][j] = amplValuesTmp;
	  //cout<<"amplValuesTmp = "<<i<<" "<<j<<" "<<amplValuesTmp<<endl;
	}
	//cout<<"Channel    "<<channel<<endl;
	//cout<<"EventIDsamIndex    "<<EventIDsamIndex<<endl;
      }
      //cout<<"EventNumber         "<<EventNumber<<endl;
      //cout<<"totFileSizeByte     "<<totFileSizeByte<<endl
      //<<"currentPositionByte "<<currentPositionByte<<endl;
      //for(i = 0;i<nCh;i++){
      //cout<<"MeasuredBaseline[i] "<<MeasuredBaseline[i]<<endl;
      //cout<<"AmplitudeValue[i]   "<<AmplitudeValue[i]<<endl;
      //}
      //printf ("EpochTime %13.0f \n", EpochTime);
      //cout<<"Year         "<<Year<<endl
      //<<"Month        "<<Month<<endl
      //<<"Day          "<<Day<<endl
      //<<"Hour         "<<Hour<<endl
      //<<"Minute       "<<Minute<<endl
      //<<"Second       "<<Second<<endl
      //<<"Millisecond  "<<Millisecond<<endl;
      tree->Fill();
    } 
    fclose(pFile);
  }
  indataList.close();
  
  /////////////////////////////
  
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();  
}

/*
fwrite (&EventNumber, sizeof(int), 1, FileForSavingDataPtr);
fwrite (&EpochTime, sizeof(double), 1, FileForSavingDataPtr);
fwrite (&Year, sizeof(unsigned int), 1, FileForSavingDataPtr);
fwrite (&Month, sizeof(unsigned int), 1, FileForSavingDataPtr);
fwrite (&Day, sizeof(unsigned int), 1, FileForSavingDataPtr);
fwrite (&Hour, sizeof(unsigned int), 1, FileForSavingDataPtr);
fwrite (&Minute, sizeof(unsigned int), 1, FileForSavingDataPtr);
fwrite (&Second, sizeof(unsigned int), 1, FileForSavingDataPtr);
uint_millisecond =(unsigned int)Millisecond;
fwrite (&uint_millisecond, sizeof(unsigned int), 1, FileForSavingDataPtr);
for (samIndex = 0; samIndex < SystemParams.NbOfFeSamBlocks; samIndex++){
  for(channel = 0; channel<NB_OF_CHANNELS_IN_SAMBLOCK; channel ++){
    if(SystemParams.ChannelMask[samIndex*NB_OF_CHANNELS_IN_SAMBLOCK+channel] == ENABLED){
      fwrite(&channel, sizeof(int), 1,FileForSavingDataPtr);
      fwrite(EventID+samIndex, sizeof(int), 1,FileForSavingDataPtr);
      fwrite(FirstCellToPlot+samIndex, sizeof(int), 1,FileForSavingDataPtr);
      if(AcqParams.AddAllMeasurementsToFile == TRUE){
	baseline = MeasuredBaseline[samIndex][channel];
	amplitude = AmplitudeValue[samIndex][channel];
	charge = ComputedCharge[samIndex][channel];
	riseTimeInstant = RiseTimeInstant[samIndex][channel];
	fallTimeInstant = FallTimeInstant[samIndex][channel];
	ratecounter = RawTriggerRate[channel+ 2*samIndex];
      }
      else{
	baseline = 0.0;
	amplitude = 0.0;
	charge = 0.0;
	riseTimeInstant =0.0;
	fallTimeInstant =0.0;
	ratecounter = 0.0;
      }
      fwrite(&baseline, sizeof(float), 1, FileForSavingDataPtr);
      fwrite(&amplitude, sizeof(float), 1, FileForSavingDataPtr);
      fwrite(&charge, sizeof(float), 1, FileForSavingDataPtr);
      fwrite(&riseTimeInstant, sizeof(float), 1, FileForSavingDataPtr);
      fwrite(&fallTimeInstant, sizeof(float), 1, FileForSavingDataPtr);
      fwrite(&ratecounter, sizeof(float), 1, FileForSavingDataPtr);
      for(k= 0; k<SystemParams.NbOfSamples; k++){
	DataTempBuffer[k]= (signed short)(FullyOrderedValues[samIndex][channel][k]*10);
      }
      fwrite (DataTempBuffer, sizeof(signed short), SystemParams.NbOfSamples,FileForSavingDataPtr);
    }
  }
}
*/
