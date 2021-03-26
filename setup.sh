#/bin/bash

cd AIC21-Client-Cpp
mkdir build
cd build
cmake ..
make
cd client
cp ./client ../../../server/
cd ../../../server/
sudo java -jar server-v1.2.2.jar --first-team=./client --second-team=./client
cp ./log.json ../WebGlV1.0/
cd ../WebGlV1.0
python -m http.server --cgi 8360
