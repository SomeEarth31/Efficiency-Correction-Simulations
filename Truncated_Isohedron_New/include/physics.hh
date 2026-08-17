#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4RadioactiveDecay.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"


class MyPhysicsList : public G4VModularPhysicsList
{
public:
	MyPhysicsList();
	~MyPhysicsList();

    void SetCuts() override;

};

#endif
