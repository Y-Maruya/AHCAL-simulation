#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <vector>
int make_mac_hadron(){
    
    std::ofstream ofs("./mac/run_nbar.mac");
    ofs<<"/control/verbose 2"<<std::endl;
    ofs<<"/run/verbose 1"<<std::endl;
    ofs<<"/event/verbose 0"<<std::endl;
    ofs<<"/tracking/verbose 0"<<std::endl;
    ofs<<"/generator/inputFileName /afs/cern.ch/user/y/ymaruya/private/FASERlink/FASER_neBkg/hadron_nbar.W.100000.0.gfaser.root"<<std::endl;
    ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
    ofs<<"/run/initialize"<<std::endl;
    ofs<<"/run/beamOn 10000"<<std::endl;
    return 0;
}