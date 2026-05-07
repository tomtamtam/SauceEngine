cd ./build
cmake ..
make clean
bear -- make
cp ./compile_commands.json ../
