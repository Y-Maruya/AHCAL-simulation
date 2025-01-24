#include <iostream>
#include <vector>
#include <string>
#include <H5Cpp.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TRandom.h>
#include <TEventList.h>
#include <TBranch.h>
const Double_t MIPEnergy = 0.461; // Visible
const Double_t MIPCut = 0.5;
// const Double_t TimeCut = 150; // ns
const Double_t ADCHLRatio = 37.5;
const Double_t ADCError = 0.0002;
const Int_t ADCLimit = 4000;
const Int_t ADCSWITCH = 3000;
const Int_t ADCBaseline = 350;
const Double_t MIPResponse[2] = {17., 20.};
const Double_t PEChargeMean[2] = {19.75, 29.4};
const Double_t PEChargeSigma[2] = {3., 6.};
const Double_t PEChargeNoise = 3;
using namespace H5;
Int_t GetBin1d(int bin1,int bin2,int bin3){
    return (bin1)*42+(bin2)+(bin3)*42*5;
}
Int_t GetBin1d_AHCAL(int bin1,int bin2,int bin3){
   return (bin1)*18+(bin2)+(bin3)*18*18;
}
// auto SiPMResponseFit = new TF1("SiPMResponseFit", "0.876*((1-[1])*[0]*(1-exp(-x/[0]))+[1]*x)*([2]+1)/( [2]+x / ([0]*(1-exp(-x/[0]))) )*(1+[3]*exp(-x/[0]))*(1+[4])");

// Int_t ADCDigi(Int_t fADC)
// {
//     Int_t sAdc = -1;
//     while(sAdc < 0)
//         sAdc = std::round(gRandom->Gaus(fADC + ADCBaseline, (fADC + ADCBaseline) * ADCError));
//     if (sAdc > ADCLimit)
//         sAdc = ADCLimit;
//     if (sAdc > ADCSWITCH)
//     {
//         sAdc = -1;
//         while(sAdc < 0)
//             sAdc = std::round(gRandom->Gaus(fADC/ADCHLRatio + ADCBaseline, (fADC/ADCHLRatio + ADCBaseline) * ADCError));
//         if (sAdc > ADCLimit)
//             sAdc = ADCLimit;
//         return std::round((sAdc - ADCBaseline) * ADCHLRatio);
//     }
//     else
//     {
//         return (sAdc - ADCBaseline);
//     }
// }
// Double_t SiPMDigi(Double_t edep, Int_t i, Int_t m)
// {
//     // The Poisson distribution of scintillation process is not considered
//     // sPhoton = gRandom->Poisson(edep * LightYieldperMeV); 10000p.e./MeV
//     Int_t sPix = 0;
//     sPix = gRandom->Poisson(edep / MIPEnergy * MIPResponse[i]);
//     if (sPix > 2500)
//         sPix = std::round(SiPMResponseFit->Eval(sPix)); // HPK and NDL SiPMs has similar pixel number, assume equal
//     Double_t sChargeOutMean = sPix * PEChargeMean[i];
//     Double_t sChargeOutSigma = sqrt(sPix * PEChargeSigma[i] * PEChargeSigma[i] + PEChargeNoise * PEChargeNoise);
//     Int_t sChargeOut = -1;
//     while(sChargeOut < 0)
//         sChargeOut = std::round(gRandom->Gaus(sChargeOutMean, sChargeOutSigma));
//     if (m > 0)
//         sChargeOut = ADCDigi(sChargeOut);
//     Double_t sMIP = sChargeOut / PEChargeMean[i] / MIPResponse[i];
//     if (sMIP < MIPCut)
//         return 0;
//     else
//         return sMIP * MIPEnergy;
// }
void convert_caloroot_to_h5(const std::string& root_file_path, const std::string& tree_name, const std::string& h5_file_path, size_t batch_size = 1000) {
    // Open the ROOT file
    TFile* root_file = TFile::Open(root_file_path.c_str());
    if (!root_file || root_file->IsZombie()) {
        std::cerr << "Error opening ROOT file: " << root_file_path << std::endl;
        return;
    }

    // Get the tree from the ROOT file
    TTree* tree = (TTree*)root_file->Get(tree_name.c_str());
    if (!tree) {
        std::cerr << "Error getting tree: " << tree_name << " from ROOT file: " << root_file_path << std::endl;
        root_file->Close();
        return;
    }
    tree->Draw(">>elist", "ftagNulabel<5");
    TEventList *elist = (TEventList*)gDirectory->Get("elist");
    tree->SetEventList(elist);
    // SiPMResponseFit->SetParameters(3082.88, 1.35524, 4.0577, 0.0206382, 0.109543);
    std::cout << "Number of entries in the tree: " << elist->GetN() << std::endl;
    // Variables to hold the data
    // int ftagNulabel;
    // double Interaction_x, Interaction_y, Interaction_z;
    // double energy_deposit_SciW[16*42*5*2];
    // double energy_deposit_AHCAL[18*18*40];
    // Set branch addresses
    Int_t EvtID;
    Double_t ParticleEnergy;
    Double_t Interaction_x;
    Double_t Interaction_y;
    Double_t Interaction_z;
    Double_t Sum;
    Int_t ftagNulabel;
    // Double_t CaloEdepSum;
    // Double_t CaloVisibleEdepSum;
    // Double_t EcalEdepSum;
    // Double_t EcalVisibleEdepSum;
    // Double_t HcalEdepSum;
    // Double_t HcalVisibleEdepSum;
    // Double_t EcalMaxEdepCell;
    // Double_t HcalMaxEdepCell;
    std::vector<Int_t>* vecEcalCellID = nullptr;
    // std::vector<Int_t>* vecEcalStepsCell = nullptr;
    // std::vector<Double_t>* vecEcalEdepCell = nullptr;
    std::vector<Double_t>* vecEcalVisibleEdepCell = nullptr;
    // std::vector<Double_t>* vecEcalHitTimeCell = nullptr;
    // std::vector<Double_t>* vecEcalToaCell = nullptr;
    std::vector<Int_t>* vecHcalCellID = nullptr;
    // std::vector<Int_t>* vecHcalStepsCell = nullptr;
    std::vector<Double_t>* vecHcalEdepCell = nullptr;
    std::vector<Double_t>* vecHcalVisibleEdepCell = nullptr;
    std::vector<Double_t>* vecHcalHitTimeCell = nullptr;
    // std::vector<Double_t>* vecHcalToaCell = nullptr;
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("EvtID", 1);
    tree->SetBranchStatus("ParticleEnergy", 1);
    tree->SetBranchStatus("Interaction_x", 1);
    tree->SetBranchStatus("Interaction_y", 1);
    tree->SetBranchStatus("Interaction_z", 1);
    tree->SetBranchStatus("ftagNulabel", 1);
    // tree->SetBranchStatus("CaloEdepSum", 1);
    // tree->SetBranchStatus("CaloVisibleEdepSum", 1);
    // tree->SetBranchStatus("EcalEdepSum", 1);
    // tree->SetBranchStatus("EcalVisibleEdepSum", 1);
    // tree->SetBranchStatus("HcalEdepSum", 1);
    // tree->SetBranchStatus("HcalVisibleEdepSum", 1);
    // tree->SetBranchStatus("EcalMaxEdepCell", 1);
    // tree->SetBranchStatus("HcalMaxEdepCell", 1);
    tree->SetBranchStatus("vecEcalCellID", 1);
    // tree->SetBranchStatus("vecEcalStepsCell", 1);
    // tree->SetBranchStatus("vecEcalEdepCell", 1);
    tree->SetBranchStatus("vecEcalVisibleEdepCell", 1);
    // tree->SetBranchStatus("vecEcalHitTimeCell", 1);
    // tree->SetBranchStatus("vecEcalToaCell", 1);
    tree->SetBranchStatus("vecHcalCellID", 1);
    // tree->SetBranchStatus("vecHcalStepsCell", 1);
    tree->SetBranchStatus("vecHcalEdepCell", 1);
    tree->SetBranchStatus("vecHcalVisibleEdepCell", 1);
    tree->SetBranchStatus("vecHcalHitTimeCell", 1);
    // tree->SetBranchStatus("vecHcalToaCell", 1);
    tree->SetBranchAddress("EvtID", &EvtID);
    tree->SetBranchAddress("ParticleEnergy", &ParticleEnergy);
    tree->SetBranchAddress("Interaction_x", &Interaction_x);
    tree->SetBranchAddress("Interaction_y", &Interaction_y);
    tree->SetBranchAddress("Interaction_z", &Interaction_z);
    tree->SetBranchAddress("ftagNulabel", &ftagNulabel);
    // tree->SetBranchAddress("CaloEdepSum", &CaloEdepSum);
    // tree->SetBranchAddress("CaloVisibleEdepSum", &CaloVisibleEdepSum);
    // tree->SetBranchAddress("EcalEdepSum", &EcalEdepSum);
    // tree->SetBranchAddress("EcalVisibleEdepSum", &EcalVisibleEdepSum);
    // tree->SetBranchAddress("HcalEdepSum", &HcalEdepSum);
    // tree->SetBranchAddress("HcalVisibleEdepSum", &HcalVisibleEdepSum);
    // tree->SetBranchAddress("EcalMaxEdepCell", &EcalMaxEdepCell);
    // tree->SetBranchAddress("HcalMaxEdepCell", &HcalMaxEdepCell);
    tree->SetBranchAddress("vecEcalCellID", &vecEcalCellID);
    // tree->SetBranchAddress("vecEcalStepsCell", &vecEcalStepsCell);
    // tree->SetBranchAddress("vecEcalEdepCell", &vecEcalEdepCell);
    tree->SetBranchAddress("vecEcalVisibleEdepCell", &vecEcalVisibleEdepCell);
    // tree->SetBranchAddress("vecEcalHitTimeCell", &vecEcalHitTimeCell);
    // tree->SetBranchAddress("vecEcalToaCell", &vecEcalToaCell);
    tree->SetBranchAddress("vecHcalCellID", &vecHcalCellID);
    // tree->SetBranchAddress("vecHcalStepsCell", &vecHcalStepsCell);
    tree->SetBranchAddress("vecHcalEdepCell", &vecHcalEdepCell);
    tree->SetBranchAddress("vecHcalVisibleEdepCell", &vecHcalVisibleEdepCell);
    tree->SetBranchAddress("vecHcalHitTimeCell", &vecHcalHitTimeCell);
    // tree->SetBranchAddress("vecHcalToaCell", &vecHcalToaCell);

    Double_t edep_sum[40];
    Double_t veto_sum[2];
    Double_t veto_timing[2];    // Create datasets in the HDF5 file
    hsize_t total_entries = static_cast<hsize_t>(elist->GetN());
    TFile* out_file = new TFile(h5_file_path.c_str(), "RECREATE");
    out_file->cd();

    TTree* out_tree = new TTree("tree", "tree");
    out_tree->Branch("EvtID", &EvtID, "EvtID/I");
    out_tree->Branch("ParticleEnergy", &ParticleEnergy, "ParticleEnergy/D");
    out_tree->Branch("Interaction_x", &Interaction_x, "Interaction_x/D");
    out_tree->Branch("Interaction_y", &Interaction_y, "Interaction_y/D");
    out_tree->Branch("Interaction_z", &Interaction_z, "Interaction_z/D");
    out_tree->Branch("ftagNulabel", &ftagNulabel, "ftagNulabel/I");
    // out_tree->Branch("CaloEdepSum", &HcalVisibleEdepSum, "CaloEdepSum/D");
    out_tree->Branch("edep_sum", edep_sum, "edep_sum[40]/D");
    out_tree->Branch("Sum",&Sum,"Sum/D");
    out_tree->Branch("veto_sum", veto_sum, "veto_sum[2]/D");
    out_tree->Branch("veto_timing",veto_timing,"veto_timing[2]/D");
    // Loop over the entries in the tree in batches
    for (Long64_t i = 0; i < total_entries; i += 1) {
        // root_file->cd();
        Sum=0;
        for(int j=0;j<40;j++){
            edep_sum[j]=0;
        }
        veto_sum[0]=0;
        veto_sum[1]=0;
        if (tree->GetEntry(elist->GetEntry(i)) <= 0) {
                std::cerr << "Error reading entry: " << i << std::endl;
                continue;
            }
        for (auto it = vecHcalCellID->begin(); it != vecHcalCellID->end(); it++)
        {
            Int_t it_int = *it;
            if(it_int<10000){
                Int_t vetoi = it_int-1000;
                veto_sum[vetoi] +=vecHcalEdepCell->at(std::distance(vecHcalCellID->begin(), it));
                veto_timing[vetoi] = vecHcalHitTimeCell->at(std::distance(vecHcalCellID->begin(), it));
                // veto_energy_data[index*2*vetoi]=vecHcalEdepCell->at(std::distance(vecHcalCellID->begin(), it));
                // std::cout<<"detect veto"<<vetoi<<std::endl;
                continue;
            }
            Int_t ID_X = *it % 100;
            Int_t ID_Y = *it % 10000 / 100;
            Int_t ID_Z = *it / 10000;
            Int_t ChipID = (ID_X - 1) / 6 + (ID_Y - 1) / 6 * 3;
            Int_t MemoID = 0;
            Int_t ChannelID = (ID_X - 1) % 6 + (ID_Y - 1) % 6 * 6;
            Int_t CellID = (ID_Z - 1) * 1e5 + ChipID * 1e4 + MemoID * 1e2 + ChannelID;
            double tmp_energy=vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it));
            // double tmp_energy_digi=SiPMDigi(tmp_energy,(CellID/100000 - 40) / 38+1, 1);
            edep_sum[ID_Z-1]+=tmp_energy;
            Sum+=tmp_energy;
        }
        out_tree->Fill();
    }
    // Write the tree to the HDF5 file
    out_file->cd();
    out_tree->Write();
    out_file->Close();
    // Close the ROOT file
    root_file->Close();

}

int main(int argc,char** argv){
    std::string root_file_path = argv[1];
    std::string tree_name = "treeEvt";
    std::string h5_file_path = argv[2];
    convert_caloroot_to_h5(root_file_path, tree_name, h5_file_path);
    return 0;
}
//g++ -o convert_withdigi convert_withdigi.cpp `root-config --cflags --libs` -lhdf5_cpp -lhdf5