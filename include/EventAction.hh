#ifndef EventAction_h
#define EventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "RunAction.hh"
#include <vector>

namespace SimCalModule
{
    class RunAction;

    class EventAction : public G4UserEventAction
    {
    public:
        EventAction(RunAction *);
        ~EventAction() override;
        void BeginOfEventAction(const G4Event *) override;
        void EndOfEventAction(const G4Event *) override;
        void ResetEventData();
        void AddParticle(int pdgID, double px, double py, double pz, double energy, int vertexIndex, int trackID);

    private:
        RunAction *fRunAction;
        G4int fEcalCollID;
        G4int fHcalCollID;
        G4int EvtID;
        G4double ParticleEnergy;
        G4int ftagNulabel;
        G4double interaction_x;
        G4double interaction_y;
        G4double interaction_z;
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

