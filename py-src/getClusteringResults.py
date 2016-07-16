

import sys
import os
import operator
from os import listdir
from os.path import isfile, join


if __name__ == '__main__':
	sDirectory = sys.argv[1]
	clusters = {}
	
	for root, dirs, files in os.walk(sDirectory):
		for directory in dirs:
			sClusterDirectory = join(sDirectory, directory)
			md5List = [f[:-3] for f in listdir(sClusterDirectory) if isfile(join(sClusterDirectory, f)) and f.find('.bb') != -1]

			clusters[int(directory)] = md5List


	sorted_clusters = sorted(clusters.items(), key=operator.itemgetter(0))	

	for t in sorted_clusters:
		print 'cluster', str(t[0])
		print str(t[1])
		print 
