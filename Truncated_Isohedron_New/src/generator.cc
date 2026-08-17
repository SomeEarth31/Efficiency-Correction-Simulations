#include "generator.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "messenger.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr),
   fEnergy(511 * keV),   // Default gamma energy
   fSourceType("Na22")   // Default source type
{
	//Particle Gun
	fParticleGun = new G4ParticleGun(1);
	
	// G4ThreeVector pos(0., 0., -2.);
	G4ThreeVector pos(0., 0., 0.);
	// G4ThreeVector mom(0., 0., 0.);

	fParticleGun->SetParticlePosition(pos);
	fMessenger = new GeneratorMessenger(this); // Attach messenger

}


MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
	delete fMessenger;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{

	if (fSourceType == "Na22") 
	{
		G4IonTable* ionTable = G4IonTable::GetIonTable();
    	G4ParticleDefinition* ion = ionTable->GetIon(11, 22, 0.0);

		if (ion) {
			ion->SetPDGLifeTime(1. * ns); // Force decay within 1 nanosecond
		}


		fParticleGun -> SetParticleDefinition(ion);
		fParticleGun -> SetParticleCharge(0.);
		fParticleGun -> SetParticleEnergy(0.);
	}
	else if(fSourceType == "Mn52m")
	{

		G4IonTable* ionTable = G4IonTable::GetIonTable();
		
		// Mn-52m metastable state (excitation energy ~377.7 keV)
    	G4double excitationEnergy = 377.7 * keV;
		G4ParticleDefinition* ion = ionTable->GetIon(25, 52, excitationEnergy);

		if (ion) {
			ion->SetPDGLifeTime(1. * ns); 
		}
		
		fParticleGun -> SetParticleDefinition(ion);
		fParticleGun -> SetParticleCharge(0.);
		fParticleGun -> SetParticleEnergy(0.);
	}
	else if(fSourceType == "Sc44")
	{
		G4IonTable* ionTable = G4IonTable::GetIonTable();
		G4ParticleDefinition* ion = ionTable->GetIon(21, 44, 0.0);

		if (ion) {
			ion->SetPDGLifeTime(1. * ns); 
		}
		
		fParticleGun -> SetParticleDefinition(ion);
		fParticleGun -> SetParticleCharge(0.);
		fParticleGun -> SetParticleEnergy(0.);
	}
	else if(fSourceType == "O14")
	{
	    G4IonTable* ionTable = G4IonTable::GetIonTable();
	    G4ParticleDefinition* ion = ionTable->GetIon(8, 14, 0.0);  // Oxygen is Z=8, Mass is A=14

	    if (ion) {
	        ion->SetPDGLifeTime(1. * ns); 
	    }
	
	    fParticleGun->SetParticleDefinition(ion);
	    fParticleGun->SetParticleCharge(0.);
	    fParticleGun->SetParticleEnergy(0.);  
	}
	else if (fSourceType == "Gamma") 
	{
		// Monoenergetic Gamma Source
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");

        fParticleGun->SetParticleDefinition(gamma);
        fParticleGun->SetParticleEnergy(fEnergy); // Use the energy set in the macro
        fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
	}
	fParticleGun->GeneratePrimaryVertex(anEvent);
}


