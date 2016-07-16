from tools import *
from classifier import *
from fpLoader import *
from distance import *
from sets import Set

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
	if len(fpFileLists) == 0:
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

				#print groupDistanceList
				#exit(0)

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
		



def chunks(l, n):
	n = max(1, n)
	return [l[i:i + n] for i in range(0, len(l), n)]



if __name__=='__main__':
	sDirectory = sys.argv[1]
	sDstDirectory = sys.argv[2]
	threshold = float(sys.argv[3])
	#sDirectory1 = '/home/ubuntu/ds/vt/intelligencefiles/04/dti-03.01-03.28-fix-binary-out/'
	#sDirectory2 = '/home/ubuntu/ds/vt/intelligencefiles/04/dti-03.29-fix-binary-out/'
	#sDirectory3 = '/home/ubuntu/ds/vt/intelligencefiles/04/dti-03.30-fix-binary-out/'

	#listFPs1, fpFiles1 = listFromDirectory(sDirectory1)
	#listFPs2, fpFiles2 = listFromDirectory(sDirectory2)
	#listFPs3, fpFiles3 = listFromDirectory(sDirectory3)

	#listFPs = listFPs1 + listFPs2 + listFPs3
	#fpFiles = fpFiles1 + fpFiles2 + fpFiles3

	#listFPs, fpFiles = listFromDirectory(sDirectory)
	fpFiles = [join(sDirectory, f) for f in listdir(sDirectory) if isfile(join(sDirectory, f)) and f.endswith('.bb')]
	fpFileLists = chunks(fpFiles, 500)

	for fpList in fpFileLists:
		print len(fpList),
		
	print

		

	#testFPs = []

	#for fpFile in fpFileLists[0]:
	#	testFPs.append(listFromFile(fpFile))

	#for fpFile in fpFileLists[1]:
	#	testFPs.append(listFromFile(fpFile))

	#distance = all_pair_distance_list(listFPs)	
	#classf = classifier(distance, [])
	#classf.setThreshold(0.1)

	#classf.clustering(SINGLE_LINKAGE)
	#classf.printClusters(1)
	runIncrementalClustering(fpFileLists, sDstDirectory, threshold)



