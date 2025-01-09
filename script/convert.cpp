#include <iostream>
#include <vector>
#include <string>
#include <H5Cpp.h>
#include <TFile.h>
#include <TTree.h>
#include <TEventList.h>
#include <TBranch.h>

using namespace H5;
Int_t GetBin1d(int bin1,int bin2,int bin3){
    return (bin1)*42+(bin2)+(bin3)*42*5;
}
Int_t GetBin1d_AHCAL(int bin1,int bin2,int bin3){
   return (bin1)*18+(bin2)+(bin3)*18*18;
}
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
    tree->Draw(">>elist", "ftagNulabel<4");
    TEventList *elist = (TEventList*)gDirectory->Get("elist");
    tree->SetEventList(elist);

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
    // std::vector<Double_t>* vecHcalEdepCell = nullptr;
    std::vector<Double_t>* vecHcalVisibleEdepCell = nullptr;
    // std::vector<Double_t>* vecHcalHitTimeCell = nullptr;
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
    // tree->SetBranchStatus("vecHcalEdepCell", 1);
    tree->SetBranchStatus("vecHcalVisibleEdepCell", 1);
    // tree->SetBranchStatus("vecHcalHitTimeCell", 1);
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
    // tree->SetBranchAddress("vecHcalEdepCell", &vecHcalEdepCell);
    tree->SetBranchAddress("vecHcalVisibleEdepCell", &vecHcalVisibleEdepCell);
    // tree->SetBranchAddress("vecHcalHitTimeCell", &vecHcalHitTimeCell);
    // tree->SetBranchAddress("vecHcalToaCell", &vecHcalToaCell);
    
    // Create the HDF5 file
    H5File h5_file(h5_file_path, H5F_ACC_TRUNC);

    // Create datasets in the HDF5 file
    hsize_t total_entries = static_cast<hsize_t>(elist->GetN());
    // hsize_t total_entries = 100;
    hsize_t dims[1] = { total_entries };
    DataSpace dataspace(1, dims);

    DataSet dataset_ftagNulabel = h5_file.createDataSet("ftagNulabel", PredType::NATIVE_INT, dataspace);
    DataSet dataset_Interaction_x = h5_file.createDataSet("Interaction_x", PredType::NATIVE_FLOAT, dataspace);
    DataSet dataset_Interaction_y = h5_file.createDataSet("Interaction_y", PredType::NATIVE_FLOAT, dataspace);
    DataSet dataset_Interaction_z = h5_file.createDataSet("Interaction_z", PredType::NATIVE_FLOAT, dataspace);
    DataSet dataset_total_energy_SciW = h5_file.createDataSet("total_energy_SciW", PredType::NATIVE_FLOAT, dataspace);
    DataSet dataset_total_energy_AHCAL = h5_file.createDataSet("total_energy_AHCAL", PredType::NATIVE_FLOAT, dataspace);
    // Define the shape for SciW and AHCAL data
    hsize_t dims_SciW[4] = { total_entries, 32, 5, 42 };
    hsize_t dims_AHCAL[4] = { total_entries, 40, 18, 18 };
    DataSpace dataspace_SciW(4, dims_SciW);
    DataSpace dataspace_AHCAL(4, dims_AHCAL);

    DataSet dataset_energy_deposit_SciW = h5_file.createDataSet("energy_deposit_SciW", PredType::NATIVE_FLOAT, dataspace_SciW);
    DataSet dataset_energy_deposit_AHCAL = h5_file.createDataSet("energy_deposit_AHCAL", PredType::NATIVE_FLOAT, dataspace_AHCAL);

    // Buffers to hold the data
    std::vector<int> ftagNulabel_data(batch_size);
    std::vector<float> Interaction_x_data(batch_size);
    std::vector<float> Interaction_y_data(batch_size);
    std::vector<float> Interaction_z_data(batch_size);
    std::vector<float> total_energy_SciW_data(batch_size);
    std::vector<float> total_energy_AHCAL_data(batch_size);
    std::vector<float> energy_deposit_SciW_data(batch_size * 32 * 42 * 5);
    std::vector<float> energy_deposit_AHCAL_data(batch_size * 40 * 18 * 18);

    // Loop over the entries in the tree in batches
    for (Long64_t start = 0; start < total_entries; start += batch_size) {
        Long64_t end = std::min(start + batch_size, total_entries);
        Long64_t current_batch_size = end - start;

        for (Long64_t i = start; i < end; ++i) {
            if (tree->GetEntry(elist->GetEntry(i)) <= 0) {
                std::cerr << "Error reading entry: " << i << std::endl;
                continue;
            }
            size_t index = i - start;
            ftagNulabel_data[index] = ftagNulabel;
            Interaction_x_data[index] = Interaction_x;
            Interaction_y_data[index] = Interaction_y;
            Interaction_z_data[index] = Interaction_z;
            total_energy_SciW_data[index] = 0;
            total_energy_AHCAL_data[index] = 0;
            for(int i=0;i<32*42*5;i++){
                energy_deposit_SciW_data[index*32*42*5+i]=0;
            }
            for(int i=0;i<40*18*18;i++){
                energy_deposit_AHCAL_data[index*40*18*18+i]=0;
            }
            for (auto it = vecEcalCellID->begin(); it != vecEcalCellID->end(); it++)
            {
                Int_t ID_X = *it % 100;
                Int_t ID_Y = *it % 10000 / 100;
                Int_t ID_Z = *it / 10000;
                if (ID_Z % 2 == 0)
                    std::swap(ID_X, ID_Y);
                double tmp_energy=vecEcalVisibleEdepCell->at(std::distance(vecEcalCellID->begin(), it));
                if(energy_deposit_SciW_data[index*32*42*5+GetBin1d(ID_X-1,ID_Y-1,ID_Z-1)]>0){
                    std::cout<<"Error: energy deposit already exists"<<std::endl;
                }else{
                    energy_deposit_SciW_data[index*32*42*5+GetBin1d(ID_X-1,ID_Y-1,ID_Z-1)]=tmp_energy;
                }
                total_energy_SciW_data[index]+=tmp_energy;
            }
            for (auto it = vecHcalCellID->begin(); it != vecHcalCellID->end(); it++)
            {
                Int_t ID_X = *it % 100;
                Int_t ID_Y = *it % 10000 / 100;
                Int_t ID_Z = *it / 10000;
                double tmp_energy=vecHcalVisibleEdepCell->at(std::distance(vecHcalCellID->begin(), it));
                if(energy_deposit_AHCAL_data[index*40*18*18+GetBin1d_AHCAL(ID_X-1,ID_Y-1,ID_Z-1)]>0){
                    std::cout<<"Error: energy deposit already exists"<<std::endl;
                }else{
                    energy_deposit_AHCAL_data[index*40*18*18+GetBin1d_AHCAL(ID_X-1,ID_Y-1,ID_Z-1)]=tmp_energy;
                }
                total_energy_AHCAL_data[index]+=tmp_energy;
            }
        }
        // Write the data to the HDF5 file
        hsize_t offset[1] = { static_cast<hsize_t>(start) };
        hsize_t count[1] = { static_cast<hsize_t>(current_batch_size) };
        dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        DataSpace memspace(1, count);
        dataset_ftagNulabel.write(ftagNulabel_data.data(), PredType::NATIVE_INT, memspace, dataspace);
        dataset_Interaction_x.write(Interaction_x_data.data(), PredType::NATIVE_FLOAT, memspace, dataspace);
        dataset_Interaction_y.write(Interaction_y_data.data(), PredType::NATIVE_FLOAT, memspace, dataspace);
        dataset_Interaction_z.write(Interaction_z_data.data(), PredType::NATIVE_FLOAT, memspace, dataspace);
        dataset_total_energy_SciW.write(total_energy_SciW_data.data(), PredType::NATIVE_FLOAT, memspace, dataspace);
        dataset_total_energy_AHCAL.write(total_energy_AHCAL_data.data(), PredType::NATIVE_FLOAT, memspace, dataspace);
        hsize_t dims_SciW_mem[4] = { current_batch_size, 32, 5, 42 };
        hsize_t dims_AHCAL_mem[4] = { current_batch_size, 40, 18, 18 };
        DataSpace memspace_SciW(4, dims_SciW_mem);
        DataSpace memspace_AHCAL(4, dims_AHCAL_mem);
        hsize_t offset_SciW[4] = { static_cast<hsize_t>(start), 0, 0, 0 };
        hsize_t offset_AHCAL[4] = { static_cast<hsize_t>(start), 0, 0, 0 };
        dataspace_SciW.selectHyperslab(H5S_SELECT_SET, dims_SciW_mem, offset_SciW);
        dataspace_AHCAL.selectHyperslab(H5S_SELECT_SET, dims_AHCAL_mem, offset_AHCAL);
        dataset_energy_deposit_SciW.write(energy_deposit_SciW_data.data(), PredType::NATIVE_FLOAT, memspace_SciW, dataspace_SciW);
        dataset_energy_deposit_AHCAL.write(energy_deposit_AHCAL_data.data(), PredType::NATIVE_FLOAT, memspace_AHCAL, dataspace_AHCAL);

        std::cout << "Processed entries: " << start << " to " << end << std::endl;
    }

    // Close the ROOT file
    root_file->Close();
    h5_file.close();
}

int main(int argc,char** argv){
    std::string root_file_path = argv[1];
    std::string tree_name = "treeEvt";
    std::string h5_file_path = argv[2];
    convert_caloroot_to_h5(root_file_path, tree_name, h5_file_path);
    return 0;
}
//g++ -o convert convert.cpp `root-config --cflags --libs` -lhdf5_cpp -lhdf5