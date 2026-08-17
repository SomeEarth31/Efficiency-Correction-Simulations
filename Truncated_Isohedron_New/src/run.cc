#include "run.hh"

RunAction::RunAction()
{
	G4RunManager::GetRunManager()->SetPrintProgress(1000);

	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetVerboseLevel(1);
	man->SetNtupleMerging(true);

	// Create a 1D Histogram (ID=0, Name, Title, Bins, Xmin, Xmax)
    man->CreateH1("Energy_Spectrum", "Energy Deposited (keV)", 3000, 0., 3000.);

	// Create the Ntuple (Tree) for event-by-event Python analysis
    man->CreateNtuple("GammaData", "Energy Data");
	man->CreateNtupleDColumn("fEnergy"); //Column 0
	man->FinishNtuple();
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->OpenFile("output.root");
}

void RunAction::EndOfRunAction(const G4Run *run)
{

	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();

}

void RunAction::AddEdep(G4double edep)
{
	fEdep += edep;
}
