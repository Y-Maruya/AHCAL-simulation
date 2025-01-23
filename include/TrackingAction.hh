#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Trajectory.hh"
namespace SimCalModule
{
    class TrackingAction : public G4UserTrackingAction {
    public:
        TrackingAction();
        ~TrackingAction() override;

        void PreUserTrackingAction(const G4Track*) override;
        void PostUserTrackingAction(const G4Track*) override;
    };
}
#endif // TRACKINGACTION_HH