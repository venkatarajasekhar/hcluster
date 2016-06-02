from bitset_util import *
from distance import *
from classifier import *
import sys

if __name__=='__main__':
	bbs = []
	Dist = []
	bitsetfromCCS(sys.argv[1], bbs)
	d_ok = all_pair_distance(bbs, JACCARD, Dist)

	classf = classifier(bbs, Dist)
	classf.setThreshold(0.1)

	classf.clustering(SINGLE_LINKAGE)
	classf.print_clusters(1)