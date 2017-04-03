#!/usr/bin/env bash

main() {
  # Setup the CMSSW runtime environment (often aliased cmsenv).
  eval "$(scramv1 runtime -sh)"
  # Setup a Python virtual environment using the CMSSW runtime Python.
  virtualenv -p "$(which python)" venv
  # Activate the virtual environment and install packages.
  source venv/bin/activate
  cd vhbbopt && python setup.py install && cd -
}
main

