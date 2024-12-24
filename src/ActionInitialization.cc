#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"
#include "EventAction.hh"
#include "RunAction.hh"

namespace SimCalModule
{
    ActionInitialization::ActionInitialization()
        : G4VUserActionInitialization()
    {
    }

    ActionInitialization::~ActionInitialization()
    {
    }

    void ActionInitialization::BuildForMaster() const
    {
        PrimaryGenerator *primary = new PrimaryGenerator;
        SetUserAction(new RunAction(primary));
    }

    void ActionInitialization::Build() const
    {
        PrimaryGenerator *primary = new PrimaryGenerator;
        SetUserAction(primary);
        RunAction *run = new RunAction(primary);
        SetUserAction(run);
        SetUserAction(new EventAction(run));
    }
}