#include "Global.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
using namespace std;


void BeamDataStructure(TString InputFile, TString OutputFile, Int_t ECALOption, Int_t min_event, Int_t max_event)
{
    // if (gSystem->AccessPathName(InputDir + "/" + InputFileName))
    // {
    //     std::cout << "Error! ROOT file: " << InputDir + "/" + InputFileName << " does not exist." << std::endl;
    //     return;
    // }
    //------------------------------------------------------
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kRainBow);
    Int_t EventNum;
	double total_Energy=0;
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
    TFile *ConvertFile = TFile::Open(OutputFile, "RECREATE");
    //------------------------------------------------------
    Int_t EvtID;
    Double_t ParticleEnergy;
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
    TFile *DataFile = TFile::Open(InputFile);
    TTree *treeEvt = nullptr;
    DataFile->GetObject("treeEvt",treeEvt);
    treeEvt->SetBranchAddress("EvtID", &EvtID);
    treeEvt->SetBranchAddress("ParticleEnergy", &ParticleEnergy);
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
    treeEvt->Draw(">>elist",Form("EvtID>=%d && EvtID<%d",min_event,max_event));
    TEventList *elist = (TEventList*)gDirectory->Get("elist");
    //------------------------------------------------------
    Int_t NEvent = elist->GetN();
    for (Int_t i = 0; i < NEvent; i++)
    {
        DetectorID.clear();
        CellID.clear();
        Hit_Energy.clear();
        Hit_Time.clear();
        Hit_X.clear();
        Hit_Y.clear();
        Hit_Z.clear();
		total_Energy=0;
        if(i%10000==0)cout<<i<<"  /"<<NEvent<<endl;
        treeEvt->GetEntry(elist->GetEntry(i));
        EventNum = i;
        // TH3D *totalhist_hcal = new TH3D(Form("totalhist_hcal_%d",EventID),"totalhist_hcal;x[mm];y[mm];z[mm]",20,-400,400,20,-400,400,400,3800,5000);
        TH2Poly *ecal_zx = new TH2Poly();
        ecal_zx->SetName(Form("zx_%d",EventID));
        ecal_zx->SetTitle("zx;z[mm];x[mm]");
        TH2Poly *ecal_zy = new TH2Poly();
        ecal_zy->SetName(Form("zy_%d",EventID));
        ecal_zy->SetTitle("zy;z[mm];y[mm]");
        for(int i=0;i<16;i++){
            for(int j=0;j<5;j++){
                ecal_zx->AddBin(3500+4.25-1+19.9*i,(j-2)*45.3-22.5,3500+4.25+1+19.9*i,(j-2)*45.3+22.5);
                ecal_zy->AddBin(3500+15.45-1+19.9*i,(j-2)*45.3-22.5,3500+15.45+1+19.9*i,(j-2)*45.3+22.5);
            }
            for(int j =0;j<42;j++){
                ecal_zx->AddBin(3500+15.45-1+19.9*i,(j-20.5)*5.3-2.5,3500+15.45+1+19.9*i,(j-20.5)*5.3+2.5);
                ecal_zy->AddBin(3500+4.25-1+19.9*i,(j-20.5)*5.3-2.5,3500+4.25+1+19.9*i,(j-20.5)*5.3+2.5);
            }
        }
        for(int i=0;i<40;i++){
            for(int j=0;j<18;j++){
                ecal_zx->AddBin(4483.965-1.5-29.63*i,(j-9)*40.3-20,4483.965+1.5-29.63*i,(j-9)*40.3+20);
            }
        }
        if (ECALOption == 1)
            for (auto it = vecEcalCellID->begin(); it != vecEcalCellID->end(); it++)
            {
                Int_t ID_X = *it % 100;
                Int_t ID_Y = *it % 10000 / 100;
                Int_t ID_Z = *it / 10000;
                if (ID_Z % 2 == 0)
                    std::swap(ID_X, ID_Y);
                Int_t ChipID = (ID_Y - 1) / 7;
                Int_t MemoID = 0;
                Int_t ChannelID = (ID_Y - 1) % 7 * 5 + (ID_X - 1);
                Double_t Hit_X_tmp,Hit_Y_tmp,Hit_Z_tmp;
                if(vecEcalToaCell->at(std::distance(vecEcalCellID->begin(), it))>100){
                    continue;
                }
                if (ID_Z % 2 == 0)
                {
                    Hit_X_tmp=5.3 * (20.5 - ID_Y + 1);
                    Hit_Y_tmp=45.3 * (2 - ID_X + 1);
                    Hit_Z_tmp=(ID_Z / 2 - 1) * 19.9 + 11.2 + 3500 + 4.25;
                    ecal_zx->Fill(Hit_Z_tmp,Hit_X_tmp,vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it)));
                    ecal_zy->Fill(Hit_Z_tmp,Hit_Y_tmp,vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it)));
                }
                else
                {
                    Hit_X_tmp=45.3 * (2 - ID_X + 1);
                    Hit_Y_tmp=5.3 * (20.5 - ID_Y + 1);
                    Hit_Z_tmp=(ID_Z - 1) / 2 * 19.9 + 3500 + 4.25;
                    ecal_zx->Fill(Hit_Z_tmp,Hit_X_tmp,vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it)));
                    ecal_zy->Fill(Hit_Z_tmp,Hit_Y_tmp,vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it)));
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
            Double_t Hit_X_tmp,Hit_Y_tmp,Hit_Z_tmp;
            if(vecHcalToaCell->at(std::distance(vecHcalCellID->begin(), it))>100){
                continue;
            }
            Hit_X_tmp=40.3 * (9 - 0.5 - ID_X + 1);
            Hit_Y_tmp=40.3 * (9 - 0.5 - ID_Y + 1);
            Hit_Z_tmp=(ID_Z - 1) * 29.63+4483.965;
            ecal_zx->Fill(Hit_Z_tmp,Hit_X_tmp,vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it)));
            ecal_zy->Fill(Hit_Z_tmp,Hit_Y_tmp,vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it)));
        }
        TCanvas *c1 = new TCanvas("c1", "c1", 1600,1600);
        c1->Divide(2,1);
        c1->cd(1)->SetLogz();
        ecal_zx->GetZaxis()->SetRangeUser(0.1,10000);
        ecal_zx->Draw("colz");
        c1->cd(2)->SetLogz();
        ecal_zy->GetZaxis()->SetRangeUser(0.1,10000);
        ecal_zy->Draw("colz");
        if(gSystem->AccessPathName("img")){
            gSystem->mkdir("img");
        }
        c1->SaveAs(Form("img/%d.png",EventID));
        delete c1;
    }
    delete treeEvt;
    DataFile->Close();
    ConvertFile->cd();
    delete EventTree;
    ConvertFile->Close();
}
