#define addRandoms_cxx
#include "addRandoms.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

Double_t GetDeltaPhi(Double_t phi1, Double_t phi2);

void addRandoms::Loop()
{
//   In a ROOT session, you can do:
//      root> .L addRandoms.C
//      root> addRandoms t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   
   Int_t nLines=(Int_t)nentries;
   Double_t activity = 50e6; //Bequerels
//   Double_t activity = 50e2; //Bequerels
   Double_t runTime; // seconds
   Double_t nDecays = 4e12; // number of decays simulated.
   Double_t tWindow = 1e-6; // coincidence window, seconds
   Double_t detProb = 0.31;
   Double_t dt=0;
   Int_t nPileUp=0;
   Double_t prob1, prob2;

   TRandom* myRand=new TRandom3();
//   runTime=nDecays/activity; // seconds. These should be number of simulated events, not no. detected!
   runTime=nentries/activity; // seconds. These should be number of simulated events, not no. detected!
   
   cout << "Activity: " << activity/1e6 << " MBq" << endl
         << "No. events simulated: " << nDecays << endl
         << "No. coincidences in input file: " << nentries << endl
         << "Run Time: " << runTime*1000 << " ms" <<endl
         << "Probability of detecting a pair of hits: " << detProb*100 << "%" << endl;
   
   Double_t timeStamp[nLines]; // nanoseconds
   Int_t ind[nLines];
   
   TH1F* time_h=new TH1F("time_h","Time",50000,0,1.2*runTime); // in seconds
   time_h->SetXTitle("time, s");
//   TH1F* dt_h=new TH1F("dt_h","#Delta t",50000,0,100*activity/1e9); // in nanoseconds
   TH1F* dt_h=new TH1F("dt_h","#Delta t",50000,0,10/activity); // in seconds
   dt_h->SetXTitle("#Delta t, s");

// Create random times for each event
   for(int i=0; i<nLines; i++) {
      timeStamp[i]=myRand->Uniform(runTime);
      time_h->Fill(timeStamp[i]);
   }
// Sort times in assending order
   TMath::Sort(nLines,timeStamp,ind,kFALSE);

   // create output file
   ofstream outFile;
   outFile.open("testOut.csv");

// temp storage for two coinc events
   Double_t data[2][2][10];
   Int_t pairCounter=0;
   
// Main loop here
   Long64_t nbytes = 0, nb = 0;
//   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   for (Long64_t jentry=0; jentry<nentries-1;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      data[0][0][0]=theta1*180/TMath::Pi();
      data[0][0][1]=Z1_a;
      data[0][0][2]=X1_a;
      data[0][0][3]=Y1_a;
      data[0][0][4]=E1_a;
      data[0][0][5]=E1_b;
      data[0][0][6]=phi1;
      data[0][1][0]=theta2*180/TMath::Pi();
      data[0][1][1]=Z2_a;
      data[0][1][2]=X2_a;
      data[0][1][3]=Y2_a;
      data[0][1][4]=E2_a;
      data[0][1][5]=E2_b;
      data[0][1][6]=phi2;
      fChain->GetEntry(jentry+1);
      data[1][0][0]=theta1*180/TMath::Pi();
      data[1][0][1]=Z1_a;
      data[1][0][2]=X1_a;
      data[1][0][3]=Y1_a;
      data[1][0][4]=E1_a;
      data[1][0][5]=E1_b;
      data[1][0][6]=phi1;
      data[1][1][0]=theta2*180/TMath::Pi();
      data[1][1][1]=Z2_a;
      data[1][1][2]=X2_a;
      data[1][1][3]=Y2_a;
      data[1][1][4]=E2_a;
      data[1][1][5]=E2_b;
      data[1][1][6]=phi2;

      dt = timeStamp[ind[jentry+1]] - timeStamp[ind[jentry]];
      dt_h->Fill(dt);
      
      // First event
      for(int head=0; head<2; head++) {
         for(int val=0; val<4; val++) outFile << data[0][head][val] << " ";
      outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
      pairCounter++;
      
      if(dt<tWindow){
         jentry++; // pile-up! so skip next entry (will be included with previous entry)
         nPileUp++;
         prob1=myRand->Uniform(1);
         prob2=myRand->Uniform(1);
         if(prob1<detProb) {  // first hit detected
            // write all combinations of first event and first hit
//            // First event
//            for(int head=0; head<2; head++) for(int val=0; val<10; val++) outFile << data[0][head][val] << " ";
//            outFile << endl;
            // First event, hit1, 2nd event hit 1
            for(int val=0; val<4; val++) outFile << data[0][0][val] << " ";
            for(int val=0; val<4; val++) outFile << data[1][0][val] << " ";
            outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
            pairCounter++;
            // First event, hit2, 2nd event hit 1
            for(int val=0; val<4; val++) outFile << data[0][1][val] << " ";
            for(int val=0; val<4; val++) outFile << data[1][0][val] << " ";
            outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
            pairCounter++;
            if(prob2<detProb) { // second hit detected
               // write all combinations of first event and second hit
               // First event, hit1, 2nd event hit 2
               for(int val=0; val<4; val++) outFile << data[0][0][val] << " ";
               for(int val=0; val<4; val++) outFile << data[1][1][val] << " ";
               outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
               pairCounter++;
               // First event, hit2, 2nd event hit 2
               for(int val=0; val<4; val++) outFile << data[0][1][val] << " ";
               for(int val=0; val<4; val++) outFile << data[1][1][val] << " ";
               outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
               pairCounter++;
               // write second event
               for(int head=0; head<2; head++) for(int val=0; val<4; val++) outFile << data[1][head][val] << " ";
               outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
               pairCounter++;
            }
         }
         else if(prob2<detProb) { // second hit detected
            // write all combinations of first event and second hit
//            // First event
//            for(int head=0; head<2; head++) for(int val=0; val<10; val++) outFile << data[0][head][val] << " ";
//            outFile << endl;
            // First event, hit1, 2nd event hit 2
            for(int val=0; val<4; val++) outFile << data[0][0][val] << " ";
            for(int val=0; val<4; val++) outFile << data[1][1][val] << " ";
            outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
            pairCounter++;
            // First event, hit2, 2nd event hit 2
            for(int val=0; val<4; val++) outFile << data[0][1][val] << " ";
            for(int val=0; val<4; val++) outFile << data[1][1][val] << " ";
            outFile << data[0][0][4]+data[0][1][4] << " " << GetDeltaPhi(data[0][0][6],data[0][1][6]) << endl;
            pairCounter++;
         }
      }
   }
   outFile.close();

   cout << "Coincidence window: " << tWindow*1e6 << " us" << endl
         << nPileUp << " pile up events" << endl
         << pairCounter << " pairs of hits written to output file" << endl
         << (float)nPileUp/(float)pairCounter*100 << "% of events are pile-up" << endl;

// Draw test histograms
   TCanvas* c1=new TCanvas();
   c1->Divide(1,2);
   c1->cd(1);
   time_h->Draw();
   c1->cd(2);
   dt_h->Draw();
   
}

Double_t GetDeltaPhi(Double_t phi1, Double_t phi2)
{
      Double_t deltaPhi = (phi1 + phi2)*180/TMath::Pi();
      if (deltaPhi > 180) deltaPhi -= 360;
      if (deltaPhi < -180) deltaPhi += 360;
   
      return deltaPhi;
}

//      data[0][0][1]=theta1; // swapped with phi so can loop over more easily
//      data[0][0][0]=phi1;
//      data[0][0][2]=E1_a;
//      data[0][0][3]=X1_a;
//      data[0][0][4]=Y1_a;
//      data[0][0][5]=Z1_a;
//      data[0][0][6]=E1_b;
//      data[0][0][7]=X1_b;
//      data[0][0][8]=Y1_b;
//      data[0][0][9]=Z1_b;
//      data[0][1][0]=theta2;
//      data[0][1][1]=phi2;
//      data[0][1][2]=E2_a;
//      data[0][1][3]=X2_a;
//      data[0][1][4]=Y2_a;
//      data[0][1][5]=Z2_a;
//      data[0][1][6]=E2_b;
//      data[0][1][7]=X2_b;
//      data[0][1][8]=Y2_b;
//      data[0][1][9]=Z2_b;
//      fChain->GetEntry(jentry+1);
//      data[1][0][1]=theta1;
//      data[1][0][0]=phi1;
//      data[1][0][2]=E1_a;
//      data[1][0][3]=X1_a;
//      data[1][0][4]=Y1_a;
//      data[1][0][5]=Z1_a;
//      data[1][0][6]=E1_b;
//      data[1][0][7]=X1_b;
//      data[1][0][8]=Y1_b;
//      data[1][0][9]=Z1_b;
//      data[1][1][0]=theta2;
//      data[1][1][1]=phi2;
//      data[1][1][2]=E2_a;
//      data[1][1][3]=X2_a;
//      data[1][1][4]=Y2_a;
//      data[1][1][5]=Z2_a;
//      data[1][1][6]=E2_b;
//      data[1][1][7]=X2_b;
//      data[1][1][8]=Y2_b;
//      data[1][1][9]=Z2_b;
