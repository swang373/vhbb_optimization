# VHbb Optimization User's Guide

## 1. Installation

Clone this repository within the src/ directory of a CMSSW release area, then run the setup script which creates a Python virtual environment named venv/ and installs the required packages.

```bash
cd CMSSW_X_Y_Z/src
git clone git@github.com:swang373/vhbb_optimization.git
cd vhbb_optimization
./setup.sh
```

The installation succeeds when using CMSSW_7_4_7 and CMSSW_8_0_26, and should work as long as the Python version distributed with the CMSSW release isn't too old. Use the following command to activate the virtual environment. This will have to be done every time we want to setup the environment.

```bash
# Set up the CMSSW environment first, since the virtual
# enviornment relies on its Python interpreter.
cmsenv
# Now you can activate the Python virtual environment.
source venv/bin/activate
```

Assuming you encountered no problems, the framework is ready to use.

## 2. About Configuration

The config/ directory is actually a Python module that is dynamically loaded by the command line tools which are available when the Python virtual environment is active. That being so, the command line tools must be called from the top-level directory containing such a configuration module. The files within the configuration module divide the options into related groupings. The options used by the framework are declared in all UPPERCASE letters.

## 3. Preprocessing

The first step is to preprocess the samples for use during optimization. The idea is to reduce the size of the ntuples to help increase the speed of training and evaluating a trial BDT during the sequential optimization. These compact ntuples contain only those events which pass the signal region selection and only those branches which may be relevant for use as a training feature or as an event weight. A new branch is also added which contains the value necessary to scale a sample to a target luminosity.

Go ahead and modify the following options.

In `config/preprocess.py`
- SELECTION: The signal region selection.
- TARGET_LUMI: The target luminosity in inverse picobarns (pb^-1).
- BRANCHES: The names of the branches to keep in the ntuple

In `config/samples.py`
- DIRECTORY: The parent directory of all the sample files IF they have one in common.
- SIGNAL: A list of the signal samples and their properties.
- BACKGROUND: A list of the background samples and their properties.

Once the options are ready, simply call the `preprocess` command line tool in your terminal. A progress bar should appear with the title "Preprocessing Samples" and an absolute count of the completed samples. For example,

```
(venv)[swang373@lxplus034 vhbb_optimization(master)]$ preprocess 
Preprocessing Samples  [######------------------------------]  6/35  0d 00:26:43
```

Because the samples are processed in parallel, the estimated preprocessing time is inaccurate. It usually takes about half an hour for me to finish preprocessing 35 samples. The preprocessed samples are placed inside the output directory sample/ created in the current working directory.

## 4. Optimizating



If anything is unclear, please contact me and I will attempt to help as soon as I can.
