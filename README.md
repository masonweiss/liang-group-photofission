# liang-group-photofission
Photofission Study for Prof. Edison Liang - Geant4

## Installation Instructions
In an effort to standardize use of monte carlo simulation programs such as Geant4, it makes sense to ensure that all versions of geant4 and data datables are the same. As of 11/7/2024, the most recent version of Geant4 which is compatible with the versions of gcc and cmake available on the SDSC Expanse-g cluster is Geant4-v10.7.0. It may prove useful to install the same version both locally and on the cluster; complete all development of a particular simulation, and then clone this repository to copy the code over on the remote (expanse) server. 


## Local Geant-4 Installation
For my ease of access, my local code is contained within a directory called $HOME/liang-group. I'll install geant4 within this folder. Feel free to also copy the simulations contained in this repository as well via cloning the repository with HTTPS.
```
cd $HOME/liang-group
git clone <COPY REPOSITORY SSH LINK HERE>
```

After installing Geant4 locally, you can test the simulation1 code. Inside this directory is a copy of the exampleB1 simulation from the official geant4 release.

## Remote Geant-4 Installation
Access the expanse cluster via: ```ssh <ACCESS_ID>@login.expanse.sdsc.edu```

1. Download and unpack the version of Geant4 to a subdirectory in the home directory named 'geant4' via:
```
cd $HOME
mkdir geant4
wget https://gitlab.cern.ch/geant4/geant4/-/archive/v10.7.0/geant4-v10.7.0.tar.gz
tar -xzvf geant4-v10.7.0.tar.gz
```

2. Load the Expanse modules from spider for C/C++ compilation
```
module load gcc/10.2.0
module load cmake/3.21.4
```

3. Verify the compressed source code has been unpacked and make an installation directory
```
rm geant4-v10.7.0.tar.gz
mkdir geant4-v10.7.0-install
```
There should now only be two objects in the Geant4 folder, both directories: geant4-v10.7.0-install and geant4-v10.7.0

4. Prepare build files for Geant4 using the following commands:
```
cd geant4-v10.7.0
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/geant4/geant4-v10.7.0-install \
-DGEANT4_BUILD_MULTITHREADED=ON \
-DGEANT4_INSTALL_DATA=ON \
-DGEANT4_BUILD_CXXSTD=17
```

Note: this method installs all of the default Geant4 data. The download is completed in subsequent steps, which may take quite a long time! If you're concerned about disconnecting, before running the code, you can type ```screen``` to start a screen session, run any shell command as if it were a normal terminal, and then detach the screen using Ctrl-A then Ctrl-D. To resume the screen session (assuming you haven't made more than one), press Ctrl-R. The code will run in a detached screen even if your VM connection fails. Note that you can't view the full output to console from a screen session, though. 

4. If everything goes correctly, the following commands should finish the installation without any fatal errors (some warnings may exist)
```
make 
make install
```

5. Note, after installing the 
```
export G4DATA=$HOME/geant4-install/share/Geant4-10.7.0/data
```


## Verifying the Installation


