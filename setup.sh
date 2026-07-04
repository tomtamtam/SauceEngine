rm -rf build
mkdir build
cd build
cmake ..
make -j$(( $(nproc) - 1 ))
sudo cp libSauceEngine.so /usr/lib/libSauceEngine.so
sudo rm -rf /usr/include/SauceEngine/
sudo cp -r ../Engine /usr/include/SauceEngine/

sudo cp sauce-engine /usr/bin/sauce-engine
sudo rm -rf /usr/share/sauce-engine/
sudo mkdir /usr/share/sauce-engine/
sudo cp -r ../Presets /usr/share/sauce-engine/
