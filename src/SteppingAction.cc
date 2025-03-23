#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include <G4RunManager.hh>
#include "G4EventManager.hh"
#include <fstream>

SteppingAction::SteppingAction() : G4UserSteppingAction() {
    fEventAction = nullptr;
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4double zplane = 10.0 * mm;
    fEventAction = static_cast<SimCalModule::EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
    G4StepPoint* preStep = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();

    G4ThreeVector prePos = preStep->GetPosition();
    G4ThreeVector postPos = postStep->GetPosition();
    if (prePos.z() < zplane && postPos.z() > zplane) {
        auto track = step->GetTrack();
        int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
        G4int trackID = track->GetTrackID();
        G4int parentID = track->GetParentID();
        G4double energy = track->GetTotalEnergy() / GeV;
        G4double px = track->GetMomentum().x() / GeV;
        G4double py = track->GetMomentum().y() / GeV;
        G4double pz = track->GetMomentum().z() / GeV;
        G4double vx = prePos.x() / mm;
        G4double vy = prePos.y() / mm;
        G4double vz = prePos.z() / mm;        
        int numuCC = fEventAction->GetfNumuCClabel();
        int primary_trackid = fEventAction->GetPrimary_trackid();
        int D_id = fEventAction->GetfD_id();
        int primary_Dmeson = 0;
        if (trackID == primary_trackid) {
            primary_Dmeson = 1;
        }
        if (numuCC >= 0) {
            if (abs(pdgID) == 13 && parentID == D_id) {
                primary_Dmeson = 2;
            }
        }
        fEventAction->AddPlaneParticle(pdgID, px, py, pz,energy,parentID,trackID, vx, vy, vz, primary_Dmeson);
    }
}