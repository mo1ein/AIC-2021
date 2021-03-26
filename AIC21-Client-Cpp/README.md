# Requirements 

## Build

### Unix-Like (Ubuntu, MacOS)

To fetch and build the project use the following commands:

```
cd <path to working directory>
git clone --depth=1 https://github.com/SharifAIChallenge/AIC21-Client-Cpp
cd AIC21-Client-Cpp
mkdir build
cd build
cmake ..
make
```

After making any changes to any file just run `make` again.
If a new source file was added (or deleted) to the project run `cmake ..` and then `make` again.
Use executable output with `--first-team` or `--second-team` args for server.

### Windows

Below are the instructions to build the project in windows using Cygwin.

1) Install the needed building tools

1.1) Install the latest version of CMake from [https://cmake.org/download/](https://cmake.org/download/)

1.2.1) Download the latest Cygwin installer from [https://cygwin.com/install.html](https://cygwin.com/install.html)

1.2.2) Run the installer and select the following packages
* `gcc-g++`
* `make`
* `gdb`

To select a package, type its name in the **Search** field and then 
click it in the list until a tick mark appears in the **Bin?** column:

![Cygwin Setup - Select](https://user-images.githubusercontent.com/19154217/74582910-f9c11e00-4fd6-11ea-8b01-6e8b2e5fd348.png)

1.3) Add the Cygwin `bin` directory in the `Path` enviroment variable. If you don't know how, follow the below steps:

1.3.1) Find the Cygwin `bin` directory. By default it will be `C:\cygwin64\bin`.

1.3.2) In the windows start bar search for `enviroment variable` and select `Edit the system environment variables`.

1.3.3) In the appeared window click `Environment Variables...`.

![ENV VAR FORM](https://user-images.githubusercontent.com/19154217/74583536-2710ca80-4fdd-11ea-8027-6e64e1437960.png)

1.3.4) In the next appeared window edit `Path` in the `System variables` and add `C:\cygwin64\bin` to it. Then 
click `Move Up` multiple times until it appears at the top of the list. Then press OK in the two windows. Now the `Path` 
environment variable is set.

2) Download or clone the `aic20_client_cpp` project somewhere form 
[https://github.com/SharifAIChallenge/AIC20-Client-Cpp](https://github.com/SharifAIChallenge/AIC20-Client-Cpp)

3) Using CMake (CMake GUI)

3.1) Open CMake (CMake GUI)

3.2) In the opened window specify the project root folder (For example mine is `C:/sharif/AIC20/aic20_client_cpp`):

![Specify root project](https://user-images.githubusercontent.com/19154217/74583022-1c076b80-4fd8-11ea-963c-9028b89669ba.png)

3.3) Make a new folder, preferably in the project root directory and name it `build`. In this folder the building scripts will be 
generated.

3.4) Back to the CMake window, specify the build folder (For me it's `C:/sharif/AIC20/aic20_client_cpp/build`):

![Specify Build Directory](https://user-images.githubusercontent.com/19154217/74583144-24ac7180-4fd9-11ea-9a3c-8c016f6c8795.png)

3.5) Click configure. Then a window will appear. Specify the generator to `Unix Makefiles` and select the `Specify native compilers` option then press finish.

![Generator settings](https://user-images.githubusercontent.com/19154217/74583238-5e31ac80-4fda-11ea-8b18-49887ec50081.png)

3.6) Specify the gcc and g++ compilers form Cygwin (It should be located in `C:/`) as below (You may leave `Fortran` empty), then click Finish:

![Specify Compilers](https://user-images.githubusercontent.com/19154217/74583334-43136c80-4fdb-11ea-875a-55ce30933479.png)

3.7) Click Generate. Now the building scripts are generated.

4) Using Cygwin Bash

4.1) In the Cygwin `bin` directory (Default: `C:\cygwin64\bin`) open `bash.exe`.

4.2) Navigate to the build directory where the build scripts are (Using `cd`):

![CD command](https://user-images.githubusercontent.com/19154217/74583724-57596880-4fdf-11ea-8dcf-52945686935b.png)

4.3) Run `make`. If everthing goes fine you should see the following outputs:

![MAKE command](https://user-images.githubusercontent.com/19154217/74583759-a3a4a880-4fdf-11ea-9d93-6dadba486692.png)

4.4) Now the program is built. Run `./client/client.exe` to execute the new client:

![RUN command](https://user-images.githubusercontent.com/19154217/74583814-3f361900-4fe0-11ea-901c-91a45461c150.png)

After each changes made to any source file you have to run `make` again.

If a new source file was added to the project, redo the steps starting from CMake (step 3).

#### CLion on Windows

To build this project in CLion, Cygwin has to be installed and defined in CLion. 
Read the Cygwin secton of the below link to find out how:

[https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html#](https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html#)

# Test
## Valgrind
*Valgrind* has been used to test the memory leak. 
For more information visit: 
[www.valgrind.org](http://valgrind.org)


