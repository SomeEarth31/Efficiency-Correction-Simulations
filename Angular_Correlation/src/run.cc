#include "run.hh"

RunAction::RunAction()
{
	G4RunManager::GetRunManager()->SetPrintProgress(1000);

	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetVerboseLevel(1);
	man->SetNtupleMerging(true);
	man->CreateNtuple("No. of Gamma Rays", "No. of Gamma Rays vs. Energy");
	man->CreateNtupleDColumn("fEnergy");
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
