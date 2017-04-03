import multiprocessing
import os

import click
import futures
from rootpy import asrootpy, ROOT
from rootpy.io import root_open, TemporaryFile

from vhbbopt.utils import load_config, safe_makedirs


ROOT.gROOT.SetBatch(True)


def split_train_test(src, dst, subset=None, selection='', branches=None):
    """Split a sample's events into training and testing sets based on the
    parity of an event. Odd numbered events are reserved for training,
    while even numbered events are reserved for testing.

    Parameters
    ----------
    src : path
        The path to the input sample.

    dst : path
        The path to the output sample containing the train and test trees.

    subset : string, optional
        A selection applied to the sample's tree to choose only a 
        subset of events to split into training and teseting events.
        The default is None to consider all events.

    selection : string, optional
        A selection applied to the sample's tree to define training
        and testing events. The default is None for no selection.

    branches : list of strings, optional
        The list of branches to be kept in the output sample.
        The default is None to keep all branches.
    """
    if subset:
        if selection:
            selection = '({0})&&({1})'.format(selection, subset)
        else:
            selection = subset
    with root_open(src) as infile, TemporaryFile():
        tree = infile.Get('tree')
        train_temp = asrootpy(tree.CopyTree('{}&&(evt%2==1)'.format(selection)))
        train_temp.SetName('train')
        test_temp = asrootpy(tree.CopyTree('{}&&(evt%2==0)'.format(selection)))
        test_temp.SetName('test')
        with root_open(dst, 'w') as outfile:
            if branches:
                train_temp.activate(branches, exclusive=True)
                test_temp.activate(branches, exclusive=True)
            train = train_temp.CloneTree()
            test = test_temp.CloneTree()
            outfile.Write()


def append_lumi_scale_branch(src, dst, xsec, target_lumi):
    """Append a branch to the train and test trees of a sample for scaling
    the MC sample luminosity to the target luminosity. The scaling value is
    calculated as the ratio of the target luminosity to the sample luminosity,
    where the sample luminosity is defined as the difference between the number
    of positively and negatively weighted events of the full MC sample divided
    by the cross-section of the MC sample.

    Parameters
    ----------
    src : string
        The path to the sample.

    dst : string
        The output path to the new sample with training and testing trees.

    xsec : numeric
        The cross-section of the Monte-Carlo sample in units of picobarns (pb).

    target_lumi : numeric
        The target luminosity in units of inverse picobarns (pb-1).
    """
    with root_open(src) as f:
        n_pos = f.Get('CountPosWeight').GetBinContent(1)
        n_neg = f.Get('CountNegWeight').GetBinContent(1)
    sample_lumi = (n_pos - n_neg) / float(xsec)
    lumi_scale = target_lumi / sample_lumi
    with root_open(dst, 'a') as f:
        for name in ['train', 'test']:
            t = f.Get(name)
            t.create_branches({'lumi_scale': 'F'})
            b = t.GetBranch('lumi_scale')
            for entry in t:
                entry.lumi_scale = lumi_scale
                b.Fill()
            t.Write()


def worker(sample, directory, selection, branches, target_lumi):
    if directory:
        src = os.path.join(directory, sample.path)
    else:
        src = sample.path
    dst = os.path.join('sample', sample.name + '.root')
    split_train_test(src, dst, sample.subset, selection, branches)
    append_lumi_scale_branch(src, dst, sample.xsec, target_lumi)


@click.command(context_settings={'help_option_names': ['-h', '--help']})
def cli():
    """Preprocess the samples for BDT optimization.
    """
    # Load the configuration module.
    config = load_config()
    # Create the output directory.
    safe_makedirs('sample')
    # Preprocess the samples in parallel. To guard against deadlock, the number
    # of workers is chosen to be the smaller of the number of available cores
    # or the number of samples to preprocess.
    samples = config.SIGNAL + config.BACKGROUND
    max_workers = min(multiprocessing.cpu_count(), len(samples))
    tasks = []
    with futures.ProcessPoolExecutor(max_workers) as executor:
        for sample in samples:
            # The configuration module cannot be pickled, so pass the options directly.
            tasks.append(executor.submit(worker, sample, config.DIRECTORY, config.SELECTION, config.BRANCHES, config.TARGET_LUMI))
        with click.progressbar(label='Preprocessing Samples', length=len(samples), show_pos=True, show_percent=False) as bar:
            for task in futures.as_completed(tasks):
                bar.update(1)

