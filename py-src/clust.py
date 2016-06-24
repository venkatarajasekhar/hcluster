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
	classf.setThreshold(0.2)

	classf.clustering(SINGLE_LINKAGE)

	classf.printClusters(1)


def runDirectoryClustering(sDirectory):
	bsList = []
	distanceList = []
	fileList = []

	bitsetfromDirectory(sDirectory, bsList, fileList)
	d_ok = all_pair_distance(bsList, JACCARD, distanceList)
	classf = classifier(bsList, distanceList, [])
	classf.setThreshold(0.2)

	classf.clustering(SINGLE_LINKAGE)
	classf.printClusters(1)


def runIncrementalClusteringFromOneDirectory(sFPDirectory, threshold, sDstDirectory):
	bss = []
	fpFiles = []
	bitsetfromDirectory(sFPDirectory, bss, fpFiles)

	distanceList = []
	d_ok = all_pair_distance_numpy(bss, distanceList)
	classf = classifier(bss, distanceList, fpFiles)
	classf.setThreshold(threshold)
	
	classf.clustering(SINGLE_LINKAGE)
	classf.separateFilesToClusters(sDstDirectory, 1)
	



def runDayClustering(sFamilyName, sFPDirectory, sMD5Directory, sDstDirectory, threshold):
	familyFiles = [f for f in listdir(sMD5Directory) if isfile(join(sMD5Directory, f)) and f.endswith('.top5.family.txt')]
	familyFiles.sort()

	fFirstDay = open(join(sMD5Directory, familyFiles[0]))
	md5List = []

	while True:
		line = fFirstDay.readline()
		if not line:
			break
		tmpList = line.split()
		if cmp(sFamilyName, tmpList[1]) == 0:
			md5List.append(tmpList[0])

	fFirstDay.close()

	fpFiles = []
	bsList = []
	sCurrentFPDirectory = join(sFPDirectory, familyFiles[0][0:-16])
	bitsetfromDirectoryMD5(sCurrentFPDirectory, bsList, fpFiles, md5List)

	#print len(bsList), len(fpFiles)
	distanceList = []
	d_ok = all_pair_distance(bsList, JACCARD, distanceList)
	classf = classifier(bsList, distanceList, fpFiles)
	classf.setThreshold(threshold)

	classf.clustering(SINGLE_LINKAGE)
	classf.separateFilesToClusters(sDstDirectory, 1)

	index = 1

	while index < len(familyFiles):

		print index, familyFiles[index]
		fDay = open(join(sMD5Directory, familyFiles[index]))
		md5List = []
		while True:
			line = fDay.readline()
			if not line:
				break

			tmpList = line.split()
			if cmp(sFamilyName, tmpList[1]) == 0:
				md5List.append(tmpList[0]) 
	
		fDay.close()

		newBSList = []
		newFileList = []
		sCurrentFPDirectory = join(sFPDirectory, familyFiles[index][:-16])
		bitsetfromDirectoryMD5(sCurrentFPDirectory, newBSList, newFileList, md5List)
		pairDistanceList = []
		d_ok = all_pair_distance(newBSList, JACCARD, pairDistanceList)

		for root, dirs, files in os.walk(sDstDirectory):
			for directory in dirs:
				sCurrentClusterDirectory = join(sDstDirectory, directory)
				clusterBSList = []
				clusterFileList = []
				bitsetfromDirectory(sCurrentClusterDirectory, clusterBSList, clusterFileList)

				groupDistanceList = []
				group_distance(newBSList, clusterBSList, JACCARD, groupDistanceList)
				
				setInsert = Set([])

				for i in range(len(groupDistanceList)):
					flag = False
					for j in range(len(groupDistanceList[i])):
						if groupDistanceList[i][j] <= threshold:
							flag = True
							break
					if flag:
						setInsert.add(i)
						for j in range(len(pairDistanceList[i])):
							if pairDistanceList[i][j] <= threshold:
								setInsert.add(j)

				indexList = sorted(setInsert, reverse = True)
				for indexDel in indexList:
					shutil.copy(newFileList[indexDel], sCurrentClusterDirectory)
					del newBSList[indexDel]
					del newFileList[indexDel]

					for i in range(len(pairDistanceList)):
						del pairDistanceList[i][indexDel]

					del pairDistanceList[indexDel]			
	

		if len(newFileList) > 0:
			classf1 = classifier(newBSList, pairDistanceList, newFileList)
			classf1.setThreshold(threshold)

			classf1.clustering(SINGLE_LINKAGE)
			classf1.separateFilesToClusters(sDstDirectory, 1)
		index += 1
		#break


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
					if groupDistanceList[i][j] <= 0.2:
						flag = True
						break

				if flag:
					setInsert.add(i)

					for j in range(len(pairDistanceList[i])):
						if pairDistanceList[i][j] <= 0.2:
							setInsert.add(j)

			
			indexList = sorted(setInsert, reverse = True)
			for index in indexList:
				shutil.copy(fileList[index], sCurrentClusterDirectory)
				del newBSList[index]
				del fileList[index]

				for i in range(len(pairDistanceList)):
					del pairDistanceList[i][index]

				del pairDistanceList[index]


	
	classf = classifier(newBSList, pairDistanceList, fileList)
	classf.setThreshold(0.2)

	classf.clustering(SINGLE_LINKAGE)

	classf.separateFilesToClusters(sClusterDirectory, 1)





if __name__=='__main__':
	#FamilyNameList = ['Microsoft$Virus:Win32/Parite.B', 'Microsoft$Trojan:Win32/Skeeyah.A!rfn', 'Microsoft$Backdoor:Win32/Fynloski.A', 'Microsoft$Backdoor:MSIL/Bladabindi.AJ', 'Microsoft$Virus:Win32/Virut.K']
	#runDayClustering(FamilyNameList[int(sys.argv[4])], sys.argv[1], sys.argv[2], sys.argv[3], float(sys.argv[5]))
	#runIncrementalClustering(sys.argv[2], sys.argv[1])
	#runDirectoryClustering(sys.argv[1])
	runIncrementalClusteringFromOneDirectory(sys.argv[1], float(sys.argv[3]), sys.argv[2])
