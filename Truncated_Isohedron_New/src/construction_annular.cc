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

	// //-------------------CRYSTAL-----------------------------//

	// G4Material *NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

	// auto detector = CADMesh::TessellatedMesh::FromSTL("Trunc_Icosa_STL2.STL");
	// detector->SetOffset(G4ThreeVector()); // Find correct offset

	// G4VSolid *solidDetec = detector->GetSolid();
	// G4LogicalVolume *logicDetec = new G4LogicalVolume(solidDetec, NaI, "logicaDetec");
	// G4VPhysicalVolume *physDetec = new G4PVPlacement(nullptr, G4ThreeVector(-16.4 * cm, -17.8 * cm, -16.2 * cm), logicDetec, "physDetec", logicWorld, false, 0, true);
	// logicDetec->SetVisAttributes(detecColor);

	// fScoringVolume = logicDetec;

	// //-------------------Aluminium-----------------------------//

	// G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

	// auto casing = CADMesh::TessellatedMesh::FromSTL("Trunc_Icosa_Al_STL3.STL");
	// casing->SetOffset(G4ThreeVector()); // Find correct offset

	// G4VSolid *solidCase = casing->GetSolid();
	// G4LogicalVolume *logicCase = new G4LogicalVolume(solidCase, Al, "logicaCase");
	// G4VPhysicalVolume *physCase = new G4PVPlacement(nullptr, G4ThreeVector(-16.4 * cm, -17.8 * cm, -16.2 * cm), logicCase, "physCase", logicWorld, false, 0, true);
	// logicCase->SetVisAttributes(caseColor);

	// //-------------------Plastic-----------------------------//

	// G4Material *Plastic = new G4Material("Plastic", 1.032 * g / cm3, 2);
	// Plastic->AddElement(nist->FindOrBuildElement("C"), 50. * perCent);
	// Plastic->AddElement(nist->FindOrBuildElement("H"), 50. * perCent);

	// G4VSolid *solidPlas = new G4Tubs("solidPlas", 0. * cm, 1.27 * cm, 3., 0. * deg, 360. * deg); // crystal
	// G4LogicalVolume *logicPlas = new G4LogicalVolume(solidPlas, Plastic, "logicPlas");
	// // G4VPhysicalVolume *physPlas = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -2.), logicPlas, "physPlas", logicWorld, false, 0, true);
	// G4VPhysicalVolume *physPlas = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicPlas, "physPlas", logicWorld, false, 0, true);
	// logicPlas->SetVisAttributes(plasColor);
	
	// //--------------Al Cylinder Around Source--------------------//
	// G4VSolid *Cage = new G4Tubs("Cage", 6 * cm, 6.2 * cm, 3* cm, 0. * deg, 360. * deg); // crystal
	// G4LogicalVolume *logicCage = new G4LogicalVolume(Cage, Al, "logicCage");
	// // G4VPhysicalVolume *physCage = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -2.), logicCage, "physCage", logicWorld, false, 0, true);
	// G4VPhysicalVolume *physCage = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicCage, "physCage", logicWorld, false, 0, true);
	// logicCage->SetVisAttributes(cageColor);


//-------------------CRYSTAL (NaI)-----------------------------//

    G4Material *NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    // Dimensions for Crystal (Inner 2", Outer 4", Length 2")
    G4double crystal_rmin = 2.0 * 2.54 * cm; // 5.08 cm
    G4double crystal_rmax = 4.0 * 2.54 * cm; // 10.16 cm
    G4double crystal_hz   = 1.0 * 2.54 * cm; // 2.54 cm (G4Tubs uses HALF-length!)

    G4VSolid *solidDetec = new G4Tubs("solidDetec", crystal_rmin, crystal_rmax, crystal_hz, 0.*deg, 360.*deg);
    G4LogicalVolume *logicDetec = new G4LogicalVolume(solidDetec, NaI, "logicDetec");
    G4VPhysicalVolume *physDetec = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicDetec, "physDetec", logicWorld, false, 0, true);
    
    logicDetec->SetVisAttributes(detecColor);
    fScoringVolume = logicDetec;

    //-------------------Aluminium Casing-----------------------------//

    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    G4double gap = 2.0 * mm;
    G4double case_thickness = 0.8 * mm;

    // Casing half-length covers crystal + gap + end-cap thickness
    G4double case_hz = crystal_hz;

    // 1. Inner Aluminium Casing (Cylinder inside the hollow crystal)
    G4double innerCase_rmax = crystal_rmin - gap;                     // 4.88 cm
    G4double innerCase_rmin = innerCase_rmax - case_thickness;        // 4.80 cm

    G4VSolid *solidInnerCase = new G4Tubs("solidInnerCase", innerCase_rmin, innerCase_rmax, case_hz, 0.*deg, 360.*deg);
    G4LogicalVolume *logicInnerCase = new G4LogicalVolume(solidInnerCase, Al, "logicInnerCase");
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicInnerCase, "physInnerCase", logicWorld, false, 0, true);
    
    logicInnerCase->SetVisAttributes(caseColor);

    // 2. Outer Aluminium Casing (Cylinder outside the crystal)
    G4double outerCase_rmin = crystal_rmax + gap;                     // 10.36 cm
    G4double outerCase_rmax = outerCase_rmin + case_thickness;        // 10.44 cm

    G4VSolid *solidOuterCase = new G4Tubs("solidOuterCase", outerCase_rmin, outerCase_rmax, case_hz, 0.*deg, 360.*deg);
    G4LogicalVolume *logicOuterCase = new G4LogicalVolume(solidOuterCase, Al, "logicOuterCase");
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicOuterCase, "physOuterCase", logicWorld, false, 0, true);
    
    logicOuterCase->SetVisAttributes(caseColor);


    //-------------------Plastic-----------------------------//

    G4Material *Plastic = new G4Material("Plastic", 1.032 * g / cm3, 2);
    Plastic->AddElement(nist->FindOrBuildElement("C"), 50. * perCent);
    Plastic->AddElement(nist->FindOrBuildElement("H"), 50. * perCent);

    // Kept the same as your request (with units explicitly assigned for Z half-length)
    G4VSolid *solidPlas = new G4Tubs("solidPlas", 0. * cm, 1.27 * cm, 3. * cm, 0. * deg, 360. * deg);
    G4LogicalVolume *logicPlas = new G4LogicalVolume(solidPlas, Plastic, "logicPlas");
    G4VPhysicalVolume *physPlas = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.), logicPlas, "physPlas", logicWorld, false, 0, true);
    
    logicPlas->SetVisAttributes(plasColor);

	return physWorld;
}
