#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <map>
#include <stdlib.h>
#include <iomanip>

using namespace std;

int getTimePos(TString inFileN){
  cout<<"inFileN  "<<inFileN<<endl;
  // Array with months as indexes
  map <string,int> month_t;
  month_t["January"]    = 1;
  month_t["February"]   = 2;
  month_t["March"]      = 3;
  month_t["April"]      = 4;
  month_t["May"]        = 5;
  month_t["June"]       = 6;
  month_t["July"]       = 7;
  month_t["August"]     = 8;
  month_t["September"]  = 9;
  month_t["October"]    = 10;
  month_t["November"]   = 11;
  month_t["December"]   = 12;
  
  // Variables for converting to unix time
  int month = 0, day = 0, year = 0, hours = 0, minutes = 0, seconds = 0;
  // Position of the motor
  double position = 0.0;
  
  // String variables to read the file
  string newline, oldline, word;
  // Open file in stream
  //ifstream infile("logfile_run10.txt");
  ifstream infile(inFileN.Data());
  
  TString outFileN = inFileN;
  outFileN += ".out";
  FILE * pFile;
  pFile = fopen (outFileN.Data(),"w");
  cout<<"outFileN "<<outFileN<<endl;

  bool status_scan   = false;
  string w_time = "-999";
  fprintf(pFile,"Position[mm]  UnixTime[sec]  Waiting[sec]\n");
  
  // Read file line by line until its end
  while(getline(infile,newline))
    {
      // Loking for Scanning mode
      size_t found_scan_start  = newline.find("Scanning mode was started");
      size_t found_scan_stop  = newline.find("Scanning mode was stopped");
      if(found_scan_start   !=  string::npos && !status_scan){status_scan = true;}
      if(!status_scan) continue;
      if(found_scan_stop   !=  string::npos && status_scan){break;}
      
      // Change symbols for more convinient work
      for(int i = 0; i < newline.length(); i++)
        {
	  if(newline[i] == ',' || newline[i] == ':') {newline[i] = ' ';}
        }
      // Find the position of the first letter of the searching string.
      // When the motor is started/stopped it is written the next: "Motor has started to move."/"Motor has been stopped." These are our keywords.
      
      // Depends on which type of translation stage do you use, you have to choose which command are you looking for.
      size_t found_start  = oldline.find("Motor has started to move.");
      // size_t found_start  = oldline.find("Motor I has started to move.");
      // size_t found_start  = oldline.find("Motor II has started to move.");
      // size_t found_start  = oldline.find("Motor I & Motor II have started to move.");
      
      size_t found_stop   = newline.find("Motor has been stopped.");
      // size_t found_stop  = newline.find("Motor I has been stopped.");
      // size_t found_stop  = newline.find("Motor II has been stopped.");
      // size_t found_stop  = newline.find("Motor I & Motor II have been stopped.");
      
      bool status_start   = false;
      bool status_stop    = false;
      string editline;
      // If its position is not equal to the maximum posible position
      // if(found_start  !=  string::npos){status_start  = true; editline = newline; printf("## Starting data ##\n");}
      if(found_stop   !=  string::npos){status_stop   = true; editline = oldline; /*printf("## Stopping data ##\n");*/}
      
      if(status_start || status_stop){
	// Put the line to the stream
	istringstream _stream(editline);
	
	// Read and convert all data
	_stream>>word;      month   = month_t[word];
	_stream>>word;      day     = atoi(word.c_str());
	_stream>>word;      year    = atoi(word.c_str());
	_stream>>word;      hours   = atoi(word.c_str());
	_stream>>word;      minutes = atoi(word.c_str());
	_stream>>word;      seconds = atoi(word.c_str());
	_stream>>word;
	// Add 12 hours if we have PM
	if(word == "PM" && hours != 12){
	  hours += 12;
	}
	if(word == "AM" && hours == 12){
	  hours = 0;
	}
	// Read position of the motor
	_stream>>word;
	
	// Depends on which type of translation stage do you use, you have to choose which command are you looking for.
	//if(word == "PosMotI")
	//if(word == "PosMotII")
	if(word == "PosMot"){
	    _stream>>word;
	    _stream>>word;
	    position = atof(word.c_str());
	}
	// Convert received date to the unix time.
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	timeinfo = localtime ( &rawtime );
	timeinfo.tm_year    = year  - 1900;
	timeinfo.tm_mon     = month - 1;
	timeinfo.tm_mday    = day;
	timeinfo.tm_hour    = hours;
	timeinfo.tm_min     = minutes;
	timeinfo.tm_sec     = seconds;
	
	time_t unixtime = mktime(&timeinfo);
	
	// Print results to the terminal
	printf("%10.6f %12d %10s\n",position,unixtime,w_time.c_str());
	fprintf(pFile,"%10.6f %12d %10s\n",position,unixtime,w_time.c_str());
      }
      
      // Find the waiting time
      bool status_time   = false;
      size_t found_time   = newline.find("Waiting time is");
      if(found_time   !=  string::npos){status_time = true;}
      if(status_time){
	  
	//cout<<newline<<endl;
	istringstream _stream(newline);
	_stream>>word;  _stream>>word;
	_stream>>word;  _stream>>word;
	_stream>>word;  _stream>>word;
	_stream>>word;  _stream>>word;
	_stream>>word;  _stream>>word;
	_stream>>word;  w_time = word;
      }
      // Copy newline to the oldline
      oldline = newline;
    }
  return 0;
}
