cd ~/Escritorio/MultipleFibbersOpticalTime
rm -Rf build
mkdir build
cd ~/Escritorio/MultipleFibbersOpticalTime/build
#cmake -DGeant4_DIR=~/Documents/Programacion/Geant4/geant4.10.03-intall/lib/Geant4-10.3.1 ~/Desktop/GEANT4-Minimal-Example/Scintillator
cmake ..
make -j8
./MultipleFibbers run1 1 1

