#define addRandoms_cxx
#include "addRandoms.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>

Double_t GetDeltaPhi(Double_t phi1, Double_t phi2);

void addRandoms::Loop(Long64_t toProcess=0)
{
// Specify filename to be processed in header file (in two places!)
//   In a ROOT session, you can do:
//      root> .L addRandoms.C
//      root> addRandoms t
//      root> t.Loop();       // Loop on all entries
//      root> t.Loop(1000000);       // e.g. Loop on 1000000 entries

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   
   if(toProcess==0) toProcess=nentries;
   Double_t activity = 50e6; //Bequerels
   Double_t crystalSize = 10; //mm
   Double_t ringRadius = 60; //mm
   Double_t nCrystals = 35; // number of crystals in ring
   Double_t geomEff = nCrystals*pow(crystalSize,2)/(4*TMath::Pi()*pow(ringRadius,2));
   Double_t act_eff = activity*geomEff; // effective activity incident on detectors
   Double_t tWindow = 1e-6; // coincidence window, seconds
   Double_t detProb = 0.31;

   std::cout << std::endl << "----------------------------------------" << std::endl
         << "Input file contains " << nentries << " entries" << std::endl
         << "Processing " << toProcess << " entries" << std::endl
         << "----------------------------------------" << std::endl;

   std::cout << std::endl << "----------------------------------------" << std::endl
         << "Parameters used for pile-up calculations" << std::endl
         << "----------------------------------------" << std::endl
         << "Activity: " << activity/1e6 << " MBq" << std::endl
         << "Geometeric efficiency: " << geomEff*100 << "%" << std::endl
         << "Effective activity: " << act_eff/1e6 << " MBq" << std::endl
         << "Time window: " << tWindow/1e-6 << " us" << std::endl
         << "Mean decays per window: " << act_eff*tWindow << std::endl
         << "Detection probability: " << detProb*100 << "%" << std::endl << std::endl;
   
   if(act_eff*tWindow<1)
      std::cout << "----------------------------------------------" << std::endl
            << "WARNING: Mean decays per window is less than 1." << std::endl
            << "Expect very little pile-up. This may take a " << std::endl
            << "LONG time to run. Consider killing program!" << std::endl
            << "----------------------------------------------" << std::endl << std::endl;
   
   Bool_t detectArray[100]; // 1 gamma was detected, 0 wasn't
   for(int i=0; i<100; i++) detectArray[i]=0;
   Double_t rateProb=0;
   
   Double_t rateProbArray[100];
   rateProbArray[0]=rateProbArray[0]=pow(act_eff*tWindow,0)*exp(-act_eff*tWindow)/(TMath::Factorial(0));
   std::cout << "------------------------------------------------" << std::endl
         << "Detection probability thresholds (upto 0.999999)" << std::endl
         << "------------------------------------------------" << std::endl;
   std::cout << "n  P(n)" << std::endl;
   for(int n=1; n<100; n++) {
      rateProbArray[n]=pow(act_eff*tWindow,n)*exp(-act_eff*tWindow)/(TMath::Factorial(n)) + rateProbArray[n-1];
      if(rateProbArray[n]<0.999999) std::cout <<  n << "   " << rateProbArray[n] << std::endl;
   }
   std::cout << std::endl;
   
   TH1F* nDecays_h = new TH1F("nDecays_h","Number of incident decays per time window",100,0,100);
   TH1F* nGammas_h = new TH1F("nGammas_h","Number of detected gammas per time window",100,0,100);
   TH1F* nOutput_h = new TH1F("nOutput_h","Number of gammas written to output per time window",100,0,100);
   
   TRandom* myRand=new TRandom3();

   // create output file
   ofstream outFile;
   outFile.open("addRandoms_out.txt");

   Double_t gammaData[100][6];
   
   Long64_t nbytes = 0;
   Long64_t jentry=0;
   Long64_t loopCounter=0;
   Long64_t nLines=0;
// Main loop here
   while(jentry<toProcess) {
      // randomly determine number of decays to consider
      rateProb=myRand->Uniform(1);
//      std::cout << "Rate Probability is: " << rateProb << std::endl;
      int nDecays=0; // number of decays hitting the detector ring in time window
      while(rateProb>rateProbArray[nDecays]) nDecays++;
      nDecays_h->Fill(nDecays);
//      std::cout << nDecays << " decays in time window" << std::endl;
      
      // randomly determine if each gamma is detected
      int nGammas=0;
      for(int i=0; i<100; i++) detectArray[i]=0;
      for(int decay=0; decay<nDecays; decay++) {
         nbytes = fChain->GetEntry(jentry); // Get a new line for each decay
         // test if first gamma is detected and read in
         if(nbytes<=0) break;
         if(myRand->Uniform(1)<detProb) {
            detectArray[decay]=1;
            gammaData[nGammas][0]=theta1;
            gammaData[nGammas][1]=Z1_a;
            gammaData[nGammas][2]=X1_a;
            gammaData[nGammas][3]=Y1_a;
            gammaData[nGammas][4]=phi1;
            gammaData[nGammas][5]=E1;
            nGammas++;
         }
         // test if second gamma is detected and read in
         if(myRand->Uniform(1)<detProb) {
            detectArray[decay]=1;
            gammaData[nGammas][0]=theta2;
            gammaData[nGammas][1]=Z2_a;
            gammaData[nGammas][2]=X2_a;
            gammaData[nGammas][3]=Y2_a;
            gammaData[nGammas][4]=phi2;
            gammaData[nGammas][5]=E2;
            nGammas++;
         }
         if(nGammas>1 && detectArray[decay]==1) {
            jentry++;
            if(jentry!=0 && jentry%1000000==0) std::cout << jentry << " entries processed" << std::endl;
            nbytes=0;
         }
      }
      nGammas_h->Fill(nGammas);
      if(nGammas>1) nOutput_h->Fill(nGammas);
//      std::cout << nGammas << " gammas detected in time window" << std::endl;
      
      for(int gamma1=0; gamma1<nGammas-1; gamma1++) {
         for(int gamma2=gamma1+1; gamma2<nGammas; gamma2++) {
            for(int val=0; val<4; val++) outFile << gammaData[gamma1][val] << " ";
            for(int val=0; val<4; val++) outFile << gammaData[gamma2][val] << " ";
            outFile << GetDeltaPhi(gammaData[gamma1][4],gammaData[gamma2][4]) << std::endl;
            nLines++;
         }
      }
      loopCounter++;
   } // end of loop over entries
   outFile.close();

   std::cout << "----------------------------------------" << std::endl
         << jentry << " entries processed" << std::endl
         << nLines << " output lines (i.e. pairs) written" << std::endl
         << "----------------------------------------" << std::endl;

// Draw test histograms
   TCanvas* c1=new TCanvas("c1","addRandoms");
   c1->Divide(1,3);
   c1->cd(1);
   nDecays_h->Draw();
   c1->cd(2);
   nGammas_h->Draw();
   c1->cd(3);
   nOutput_h->Draw();
   c1->SaveAs("addRandoms.pdf");
   
   // create log file
   outFile.open("addRandoms.log");
   outFile << "Activity: " << activity/1e6 << " MBq" << std::endl
         << "Geometeric efficiency: " << geomEff*100 << "%" << std::endl
         << "(based on " << nCrystals << ", " <<  crystalSize
            << " mm crystals, in a " << ringRadius << " mm ring)" << std::endl
         << "Effective activity: " << act_eff/1e6 << " MBq" << std::endl
         << "Time window: " << tWindow/1e-6 << " us" << std::endl
         << "Mean decays per window: " << act_eff*tWindow << std::endl
         << "Detection probability: " << detProb*100 << "%" << std::endl
         << std::endl << "Entries present in input file: " << nentries << std::endl
         << toProcess << " entries processed" << std::endl
         << nLines << " output lines (i.e. pairs) written" << std::endl
         << std::endl << "Detection probability thresholds" << std::endl
         << "n  P(n)" << std::endl;
   for(int n=0; n<100; n++) outFile <<  n << "   " << rateProbArray[n] << std::endl;
   outFile.close();
  
} // End of Loop()

Double_t GetDeltaPhi(Double_t phi1, Double_t phi2)
{
      Double_t deltaPhi = phi1 + phi2;
      if (deltaPhi > 180) deltaPhi -= 360;
      if (deltaPhi < -180) deltaPhi += 360;
   
      return deltaPhi;
}
