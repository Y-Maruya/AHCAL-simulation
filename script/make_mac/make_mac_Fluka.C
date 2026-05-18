#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <vector>
int make_mac_Fluka(){
    for( int i =0 ; i<100 ;i++){
        std::ofstream ofs(Form("./mac/run_Fluka_%d.mac",i));
        ofs<<"/control/verbose 2"<<std::endl;
        ofs<<"/run/verbose 1"<<std::endl;
        ofs<<"/event/verbose 0"<<std::endl;
        ofs<<"/tracking/verbose 0"<<std::endl;
        ofs<<"/generator/inputFileName /afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_"<<i<<".root"<<std::endl;
        ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
        TFile * file = new TFile(Form("/afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_%d.root",i),"READ");
        TTree * tree = (TTree*)file->Get("gFaser");
        int N = tree->GetEntriesFast();
        ofs<<"/run/initialize"<<std::endl;
        ofs<<"/run/beamOn "<<N<<std::endl;
    }
    return 0;
}