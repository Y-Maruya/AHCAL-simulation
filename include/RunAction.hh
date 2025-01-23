#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "EventAction.hh"
#include "PrimaryGenerator.hh"
#include "TFile.h"
#include "TTree.h"

namespace SimCalModule
{
    enum SimuData
    {
        EvtID_Data,
        ParticleEnergy_Data,
        Interaction_x_Data,
        Interaction_y_Data,
        Interaction_z_Data,
        ftagNulabel_Data,
        CaloEdepSum_Data,
        CaloVisibleEdepSum_Data,
        EcalEdepSum_Data,
        EcalVisibleEdepSum_Data,
        HcalEdepSum_Data,
        HcalVisibleEdepSum_Data,
        EcalMaxEdepCell_Data,
        HcalMaxEdepCell_Data,
        vecEcalCellID_Data,
        vecEcalStepsCell_Data,
        vecEcalEdepCell_Data,
        vecEcalVisibleEdepCell_Data,
        vecEcalHitTimeCell_Data,
        vecEcalToaCell_Data,
        vecHcalCellID_Data,
        vecHcalStepsCell_Data,
        vecHcalEdepCell_Data,
        vecHcalVisibleEdepCell_Data,
        vecHcalHitTimeCell_Data,
        vecHcalToaCell_Data,
        nstoredTruthParticles_Data,
        vecTruth_pdgID_Data,
        vecTruth_px_Data,
        vecTruth_py_Data,
        vecTruth_pz_Data,
        vecTruth_x_Data,
        vecTruth_y_Data,
        vecTruth_z_Data,
        vecTruth_energy_Data,
        vecTruth_vertexIndex_Data,
        vecTruth_trackID_Data
    };

    class EventAction;
    class PrimaryGenerator;

    class RunAction : public G4UserRunAction
    {

    public:
        RunAction(PrimaryGenerator *);
        ~RunAction() override;
        void BeginOfRunAction(const G4Run *) override;
        void EndOfRunAction(const G4Run *) override;
        void FillEvent();
        void TransferData(G4int, SimuData);
        void TransferData(G4double, SimuData);
        void TransferData(std::vector<G4int>, SimuData);
        void TransferData(std::vector<G4double>, SimuData);

    private:
        PrimaryGenerator *fPrimaryGen;
        TFile *fileRun;
        TTree *treeEvt;
        G4int EvtID;
        G4double ParticleEnergy;
        G4int ftagNulabel;
        G4double Interaction_x;
        G4double Interaction_y;
        G4double Interaction_z;
        G4double CaloEdepSum;
        G4double CaloVisibleEdepSum;
        G4double EcalEdepSum;
        G4double EcalVisibleEdepSum;
        G4double HcalEdepSum;
        G4double HcalVisibleEdepSum;
        G4double EcalMaxEdepCell;
        G4double HcalMaxEdepCell;
        std::vector<G4int> vecEcalCellID;
        std::vector<G4int> vecEcalStepsCell;
        std::vector<G4double> vecEcalEdepCell;
        std::vector<G4double> vecEcalVisibleEdepCell;
        std::vector<G4double> vecEcalHitTimeCell;
        std::vector<G4double> vecEcalToaCell;
        std::vector<G4int> vecHcalCellID;
        std::vector<G4int> vecHcalStepsCell;
        std::vector<G4double> vecHcalEdepCell;
        std::vector<G4double> vecHcalVisibleEdepCell;
        std::vector<G4double> vecHcalHitTimeCell;
        std::vector<G4double> vecHcalToaCell;
        G4int nstoredTruthParticles;
        std::vector<G4int> vecTruth_pdgID;
        std::vector<G4double> vecTruth_px;
        std::vector<G4double> vecTruth_py;
        std::vector<G4double> vecTruth_pz;
        std::vector<G4double> vecTruth_x;
        std::vector<G4double> vecTruth_y;
        std::vector<G4double> vecTruth_z;
        std::vector<G4double> vecTruth_energy;
        std::vector<G4int> vecTruth_vertexIndex;
        std::vector<G4int> vecTruth_trackID;
    };
}

#endif
