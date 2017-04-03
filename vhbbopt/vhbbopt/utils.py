import errno
import imp
import os


def load_config():
    """Dynamically import the config module from the current working
    directory. If successfully located, it is loaded and returned.
    """
    try:
        fp, pathname, description = imp.find_module('config', [os.getcwd()])
    except ImportError:
        raise ImportError('Unable to locate the config module in the current working directory')
    try:
        config = imp.load_module('config', fp, pathname, description)
    except ImportError:
        raise
    else:
        return config
    finally:
        if fp:
            fp.close()


def safe_makedirs(path):
    """
    An idiom for recursive directory creation which is free of race conditions.
    See http://stackoverflow.com/a/5032238 and http://stackoverflow.com/a/600612.

    Parameters
    ----------
    path : path
        The path of the directory to be created.
    """
    try:
        os.makedirs(path)
    except OSError as e:
        if e.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

