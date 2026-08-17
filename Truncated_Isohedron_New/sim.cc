#include <iostream>
#include <fstream>
#include <cmath>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

void Interactive_Mode(int argc, char **argv)
{
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
    
    G4RunManager *runManager = new G4RunManager();


    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyActionInitialization());
    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute in.mac");
    ui->SessionStart();
    delete ui;
    delete visManager;
    delete runManager;
}

void Batch_Mode(int argc, char **argv)
{
    G4MTRunManager *runManager = new G4MTRunManager();
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyActionInitialization());
    // runManager->Initialize();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String macros_name = argv[1];
    UImanager->ApplyCommand(command + macros_name);
    delete runManager;
}

int main(int argc, char **argv)
{
    if(argc==1)
        Interactive_Mode(argc, argv);
    else
        Batch_Mode(argc, argv);
   

    return 0;
}
