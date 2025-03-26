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
    G4double zplane = 7282.2 * mm;
    fEventAction = const_cast<SimCalModule::EventAction*>(static_cast<const SimCalModule::EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));
    if (!fEventAction){
        G4cout<<"eventactioneeror"<<G4endl;
    }
    G4StepPoint* preStep = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();

    G4ThreeVector prePos = preStep->GetPosition();
    G4ThreeVector postPos = postStep->GetPosition();
    int numuCC = fEventAction->GetfNumuCClabel();
    int primary_trackid = fEventAction->GetPrimary_trackid();
    int D_id = fEventAction->GetfD_id();
    auto track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    if (numuCC > 0) {
        if (trackID == D_id) {
            if (track->GetTrackStatus() == fStopAndKill) {  // D メソンが消滅した場合
                G4cout << "D meson interaction at position: " 
                       << track->GetPosition() / mm << " mm" << G4endl;
                const G4VProcess* process = track->GetCreatorProcess();
                if (process) {
                    G4cout << "D meson disappeared due to: " << process->GetProcessName() << G4endl;
                }
                // 生成された二次粒子（崩壊 or 非弾性散乱の結果）を取得
                const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
                if (!secondaries->empty()) {
                    G4cout << "Outgoing particles from D meson reaction:" << G4endl;
                    for (const G4Track* secTrack : *secondaries) {
                        G4cout << " - " << secTrack->GetDefinition()->GetParticleName()
                               << " (Momentum: " << secTrack->GetMomentum().mag() / GeV << " GeV)"
                               << G4endl;
                    }
                }
            }
        }
    }
    if (prePos.z() < zplane && postPos.z() > zplane) {
        int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
        G4int parentID = track->GetParentID();
        G4double energy = track->GetTotalEnergy() / GeV;
        G4double px = track->GetMomentum().x() / GeV;
        G4double py = track->GetMomentum().y() / GeV;
        G4double pz = track->GetMomentum().z() / GeV;
        G4double vx = prePos.x() / mm;
        G4double vy = prePos.y() / mm;
        G4double vz = prePos.z() / mm;        
        int primary_Dmeson = 0;
        if (trackID == primary_trackid) {
            primary_Dmeson = 1;
        }
        if (numuCC > 0) {
            if (trackID == D_id) {
                primary_Dmeson = 2;
                G4cout<<"D meson decay to muon"<<G4endl;
            }
        }
        fEventAction->AddPlaneParticle(pdgID, px, py, pz,energy,parentID,trackID, vx, vy, vz, primary_Dmeson);
    }
}