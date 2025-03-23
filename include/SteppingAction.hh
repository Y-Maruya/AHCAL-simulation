#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>
#include <G4Event.hh>
#include <EventAction.hh>

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

private:
    // Add any private members or methods here
    SimCalModule::EventAction* fEventAction;
};

#endif // STEPPINGACTION_HH