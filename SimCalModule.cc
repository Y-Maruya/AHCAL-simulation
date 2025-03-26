#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "Py8DecayerPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4DecayPhysics.hh"
#include "Randomize.hh"

#include "G4GDMLParser.hh"
#include "G4TransportationManager.hh"

using namespace SimCalModule;
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4ParticleDefinition.hh"

void PrintDmesonDecayModes() {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* Dplus = particleTable->FindParticle("D0");

    if (Dplus) {
        G4DecayTable* decayTable = Dplus->GetDecayTable();
        if (decayTable) {
            G4cout << "Decay modes for D0 meson:" << G4endl;
            decayTable->DumpInfo();
        } else {
            G4cout << "No decay table found for D0!" << G4endl;
        }
    } else {
        G4cout << "D+ meson not found in particle table!" << G4endl;
    }
}
int main(int argc, char **argv)
{
    G4UIExecutive *ui = nullptr;
    if (argc == 1)
        ui = new G4UIExecutive(argc, argv);

    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    std::time_t systime = std::time(nullptr);
    G4int seed = (long)systime;
    if (argc >= 3)
        seed = atoi(argv[2]); // OR: seed = atoi(argv[2]) * seed;
    G4Random::setTheSeed(seed);

	// auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
	auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
    // G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 10);
	// runManager->SetNumberOfThreads(nThreads);

    auto Detector = new DetectorConstruction();
    runManager->SetUserInitialization(Detector);
    G4PhysListRegistry* plReg = G4PhysListRegistry::Instance();
    plReg->AddPhysicsExtension("PY8DK", "Py8DecayerPhysics");
    auto physicsList = new FTFP_BERT;
    // physicsList->RegisterPhysics(new G4DecayPhysics());
    // physicsList->SetDefaultCutValue(0.05 * mm);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new ActionInitialization);

    // initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    // get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if (ui)
    {
        // interactive mode
        UImanager->ApplyCommand("/control/execute vis.mac");
        if (ui->IsGUI())
        {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // GDML
    if (argc == 4)
    {
        G4GDMLParser parser;
        // parser.SetRegionExport(true);
        // parser.SetEnergyCutsExport(true);
        // parser.SetSDExport(true);
        parser.SetOutputFileOverwrite(true);
        parser.Write(argv[3], G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume());
    }
    PrintDmesonDecayModes();
    // job termination
    delete visManager;
    delete runManager;
    return 0;
}
