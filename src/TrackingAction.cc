#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
namespace SimCalModule
{
    TrackingAction::TrackingAction() : G4UserTrackingAction() {}

    TrackingAction::~TrackingAction() {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track) {
        fpTrackingManager->SetTrajectory(new G4Trajectory(track));
        // fpTrackingManager->SetStoreTrajectory(true);
        if ((track->GetVolume()->GetLogicalVolume()->GetName() == "TrenchAndWallLog" || track->GetVolume()->GetLogicalVolume()->GetName() == "World_Logical")
        && (track->GetNextVolume()->GetLogicalVolume()->GetName() == "TrenchAndWallLog" || track->GetNextVolume()->GetLogicalVolume()->GetName() == "World_Logical"))
        {
            if(track->GetParentID() != 0){
                track->SetStoreTrajectory(false);
            }
        }else{
            track->SetStoreTrajectory(true);
        }
    }
    void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    }
}