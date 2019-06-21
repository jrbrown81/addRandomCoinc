{
   TFile *f=new TFile("500BillCapillary2Phi.root","recreate");
   TTree* t=new TTree("tree","capilary entangled sim");
   Long64_t nlines=t->ReadFile("/home/userfs/j/jb536/physhad/Medical/SimulationData/capillaryimage/capillary-edit/capillaryimage/500BillCapillary2Phi.csv",
   "annihil_N:scat1:theta1:phi1:X1_a:Y1_a:Z1_a:E1:scat2:theta2:phi2:X2_a:Y2_a:Z2_a:E2");
   cout << nlines << " lines found in file" << endl;
   t->Write();
   f->Close();
}
