#include "construction.hh"
#include "CADMesh.hh"
#include "G4SubtractionSolid.hh"

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

//-------------------Materials-----------------------------//
    
    G4Material *NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    G4Material *SSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    //-------------------Dimensions-----------------------------//

    // Crystal dimensions (10.2 cm x 10.2 cm x 40.6 cm)
    // Geant4 G4Box uses HALF-lengths
    G4double crys_hx = 5.1 * cm;
    G4double crys_hy = 5.1 * cm;
    G4double crys_hz = 20.3 * cm;

    G4double gap = 2.0 * mm;
    G4double case_thick = 1.0 * mm;

    // Inner hollow space (Crystal + gap)
    G4double inner_hx = crys_hx + gap;
    G4double inner_hy = crys_hy + gap;
    G4double inner_hz = crys_hz + gap;

    // Outer casing size (Inner space + casing thickness)
    G4double outer_hx = inner_hx + case_thick;
    G4double outer_hy = inner_hy + case_thick;
    G4double outer_hz = inner_hz + case_thick;

    //-------------------Solids & Logic Volumes-----------------------------//

    // 1. NaI Crystal
    G4VSolid *solidDetec = new G4Box("solidDetec", crys_hx, crys_hy, crys_hz);
    G4LogicalVolume *logicDetec = new G4LogicalVolume(solidDetec, NaI, "logicDetec");
    logicDetec->SetVisAttributes(detecColor); // Assuming detecColor is defined elsewhere
    
    // 2. Stainless Steel Casing (Hollowed out)
    G4Box *outerBox = new G4Box("outerBox", outer_hx, outer_hy, outer_hz);
    G4Box *innerBox = new G4Box("innerBox", inner_hx, inner_hy, inner_hz);
    
    // Subtract inner box from outer box to create a 1mm thick shell with a 2mm internal gap
    G4VSolid *solidCase = new G4SubtractionSolid("solidCase", outerBox, innerBox);
    G4LogicalVolume *logicCase = new G4LogicalVolume(solidCase, SSteel, "logicCase");
    logicCase->SetVisAttributes(caseColor); // Assuming caseColor is defined elsewhere

    //-------------------Placements (4 Detectors)-----------------------------//

    // Calculate center-to-center offset to place them flush against each other's casings
    // Total width of one detector casing is outer_hx * 2
    G4double offset = outer_hx * 2.0;

    // Placed in a cross around the center origin (Top, Bottom, Left, Right)
    std::vector<G4ThreeVector> detectorPositions = {
        G4ThreeVector(offset, 0., 0.),   // Right
        G4ThreeVector(-offset, 0., 0.),  // Left
        G4ThreeVector(0., offset, 0.),   // Top
        G4ThreeVector(0., -offset, 0.)   // Bottom
    };

    // Note: If you ever want the full 8-detector array from image_1ef3a7.png, 
    // you would just add the 4 corners: (offset, offset, 0), (-offset, offset, 0), etc.

    for (size_t i = 0; i < detectorPositions.size(); ++i) {
        // Place Crystal
        new G4PVPlacement(nullptr, detectorPositions[i], logicDetec, 
                          "physDetec_" + std::to_string(i), logicWorld, false, i, true);
        
        // Place Casing
        new G4PVPlacement(nullptr, detectorPositions[i], logicCase, 
                          "physCase_" + std::to_string(i), logicWorld, false, i, true);
    }

    fScoringVolume = logicDetec;

    //-------------------Plastic (Kept Same)-----------------------------//

    G4Material *Plastic = new G4Material("Plastic", 1.032 * g / cm3, 2);
    Plastic->AddElement(nist->FindOrBuildElement("C"), 50. * perCent);
    Plastic->AddElement(nist->FindOrBuildElement("H"), 50. * perCent);

    // Placed at the origin (center void of the array)
    G4VSolid *solidPlas = new G4Tubs("solidPlas", 0. * cm, 1.27 * cm, 3. * cm, 0. * deg, 360. * deg);
    G4LogicalVolume *logicPlas = new G4LogicalVolume(solidPlas, Plastic, "logicPlas");
    G4VPhysicalVolume *physPlas = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicPlas, "physPlas", logicWorld, false, 0, true);
    
    logicPlas->SetVisAttributes(plasColor);

	return physWorld;
}
