cd ~/Escritorio/1_fibra/Scintillator
rm -Rf build
mkdir build
cd ~/Escritorio/1_fibra/Scintillator/build
#cmake -DGeant4_DIR=~/Documents/Programacion/Geant4/geant4.10.03-intall/lib/Geant4-10.3.1 ~/Desktop/GEANT4-Minimal-Example/Scintillator
cmake ..
make -j8
./Scintillator 1

