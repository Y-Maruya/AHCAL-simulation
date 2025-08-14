#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <vector>
int make_mac_off_model_Gx(){
    std::string gx[3] = {"Gx600_run2026","Gx650_run2026","Gx700_run2026"};
    std::string model_light[3] = {"EPOSLHC","SIBYLL","QGSJET"};
    std::string model_charm[3] = {"central","max","min"};
    for (int k = 0; k<3;k++){
        for (int j = 0; j<3;j++){
            for( int i =0 ; i<10 ;i++){
                std::ofstream ofs(Form("./mac/run_off_AHCAL_%s_%s_%d.mac",gx[k].c_str(),model_charm[j].c_str(),i));
                ofs<<"/control/verbose 2"<<std::endl;
                ofs<<"/run/verbose 1"<<std::endl;
                ofs<<"/event/verbose 0"<<std::endl;
                ofs<<"/tracking/verbose 0"<<std::endl;
                ofs<<"/generator/inputFileName /eos/user/y/ymaruya/FASER/genie_data/faser_AHCAL_"<<gx[k]<<"_charm."<<model_charm[j]<<".1000ifb."<<i<<".gfaser.root"<<std::endl;
                ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
                ofs<<"/run/initialize"<<std::endl;
                TFile * file = new TFile(Form("/eos/user/y/ymaruya/FASER/genie_data/faser_AHCAL_%s_charm.%s.1000ifb.%d.gfaser.root",gx[k].c_str(),model_charm[j].c_str(),i),"READ");
                TTree * tree = (TTree*)file->Get("gFaser");
                int N = tree->GetEntriesFast();
                ofs<<"/run/beamOn "<<N<<std::endl;
            }
            std::ofstream ofs1(Form("./script/submit_off_AHCAL_%s_%s.sdf",gx[k].c_str(),model_charm[j].c_str()));
            ofs1<< "Universe = Vanilla" << std::endl;
            ofs1<< "Getenv = True" << std::endl;
            ofs1<< "Executable = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/bin/SimCalModule_"<<gx[k]<<std::endl;
            ofs1<< "Arguments = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/mac/run_off_AHCAL_"<<gx[k]<<"_"<<model_charm[j]<<"_$(Process).mac" << std::endl;
            ofs1<< "+JobFlavour = \"testmatch\"" << std::endl;
            ofs1<< "Output = out/run_off_AHCAL_"<< gx[k]<<"_"<<model_charm[j]<<"_$(Process).out" << std::endl;
            ofs1<< "Error = err/run_off_AHCAL_"<< gx[k]<<"_"<<model_charm[j]<<"_$(Process).err" << std::endl;
            ofs1<< "Log = log/run_off_AHCAL_"<< gx[k]<<"_"<<model_charm[j]<<"_$(Process).log" << std::endl; 
            ofs1<< "Queue 10" << std::endl;
        }
        for (int j = 0; j<3;j++){
            for( int i =10 ; i<20 ;i++){
                std::ofstream ofs(Form("./mac/run_off_AHCAL_%s_%s_%d.mac",gx[k].c_str(),model_light[j].c_str(),i));
                ofs<<"/control/verbose 2"<<std::endl;
                ofs<<"/run/verbose 1"<<std::endl;
                ofs<<"/event/verbose 0"<<std::endl;
                ofs<<"/tracking/verbose 0"<<std::endl;
                ofs<<"/generator/inputFileName /eos/user/y/ymaruya/FASER/genie_data/faser_AHCAL_"<<gx[k]<<"_light."<<model_light[j]<<".1000ifb."<<i<<".gfaser.root"<<std::endl;
                ofs<<"/generator/FixedPrimaryVertexPosition false"<<std::endl;
                ofs<<"/run/initialize"<<std::endl;
                TFile * file = new TFile(Form("/eos/user/y/ymaruya/FASER/genie_data/faser_AHCAL_%s_light.%s.1000ifb.%d.gfaser.root",gx[k].c_str(),model_light[j].c_str(),i),"READ");
                TTree * tree = (TTree*)file->Get("gFaser");
                int N = tree->GetEntriesFast();
                ofs<<"/run/beamOn "<<N<<std::endl;
            }
            std::ofstream ofs1(Form("./script/submit_off_AHCAL_%s_%s.sdf",gx[k].c_str(),model_light[j].c_str()));
            ofs1<< "Universe = Vanilla" << std::endl;
            ofs1<< "Getenv = True" << std::endl;
            ofs1<< "Avalie = $(Process)+10" << std::endl;
            ofs1<< "Jobid = $INT(Avalie)" << std::endl;
            ofs1<< "Executable = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/bin/SimCalModule_"<<gx[k]<<std::endl;
            ofs1<< "Arguments = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/mac/run_off_AHCAL_"<<gx[k]<<"_"<<model_light[j]<<"_$(Jobid).mac" << std::endl;
            ofs1<< "+JobFlavour = \"testmatch\"" << std::endl;
            ofs1<< "Output = out/run_off_AHCAL_"<< gx[k]<<"_"<<model_light[j]<<"_$(Jobid).out" << std::endl;
            ofs1<< "Error = err/run_off_AHCAL_"<< gx[k]<<"_"<<model_light[j]<<"_$(Jobid).err" << std::endl;
            ofs1<< "Log = log/run_off_AHCAL_"<< gx[k]<<"_"<<model_light[j]<<"_$(Jobid).log" << std::endl;
            ofs1<< "Queue 10" << std::endl;
        }
    }
    return 0;
}