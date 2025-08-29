#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "EventAction.hh"
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
        if (track->GetParentID() == 0 )
        {
            // if(track->GetVolume()->GetLogicalVolume()->GetName() == "World_Logical")
            // {
            //     fpTrackingManager->EventAborted();
            // }
            fpTrackingManager->SetStoreTrajectory(true);
        // }else if (track->GetParentID() < 20){
        //     fpTrackingManager->SetStoreTrajectory(true);
        }else{
            fpTrackingManager->SetStoreTrajectory(false);
        }
        auto particle = track->GetParticleDefinition();
        auto fEventAction = const_cast<SimCalModule::EventAction*>(static_cast<const SimCalModule::EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));
        if(!fEventAction){
            G4cout << "EventAction not found!" << G4endl;
            return;
        }
        int D_id = fEventAction->GetfD_id();
        int primary_trackid = fEventAction->GetPrimary_trackid();
        if (track->GetTrackID() == primary_trackid || track->GetParentID() == primary_trackid) {
            fpTrackingManager->SetStoreTrajectory(true);
        }
        if (track->GetTrackID() == D_id || track->GetParentID() == D_id) {
            fpTrackingManager->SetStoreTrajectory(true);
        }
        // G4ThreeVector vposition = track->GetVertexPosition();
        // // Bool_t is_x = abs(vposition.x() -850*mm )< 500*mm;
        // // Bool_t is_y = abs(vposition.y() -330*mm )< 500*mm;
        // // Bool_t isFront_z = (vposition.z()-5570*mm)< 0;
        // // Bool_t isFront = is_x && is_y && isFront_z;
        // if ( abs(vposition.x() -850 * mm )< 500 * mm && abs(vposition.y() -330 * mm )< 500 * mm)
        // {
        //     fpTrackingManager->SetStoreTrajectory(true);
            
        // }else if( track->GetParentID() == 0 && track->GetTrackID() == 1){
        //     fpTrackingManager->SetStoreTrajectory(true);
        // }
        // {
        //     fpTrackingManager->SetStoreTrajectory(false);
        // }
        
    }
    void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    }
}