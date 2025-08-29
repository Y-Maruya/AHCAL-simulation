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
        SecondaryEnergy_Data,
        Secondarypdgid_Data,
        SecondaryMomentum_px_Data,
        SecondaryMomentum_py_Data,
        SecondaryMomentum_pz_Data,
        fNumuCClabel_Data,
        fD_id_Data,
        Primary_trackid_Data,
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
        vecTruth_charge_Data,
        vecTruth_px_Data,
        vecTruth_py_Data,
        vecTruth_pz_Data,
        vecTruth_pre_x_Data,
        vecTruth_pre_y_Data,
        vecTruth_pre_z_Data,
        vecTruth_post_x_Data,
        vecTruth_post_y_Data,
        vecTruth_post_z_Data,
        vecTruth_energy_Data,
        vecTruth_vertexIndex_Data,
        vecTruth_trackID_Data,
        nstoredPlaneParticles_Data,
        vecPlane_pdgID_Data,
        vecPlane_charge_Data,
        vecPlane_globalTime_Data,
        vecPlane_px_Data,
        vecPlane_py_Data,
        vecPlane_pz_Data,
        vecPlane_x_Data,
        vecPlane_y_Data,
        vecPlane_z_Data,
        vecPlane_energy_Data,
        vecPlane_vertexIndex_Data,
        vecPlane_trackID_Data,
        vecPlane_primary_Dmeson_Data,
        vecPlane_zplane_Data
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
        G4double SecondaryEnergy;
        G4int Secondarypdgid;
        G4double SecondaryMomentum_px;
        G4double SecondaryMomentum_py;
        G4double SecondaryMomentum_pz;
        G4int fNumuCClabel;
        G4int fD_id;
        G4int fPrimary_trackid;  
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
        std::vector<G4double> vecTruth_charge;
        std::vector<G4double> vecTruth_px;
        std::vector<G4double> vecTruth_py;
        std::vector<G4double> vecTruth_pz;
        std::vector<G4double> vecTruth_pre_x;
        std::vector<G4double> vecTruth_pre_y;
        std::vector<G4double> vecTruth_pre_z;
        std::vector<G4double> vecTruth_post_x;
        std::vector<G4double> vecTruth_post_y;
        std::vector<G4double> vecTruth_post_z;
        std::vector<G4double> vecTruth_energy;
        std::vector<G4int> vecTruth_vertexIndex;
        std::vector<G4int> vecTruth_trackID;
        G4int nstoredPlaneParticles;
        std::vector<G4int> vecPlane_pdgID;
        std::vector<G4double> vecPlane_charge;
        std::vector<G4double> vecPlane_globalTime;
        std::vector<G4double> vecPlane_px;
        std::vector<G4double> vecPlane_py;
        std::vector<G4double> vecPlane_pz;
        std::vector<G4double> vecPlane_x;
        std::vector<G4double> vecPlane_y;
        std::vector<G4double> vecPlane_z;
        std::vector<G4double> vecPlane_energy;
        std::vector<G4int> vecPlane_vertexIndex;
        std::vector<G4int> vecPlane_trackID;
        std::vector<G4int> vecPlane_primary_Dmeson;
        std::vector<G4double> vecPlane_zplane;
    };
}

#endif
