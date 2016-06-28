from tools import *
from classifier import *

def clustering(distance, FileList, sDstDirectory, threshold):
	classf = classifier(distance, FileList)
	classf.setThreshold(threshold)

	classf.clustering(SINGLE_LINKAGE)
	classf.separateFilesToClusters(sDstDirectory, 1)



def initClustering(fpFileList, sDstDirectory, threshold):
	fpFileFilteredList = []
	fpList = []

	for fpFile in fpFileList:
		fp = listFromFile(fpFile)

		if len(fp) == 0:
			continue

		fpFileFilteredList.append(fpFile)
		fpList.append(fp)

	distance = all_pair_distance_list(fpList)

	return fpList, fpFileFilteredList, distance
	
def runClustering(fpFileList, sDstDirectory, threshold):
	fpList, fpFileFilteredList, distance = initClustering(fpFileList, sDstDirectory, threshold)
	clustering(distance, fpFileFilteredList, sDstDirectory, threshold)


def runIncrementalClustering(fpFileLists, sDstDirectory, threshold):
	if len(fpLists) == 0:
		return 

	runClustering(fpFileLists[0], sDstDirectory, threshold)
	index = 1

	while index < len(fpFileLists):

		fpFileFilteredList = []
		fpList = []

		for fpFile in fpFileLists[index]:
			fp = listFromFile(fpFile)

			if len(fp) == 0:
				continue

			fpFileFilteredList.append(fpFile)
			fpList.append(fp)

		pairDistance = all_pair_distance_list(fpList)

		for root, dirs, files in os.walk(sDstDirectory):
			for directory in dirs:
				sCurrentClusterDirectory = join(sDstDirectory, directory)

				clusterFPList, clusterFileList = listFromDirectory(sCurrentClusterDirectory)
				groupDistanceList = group_distance_list(fpList, clusterFPList)

				setInsert = Set([])

				for i in range(len(groupDistanceList)):
					flag = False
					for j in range(len(groupDistanceList[i])):
						if groupDistanceList[i][j] <= threshold:
							flag = True
							break
					if flag:
						setInsert.add(i)
						for j in range(len(pairDistance[i])):
							if pairDistance[i][j] <= threshold:
								setInsert.add(j)

				indexList = sorted(setInsert, reverse = True)
				for indexDel in indexList:
					shutil.copy(fpFileFilteredList[indexDel], sCurrentClusterDirectory)
					del fpList[indexDel]
					del fpFileFilteredList[indexDel]

					for i in range(len(pairDistance)):
						del pairDistance[i][indexDel]

					del pairDistance[indexDel]		


		if len(fpFileFilteredList) > 0:
			clustering(pairDistance, fpFileFilteredList, sDstDirectory, threshold)
		
		index += 1









if __name__=='__main__':

	print 'hello world'