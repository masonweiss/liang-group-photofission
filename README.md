# liang-group-photofission
Photofission Study for Prof. Edison Liang - Geant4

## Installation Instructions
In an effort to standardize use of monte carlo simulation programs such as Geant4, it makes sense to ensure that all versions of geant4 and data datables are the same. As of 04/01/2025, the most recent version of Geant4 which is compatible with the versions of gcc and cmake available on the SDSC Expanse-g cluster is Geant4-v11.2.2. It may prove useful to install the same version both locally and on the cluster; complete all development of a particular simulation, and then clone this repository to copy the code over on the remote (expanse) server.

### Local Geant-4 Installation
For my ease of access, my local code is contained within a directory called $HOME/liang-group. I'll install geant4 within this folder. Feel free to also copy the simulations contained in this repository as well via cloning the repository with HTTPS.

```
cd $HOME/liang-group
git clone <COPY REPOSITORY SSH LINK HERE>
```

1. Download possible pre-requisites:
In order for Geant4 visualization to work as prescribed here, you may need to install XQuartz, Xerces-C, and/or Qt5. On Mac, you can do this via homebrew. It may make sense to attempt the installation through step 4, and then only run these lines (only the ones you need!) if it throws an error. Some MacOS distributions already have these programs already installed, or you may have already installed it with XCode. Other MacOS machines (M-series processors) may be incompatible with Qt5, so it may be preferred to generate static .prim files and view them using the [DAWN](https://conferences.fnal.gov/g4tutorial/g4cd/Documentation/Visualization/G4DAWNTutorial/G4DAWNTutorial.html) editor.

```
brew install --cask xquartz
brew install qt@5 
brew install xerces-c
```

2. Download and unpack the version of Geant4
```
mkdir geant4
cd geant4
wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.2.2/geant4-v11.2.2.tar.gz
tar -xzvf geant4-v11.2.2.tar.gz
```

3. Verify the compressed source code has been unpacked and make an installation directory
```
rm geant4-v11.2.2.tar.gz
mkdir geant4-v11.2.2-install
```
There should now only be two objects in the Geant4 folder, both directories: geant4-v11.2.2-install and geant4-v11.2.2

Prepare build files for Geant4 using the following commands:
```
cd geant4-v11.2.2
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/liang-group/geant4/geant4-v11.2.2-install \
-DGEANT4_BUILD_MULTITHREADED=ON \
-DGEANT4_INSTALL_DATA=ON \
-DGEANT4_USE_GDML=ON \
-DGEANT4_USE_OPENGL_X11=ON \
-DGEANT4_USE_QT=ON \
-DGEANT4_USE_RAYTRACER_X11=ON
```

5. If everything goes correctly, the following commands should finish the installation without any fatal errors (some warnings may exist)

```
make 
make install
```

6. Note, after installing the data, you may need to specify the path for Geant4 to recognize it. You may also need to specifically add the executable to the path.

```
export G4DATA=$HOME/liang-group/geant4/geant4-v11.2.2-install/share/Geant4-11.2.2/data
source $HOME/liang-group/geant4/geant4-v11.2.2-install/bin/geant4.sh
```

After installing Geant4 locally, you can test the exampleB1 simulation from the official Geant4 release.

## Remote Geant-4 Installation
Access the expanse cluster via: ssh <ACCESS_ID>@login.expanse.sdsc.edu

1. Download and unpack the version of Geant4 to a subdirectory in the home directory named 'geant4' via:
```
cd $HOME
mkdir geant4
wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.2.2/geant4-v11.2.2.tar.gz
tar -xzvf geant4-v11.2.2.tar.gz
```

2. Load the Expanse modules from spider for C/C++ compilation
```
module load gcc/10.2.0
module load cmake/3.21.4
```

3. Verify the compressed source code has been unpacked and make an installation directory
```
rm geant4-v11.2.2.tar.gz
mkdir geant4-v11.2.2-install
```
There should now only be two objects in the Geant4 folder, both directories: geant4-v11.2.2-install and geant4-v11.2.2

4. Prepare build files for Geant4 using the following commands:
```
cd geant4-v11.2.2
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/geant4/geant4-v11.2.2-install \
-DGEANT4_BUILD_MULTITHREADED=ON \
-DGEANT4_INSTALL_DATA=ON \
-DGEANT4_BUILD_CXXSTD=17
```

Note: this method installs all of the default Geant4 data. The download is completed in subsequent steps, which may take quite a long time! If you're concerned about disconnecting, before running the code, you can type screen to start a screen session, run any shell command as if it were a normal terminal, and then detach the screen using Ctrl-A then Ctrl-D. To resume the screen session (assuming you haven't made more than one), press Ctrl-R. The code will run in a detached screen even if your VM connection fails. Note that you can't view the full output to console from a screen session, though.

If everything goes correctly, the following commands should finish the installation without any fatal errors (some warnings may exist)
```
make 
make install
```

Note, after installing the data, you may need to specify the path for Geant4 to recognize it. You may also need to specifically add the executable to the path.
```
export G4DATA=$HOME/geant4/geant4-v11.2.2-install/share/Geant4-11.2.2/data
source $HOME/geant4/geant4-v11.2.2-install/bin/geant4.sh
```

## Running simulation5

With Geant4 properly configured, this repository can be cloned on the Expanse cluster (or your own machine). If on expanse, ensure gcc/10.2.0 and cmake/3.21.4 have been loaded in properly. From the liang-group-photofission/v11.2.2/simulation5 folder, the most recent version of the project can be built as follows:

```
cd liang-group-photofission/v11.2.2/simulation5
mkdir build
cd build
cmake ..
make
```

The macro file **singlefission.mac** is configured to set the capsule radius to 1.0 cm and the material to U233; this can easily be modified. To run the simulation with the provided number of photons in the macro file, call:

```
./simulation5 singleFission.mac
```

This launches one run of 13.8 MeV photons, and records the information at creation and detection into a selection of root histograms as follows:

| Histogram # | Description |
|----------|-------------|
| 1        | Energy deposition at detector |
| 2        | Energy deposition at detector, z location |
| 3        | Energy flow |
| 4–14     | Energy spectrum of (γ, e⁻, e⁺, n, p, d, α, ion, baryon, meson, ν) at creation |
| 15–25    | Energy spectrum of (γ, e⁻, e⁺, n, p, d, α, ion, baryon, meson, ν) at detector |
| 26       | Total energy created |
| 27–33    | Solid polar angle of (γ, e⁻, e⁺, n, p, d, α) at detector |
| 34–40    | Radial phi angle of (γ, e⁻, e⁺, n, p, d, α) at detector |
| 41–42    | Energy spectrum of (photoneutrons, nFission neutrons) at creation |
| 43–44    | Nucleon number distribution for (photonuclear, nFission) reactions |
| 45–46    | Energy from photonuclear reactions at creation for (all particles, fission fragments) |
| 47–48    | Energy from nFission reactions at creation for (all particles, fission fragments) |
| 49       | Energy of e⁺ and e⁻ from pair production at creation |
| 50–51    | Energy of e⁻ from (Compton, pair production) at creation |
| 52       | Energy of all particles at detector |
| 53–55    | Energy of charged particles from (photonuclear, nFission, pair production) at creation |

## Submitting Jobs on Expanse

A slurm batch script can be used to submit a series of jobs (in an array format) to the Expanse cluster. In run/saved_samples, two submission scripts are provided: geant4_simulation5_multi.sb and geant4_simulation5_multi_30.sb. The first was used to generate 50 total runs consisting of 10 runs of 100 million gammas at 5 specific radius values. The second was used to generate 30 total runs consisting of 1 run each with 100 million gammas at 30 specific radius values. The results are found in the other provided directories. Various analysis codes are provided to process the root files and generate the data found [here]([url](https://docs.google.com/spreadsheets/d/1vCgci1fYrsasOQ67LbEuTv-OTo1NwpQssSS7F0jTfH0/edit?usp=sharing)).

Of particular importance are the number of process calls and the associated energy (as found in charged particles at creation) resulting from neutron fission, photofission, and pair production. 

The job can be submitted from the web browser Expanse job composer tool [here]([url](https://portal.expanse.sdsc.edu/pun/sys/myjobs)). Note that the provided .sb files only run the executable; before submitting the job, you should follow the steps just above to make a build folder and build the executable.

The job's progress can be monitored (with verbose output) with the following command:
```
scontrol show job <id>
```
The total utilization for the user, and the overall project, can be found with the following commands:
```
expanse-client user -r expanse
expanse-client project <project-id> -p
```
