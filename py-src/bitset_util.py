from fhash import *
from tools import *
import sys
import os
from os import listdir
from os.path import isfile, join

def bitsetfromCCS(prefix, bss):
	filename = prefix + '_dim'
	with open(filename, 'r') as dimFile:
		ContentList = dimFile.readlines()

	numList = []

	for line in ContentList:
		numList = numList + line.split()

	assert len(numList) == 3

	numRow = int(numList[0])
	numCol = int(numList[1])
	numVal = int(numList[2])

	print 'numRow', numRow
	print 'numCOl', numCol
	print 'numVal', numVal


	filename = prefix + '_col_ccs'
	with open(filename, 'r') as colIdxFile:
		ContentList = colIdxFile.readlines()

	numList = []
	for line in ContentList:
		numList = numList + line.split()

	assert len(numList) == numCol + 1

	colPtr = []
	for i in range(numCol + 1):
		colPtr.append(int(numList[i]))


	filename = prefix + '_row_ccs'
	with open(filename, 'r') as rowPtrFile:
		ContentList = rowPtrFile.readlines()

	numList = []
	for line in ContentList:
		numList = numList + line.split()

	assert len(numList) == numVal

	rowIdx = []
	for i in range(numVal):
		rowIdx.append(int(numList[i]))

	for i in range(numRow):
		bs = bitarray(FEATURE_SIZE)
		bs.setall(False)
		bss.append(bs)

	for i in range(numCol):
		for j in range(colPtr[i], colPtr[i+1]):
			bss[rowIdx[j]][i] = 1	

def bitsetfromDirectory(sDirectory, bss, fpFiles):
	onlyfiles = [f for f in listdir(sDirectory) if isfile(join(sDirectory, f))]

	for f in onlyfiles:
		if not f.endswith('.bb'):
			continue

		bs = bitarray(FEATURE_SIZE)
		bs.setall(False)
		
		sFileName = join(sDirectory, f)
		
		fp = open(sFileName, 'r')
                
                sFirstLine = fp.readline()
                sSecondLine = fp.readline()
                iNum = int(sFirstLine)

                if int(sSecondLine) < 10:
                        continue

                while True:
                        line = fp.readline()
                        if not line:
                                break

                        numList = line.split()
                        bs[int(numList[0])] = 1

                assert bs.count() == iNum

		fp.close()
		
		fpFiles.append(join(sDirectory, f))
		bss.append(bs)




def bitsetfromDirectoryMD5(sDirectory, bss, fpFiles, md5List):
	for i in range(len(md5List)):
		sFileName = join(sDirectory, md5List[i] + '.bb')
		bs = bitarray(FEATURE_SIZE)
		bs.setall(False)

		if not os.path.exists(sFileName):
			continue

		fpFiles.append(sFileName)
		fp = open(sFileName, 'r')

		sFirstLine = fp.readline()
		sSecondLine = fp.readline()
		iNum = int(sFirstLine)

		while True:
			line = fp.readline()
			if not line:
				break
			numList = line.split()
			bs[int(numList[0])] = 1

		assert bs.count() == iNum

		fp.close()
		bss.append(bs)

if __name__=='__main__':
	sPrefix = sys.argv[1]
	bss = []
	bitsetfromCCS(sPrefix, bss)
	print len(bss)

	total = 0
	for bs in bss:
		total += bs.count()

	print total

