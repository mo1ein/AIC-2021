#/bin/bash

cd AIC21-Client-Cpp
mkdir build
cd build
cmake ..
make
cd client
cp ./client ../../../server/
cd ../../../server/
sudo java -jar server-v1.2.5.jar --first-team=./client --second-team=./client
