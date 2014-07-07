
from ROOT import *
import sys
## global variables

#_______________________________________________________________________________
def getH(dir, histo_name):
  """Get the histogram from a directory"""
  histo = dir.Get("%s;1"%(histo_name))
  if not histo:
    print "No such histogram: ", histo_name
  return histo

#_______________________________________________________________________________
def getRootDirectory(input_dir, file_name, dir_name = "L1TAnalyser"):
    """Get the ROOT directory from the GEMCSCTriggerEfficiency analyzer. Normally,
    the directory should be called GEMCSCTriggerEfficiency. You should check it anyway"""

    ### get the ROOT file
    file = TFile.Open(input_dir + file_name)
    if not file:
        sys.exit('Input ROOT file %s is missing.' %(file_name))

    ## get the ROOT directory
    dir = file.Get(dir_name)
    if not dir:
        sys.exit('Directory %s does not exist.' %(dir_name))

    return dir

#_______________________________________________________________________________
def Print(c, name):
  """Print the histogram"""
  c.Print("%s/%s"%(pdir,name))
