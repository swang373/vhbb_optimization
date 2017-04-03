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

The config/ directory is actually a Python module that is dynamically loaded by the command line tools which are available when the Python virtual environment is active. That being so, the command line tools must be called from the top-level directory containing this configuration module. The files within the configuration module divide the options into related groupings. The options used by the framework are declared in all UPPERCASE letters.

## 3. Preprocessing

The first step is to preprocess the samples for use during optimization. The idea is to reduce the size of the ntuples to help increase the speed of training and evaluating a trial BDT during the sequential optimization. These compact ntuples contain only those events which pass the signal region selection and only those branches which may be relevant for use as a training feature or as an event weight. A new branch is also added which contains the value necessary to scale a sample to a target luminosity. Modify the following options

in `config/preprocess.py`:
- SELECTION: The signal region selection.
- TARGET_LUMI: The target luminosity in inverse picobarns (pb^-1).
- BRANCHES: The names of the branches to keep in the ntuple

in `config/samples.py`:
- DIRECTORY: The parent directory of all the sample files IF they have one in common.
- SIGNAL: A list of the signal samples and their properties.
- BACKGROUND: A list of the background samples and their properties.

Once the options are set, call the `preprocess` command line tool in your terminal. A progress bar should appear with the title "Preprocessing Samples" and an absolute count of the completed samples. For example,

```
(venv)[swang373@lxplus034 vhbb_optimization(master)]$ preprocess 
Preprocessing Samples  [######------------------------------]  6/35  0d 00:26:43
```

Because the samples are processed in parallel, the estimated preprocessing time is inaccurate. It usually takes under half an hour for me to finish preprocessing 35 samples. The preprocessed samples are placed inside the output directory sample/ created in the current working directory.

## 4. Optimization

The main step is to optimize the hyperparameters of the BDT trained to classify the events in the samples as signal or background. Modify the following options.

In `config/bdt.py`:
- EVENT_WEIGHT: The formula expression defining how the signal and background events are weighted.
- FEATURES: The formulas for the training features.

The event weight usually doesn't refer only to branches in the ntuples, but also external functions defined in ROOT macros. If that is the case, their shared object libraries and relevant files must be placed within the macros/ directory. For example, the weighting functions I use are defined in the file VHbbNamespace.h in the Xbb analysis framework, so I would have to copy that into the macros/ directory and compile it using ACLiC.

```bash
(venv)[swang373@lxplus034 vhbb_optimization(master)]$ cp /some/path/Xbb/interface/VHbbNamespace.h macros/
(venv)[swang373@lxplus034 vhbb_optimization(master)]$ root -l
root [0] .L macros/VHbbNameSpace.h++
```

The command line tool takes care of loading the libraries into ROOT's namespace.

Once the options are set, call the `optimize` command line tool in your terminal. Check its help option to see how to set the number of trials and the verbosity. If you increase the verbosity to allow debugging messages, I encourage you to redirect standard output and error to a file to keep the logged messages.

```bash
optimize --num-trials 10 --verbose >& optimization.log
# Below are log file contents.
[optimize] INFO - Performing hyperparameter optimization search...
[hyperopt.tpe] INFO - tpe_transform took 0.017382 seconds
[hyperopt.tpe] INFO - TPE using 0 trials
[hyperopt.tpe] INFO - tpe_transform took 0.089966 seconds
[hyperopt.tpe] INFO - TPE using 1/1 trials with best loss 0.201629
...
[optimize] INFO - Best Trial Loss: 0.153359896382
[optimize] INFO - Best Trial TMVA BDT Options: "NTrees=808.0:Shrinkage=0.218553351247:nCuts=29.0:Grad_NodePurityLimit=0.675205542335:MinNodeSize=3.97883370835:UseBaggedBoost=True:MaxDepth=5.0:BoostType=Grad:SeparationType=SDivSqrtSPlusB"
```

Once the optimization completes, the best loss value is reported along with its corresponding set of hyperparameters formatted as a TMVA BDT option string. The default number of trials is 100, which can take about half a day to complete. I usually launch this in a tmux session and check on it later in the day. You can now update your BDT option string with the optimized set of hyperparameters. Due to the nature of the optimization algorithm, the first few trials are used to initialize searching behaviour, so it is recommended to use more than five trials.

## 5. Optimization Details

The optimization is performed using the Tree of Parzen Estimators algorithm implemented by Dr. James Bergstra in his package [hyperopt](https://hyperopt.github.io/hyperopt/) and discussed in the publication ["Algorithms for Hyper-Parameter Optimization"](https://papers.nips.cc/paper/4443-algorithms-for-hyper-parameter-optimization.pdf). 

Because the optimization is specifically a minimization, the objective function is defined to be 1 - AUC_test, where AUC_test is the area under the ROC curve for the BDT as evaluated on the test set. Minimizing this value means maximizing AUC_test. We do not maximize AUC_train to avoid grossly overfitting on the training set, though optimizing the hyperparameters using the test set still biases the results. This will have to suffice perhaps until the VHbb analysis channels move to a strategy such as k-fold cross-validation.

The hyperparameter search space is defined internally rather than presented as a configurable option in part due to a segmentation fault otherwise. It covers most if not all of the relevant TMVA BDT options and is sampled once per trial.

[List of TMVA BDT Options](https://tmva.sourceforge.net/optionRef.html#MVA::BDT)

Hyperparameter | Description | Sampling Distribution
--- | --- | ---
NTrees | Number of trees in the forest | Discrete Uniform {100, 900}
MaxDepth | Max depth of the decision tree allowed | Discrete Uniform {2, 5}
MinNodeSize | Minimum percentage of training events required in a leaf node | Uniform (0, 10)
nCuts | Number of grid points in variable range used in finding optimal cut in node splitting | Discrete Uniform {2, 40}
BoostType | Boosting type for the trees in the forest | Choice of {AdaBoost, RealAdaBoost, Bagging, Grad}
UseBaggedBoost | Use only a random subsample of all events for growing the trees in each iteration | Choice of {True, False}
SeparationType | Separation criterion for node splitting | Choice of {CrossEntropy, GiniIndex, MisClassificationError, SDivSqrtSPlusB}

Depending on the BoostType chosen for a trial, the following hyperparameters are also present.

- **AdaBoost**

Hyperparameter | Description | Sampling Distribution
--- | --- | ---
AdaBoostBeta | Learning rate for AdaBoost algorithm | Log Uniform (-5, 1)
NodePurityLimit | In boosting/pruning, nodes with purity > NodePurityLimit are signal; background otherwise | Uniform (0, 1)

- **RealAdaBoost**

Hyperparameter | Description | Sampling Distribution
--- | --- | ---
AdaBoostBeta | Learning rate for AdaBoost algorithm | Log Uniform (-5, 1)
UseYesNoLeaf | Use Sig or Bkg categories, or the purity=S/(S+B) as classification of the leaf node -> Real-AdaBoost | Choice of {True, False}
SigToBkgFraction | Sig to Bkg ratio used in Training (similar to NodePurityLimit, which cannot be used in real adaboost | Uniform (0, 1)

- **Bagging**

Hyperparameter | Description | Sampling Distribution
--- | --- | ---
BaggedSampleFraction | Relative size of bagged event sample to original size of the data sample (used whenever bagging is used (i.e. UseBaggedGrad, Bagging,) | Uniform (0, 1)
NodePurityLimit | In boosting/pruning, nodes with purity > NodePurityLimit are signal; background otherwise | Uniform (0, 1)

- **Grad**

Hyperparameter | Description | Sampling Distribution
--- | --- | ---
Shrinkage | Learning rate for GradBoost algorithm | Log Uniform (-5, 1)
NodePurityLimit | In boosting/pruning, nodes with purity > NodePurityLimit are signal; background otherwise | Uniform (0, 1)

## Feedback
Thanks for beta testing the code! If anything is unclear or breaks, please contact me and I will attempt to help as soon as I can.
