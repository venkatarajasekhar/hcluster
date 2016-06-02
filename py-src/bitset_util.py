from fhash import *
from tools import *
import sys

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

if __name__=='__main__':
	sPrefix = sys.argv[1]
	bss = []
	bitsetfromCCS(sPrefix, bss)
	print len(bss)

	total = 0
	for bs in bss:
		total += bs.count()

	print total


