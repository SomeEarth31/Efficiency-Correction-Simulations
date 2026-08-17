#include "messenger.hh"
#include "G4SystemOfUnits.hh"
#include "generator.hh"

GeneratorMessenger::GeneratorMessenger(MyPrimaryGenerator* generator)
    : G4UImessenger(), fGenerator(generator)
{
    // Create command for setting gamma energy
    fEnergyCmd = new G4UIcmdWithADoubleAndUnit("/gun/Energy", this);
    fEnergyCmd->SetGuidance("Set gamma energy (for Gamma source).");
    fEnergyCmd->SetParameterName("energy", false);
    fEnergyCmd->SetUnitCategory("Energy");
    fEnergyCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);

    // Create command for selecting source type
    fSourceTypeCmd = new G4UIcmdWithAString("/gun/sourceType", this);
    fSourceTypeCmd->SetGuidance("Set the source type: Na22 or Mn-52m or Sc-44 or Gamma.");
    fSourceTypeCmd->SetParameterName("source", false);
    fSourceTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


}

GeneratorMessenger::~GeneratorMessenger()
{
    delete fSourceTypeCmd;
    delete fEnergyCmd;
}

void GeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fSourceTypeCmd) 
    {
        fGenerator->SetSourceType(newValue);
        G4cout << "Source type set to: " << newValue << G4endl;
    } 
    else if (command == fEnergyCmd) 
    {
        G4double energy = fEnergyCmd->GetNewDoubleValue(newValue);
        fGenerator->SetEnergy(energy);
        G4cout << "Gamma energy set to: " << energy / keV << " keV" << G4endl;
    }
}
