#!/usr/bin/env python
import os
import sys

from setuptools import find_packages, setup


PACKAGE_PATH = os.path.abspath(os.path.dirname(__file__))


# Check CMSSW Environment
if not os.getenv('CMSSW_BASE'):
    raise RuntimeError('Must be installed within a CMSSW environment.')

# Check Python Version
if sys.version_info[:2] < (2, 7) or sys.version_info[0] >= 3:
    raise RuntimeError('Python 2.7 required. Python 3 is not supported.')

# Check PyROOT Dependency
try:
    import ROOT
except ImportError:
    raise ImportError('PyROOT is not installed or enabled.')
else:
    ROOT.PyConfig.IgnoreCommandLineOptions = True

# Taken from rootpy's setup.py: Prevent distutils from trying to create hard links
# which are not allowed on AFS between directories. This is a hack to force copying.
try:
    del os.link
except AttributeError:
    pass

# Return the contents of README.rst for use as the long description.
def readme():
    # Find the absolute path to the vhbbutils package in case
    # setup.py was called from outside the base directory.
    with open(os.path.join(PACKAGE_PATH, 'README.rst')) as f:
        return f.read()

# Package Setup
setup(
    name = 'vhbbopt',
    version = '0.9.0',
    description = 'A command line tool for hyperparameter optimization.',
    long_description = readme(),
    author = 'Sean-Jiun Wang',
    author_email = 'sean-jiun.wang@cern.ch',
    url = 'https://github.com/swang373/vhbb_optimization',
    download_url = '',
    license = 'MIT',
    classifiers = [
        'Development Status :: 4 - Beta',
        'Intended Audience :: Science/Research',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python :: 2 :: Only',
        'Topic :: Scientific/Engineering :: Physics',
    ],
    packages = find_packages(),
    install_requires = [
        'click',
        'contextlib2',
        'futures',
        'hyperopt',
        'rootpy==0.8.3',
    ],
    entry_points = {
        'console_scripts': [
            'optimize = vhbbopt.optimize:cli',
            'preprocess = vhbbopt.preprocess:cli',
        ],
    },
)
