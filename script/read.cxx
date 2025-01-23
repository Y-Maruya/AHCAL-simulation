#include "Global.h"
#include <TH2.h>
#include <TH2Poly.h>
#include <TH3D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TEventList.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TROOT.h>
#include <TChain.h>
#include <iostream>
#include <TF1.h>
using namespace std;


void read(TString InputFile, TString OutputFile, Int_t min_event, Int_t max_event)
{
    // if (gSystem->AccessPathName(InputDir + "/" + InputFileName))
    // {
    //     std::cout << "Error! ROOT file: " << InputDir + "/" + InputFileName << " does not exist." << std::endl;
    //     return;
    // }
    //------------------------------------------------------
    // gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);
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
    // if (!gSystem->AccessPathName(OutputDir + "/" + OutputFile))
    // {
    //     std::cout << "The MC file already exists. Skipped..." << std::endl;
    //     return;
    // }
    TString OutputFilae = OutputFile +"/readed.root";
    TFile *ConvertFile = TFile::Open(OutputFilae, "RECREATE");
    TH1F *h_muon_veto0 = new TH1F("h_muon_veto0", "h_muon_veto0;E_{dep}[MeV];Entries", 100, 0, 6);
    TH1F *h_muon_veto1 = new TH1F("h_muon_veto1", "h_muon_veto1;E_{dep}[MeV];Entries", 100, 0, 6);
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
    double initial_z =5570;
    //------------------------------------------------------
    Int_t NEvent = elist->GetN();
    DataFile->cd();
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
        for (auto it = vecHcalCellID->begin(); it != vecHcalCellID->end(); it++)
        {
            Int_t it_int = *it;
            if(it_int < 10000){
                Int_t o = it_int-1000;
                if (o==0){
                    h_muon_veto0->Fill(vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it)));
                }else if(o==1){
                    h_muon_veto1->Fill(vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it)));
                }
            }
            // Int_t ID_X = *it % 100;
            // Int_t ID_Y = *it % 10000 / 100;
            // Int_t ID_Z = *it / 10000;
            // Int_t ChipID = (ID_X - 1) / 6 + (ID_Y - 1) / 6 * 3;
            // Int_t MemoID = 0;
            // Int_t ChannelID = (ID_X - 1) % 6 + (ID_Y - 1) % 6 * 6;
            // Double_t Hit_X_tmp,Hit_Y_tmp,Hit_Z_tmp;
            // if(vecHcalToaCell->at(std::distance(vecHcalCellID->begin(), it))>100){
            //     continue;
            // }
            // Hit_X_tmp=40.3 * (9 - 0.5 - ID_X + 1);
            // Hit_Y_tmp=40.3 * (9 - 0.5 - ID_Y + 1);
            // Hit_Z_tmp=(ID_Z - 1) * 29.63+5765.565;
            // double tmp_energy = vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it));
            // ecal_zx->Fill(Hit_Z_tmp,Hit_X_tmp,tmp_energy);
            // ecal_zy->Fill(Hit_Z_tmp,Hit_Y_tmp,tmp_energy);
        }
    }
    delete treeEvt;
    DataFile->Close();
    ConvertFile->cd();
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    h_muon_veto0->Draw();
    TF1*landau= new TF1("landau","landau");
    landau->SetParameter(2,0.18);
    h_muon_veto0->Fit(landau,"M");

    c1->SaveAs(OutputFile + "/h_muon_veto0.png");
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    h_muon_veto1->Draw();
    h_muon_veto1->Fit(landau,"M");
    c2->SaveAs(OutputFile + "/h_muon_veto1.png");
    h_muon_veto0->Write();
    h_muon_veto1->Write();
    ConvertFile->Close();
}

int main (int argc, char **argv){
   if(argc != 5){
      std::cout << "Usage: ./visualize inputname outputname startevent endevent" << std::endl;
      return 1;
   }
   std::string inputname = argv[1];
   std::string outputname = argv[2];
   int startevent = std::stoi(argv[3]);
   int endevent = std::stoi(argv[4]);
   read(inputname,outputname,startevent,endevent);
   return 0;
}