from tools import *
from fhash import *

def dot_distance(v1, v2):
	d_distance = 0
	index = 0

	while index < v2.length():
		d_distance+= v1[index]*v2[index];
		index += 1

	return d_distance

def dot_set_distance(v1, v2):
	
	d_distance = 0
	for v1_it in v1:
		if v1_it in v2:
			d_distance += 1

	return d_distance

def jaccard_distance(a, b):
	sim = jaccard_index(a, b)
	d_distance = 1.0 - sim
	return d_distance


def all_pair_distance(features, distanceType, Dist):
	N = len(features)
	print ('  Computing', N, 'X', N, 'distance matrix ...')

	del Dist[:]

	for i in range(N):
		d_temp = {}
		for j in range(N):
			if distanceType == JACCARD:
				d_temp[j] = jaccard_distance(features[i], features[j])
			else:
				d_temp[j] = -dot_distance(features[i], features[j])

		Dist.append(d_temp)

	return 1


def testList(tmpList):
	del tmpList[:]
	tmpList.append(0)

if __name__=='__main__':
	tmpList = [1,2,3]
	testList(tmpList)

	print (tmpList)