set -e # Immediately exit if a command fails
echo "Building dependencies for 2D..."
mkdir build_sf
cd build_sf
echo "Building SFML..."
mkdir sfml
cd sfml
sudo apt-get install -y libglew-dev libopenal-dev libsndfile-dev -qq
wget https://github.com/LaurentGomila/SFML/archive/2.1.zip
unzip 2.1.zip -d . > /dev/null
rm 2.1.zip
cd SFML-2.1
cmake .
make
sudo make install
sudo cp /usr/local/share/SFML/cmake/Modules/FindSFML.cmake /usr/share/cmake-*/Modules/
cd ..
rm -rf SFML-2.1

echo "Building SFGUI..."
mkdir sfgui
cd sfgui
wget -O sfgui.zip http://sfgui.sfml-dev.de/download/29
unzip sfgui.zip -d . > /dev/null
rm sfgui.zip
cd SFGUI-0.2.0/
cmake -DSFGUI_BUILD_EXAMPLES=FALSE .
make
sudo make install
sudo cp cmake/FindSFGUI.cmake /usr/share/cmake-*/Modules/
cd ..
rm -rf SFGUI-0.2.0
cd ..
set +e # Disable the auto fails mode
