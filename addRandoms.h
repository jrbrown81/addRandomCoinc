//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 20 18:13:24 2019 by ROOT version 6.16/00
// from TTree tree/capilary entangled sim
// found on file: 500BillCapillary2Phi.root
//////////////////////////////////////////////////////////

#ifndef addRandoms_h
#define addRandoms_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class addRandoms {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         annihil_N;
   Float_t         scat1;
   Float_t         theta1;
   Float_t         phi1;
   Float_t         X1_a;
   Float_t         Y1_a;
   Float_t         Z1_a;
   Float_t         E1;
   Float_t         scat2;
   Float_t         theta2;
   Float_t         phi2;
   Float_t         X2_a;
   Float_t         Y2_a;
   Float_t         Z2_a;
   Float_t         E2;

   // List of branches
   TBranch        *b_annihil_N;   //!
   TBranch        *b_scat1;   //!
   TBranch        *b_theta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_X1_a;   //!
   TBranch        *b_Y1_a;   //!
   TBranch        *b_Z1_a;   //!
   TBranch        *b_E1;   //!
   TBranch        *b_scat2;   //!
   TBranch        *b_theta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_X2_a;   //!
   TBranch        *b_Y2_a;   //!
   TBranch        *b_Z2_a;   //!
   TBranch        *b_E2;   //!

   addRandoms(TTree *tree=0);
   virtual ~addRandoms();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Long64_t toProcess=0);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef addRandoms_cxx
addRandoms::addRandoms(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("500BillCapillary2Phi.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("500BillCapillary2Phi.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

addRandoms::~addRandoms()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t addRandoms::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t addRandoms::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void addRandoms::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("annihil_N", &annihil_N, &b_annihil_N);
   fChain->SetBranchAddress("scat1", &scat1, &b_scat1);
   fChain->SetBranchAddress("theta1", &theta1, &b_theta1);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("X1_a", &X1_a, &b_X1_a);
   fChain->SetBranchAddress("Y1_a", &Y1_a, &b_Y1_a);
   fChain->SetBranchAddress("Z1_a", &Z1_a, &b_Z1_a);
   fChain->SetBranchAddress("E1", &E1, &b_E1);
   fChain->SetBranchAddress("scat2", &scat2, &b_scat2);
   fChain->SetBranchAddress("theta2", &theta2, &b_theta2);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("X2_a", &X2_a, &b_X2_a);
   fChain->SetBranchAddress("Y2_a", &Y2_a, &b_Y2_a);
   fChain->SetBranchAddress("Z2_a", &Z2_a, &b_Z2_a);
   fChain->SetBranchAddress("E2", &E2, &b_E2);
   Notify();
}

Bool_t addRandoms::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void addRandoms::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t addRandoms::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef addRandoms_cxx
