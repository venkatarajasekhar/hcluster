from bitset_util import *
from distance import *
from classifier import *
import sys

from sets import Set

def runClustering(sPrefix):
	bsList = []
	distanceList = []

	bitsetfromCCS(sPrefix, bsList)
	d_ok = all_pair_distance(bsList, JACCARD, distanceList)

	classf = classifier(bsList, distanceList, [])
	classf.setThreshold(0.1)

	classf.clustering(SINGLE_LINKAGE)

	classf.printClusters(1)


def runIncrementalClustering(sClusterDirectory, sNewFPDirectory):
	newBSList = []
	fileList = []
	bitsetfromDirectory(sNewFPDirectory, newBSList, fileList)


	for i in range(len(fileList)):
		fileList[i] = join(sNewFPDirectory, fileList[i])


	pairDistanceList = []
	d_ok = all_pair_distance(newBSList, JACCARD, pairDistanceList)

	for root, dirs, files in os.walk(sClusterDirectory):
		for directory in dirs:
			sCurrentClusterDirectory = join(sClusterDirectory, directory)
			clusterBSList = []
			clusterFileList = []
			bitsetfromDirectory(sCurrentClusterDirectory, clusterBSList, clusterFileList)

			groupDistanceList = []
			group_distance(newBSList, clusterBSList, JACCARD, groupDistanceList)

			setInsert = Set([])

			for i in range(len(groupDistanceList)):
				flag = False
				for j in range(len(groupDistanceList[i])):
					if groupDistanceList[i][j] <= 0.1:
						flag = True
						break

				if flag:
					setInsert.add(i)

					for j in range(len(pairDistanceList[i])):
						if pairDistanceList[i][j] <= 0.1:
							setInsert.add(j)

			
			indexList = sorted(setInsert, reverse = True)
			for index in indexList:
				shutil.copy(fileList[index], sCurrentClusterDirectory)
				del newBSList[index]
				del fileList[index]

				for i in len(pairDistanceList):
					del pairDistanceList[i][index]

				del pairDistanceList[index]


	
	classf = classifier(newBSList, pairDistanceList, fileList)
	classf.setThreshold(0.1)

	classf.clustering(SINGLE_LINKAGE)

	classf.separateFilesToClusters(sClusterDirectory, 1)





if __name__=='__main__':
	
	
	runIncrementalClustering(sys.argv[2], sys.argv[1])


