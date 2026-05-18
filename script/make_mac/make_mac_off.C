#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <vector>
int make_mac_off(){
    for( int i =0 ; i<10 ;i++){
        std::ofstream ofs(Form("./mac/run_off_AHCALwg_%d.mac",i));
        ofs<<"/control/verbose 2"<<std::endl;
        ofs<<"/run/verbose 1"<<std::endl;
        ofs<<"/event/verbose 0"<<std::endl;
        ofs<<"/tracking/verbose 0"<<std::endl;
        ofs<<"/generator/inputFileName /afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_off_AHCALwg_charm.1000ifb."<<i<<".gfaser.root"<<std::endl;
        ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
        ofs<<"/run/initialize"<<std::endl;
        TFile * file = new TFile(Form("/afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_off_AHCALwg_charm.1000ifb.%d.gfaser.root",i),"READ");
        TTree * tree = (TTree*)file->Get("gFaser");
        int N = tree->GetEntriesFast();
        ofs<<"/run/beamOn "<<N<<std::endl;
    }
    for( int i =10 ; i<20 ;i++){
        std::ofstream ofs(Form("./mac/run_off_AHCALwg_%d.mac",i));
        ofs<<"/control/verbose 2"<<std::endl;
        ofs<<"/run/verbose 1"<<std::endl;
        ofs<<"/event/verbose 0"<<std::endl;
        ofs<<"/tracking/verbose 0"<<std::endl;
        ofs<<"/generator/inputFileName /afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_off_AHCALwg_light.1000ifb."<<i<<".gfaser.root"<<std::endl;
        ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
        ofs<<"/run/initialize"<<std::endl;
        TFile * file = new TFile(Form("/afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_off_AHCALwg_light.1000ifb.%d.gfaser.root",i),"READ");
        TTree * tree = (TTree*)file->Get("gFaser");
        int N = tree->GetEntriesFast();
        ofs<<"/run/beamOn "<<N<<std::endl;
    }
    return 0;
}