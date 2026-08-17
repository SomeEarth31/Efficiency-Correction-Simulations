#ifndef MESSENGER_HH
#define MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
// #include "generator.hh"

class MyPrimaryGenerator;

class GeneratorMessenger : public G4UImessenger
{
public:
    GeneratorMessenger(MyPrimaryGenerator* generator);
    ~GeneratorMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    MyPrimaryGenerator* fGenerator;
    G4UIcmdWithAString* fSourceTypeCmd;
    G4UIcmdWithADoubleAndUnit* fEnergyCmd;
};

#endif
