#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
namespace SimCalModule
{
    TrackingAction::TrackingAction() : G4UserTrackingAction() {}

    TrackingAction::~TrackingAction() {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track) {
        fpTrackingManager->SetTrajectory(new G4Trajectory(track));
        fpTrackingManager->SetStoreTrajectory(true);
        if (track->GetVolume()->GetLogicalVolume()->GetName() 
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    }
}