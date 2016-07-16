import sys
import os
from os import listdir
from os.path import isfile, join
import numpy as np 
from bitarray import bitarray

from tools import *


def bitsetFromCCS(prefix):

	bss = []
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

	return bss

def listFromCCS(prefix):
	bss = bitsetFromCCS(prefix)
	fpList = []

	for i in range(len(bss)):
		fp = []
		for j in range(bss[i].length()):
			if bss[i][j]:
				fp.append(1)
			else:
				fp.append(0)

		fpList.append(fp)

	return fpList


def listFromFile(sFileName):
	fp = [0] * FEATURE_SIZE
	ffpFile = open(sFileName, 'r')

	sFirstLine = ffpFile.readline()
	sSecondLine = ffpFile.readline()

	iNum = int(sFirstLine)

	if iNum < 10:
		ffpFile.close()
		return []

	while True:
		line = ffpFile.readline()
		if not line:
			break

		numList = line.split()
		fp[int(numList[0])] = 1

	assert sum(fp) == iNum

	ffpFile.close()
	return fp





def listFromDirectory(sDirectory):
	listFPs = []
	fpFiles = []

	onlyfiles = [f for f in listdir(sDirectory) if isfile(join(sDirectory, f))]

	for f in onlyfiles:
		if not f.endswith('.bb'):
			continue

		fp = [0] * FEATURE_SIZE

		sFileName = join(sDirectory, f)
		
		ffpFile = open(sFileName, 'r')

		sFirstLine = ffpFile.readline()
		sSecondLine = ffpFile.readline()
		iNum = int(sFirstLine)

		if iNum < 10 or iNum > 168005:
			ffpFile.close()
			continue

		while True:
			line = ffpFile.readline()
			if not line:
				break

			numList = line.split()
			fp[int(numList[0])] = 1

		assert sum(fp) == iNum

		ffpFile.close()


		fpFiles.append(sFileName)
		listFPs.append(fp)

	return listFPs, fpFiles


def numpyArrayFromDirectory(sDirectory):
	listFPs, fpFiles = listfromDirectory(sDirectory)
	return np.array(listFPs), fpFiles


def bitsetFromDirectory(sDirectory):

	bss = []
	fpFiles = []
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

		if iNum < 10:
			fp.close()
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

	return bss, fpFiles



def bitsetFromDirectoryMD5(sDirectory, md5List):

	bss = []
	fpFiles = []

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

	return bss, fpFiles

if __name__=='__main__':
	sPrefix = sys.argv[1]
	bss = []
	bitsetFromCCS(sPrefix, bss)
	print len(bss)

	total = 0
	for bs in bss:
		total += bs.count()

	print total


