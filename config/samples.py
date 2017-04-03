#-------------------------------------------------
# Samples Configuration
#-------------------------------------------------

## A Sample is a tuple with the following named fields:
## name : The name used to refer to the sample.
## path : The path to the sample's file.
## xsec : The cross-section of the sample in picobarns (pb).
## subset : A cut expression used to restrict a sample to a subset of its events.
from vhbbopt import Sample

## The parent directory of the samples. If the samples are in different
## directories, set this to None and give their full paths below.
DIRECTORY = '/afs/cern.ch/work/s/swang373/private/Xbb_ICHEP/src/Xbb/MVAout'

## The signal samples.
SIGNAL = [
    Sample('ZH', 'ZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8.root', xsec=(1.773E-01 - 2.455E-02) * 0.5809 / 0.956),
    Sample('ggZH', 'ggZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8.root', xsec=2.455E-02 * 0.5809),
    Sample('WminusH', 'WminusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8.root', xsec=5.967E-02 * 0.5809 * 3),
    Sample('WplusH', 'WplusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8.root', xsec=9.404E-02 * 0.5809 * 3),
]

## The background samples.
BACKGROUND = [
    # W+Jets
    Sample('WJetsHT100', 'WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=1346 * 1.21),
    Sample('WJetsHT200', 'WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=360.1 * 1.21),
    Sample('WJetsHT400', 'WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=48.8 * 1.21),
    Sample('WJetsHT600', 'WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=12.07 * 1.21),
    Sample('WJetsHT800', 'WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=5.497 *1.21),
    Sample('WJetsHT1200', 'WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=1.329 * 1.21),
    Sample('WJetsHT2500', 'WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=0.03209 * 1.21),
    # Z+Jets
    Sample('ZJetsHT100', 'ZJetsToNuNu_HT-100To200_13TeV-madgraph.root', xsec=280.05 * 1.41819),
    Sample('ZJetsHT200', 'ZJetsToNuNu_HT-200To400_13TeV-madgraph.root', xsec=77.55 * 1.41819),
    Sample('ZJetsHT400', 'ZJetsToNuNu_HT-400To600_13TeV-madgraph.root', xsec=10.752 * 1.41819),
    Sample('ZJetsHT600', 'ZJetsToNuNu_HT-600To800_13TeV-madgraph.root', xsec=2.559 * 1.41819),
    Sample('ZJetsHT800', 'ZJetsToNuNu_HT-800To1200_13TeV-madgraph.root', xsec=1.1802 * 1.41819),
    Sample('ZJetsHT1200', 'ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.root', xsec=0.28629 * 1.41819),
    Sample('ZJetsHT2500', 'ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.root', xsec=0.006912 * 1.41819),
    # TTbar
    Sample('TT', 'TT_TuneCUETP8M2T4_13TeV-powheg-pythia8.root', xsec=831.76),
    # Single Top
    Sample('ST_s', 'ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1.root', xsec=3.365),
    Sample('ST_t_antitop', 'ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1.root', xsec=80.95),
    Sample('ST_t_top', 'ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1.root', xsec=136.02),
    Sample('ST_tW_antitop', 'ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.root', xsec=38.06),
    Sample('ST_tW_top', 'ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.root', xsec=38.09),
    # QCD
    Sample('QCDHT100', 'QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=28060000),
    Sample('QCDHT200', 'QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=1710000),
    Sample('QCDHT300', 'QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=347500),
    Sample('QCDHT500', 'QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=32060),
    Sample('QCDHT700', 'QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=6829),
    Sample('QCDHT1000', 'QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=1207),
    Sample('QCDHT1500', 'QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=120),
    Sample('QCDHT2000', 'QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root', xsec=25.25),
    # Diboson
    Sample('WW', 'WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8.root', xsec=45.68),
    Sample('WZ', 'WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8.root', xsec=10.73),
    Sample('ZZ', 'ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8.root', xsec=4.033),
]

