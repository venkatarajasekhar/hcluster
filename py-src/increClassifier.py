from tools import *
from classifier import *
from fpLoader import *
from distance import *
from sets import Set

CHUCK_SIZE = 200


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


def chunks(l, n):
	n = max(1, n)
	return [l[i:i + n] for i in range(0, len(l), n)]


def runIncrementalClustering(fpFileLists, sDstDirectory, threshold):
	if len(fpFileLists) == 0:
		return 

	runClustering(fpFileLists[0], sDstDirectory, threshold)
	index = 1

	#boolDebug = False
	#numDebug = -1

	while index < len(fpFileLists):

		fpFileFilteredList = []
		fpList = []


		numErrorIndex = 0
		for fpFile in fpFileLists[index]:
			fp = listFromFile(fpFile)

			#if fpFile.endswith('00eb5010c42198d26a8e0d7f0fb2b308.bb'):
			#	numDebug = numErrorIndex
			#	boolDebug = True

			if len(fp) == 0:
				continue

			fpFileFilteredList.append(fpFile)
			fpList.append(fp)
			numErrorIndex += 1

		pairDistance = all_pair_distance_list(fpList)

		#if boolDebug:
		#	print pairDistance[numDebug]


		sampleList = []
		for root, directories, filenames in os.walk(sDstDirectory):
			for filename in filenames:
				if cmp(filename[-3:], '.bb') == 0:
					sampleList.append(os.path.join(root,filename))

		sampleLists = chunks(sampleList, CHUCK_SIZE)

		#print len(sampleLists)
		for sampleList in sampleLists:
			clusterFPList = []

			for sample in sampleList:
				#if sample.endswith('00b102362d8266c5df74806d0ba64fa1.bb') and boolDebug:
				#	print sample

				clusterFPList.append(listFromFile(sample))

			groupDistanceList = group_distance_list(fpList, clusterFPList)

			#if boolDebug:
			#	print groupDistanceList[numDebug]
			#for i in range(len(groupDistanceList)):
			i = 0

			while i < len(groupDistanceList):	
				matchIndex = -1
				flag = False
				for j in range(len(groupDistanceList[i])):
					if groupDistanceList[i][j] <= threshold:
						flag = True
						matchIndex = j
						break

				#if boolDebug and i == numDebug:
				#	print groupDistanceList[numDebug], flag


				if flag:
					setInsert = Set([])
					setInsert.add(i)
					for j in range(len(pairDistance[i])):
						if pairDistance[i][j] <= threshold:
							setInsert.add(j)

					#if boolDebug and numDebug in setInsert:
					#	print 'before delete'

					indexList = sorted(setInsert, reverse = True)

					sCurrentClusterDirectory = os.path.dirname(sampleList[matchIndex])
					
					for indexDel in indexList:
						shutil.copy(fpFileFilteredList[indexDel], sCurrentClusterDirectory)
						del fpList[indexDel]
						del fpFileFilteredList[indexDel]

						for iindexDel in range(len(pairDistance)):
							del pairDistance[iindexDel][indexDel]

						del pairDistance[indexDel]
						del groupDistanceList[indexDel]

						#if indexDel <= numDebug:
						#	numDebug -= 1
				else:
					i += 1

		#if boolDebug:
		#	exit()

		if len(fpFileFilteredList) > 0:

			clustering(pairDistance, fpFileFilteredList, sDstDirectory, threshold)
		
		index += 1
		







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

	fpFileLists = chunks(fpFiles, CHUCK_SIZE)

	#index = 0

	#for index in range(len(fpFiles)):
	#	if fpFiles[index].endswith('00eb5010c42198d26a8e0d7f0fb2b308.bb'):
	#		print index


	#for fpList in fpFileLists:
	#	print len(fpList),
		
	#print

		

	#testFPs = []

	#for fpFile in fpFileLists[0]:
	#	testFPs.append(listFromFile(fpFile))

	#for fpFile in fpFileLists[1]:
	#	testFPs.append(listFromFile(fpFile))

	#distance = all_pair_distance_list(listFPs)

	#for i in range(len(distance)):
	#	print distance[i]

	#classf = classifier(distance, [])
	#classf.setThreshold(0.1)

	#classf.clustering(SINGLE_LINKAGE)
	#classf.printClusters(1)
	runIncrementalClustering(fpFileLists, sDstDirectory, threshold)



