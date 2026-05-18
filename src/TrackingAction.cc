#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

namespace SimCalModule
{
    TrackingAction::TrackingAction() : G4UserTrackingAction() {}

    TrackingAction::~TrackingAction() {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track)
    {
        fpTrackingManager->SetTrajectory(new G4Trajectory(track));
        if (track->GetParentID() == 0) {
            fpTrackingManager->SetStoreTrajectory(true);
        } else if (track->GetParentID() < 20) {
            fpTrackingManager->SetStoreTrajectory(true);
        } else {
            fpTrackingManager->SetStoreTrajectory(false);
        }
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track)
    {
    }
}
