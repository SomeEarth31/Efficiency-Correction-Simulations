#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	//Debug Messages
	SetVerboseLevel(1);

	RegisterPhysics(new G4EmStandardPhysics());
	// RegisterPhysics (new G4OpticalPhysics());
	RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4RadioactiveDecayPhysics());

	//Enable Decay
	G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
    radioactiveDecay->SetVerboseLevel(1);
}

MyPhysicsList::~MyPhysicsList()
{
}

void MyPhysicsList::SetCuts()
{
	SetCutValue(0.1 * mm, "gamma");
}