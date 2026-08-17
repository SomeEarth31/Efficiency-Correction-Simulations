#include "event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh" // Make sure this is included for CLHEP units
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

EventAction::EventAction(RunAction *runAction)
    : fRunAction(runAction)
{
	// myfile.open("output.txt", std::ios::out | std::ios::trunc);
  // myfile.close();
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{
  fEdep = 0.;
  // myfile.open("output.txt", std::ios::app);
}

void EventAction::EndOfEventAction(const G4Event *event)
{
 fRunAction->AddEdep(fEdep);
    
    if (fEdep > 0.)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        G4double energy_keV = fEdep / CLHEP::keV;

        man->FillH1(0, energy_keV);
        man->FillNtupleDColumn(0, energy_keV);
        man->AddNtupleRow();
    }

}
