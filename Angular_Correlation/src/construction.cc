#include "construction.hh"
#include "CADMesh.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	detecColor = new G4VisAttributes(G4Colour(0., 0., 0.55));
	caseColor = new G4VisAttributes(G4Colour(0.75, 0., 0.35));
	plasColor = new G4VisAttributes(G4Colour(0.75, 0., 0.));
	cageColor = new G4VisAttributes(G4Colour(0., 0.55, 0.));
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	//-------------------WORLD-----------------------------// 
	// This is used to define a material
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

	// This is the process to define volume, there are three volume types that are required#
	G4Box *solidWorld = new G4Box("solidWorld", 1 * m, 1 * m, 1 * m); // default unit of length is meters, so multiplying by m will put it in meter

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

	G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0, true);

	//-------------------CRYSTAL-----------------------------//

	G4Material *NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

	auto detector = CADMesh::TessellatedMesh::FromPLY("Trunc_Icosa_STL2.STL");
	detector->SetOffset(G4ThreeVector()); // Find correct offset

	G4VSolid *solidDetec = detector->GetSolid();
	G4LogicalVolume *logicDetec = new G4LogicalVolume(solidDetec, NaI, "logicaDetec");
	G4VPhysicalVolume *physDetec = new G4PVPlacement(nullptr, G4ThreeVector(-16.4 * cm, -17.8 * cm, -16.2 * cm), logicDetec, "physDetec", logicWorld, false, 0, true);
	logicDetec->SetVisAttributes(detecColor);

	fScoringVolume = logicDetec;

	//-------------------Aluminium-----------------------------//

	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

	auto casing = CADMesh::TessellatedMesh::FromPLY("Trunc_Icosa_Al_STL3.STL");
	casing->SetOffset(G4ThreeVector()); // Find correct offset

	G4VSolid *solidCase = casing->GetSolid();
	G4LogicalVolume *logicCase = new G4LogicalVolume(solidCase, Al, "logicaCase");
	G4VPhysicalVolume *physCase = new G4PVPlacement(nullptr, G4ThreeVector(-16.4 * cm, -17.8 * cm, -16.2 * cm), logicCase, "physCase", logicWorld, false, 0, true);
	logicCase->SetVisAttributes(caseColor);

	//-------------------Plastic-----------------------------//

	G4Material *Plastic = new G4Material("Plsatic", 1.032 * g / cm3, 2);
	Plastic->AddElement(nist->FindOrBuildElement("C"), 50. * perCent);
	Plastic->AddElement(nist->FindOrBuildElement("H"), 50. * perCent);

	G4VSolid *solidPlas = new G4Tubs("solidPlas", 0. * cm, 1.27 * cm, 3., 0. * deg, 360. * deg); // crystal
	G4LogicalVolume *logicPlas = new G4LogicalVolume(solidPlas, Plastic, "logicPlas");
	G4VPhysicalVolume *physPlas = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -2.), logicPlas, "physPlas", logicWorld, false, 0, true);
	logicPlas->SetVisAttributes(plasColor);
	
	//--------------Al Cylinder Around Source--------------------//
	G4VSolid *Cage = new G4Tubs("Cage", 6 * cm, 6.2 * cm, 3* cm, 0. * deg, 360. * deg); // crystal
	G4LogicalVolume *logicCage = new G4LogicalVolume(Cage, Al, "logicCage");
	G4VPhysicalVolume *physCage = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -2.), logicCage, "physCage", logicWorld, false, 0, true);
	logicCage->SetVisAttributes(cageColor);

	return physWorld;
}
