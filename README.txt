Root code to read in simulated data of pairs of gamma detections, and output 
the same data with pile-up, dependent on specified parameters, i.e. 
activity, 
coincidence window, 
geometric efficieny (calculated based on detector geometry),
detection probability.

These are all specified close to the top of addRandoms.C

The file to read in is a ROOT file: 
/shared/storage/physhad/Medical/jb536/simulations/capillaryimage/500BillCapillary2Phi.root
created from: 
/shared/storage/physhad/Medical/SimulationData/capillaryimage/capillary-edit/capillaryimage/500BillCapillary2Phi.csv

The format of this file is:
annihil_N, scat1, theta1, phi1, X1_a, Y1_a, Z1_a, E1, scat2, theta2, phi2, X2_a, Y2_a, Z2_a, E2

The root file was created using simsCSV2ROOT.C. To run addRandoms on on a different file (probably shouldn't need to 
do this), first create the root file using simsCSV2ROOT.C (change the input (.csv) and output (.root) file names). 
Then change the name and path in addRandoms.h (in two places!) to point to your new .root file.

///////////////////////
// The important bit //
///////////////////////

The code can be run (using ROOT/6.16.0, and probably other versions, though not tested) by running the following 
three commands within a root session.
   
      root> .L addRandoms.C
      root> addRandoms t
      root> t.Loop();       // to loop on all entries
or
      root> t.Loop(1000000);       // to loop on only 1000000 entries, for example

The code prints some settings to screen and reports it's progress. If the event rate is very low, a warning is 
issued as the code may not produce many pile-up events, and may take a very long time to run so may need to be
killed (ctrl-c).

The code will produce some plots in a pdf as a check that the distributions (of N gammas detected within the time 
window, etc) are reasonable, as well as a log file detailing the settings

The main output is a text file containing the original gamma events, but piled-up in all possible combinations, 
according to the probabilities detecting N gammas within the time window.

The output file format is (all in degress or mm):
theta1 Z1 X1 Y1 theta2 Z2 X2 Y2 deltaPhi

Note that output files will be overwritten if you run the code again so change file names after code finishes running 
(.txt, .log, and .pdf) so we have a record of what was run.
