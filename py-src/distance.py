from tools import *
import numpy as np



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

def jaccard_index(a, b):
	num = (a & b).count()
	den = (a | b).count()

	if abs(den) < 1e-10:
		return 0.0

	return (num*1.0)/den

def jaccard_distance(a, b):
	sim = jaccard_index(a, b)
	d_distance = 1.0 - sim
	return d_distance

def all_pair_distance(features, distanceType):
	N = len(features)
	print '  Computing', N, 'X', N, 'distance matrix ...'

	Dist = []

	for i in range(N):
		d_temp = []
		for j in range(N):
			if distanceType == JACCARD:
				d_temp.append(jaccard_distance(features[i], features[j]))
				
			else:
				d_temp.append(-dot_distance(features[i], features[j]))

		Dist.append(d_temp)
		
	return Dist

def all_pair_distance_dict(features, distanceType):
	DistList = all_pair_distance(features, distanceType)
	Dist = []
	for i in range(N):
		d_temp = {}
		for j in range(N):
			d_temp[j] = DistList[i][j]

		Dist.append(d_temp)

	return Dist


def all_pair_distance_numpy(features):
	N = features.shape[0]
	print '  Computing', N, 'X', N, 'distance matrix ...'

	Dist = []
	sumFeatures = np.sum(features, axis = 1)
	distMatrix = features.dot(np.transpose(features))

	for i in range(N):
		d_temp = []
		for j in range(N):
			union = sumFeatures[i] + sumFeatures[j] - distMatrix[i, j]
			if union == 0:
				d_temp.append(1)
			else:
				d_temp.append(1 - distMatrix[i, j] * 1.0 / union)

		Dist.append(d_temp)

	return Dist

def all_pair_distance_list(features):
	N = len(features)

	A = np.zeros((N, FEATURE_SIZE))
	for i in range(N):
		for j in range(features[i].length()):
			A[i, j] = features[i][j]

	return all_pair_distance_numpy(A)



def group_distance_numpy(features1, features2):
	N = features1.shape[0]
	M = features2.shape[1]

	print '  Computing', N, 'X', M, 'distance matrix ...'

	Dist = []
	sumFeatures1 = np.sum(features1, axis = 1)
	sumFeatures2 = np.sum(features2, axis = 1)

	distMatrix = features1.dot(np.transpose(features2))

	for i in range(N):
		d_temp = []
		for j in range(M):
			union = sumFeatures1[i] + sumFeatures2[j] - distMatrix[i, j]
			if union == 0:
				d_temp.append(1)
			else:
				d_temp.append(1 - distMatrix[i, j] * 1.0 / union)

		Dist.append(d_temp)

	return Dist

def group_distance_list(features1, features2):
	N = len(features1)
	M = len(features2)

	A = np.zeros((N, FEATURE_SIZE))
	B = np.zeros((M, FEATURE_SIZE))

	return group_distance_numpy(A, B)

def group_distance(features1, features2, distanceType):
	print '  Computing', len(features1), 'X', len(features2), 'distance matrix ...'
	
	Dist = []

	for i in range(len(features1)):
		d_temp = []
		for j in range(len(features2)):
			if distanceType == JACCARD:
				d_temp.append(jaccard_distance(features1[i], features2[j]))
			else:
				d_temp.append(-dot_distance(features1[i], features2[j]))

		Dist.append(d_temp)

	return Dist


def group_distance_dict(features1, features2, distanceType):
	DistList = group_distance(features1, features2, distanceType)

	Dist = []

	for i in range(features1):
		d_temp = {}
		for j in range(features2):
			d_temp[j] = DistList[i][j]

		Dist.append(d_temp)

	return Dist


if __name__=='__main__':
	tmpList = [1,2,3]
	testList(tmpList)

	print (tmpList)
