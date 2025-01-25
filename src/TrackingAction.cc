#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
namespace SimCalModule
{
    TrackingAction::TrackingAction() : G4UserTrackingAction() {}

    TrackingAction::~TrackingAction() {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track) {
        fpTrackingManager->SetTrajectory(new G4Trajectory(track));
        // if ((track->GetVolume()->GetLogicalVolume()->GetName() == "TrenchAndWallLog" || track->GetVolume()->GetLogicalVolume()->GetName() == "World_Logical")
        // && (track->GetNextVolume()->GetLogicalVolume()->GetName() == "TrenchAndWallLog" || track->GetNextVolume()->GetLogicalVolume()->GetName() == "World_Logical"))
        // {
        //     if(track->GetParentID() > 10){
        //         fpTrackingManager->SetStoreTrajectory(false);
        //     }
        //     fpTrackingManager->SetStoreTrajectory(true);
        // }    
        // else
        //     fpTrackingManager->SetStoreTrajectory(true);
        if (track->GetParentID() == 0 && track->GetTrackID() == 1)
        {
            if(track->GetVolume()->GetLogicalVolume()->GetName() == "World_Logical")
            {
                fpTrackingManager->EventAborted();
            }
            fpTrackingManager->SetStoreTrajectory(true);
        }else if (track->GetParentID() == 1){
            fpTrackingManager->SetStoreTrajectory(true);
        }else{
            fpTrackingManager->SetStoreTrajectory(false);
        }
    }
    void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    }
}