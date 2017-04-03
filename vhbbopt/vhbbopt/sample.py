import collections


Sample = collections.namedtuple('Sample', ['name', 'path', 'xsec', 'subset'])

# Inject default values for Sample namedtuples.
Sample.__new__.__defaults__ = (None,) * len(Sample._fields)

