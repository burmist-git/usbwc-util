///////////////////////////////////////////////////////////////////////
//                Date: Tue Nov  3 17:45:25 CET 2015                 //
//               Autor: Leonid Burmistrov                            //
// Program description: Convertion of the binary data into the root  //
//                      format. Initioal binary data file produced   //
//                      by the single board 8 channel                //
//                      USB-WaveCatcher (software version 2.4.11)    //
//                                                    and 2.4.18     //
///////////////////////////////////////////////////////////////////////

//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

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
const Int_t nMaxDataFiles = 100000;

//const Int_t nPointMax = 100000;
//const Int_t nSegmentsMax = 100000;//per file

void convertUSBWCRates2root8ChannelsBin(TString dataFileList,
					TString datPreff, 
					TString rootFileN);

int main(int argc, char *argv[] ){
  if(argc == 4){
    TString dataFileList = argv[1];
    TString datPreff = argv[2];
    TString rootFileN = argv[3];
    cout<<endl
	<<" File containing the list of input data files: "<<dataFileList<<endl
	<<" Path of the directory containing these input files: "<<datPreff<<endl
	<<" Name of the output ROOT file: "<<rootFileN<<endl<<endl;
    convertUSBWCRates2root8ChannelsBin(dataFileList,
				       datPreff, 
				       rootFileN);
  }
  else{
    cout<<endl
	<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : name of a text file containing the list of binary files with rates to be converted to ROOT"<<endl
	<<"             [2] : path of the directory containing the input USBWC files"<<endl
	<<"             [3] : name of the output ROOT file"<<endl;
  }
  return 0;
}

void convertUSBWCRates2root8ChannelsBin(TString dataFileList,
					TString datPreff, 
					TString rootFileN){
  FILE *pFile;
  long totFileSizeByte;
  long currentPositionByte;
  char *buffer;
  TString buffTmp;
  
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
  float	RawTriggerRate[nCh];
  float floatR;

  Int_t EventNumber_usbwc;
  Double_t EpochTime_usbwc;
  Int_t Year_usbwc;
  Int_t Month_usbwc;
  Int_t Day_usbwc;
  Int_t Hour_usbwc;
  Int_t Minute_usbwc;
  Int_t Second_usbwc;
  Int_t Millisecond_usbwc;
  Float_t RawTriggerRate_usbwc[nCh];
  Int_t nCh_usbwc = nCh;

  TFile *hfile = new TFile( rootFileN, "RECREATE", "Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << rootFileN << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  //Event
  tree->Branch("EventNumber_usbwc",&EventNumber_usbwc, "EventNumber_usbwc/I");
  tree->Branch("EpochTime_usbwc",&EpochTime_usbwc, "EpochTime_usbwc/D");
  tree->Branch("Year_usbwc",&Year_usbwc, "Year_usbwc/I");
  tree->Branch("Month_usbwc",&Month_usbwc, "Month_usbwc/I");
  tree->Branch("Day_usbwc",&Day_usbwc, "Day_usbwc/I");
  tree->Branch("Hour_usbwc",&Hour_usbwc, "Hour_usbwc/I");
  tree->Branch("Minute_usbwc",&Minute_usbwc, "Minute_usbwc/I");
  tree->Branch("Second_usbwc",&Second_usbwc, "Second_usbwc/I");
  tree->Branch("Millisecond_usbwc",&Millisecond_usbwc, "Millisecond_usbwc/I");
  tree->Branch("nCh_usbwc",&nCh_usbwc, "nCh_usbwc/I");
  tree->Branch("RawTriggerRate_usbwc", RawTriggerRate_usbwc, "RawTriggerRate_usbwc[nCh_usbwc]/F");
  Int_t i,j;

  ifstream indataFileList;
  string mot;
  TString fileN;

  indataFileList.open(dataFileList.Data()); 
  assert(indataFileList.is_open());
  while (indataFileList >> mot){
    fileN = datPreff + "/" + mot;
    pFile = fopen (fileN.Data(), "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}  
    cout<<" ---> File to convert : "<<fileN<<endl;
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
    currentPositionByte = 0;
    fread (buffer,1,206,pFile);
    currentPositionByte = currentPositionByte + 206;
    //for(j = 0;j<206;j++){
    //cout<<setw(5)<<j<<setw(5)<<buffer[j]<<endl;
    //}
    buffTmp = buffer[204];
    if( buffTmp != "="){
      cout<<" ERROR --> Wrong format"<<endl
	  <<"         : buffer[204] != ="<<endl;
      cout<<std::hex<<(int)buffer[204] << '\n';
      cout<<std::dec;
      //cout<<10<< '\n';
      assert(0);
    } 
    while(currentPositionByte<totFileSizeByte){
      //for(j = 0;j<100;j++){
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
      for(i = 0;i<nCh;i++){
	currentPositionByte += fread (&floatR,1,4,pFile); RawTriggerRate[i] = floatR;
	RawTriggerRate_usbwc[i]   = RawTriggerRate[i];
      }
      //cout<<"EventNumber         "<<EventNumber<<endl;
      //cout<<"totFileSizeByte     "<<totFileSizeByte<<endl
      //<<"currentPositionByte "<<currentPositionByte<<endl;
      //for(i = 0;i<nCh;i++){
      //cout<<"RawTriggerRate[i]   "<<RawTriggerRate[i]<<endl;
      //}
      //printf ("EpochTime        %13.3f \n", EpochTime);
      //cout<<"Year         "<<Year<<endl
      //  <<"Month        "<<Month<<endl
      //  <<"Day          "<<Day<<endl
      //  <<"Hour         "<<Hour<<endl
      //  <<"Minute       "<<Minute<<endl
      //  <<"Second       "<<Second<<endl
      //  <<"Millisecond  "<<Millisecond<<endl;   
      tree->Fill();
    } 
    // terminate
    fclose (pFile);
    free (buffer);
  }
  indataFileList.close();
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
}

/*
        if(AcqParams.SaveRateRunInFile)
         {
             sprintf(line,"=== Rates [Hz] of: ");
             for(channel = 0; channel < SystemParams.NbOfFeChannels + SystemParams.NbOfDigitalChannels; channel++)
             {
                 if(SystemParams.ChannelMask[channel] == ENABLED)
                 {
                     sprintf(lineBuf,"Ch%d ", channel);
                     strcat(line, lineBuf);
                 }
             }

             if(AcqParams.EnableRateLinearCombination == TRUE)
             {
                 sprintf(lineBuf,"Combined ch");
                 strcat(line, lineBuf);
             }

             strcat(line, "===\n");
             fputs(line,FileForSavingRateRunPtr);
         }

                fwrite (&EventNumber, sizeof(int), 1, FileForSavingRateRunPtr);
                fwrite (&EpochTime, sizeof(double), 1, FileForSavingRateRunPtr);
                fwrite (&Year, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                fwrite (&Month, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                fwrite (&Day, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                fwrite (&Hour, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                fwrite (&Minute, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                fwrite (&Second, sizeof(unsigned int), 1, FileForSavingRateRunPtr);
                uint_millisecond =(unsigned int)Millisecond;
                fwrite (&uint_millisecond, sizeof(unsigned int), 1, FileForSavingRateRunPtr);

                for(samIndex = 0; samIndex < SystemParams.NbOfFeSamBlocks; samIndex++)
                {
                    for(channel = 0; channel <NB_OF_CHANNELS_IN_SAMBLOCK; channel ++)
                    {
if(SystemParams.ChannelMask[samIndex*NB_OF_CHANNELS_IN_SAMBLOCK+channel] == ENABLED)
                        {
fwrite(&RawTriggerRate[channel+2*samIndex], sizeof(float), 1, FileForSavingRateRunPtr);
                        }
                    }
                }

                if(AcqParams.EnableRateLinearCombination == TRUE)
fwrite(&RawTriggerRate[SystemParams.NbOfFeSamBlocks + SystemParams.NbOfDigitalSamBlocks], sizeof(float), 1, FileForSavingRateRunPtr);
*/
