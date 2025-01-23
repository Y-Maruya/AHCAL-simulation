#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"

#include "TROOT.h"

namespace SimCalModule
{
    RunAction::RunAction(PrimaryGenerator *PGA)
        : G4UserRunAction(), fPrimaryGen(PGA), fileRun(nullptr), treeEvt(nullptr)
    {
        ROOT::EnableThreadSafety();
    }

    RunAction::~RunAction()
    {
    }

    void RunAction::BeginOfRunAction(const G4Run *aRun)
    {
        if (G4RunManager::GetRunManager()->GetRunManagerType() != G4RunManager::workerRM)
        {
            G4cout << ">>> The Run " << aRun->GetRunID() << " has started. <<<" << G4endl;
        }
        if (G4RunManager::GetRunManager()->GetRunManagerType() != G4RunManager::masterRM)
        {
            G4String rootFileName = fPrimaryGen->GetInputFileName().substr(0,fPrimaryGen->GetInputFileName().find(".root"))+"_calo";
            if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::workerRM)
                rootFileName += "_t" + std::to_string(G4Threading::G4GetThreadId());
            rootFileName += ".root";
            while (rootFileName.find(" ") != G4String::npos)
            {
                rootFileName.erase(rootFileName.find(" "), 1);
            }
            fileRun = TFile::Open(rootFileName, "RECREATE");
            treeEvt = new TTree("treeEvt", "Info stored at Event level");
            treeEvt->Branch("EvtID", &EvtID, "EvtID/I");
            treeEvt->Branch("ParticleEnergy", &ParticleEnergy, "ParticleEnergy/D");
            treeEvt->Branch("Interaction_x", &Interaction_x, "Interaction_x/D");
            treeEvt->Branch("Interaction_y", &Interaction_y, "Interaction_y/D");
            treeEvt->Branch("Interaction_z", &Interaction_z, "Interaction_z/D");
            treeEvt->Branch("ftagNulabel", &ftagNulabel, "ftagNulabel/I");
            treeEvt->Branch("CaloEdepSum", &CaloEdepSum, "CaloEdepSum/D");
            treeEvt->Branch("CaloVisibleEdepSum", &CaloVisibleEdepSum, "CaloVisibleEdepSum/D");
            treeEvt->Branch("EcalEdepSum", &EcalEdepSum, "EcalEdepSum/D");
            treeEvt->Branch("EcalVisibleEdepSum", &EcalVisibleEdepSum, "EcalVisibleEdepSum/D");
            treeEvt->Branch("HcalEdepSum", &HcalEdepSum, "HcalEdepSum/D");
            treeEvt->Branch("HcalVisibleEdepSum", &HcalVisibleEdepSum, "HcalVisibleEdepSum/D");
            treeEvt->Branch("EcalMaxEdepCell", &EcalMaxEdepCell, "EcalMaxEdepCell/D");
            treeEvt->Branch("HcalMaxEdepCell", &HcalMaxEdepCell, "HcalMaxEdepCell/D");
            treeEvt->Branch("vecEcalCellID", &vecEcalCellID);
            treeEvt->Branch("vecEcalStepsCell", &vecEcalStepsCell);
            treeEvt->Branch("vecEcalEdepCell", &vecEcalEdepCell);
            treeEvt->Branch("vecEcalVisibleEdepCell", &vecEcalVisibleEdepCell);
            treeEvt->Branch("vecEcalHitTimeCell", &vecEcalHitTimeCell);
            treeEvt->Branch("vecEcalToaCell", &vecEcalToaCell);
            treeEvt->Branch("vecHcalCellID", &vecHcalCellID);
            treeEvt->Branch("vecHcalStepsCell", &vecHcalStepsCell);
            treeEvt->Branch("vecHcalEdepCell", &vecHcalEdepCell);
            treeEvt->Branch("vecHcalVisibleEdepCell", &vecHcalVisibleEdepCell);
            treeEvt->Branch("vecHcalHitTimeCell", &vecHcalHitTimeCell);
            treeEvt->Branch("vecHcalToaCell", &vecHcalToaCell);
            treeEvt->Branch("nstoredTruthParticles", &nstoredTruthParticles, "nstoredTruthParticles/I");
            treeEvt->Branch("vecTruth_pdgID", &vecTruth_pdgID);
            treeEvt->Branch("vecTruth_px", &vecTruth_px);
            treeEvt->Branch("vecTruth_py", &vecTruth_py);
            treeEvt->Branch("vecTruth_pz", &vecTruth_pz);
            treeEvt->Branch("vecTruth_x", &vecTruth_x);
            treeEvt->Branch("vecTruth_y", &vecTruth_y);
            treeEvt->Branch("vecTruth_z", &vecTruth_z);
            treeEvt->Branch("vecTruth_energy", &vecTruth_energy);
            treeEvt->Branch("vecTruth_vertexIndex", &vecTruth_vertexIndex);
            treeEvt->Branch("vecTruth_trackID", &vecTruth_trackID);
        }
    }

    void RunAction::EndOfRunAction(const G4Run *aRun)
    {
        if (G4RunManager::GetRunManager()->GetRunManagerType() != G4RunManager::workerRM)
            G4cout << "The run with RunID  " << aRun->GetRunID() << " is finished. " << G4endl;
        if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::masterRM)
        {
            G4String rootFileName = fPrimaryGen->GetInputFileName().substr(0,fPrimaryGen->GetInputFileName().find(".root"))+"_calo";
            if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::workerRM)
                rootFileName += "_t" + std::to_string(G4Threading::G4GetThreadId());
            rootFileName += ".root";
            while (rootFileName.find(" ") != G4String::npos)
            {
                rootFileName.erase(rootFileName.find(" "), 1);
            }
            G4UImanager *UImanager = G4UImanager::GetUIpointer();
            UImanager->ApplyCommand(Form("/control/shell if [ -f %s.root ]; then rm %s.root; fi", rootFileName.data(), rootFileName.data()));
            UImanager->ApplyCommand(Form("/control/shell if [ `ls %s_* | wc -l` -eq 1 ]; then mv `ls %s_*` %s.root; fi", rootFileName.data(), rootFileName.data(), rootFileName.data()));
            UImanager->ApplyCommand(Form("/control/shell ls %s_* | xargs hadd %s.root", rootFileName.data(), rootFileName.data()));
            UImanager->ApplyCommand(Form("/control/shell ls %s_* | xargs rm", rootFileName.data()));
        }
        else
        {
            fileRun->cd();
            treeEvt->Write();
            delete treeEvt;
            fileRun->Close();
            delete fileRun;
        }
    }

    void RunAction::FillEvent()
    {
        treeEvt->Fill();
    }

    void RunAction::TransferData(G4int data, SimuData id)
    {
        switch (id)
        {
        case EvtID_Data:
            EvtID = data;
            break;
        case ftagNulabel_Data:
            ftagNulabel = data;
            break;
        case nstoredTruthParticles_Data:
            nstoredTruthParticles = data;
            break;
        
        default:
        {
            G4ExceptionDescription ed;
            ed << "RunAction::TransferData(): "
               << "Data error!" << G4endl;
            G4Exception("RunAction::TransferData()", "Expl02",
                        FatalException, ed,
                        "Something is wrong with data");
            break;
        }
        }
    }

    void RunAction::TransferData(G4double data, SimuData id)
    {
        switch (id)
        {
        case ParticleEnergy_Data:
            ParticleEnergy = data;
            break;
        case Interaction_x_Data:
            Interaction_x = data;
            break;
        case Interaction_y_Data:
            Interaction_y = data;
            break;
        case Interaction_z_Data:
            Interaction_z = data;
            break;        
        case CaloEdepSum_Data:
            CaloEdepSum = data;
            break;
        case CaloVisibleEdepSum_Data:
            CaloVisibleEdepSum = data;
            break;
        case EcalEdepSum_Data:
            EcalEdepSum = data;
            break;
        case EcalVisibleEdepSum_Data:
            EcalVisibleEdepSum = data;
            break;
        case HcalEdepSum_Data:
            HcalEdepSum = data;
            break;
        case HcalVisibleEdepSum_Data:
            HcalVisibleEdepSum = data;
            break;
        case EcalMaxEdepCell_Data:
            EcalMaxEdepCell = data;
            break;
        case HcalMaxEdepCell_Data:
            HcalMaxEdepCell = data;
            break;
        default:
        {
            G4ExceptionDescription ed;
            ed << "RunAction::TransferData(): "
               << "Data error!" << G4endl;
            G4Exception("RunAction::TransferData()", "Expl02",
                        FatalException, ed,
                        "Something is wrong with data");
            break;
        }
        }
    }

    void RunAction::TransferData(std::vector<G4int> data, SimuData id)
    {
        switch (id)
        {
        case vecEcalCellID_Data:
            vecEcalCellID = data;
            break;
        case vecEcalStepsCell_Data:
            vecEcalStepsCell = data;
            break;
        case vecHcalCellID_Data:
            vecHcalCellID = data;
            break;
        case vecHcalStepsCell_Data:
            vecHcalStepsCell = data;
            break;
        case vecTruth_pdgID_Data:
            vecTruth_pdgID = data;
            break;
        case vecTruth_vertexIndex_Data:
            vecTruth_vertexIndex = data;
            break;
        case vecTruth_trackID_Data:
            vecTruth_trackID = data;
            break;
        default:
        {
            G4ExceptionDescription ed;
            ed << "RunAction::TransferData(): "
               << "Data error!" << G4endl;
            G4Exception("RunAction::TransferData()", "Expl02",
                        FatalException, ed,
                        "Something is wrong with data");
            break;
        }
        }
    }

    void RunAction::TransferData(std::vector<G4double> data, SimuData id)
    {
        switch (id)
        {
        case vecEcalEdepCell_Data:
            vecEcalEdepCell = data;
            break;
        case vecEcalVisibleEdepCell_Data:
            vecEcalVisibleEdepCell = data;
            break;
        case vecEcalHitTimeCell_Data:
            vecEcalHitTimeCell = data;
            break;
        case vecEcalToaCell_Data:
            vecEcalToaCell = data;
            break;
        case vecHcalEdepCell_Data:
            vecHcalEdepCell = data;
            break;
        case vecHcalVisibleEdepCell_Data:
            vecHcalVisibleEdepCell = data;
            break;
        case vecHcalHitTimeCell_Data:
            vecHcalHitTimeCell = data;
            break;
        case vecHcalToaCell_Data:
            vecHcalToaCell = data;
            break;
        case vecTruth_px_Data:
            vecTruth_px = data;
            break;
        case vecTruth_py_Data:
            vecTruth_py = data;
            break;
        case vecTruth_pz_Data:
            vecTruth_pz = data;
            break;
        case vecTruth_x_Data:
            vecTruth_x = data;
            break;
        case vecTruth_y_Data:
            vecTruth_y = data;
            break;
        case vecTruth_z_Data:
            vecTruth_z = data;
            break;
        case vecTruth_energy_Data:
            vecTruth_energy = data;
            break;
        default:
        {
            G4ExceptionDescription ed;
            ed << "RunAction::TransferData(): "
               << "Data error!" << G4endl;
            G4Exception("RunAction::TransferData()", "Expl02",
                        FatalException, ed,
                        "Something is wrong with data");
            break;
        }
        }
    }
}