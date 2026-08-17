#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"


#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4IonTable.hh" 
#include "G4RandomDirection.hh"

class GeneratorMessenger; 

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	 MyPrimaryGenerator();
	 ~MyPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event* );

	// Set energy in macro
	void SetEnergy(G4double energy) { fEnergy = energy; } 
    void SetSourceType(G4String type) { fSourceType = type; } // Choose Na-22 or gamma

private:
	// G4GeneralParticleSource* fParticleGun;
	G4ParticleGun *fParticleGun;
	GeneratorMessenger* fMessenger;
	G4double fEnergy;
    G4String fSourceType;
};


#endif
