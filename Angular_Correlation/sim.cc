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

int Largest()
{
    float largest = -1.0;

    std::fstream myfile;
    myfile.open("output.txt", std::ios::in);

    float a;
    while (myfile >> a)
    {
        if (a > largest)
        {
            largest = a;
        }
    }
    myfile.close();

    // std::cout << trunc(largest) << std::endl;

    return round(largest);
}

void Count(int arr[])
{
    std::fstream myfile;
    myfile.open("output.txt", std::ios::in);

    float a, sum = 0; 
    while (myfile >> a)
    {   
        sum = sum + a; 
        int num = round(a);
        // std::cout << trunc(a) << std::endl;
        arr[num]++;
    }

    G4cout << G4endl
		<< " Total Energy deposited: " << sum
		<< "KeV"
		<< G4endl;

    myfile.close();
}

void OUT()
{
    int n = Largest() + 1;
    int arr[n] = {0}; // largest =  2799 -> a = 2800 -> used indices are 0-2799
    Count(arr);

    std::fstream myfile;
    myfile.open("count.txt", std::ios::out);
    myfile << "KeV : Count" << std::endl;
    int total = 0;
    for (int i = 0; i < n; i++)
    {
        myfile << i << " : " << arr[i] << std::endl;
        if (i != 0)
        {
            total += arr[i];
        }
    }

    myfile << "Total rays: " << total << std::endl;
    myfile.close();
}

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
    OUT();

    return 0;
}
