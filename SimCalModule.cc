#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysListFactoryAlt.hh"
#include "G4PhysListRegistry.hh"
#include "G4GDMLParser.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicsConstructorFactory.hh"

#include "FTFP_BERT.hh"
#include "Randomize.hh"
#include "Pythia8DecayerPhysics.hh"

using namespace SimCalModule;
void PrintAvailable(G4int verbosity)
{
    G4cout << G4endl;
    G4cout << "extensibleFactory: here are the available physics lists:" << G4endl;
    g4alt::G4PhysListFactory factory;
    factory.PrintAvailablePhysLists();

    // if user asked for extra verbosity then print physics ctors as well
    if (verbosity > 1) {
        G4cout << G4endl;
        G4cout << "extensibleFactory: "
               << "here are the available physics ctors that can be added:" << G4endl;
        G4PhysicsConstructorRegistry* g4pctorFactory = G4PhysicsConstructorRegistry::Instance();
        g4pctorFactory->PrintAvailablePhysicsConstructors();
    }
}

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    std::time_t systime = std::time(nullptr);
    G4int seed = (long)systime;
    if (argc >= 3) {
        seed = atoi(argv[2]);  // OR: seed = atoi(argv[2]) * seed;
    }
    G4Random::setTheSeed(seed);

    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    auto Detector = new DetectorConstruction();
    runManager->SetUserInitialization(Detector);

    std::string physListName = "FTFP_BERT+PY8DK";
    g4alt::G4PhysListFactory plFactory;
    G4VModularPhysicsList* physicsList = nullptr;
    plFactory.SetDefaultReferencePhysList("NO_DEFAULT_PHYSLIST");

    // set a short name for the plugin
    G4PhysListRegistry* plReg = G4PhysListRegistry::Instance();
    plReg->AddPhysicsExtension("PY8DK", "Pythia8DecayerPhysics");

    physicsList = plFactory.GetReferencePhysList(physListName);

    if (!physicsList) {
        PrintAvailable(1);

        // if we can't get what the user asked for...
        //    don't go on to use something else, that's confusing
        G4ExceptionDescription ed;
        ed << "The factory for the physicslist [" << physListName << "] does not exist!" << G4endl;
        G4Exception("extensibleFactory", "extensibleFactory001", FatalException, ed);
        exit(42);
    }

    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization);

    // initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (ui) {
        // interactive mode
        UImanager->ApplyCommand("/control/execute vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    } else {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // GDML output
    if (argc == 4) {
        G4GDMLParser parser;
        parser.SetOutputFileOverwrite(true);
        parser.Write(argv[3], G4TransportationManager::GetTransportationManager()
                                  ->GetNavigatorForTracking()
                                  ->GetWorldVolume()
                                  ->GetLogicalVolume());
    }

    // job termination
    delete visManager;
    delete runManager;
    return 0;
}
