import functools
import logging
import os

import click
import contextlib2
from hyperopt import STATUS_OK, Trials, fmin, hp, tpe
from rootpy import ROOT
from rootpy.io import root_open

from vhbbopt.utils import load_config


HYPERPARAM_SPACE = hp.choice('TMVA_BDT', [
    {
        'NTrees': hp.quniform('NTrees', 100, 2000, 1),
        'MaxDepth': hp.quniform('MaxDepth', 2, 10, 1),
        'MinNodeSize': hp.uniform('MinNodeSize', 0, 10),
        'nCuts': hp.quniform('nCuts', 2, 50, 1),
        'BoostType': hp.choice('BoostType', [
            {
                'BoostType': 'AdaBoost',
                'AdaBoostBeta': hp.loguniform('AdaBoost_Beta', -5, 1),
                'NodePurityLimit': hp.uniform('AdaBoost_NodePurityLimit', 0, 1),
            },
            {
                'BoostType': 'RealAdaBoost',
                'AdaBoostBeta': hp.loguniform('RealAdaBoost_Beta', -5, 1),
                'UseYesNoLeaf': hp.choice('UseYesNoLeaf', [
                    'True',
                    'False',
                ]),
                'SigToBkgFraction': hp.uniform('SigToBkgFraction', 0, 1),
            },
            {
                'BoostType': 'Bagging',
                'BaggedSampleFraction': hp.uniform('BaggedSampleFraction', 0, 1),
                'NodePurityLimit': hp.uniform('Bagging_NodePurityLimit', 0, 1),
            },
            {
                'BoostType': 'Grad',
                'Shrinkage': hp.loguniform('Shrinkage', -5, 1),
                'NodePurityLimit': hp.uniform('Grad_NodePurityLimit', 0, 1),
            },
        ]),
        'UseBaggedBoost': hp.choice('UseBaggedBoost', [
            'True',
            'False',
        ]),
        'SeparationType': hp.choice('SeparationType', [
            'CrossEntropy',
            'GiniIndex',
            'MisClassificationError',
            'SDivSqrtSPlusB',
        ]),
    }
])

HYPERPARAM_CHOICE_MAP = {
    'BoostType': ['AdaBoost', 'RealAdaBoost', 'Bagging', 'Grad'],
    'UseYesNoLeaf': ['True', 'False'],
    'UseBaggedBoost': ['True', 'False'],
    'SeparationType': ['CrossEntropy', 'GiniIndex', 'MisClassificationError', 'SDivSqrtSPlusB'],
}

LOGGER = logging.getLogger('optimize')


def train(name, signal_files, background_files, features, event_weight, space):
    """The TMVA BDT training routine.
    
    Parameters
    ----------
    space : hyperopt.hp
        The hyperparameter search space.

    Returns
    -------
    results : A dictionary with the keys status and loss. The status value is
        a flag for the training status, and is always STATUS_OK if successful.
        The loss value is the numerical value of the objective function being
        minimized evaluted at the sampled point in the search space.
    """
    ROOT.TMVA.Tools.Instance()
    with root_open('TMVAClassification_{}.root'.format(name), 'w') as outfile:
        factory_options = ['!V', 'Silent', '!DrawProgressBar', 'Transformations=I', 'AnalysisType=Classification']
        factory = ROOT.TMVA.Factory('TMVAClassification', outfile, ':'.join(factory_options))
        # Add the training features.
        for feature in features:
            factory.AddVariable(feature, 'F')
        # Add signal events.
        for signal in signal_files:
            factory.AddSignalTree(signal.Get('train'), 1.0, ROOT.TMVA.Types.kTraining)
            factory.AddSignalTree(signal.Get('test'), 1.0, ROOT.TMVA.Types.kTesting)
        # Add background events, guarding against empty background samples,
        # i.e. samples where no events passed the signal region selection.
        for background in background_files:
            if background.Get('train').GetEntriesFast() > 0:
                factory.AddBackgroundTree(background.Get('train'), 1.0, ROOT.TMVA.Types.kTraining)
            if background.Get('test').GetEntriesFast() > 0:
                factory.AddBackgroundTree(background.Get('test'), 1.0, ROOT.TMVA.Types.kTesting)
        # The original set of events was split by parity to obtain training and
        # test sets, so rescaling by a factor of two is necessary. The events
        # will also need to be scaled to the target luminosity.
        event_weight = '{} * 2 * lumi_scale'.format(event_weight)
        factory.SetSignalWeightExpression(event_weight)
        factory.SetBackgroundWeightExpression(event_weight)
        cut_sig = ROOT.TCut('')
        cut_bkg = ROOT.TCut('')
        factory.PrepareTrainingAndTestTree(cut_sig, cut_bkg, 'NormMode=None')
        # Format the choice of hyperparameters sampled from the search space as
        # a string of TMVA BDT options. The BoostType choices are processed
        # separately because it is nested.
        method_options = ['!H','!V']
        boost_options = ['{0}={1!s}'.format(*param) for param in space.pop('BoostType').iteritems()]
        general_options = ['{0}={1!s}'.format(*param) for param in space.iteritems()]
        bdt_methodbase = factory.BookMethod(ROOT.TMVA.Types.kBDT, 'BDT', ':'.join(method_options + boost_options + general_options))
        # Train, test, and evaluate the performance of the BDT.
        bdt_methodbase.TrainMethod()
        factory.TestAllMethods()
        factory.EvaluateAllMethods()
        outfile.Write()
        # Calculate the loss function value. Because hyperopt minimizes its
        # objective function and we want to maximize the ROC integral,
        # we return 1 - AUC_test as the loss value.
        #output_train_signal = outfile.Get('Method_BDT/BDT/MVA_BDT_Train_S')
        #output_train_background = outfile.Get('Method_BDT/BDT/MVA_BDT_Train_B')
        #auc_train = bdt_methodbase.GetROCIntegral(output_train_signal, output_train_background)
        output_test_signal = outfile.Get('Method_BDT/BDT/MVA_BDT_S')
        output_test_background = outfile.Get('Method_BDT/BDT/MVA_BDT_B')
        auc_test = bdt_methodbase.GetROCIntegral(output_test_signal, output_test_background)
        metric = 1 - auc_test
        # Return the results of the trial.
        result = {'status': STATUS_OK, 'loss': metric}
        return result


@click.command(context_settings={'help_option_names': ['-h', '--help']})
@click.argument('name')
@click.option('-n', '--num-trials', default=100, help='The number of trials evaluated. The default is 100.')
@click.option('-v', '--verbose', is_flag=True, help='Increase the verbosity level to show debug messages.')
def cli(name, num_trials, verbose):
    """Optimize the hyperparameters for a TMVA BDT. The argument NAME is used
    as the suffix for the TMVA output filename.

    This must be called from within a workspace which contains a configuration
    module named config and an optional directory named macros which contains
    shared libraries of compiled ROOT macros.

    The hyperparameter optimization uses the hyperopt package.
    Bergstra, J., Yamins, D., Cox, D. D. (2013) Making a Science of Model Search:
    Hyperparameter Optimization in Hundreds of Dimensions for Vision Architectures
    """
    logging_level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(format='[%(name)s] %(levelname)s - %(message)s', level=logging_level)
    # Set ROOT to batch mode.
    ROOT.gROOT.SetBatch(True)
    # Load macros to the global ROOT instance.
    for macro in os.listdir('macros'):
        _, ext = os.path.splitext(macro)
        if ext == '.so':
            load_status = ROOT.gSystem.Load('macros/{}'.format(macro))
            if load_status < 0:
                raise RuntimeError('Failed to load macro {}'.format(macro))
    # Dynamically load the configuration module.
    config = load_config()
    LOGGER.info('Performing hyperparameter optimization search...')
    with contextlib2.ExitStack() as stack:
        signal_files = [stack.enter_context(root_open('sample/{}.root'.format(sample.name))) for sample in config.SIGNAL]
        background_files = [stack.enter_context(root_open('sample/{}.root'.format(sample.name))) for sample in config.BACKGROUND]
        trials = Trials()
        objective = functools.partial(train, name, signal_files, background_files, config.FEATURES, config.EVENT_WEIGHT)
        best = fmin(objective, HYPERPARAM_SPACE, algo=tpe.suggest, max_evals=num_trials, trials=trials)
    LOGGER.debug('Trials: %s', trials.trials)
    LOGGER.debug('Trial Results: %s', trials.results)
    LOGGER.debug('Trial Losses: %s', trials.losses())
    LOGGER.debug('Best Trial Hyperparameters: %s', best)
    LOGGER.info('Best Trial Loss: %s', min(trials.losses()))
    for hyperparam, value in best.iteritems():
        if hyperparam in HYPERPARAM_CHOICE_MAP:
            best[hyperparam] = HYPERPARAM_CHOICE_MAP[hyperparam][value]
    del best['TMVA_BDT']
    LOGGER.info('Best Trial TMVA BDT Options: "%s"', ':'.join('{0}={1!s}'.format(*param) for param in best.iteritems()))

