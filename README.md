# radDamage

To run the reflection/transmission simulation:
git clone https://github.com/cipriangal/radDamage
git checkout simpleTransmissionReflection

Things to modify:
src/mscPrimaryGeneratorAction.cc Line 30 the energy of the neutron
src/mscDetectorConstruction.cc Line 59: the material of the target (Pb is initiated already and just needs to be modified)
src/mscDetectorConstruction.cc Line 93: the thickness of the target 
