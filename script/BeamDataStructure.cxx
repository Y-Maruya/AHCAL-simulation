#include "Global.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
using namespace std;

Int_t GetBin1d(int bin1,int bin2,int bin3){
    return (bin1)*42+(bin2)+(bin3)*42*5;
}
Int_t GetBin1d_AHCAL(int bin1,int bin2,int bin3){
   return (bin1)*18+(bin2)+(bin3)*18*18;
}

void BeamDataStructure(TString InputFile, TString OutputFile, Int_t ECALOption)
{
    // if (gSystem->AccessPathName(InputDir + "/" + InputFileName))
    // {
    //     std::cout << "Error! ROOT file: " << InputDir + "/" + InputFileName << " does not exist." << std::endl;
    //     return;
    // }
    //------------------------------------------------------
    Int_t EventNum;
    Int_t ftagNulabel;
	Double_t Energy_HCAL=0;
    Double_t Energy_ECAL=0;
    Double_t ParticleEnergy;
    Double_t Interaction_x;
    Double_t Interaction_y;
    Double_t Interaction_z;
    Double_t energy_deposit_SciW[16*42*5*2];
    Double_t energy_deposit_AHCAL[18*18*40];
    std::vector<Int_t> DetectorID;
    std::vector<Int_t> CellID;
    std::vector<Double_t> Hit_Energy;
    std::vector<Double_t> Hit_Time;
    std::vector<Double_t> Hit_X;
    std::vector<Double_t> Hit_Y;
    std::vector<Double_t> Hit_Z;
    // TString OutputFileName = "MC_" + InputFileName;
    // if (!gSystem->AccessPathName(OutputDir + "/" + OutputFileName))
    // {
        // std::cout << "The MC file already exists. Skipped..." << std::endl;
        // return;
    // }
    const Int_t numFiles = 1; // Number of files to split into
    Int_t fileIndex = 0;
    Int_t eventsPerFile = 0;
    Int_t totalEvents = 0;

    TFile *ConvertFile = nullptr;
    TTree *EventTree = nullptr;

    auto createNewFileAndTree = [&]() {
        if (ConvertFile) {
            ConvertFile->cd();
            EventTree->Write();
            delete EventTree;
            ConvertFile->Close();
            delete ConvertFile;
        }
        TString newOutputFile = OutputFile;
        newOutputFile.ReplaceAll(".root", Form("_part%d.root", fileIndex++));
        ConvertFile = TFile::Open(newOutputFile, "RECREATE");
        EventTree = new TTree("EventTree", "Info stored at event level");
        EventTree->Branch("EventNum", &EventNum, "EventNum/I");
        EventTree->Branch("ftagNulabel", &ftagNulabel, "ftagNulabel/I");
        EventTree->Branch("ParticleEnergy", &ParticleEnergy, "ParticleEnergy/D");
        EventTree->Branch("Interaction_x", &Interaction_x, "Interaction_x/D");
        EventTree->Branch("Interaction_y", &Interaction_y, "Interaction_y/D");
        EventTree->Branch("Interaction_z", &Interaction_z, "Interaction_z/D");
        EventTree->Branch("DetectorID", &DetectorID);
        EventTree->Branch("CellID", &CellID);
        EventTree->Branch("Energy_HCAL", &Energy_HCAL, "Energy_HCAL/D");
        EventTree->Branch("Energy_ECAL", &Energy_ECAL, "Energy_ECAL/D");
        EventTree->Branch("energy_deposit_SciW", energy_deposit_SciW, "energy_deposit_SciW[6720]/D");
        EventTree->Branch("energy_deposit_AHCAL", energy_deposit_AHCAL, "energy_deposit_AHCAL[16000]/D");
        EventTree->Branch("Hit_Energy", &Hit_Energy);
        EventTree->Branch("Hit_Time", &Hit_Time);
        EventTree->Branch("Hit_X", &Hit_X);
        EventTree->Branch("Hit_Y", &Hit_Y);
        EventTree->Branch("Hit_Z", &Hit_Z);
    };

    Int_t EvtID;
    Double_t CaloEdepSum;
    Double_t CaloVisibleEdepSum;
    Double_t EcalEdepSum;
    Double_t EcalVisibleEdepSum;
    Double_t HcalEdepSum;
    Double_t HcalVisibleEdepSum;
    Double_t EcalMaxEdepCell;
    Double_t HcalMaxEdepCell;
    std::vector<Int_t>* vecEcalCellID = nullptr;
    std::vector<Int_t>* vecEcalStepsCell = nullptr;
    std::vector<Double_t>* vecEcalEdepCell = nullptr;
    std::vector<Double_t>* vecEcalVisibleEdepCell = nullptr;
    std::vector<Double_t>* vecEcalHitTimeCell = nullptr;
    std::vector<Double_t>* vecEcalToaCell = nullptr;
    std::vector<Int_t>* vecHcalCellID = nullptr;
    std::vector<Int_t>* vecHcalStepsCell = nullptr;
    std::vector<Double_t>* vecHcalEdepCell = nullptr;
    std::vector<Double_t>* vecHcalVisibleEdepCell = nullptr;
    std::vector<Double_t>* vecHcalHitTimeCell = nullptr;
    std::vector<Double_t>* vecHcalToaCell = nullptr;
    // TChain *treeEvt = new TChain("treeEvt","");
    // for(int k=0;k<100;k++){
        // treeEvt->Add(Form("/eos/user/y/ymaruya/FASER/genie_data/faser_on.n100000.%d.gfaser_calo.root",k));
    // }
    TFile *DataFile = TFile::Open(InputFile);
    TTree *treeEvt = nullptr;
    DataFile->GetObject("treeEvt",treeEvt);
    treeEvt->SetBranchAddress("EvtID", &EvtID);
    treeEvt->SetBranchAddress("ParticleEnergy", &ParticleEnergy);
    treeEvt->SetBranchAddress("Interaction_x", &Interaction_x);
    treeEvt->SetBranchAddress("Interaction_y", &Interaction_y);
    treeEvt->SetBranchAddress("Interaction_z", &Interaction_z);
    treeEvt->SetBranchAddress("ftagNulabel", &ftagNulabel);
    treeEvt->SetBranchAddress("CaloVisibleEdepSum", &CaloVisibleEdepSum);
    treeEvt->SetBranchAddress("EcalEdepSum", &EcalEdepSum);
    treeEvt->SetBranchAddress("EcalVisibleEdepSum", &EcalVisibleEdepSum);
    treeEvt->SetBranchAddress("HcalEdepSum", &HcalEdepSum);
    treeEvt->SetBranchAddress("HcalVisibleEdepSum", &HcalVisibleEdepSum);
    treeEvt->SetBranchAddress("EcalMaxEdepCell", &HcalMaxEdepCell);
    treeEvt->SetBranchAddress("vecEcalCellID", &vecEcalCellID);
    treeEvt->SetBranchAddress("vecEcalStepsCell", &vecEcalStepsCell);
    treeEvt->SetBranchAddress("vecEcalEdepCell", &vecEcalEdepCell);
    treeEvt->SetBranchAddress("vecEcalVisibleEdepCell", &vecEcalVisibleEdepCell);
    treeEvt->SetBranchAddress("vecEcalHitTimeCell", &vecEcalHitTimeCell);
    treeEvt->SetBranchAddress("vecEcalToaCell", &vecEcalToaCell);
    treeEvt->SetBranchAddress("vecHcalCellID", &vecHcalCellID);
    treeEvt->SetBranchAddress("vecHcalStepsCell", &vecHcalStepsCell);
    treeEvt->SetBranchAddress("vecHcalEdepCell", &vecHcalEdepCell);
    treeEvt->SetBranchAddress("vecHcalVisibleEdepCell", &vecHcalVisibleEdepCell);
    treeEvt->SetBranchAddress("vecHcalHitTimeCell", &vecHcalHitTimeCell);
    treeEvt->SetBranchAddress("vecHcalToaCell", &vecHcalToaCell);
    //------------------------------------------------------
    Int_t NEvent = treeEvt->GetEntries();
    eventsPerFile = NEvent / numFiles + (NEvent % numFiles != 0); // Calculate events per file
    createNewFileAndTree();

    for (Int_t i = 0; i < NEvent; i++)
    {
        if (totalEvents >= eventsPerFile) {
            createNewFileAndTree();
            totalEvents = 0;
        }

        DetectorID.clear();
        CellID.clear();
        Hit_Energy.clear();
        Hit_Time.clear();
        Hit_X.clear();
        Hit_Y.clear();
        Hit_Z.clear();
        for(int k=0;k<16*42*5*2;k++){
            energy_deposit_SciW[k]=0;
        }
        for(int k=0;k<18*18*40;k++){
            energy_deposit_AHCAL[k]=0;
        }
        Energy_ECAL=0;
        Energy_HCAL=0;
        if(i%10000==0)cout<<i<<"  /"<<NEvent<<endl;
        treeEvt->GetEntry(i);
        EventNum = i;
        if (ECALOption == 1)
            for (auto it = vecEcalCellID->begin(); it != vecEcalCellID->end(); it++)
            {
                Int_t ID_X = *it % 100;
                Int_t ID_Y = *it % 10000 / 100;
                Int_t ID_Z = *it / 10000;
                if (ID_Z % 2 == 0)
                    std::swap(ID_X, ID_Y);
                double tmp_energy=vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it));
                energy_deposit_SciW[GetBin1d(ID_X-1,ID_Y-1,ID_Z-1)] += tmp_energy;
                Int_t ChipID = (ID_Y - 1) / 7;
                Int_t MemoID = 0;
                Int_t ChannelID = (ID_Y - 1) % 7 * 5 + (ID_X - 1);
                DetectorID.push_back(0);
                Energy_ECAL+= tmp_energy;
                CellID.push_back((ID_Z - 1) * 1e5 + ChipID * 1e4 + MemoID * 1e2 + ChannelID);
                Hit_Energy.push_back(tmp_energy);
                Hit_Time.push_back(vecEcalToaCell->at(std::distance(vecEcalCellID->begin(), it)));
                if (ID_Z % 2 == 0)
                {
                    Hit_X.push_back(5.3 * (20.5 - ID_Y + 1));
                    Hit_Y.push_back(45.3 * (2 - ID_X + 1));
                    Hit_Z.push_back((ID_Z / 2 - 1) * 19.9 + 11.2 + 3500 + 4.25); // + 1.25
                }
                else
                {
                    Hit_X.push_back(45.3 * (2 - ID_X + 1));
                    Hit_Y.push_back(5.3 * (20.5 - ID_Y + 1));
                    Hit_Z.push_back((ID_Z - 1) / 2 * 19.9 + 3500 + 4.25);
                }
            }
        for (auto it = vecHcalCellID->begin(); it != vecHcalCellID->end(); it++)
        {
            Int_t ID_X = *it % 100;
            Int_t ID_Y = *it % 10000 / 100;
            Int_t ID_Z = *it / 10000;
            Int_t ChipID = (ID_X - 1) / 6 + (ID_Y - 1) / 6 * 3;
            Int_t MemoID = 0;
            Int_t ChannelID = (ID_X - 1) % 6 + (ID_Y - 1) % 6 * 6;
            DetectorID.push_back(1);

			double tmp_energy=vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it));
			// double tmp_energy=vecHcalEdepCell->at(std::distance(vecHcalCellID->begin(), it));
            // Hit_Energy.push_back(vecHcalEdepCell->at(std::distance(vecHcalCellID->begin(), it)));
            energy_deposit_AHCAL[GetBin1d_AHCAL(ID_X-1,ID_Y-1,ID_Z-1)] += tmp_energy;
            Energy_HCAL+=tmp_energy;
			Hit_Energy.push_back(tmp_energy);
            Hit_Time.push_back(vecHcalToaCell->at(std::distance(vecHcalCellID->begin(), it)));
            Hit_X.push_back(40.3 * (9 - 0.5 - ID_X + 1));
            Hit_Y.push_back(40.3 * (9 - 0.5 - ID_Y + 1));
			inverse(40.3 * (9 - 0.5 - ID_X + 1),40.3 * (9 - 0.5 - ID_Y + 1),ChipID,ChannelID);
            CellID.push_back((ID_Z - 1) * 1e5 + ChipID * 1e4 + MemoID * 1e2 + ChannelID);
            if (ECALOption == 1 )
                Hit_Z.push_back((ID_Z - 1)* 29.63+4483.965 ); // 1.75 for 1/2 HCAL PS, 2 for HCAL cover, 1.25 for 1/2 ECAL PS
            else
                Hit_Z.push_back((ID_Z - 1) * 30.);
        }
        EventTree->Fill();
        totalEvents++;
    }

    // Finalize the last file
    if (ConvertFile) {
        ConvertFile->cd();
        EventTree->Write();
        delete EventTree;
        ConvertFile->Close();
        delete ConvertFile;
    }

    cout << "transform done" << endl;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: ./BeamDataStructure inputname outputname ECALOption" << std::endl;
        return 1;
    }
    TString inputname = argv[1];
    TString outputname = argv[2];
    Int_t ECALOption = std::stoi(argv[3]);
    BeamDataStructure(inputname, outputname, ECALOption);
    return 0;
}