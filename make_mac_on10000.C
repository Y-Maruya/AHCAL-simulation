#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <vector>
int make_mac_on10000(){
    for( int i =0 ; i<1000 ;i++){
        std::ofstream ofs(Form("./mac/run_%d.mac",i));
        ofs<<"/control/verbose 2"<<std::endl;
        ofs<<"/run/verbose 1"<<std::endl;
        ofs<<"/event/verbose 0"<<std::endl;
        ofs<<"/tracking/verbose 0"<<std::endl;
        ofs<<"/generator/inputFileName /afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_onSciW_n10000."<<i<<".gfaser.root"<<std::endl;
        ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
        ofs<<"/run/initialize"<<std::endl;
        ofs<<"/run/beamOn 10000"<<std::endl;
    }
    return 0;
}