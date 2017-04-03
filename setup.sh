#!/usr/bin/env bash

main() {
  local SETUP_OPTION="${1:-"activate"}"
  if [ "$SETUP_OPTION" = "install" ]; then
    # Setup the CMSSW runtime environment (often aliased cmsenv).
    eval "$(scramv1 runtime -sh)"
    # Setup a Python virtual environment using the CMSSW runtime Python.
    virtualenv -p "$(which python)" venv
    # Activate the virtual environment and install packages.
    source venv/bin/activate
	cd vhbbopt && python setup.py install && cd -
    #pip install click contextlib2 futures hyperopt rootpy==0.8.3
  elif [ "$SETUP_OPTION" = "activate" ]; then
    eval "$(scramv1 runtime -sh)"
	source venv/bin/activate
  else
    echo "Unrecognized option \"$OPTION\". Please use either \"install\" or \"activate\"".
  fi
}
main

