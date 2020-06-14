{
   gROOT->Reset();
   TFile f("./L1/Run1_Ch1_07.06.2015_s0.sig.root_L1.root");

   //tree.Draw("sqrt(x)>>+hsqrt","y>0");
   TH1D* h1_leading_edge = new TH1D("h1_leading_edge","leading edge",1000,0.0,1100000000);
   AnaSigTree.Draw("leading_edge>>h1_leading_edge");

   /*
//   Create an histogram
   TH1F *hnseg = new TH1F("hnseg","Number of segments for selected tracks",5000,0,5000);

//   Start main loop on all events
   //TClonesArray *tracks = 0;
   Event *event = new Event();   //object must be created before setting the branch address

   TBranch *leading_edge = AnaSigTree.GetBranch("leading_edge");
   //   TBranch *branch  = AnaSigTree.GetBranch("trailing_edge");
   leading_edge->SetAddress(&event);
   Int_t nevent = AnaSigTree.GetEntries();
   Int_t nselected = 0;
   for (Int_t i=0;i<nevent;i++) {
      leading_edge->GetEvent(i);
      AnaSigTree.GetEvent(i);                  //read complete accepted event in memory
      hnseg->Fill(event->GetNseg());  //Fill histogram with number of segments
      tracks = event->GetTracks();    //get pointer to the TClonesArray object
      tracks->Clear();                //clear it
   }
//  Delete ClonesArray and histogram objects
   event->Finish();

//  Draw the histogram
   hnseg->Draw();
   */
}
