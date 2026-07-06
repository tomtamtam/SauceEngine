cd build
sudo cp libSauceEngine.so /usr/lib/
sudo rm -rf /usr/include/SauceEngine/
sudo cp -r ../Engine /usr/include/SauceEngine
sudo rm -rf /usr/share/sauce-engine/presets
sudo cp -r ../Presets/ /usr/share/sauce-engine/presets
sudo cp ../build/sauce-engine /usr/bin
