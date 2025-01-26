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
        // if (track->GetParentID() == 0 && track->GetTrackID() == 1)
        // {
        //     if(track->GetVolume()->GetLogicalVolume()->GetName() == "World_Logical")
        //     {
        //         fpTrackingManager->EventAborted();
        //     }
        //     fpTrackingManager->SetStoreTrajectory(true);
        // }else if (track->GetParentID() == 1){
        //     fpTrackingManager->SetStoreTrajectory(true);
        // }else{
        //     fpTrackingManager->SetStoreTrajectory(false);
        // }
        G4ThreeVector vposition = track->GetVertexPosition()
        Bool_t is_x = abs(vposition.x() -850*mm )< 500*mm;
        Bool_t is_y = abs(vposition.y() -330*mm )< 500*mm;
        Bool_t isFront_z = (vposition.z()-5570*mm)< 0;
        Bool_t isFront = is_x && is_y && isFront_z;
        if ( isFront )
        {
            fpTrackingManager->SetStoreTrajectory(true);
            
        }else if( track->GetParentID() == 0 && track->GetTrackID() == 1){
            fpTrackingManager->SetStoreTrajectory(true);
        }
        {
            fpTrackingManager->SetStoreTrajectory(false);
        }
    }
    void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    }
}