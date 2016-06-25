from tools import *
import numpy as np

def jaccard_index(a, b):
	num = (a & b).count()
	den = (a | b).count()

	if abs(den) < 1e-10:
		return 0.0

	return (num*1.0)/den

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


def all_pair_distance_dict(features, distanceType, Dist):

	N = len(features)
	print '  Computing', N, 'X', N, 'distance matrix ...'

	del Dist[:]

	for i in range(N):
		d_temp = {}
		for j in range(N):
			if distanceType == JACCARD:
				d_temp[j] = jaccard_distance(features[i], features[j])
				#print d_temp[j]
			else:
				d_temp[j] = -dot_distance(features[i], features[j])

		Dist.append(d_temp)

def all_pair_distance_numpy(features, Dist):
	N = features.shape[0]
	print '  Computing', N, 'X', N, 'distance matrix ...'

	sumFeatures = np.sum(features, axis = 1)
	distMatrix = features.dot(np.transpose(features))

	del Dist[:]

	for i in range(N):
		d_temp = []
		for j in range(N):
			union = sumA[i] + sumA[j] - distMatrix[i, j]
			if union == 0:
				d_temp.append(1)
			else:
				d_temp.append(1 - distMatrix[i, j] * 1.0 / union)

		Dist.append(d_temp)



def all_pair_distance_list(features, Dist):
	N = len(features)
	print '  Computing', N, 'X', N, 'distance matrix ...'

	A = np.zeros((N, 240007))
	for i in range(N):
		for j in range(features[i].length()):
			A[i, j] = features[i][j]


	sumA = np.sum(A, axis=1)
	distMatrix = A.dot(np.transpose(A))
	
	del Dist[:]
	for i in range(N):
		d_temp = []
		for j in range(N):
			union = sumA[i] + sumA[j] - distMatrix[i, j]
			if union == 0:
				d_temp.append(1)
			else: 
				d_temp.append(1 - distMatrix[i, j] * 1.0 / union)

		Dist.append(d_temp)


def all_pair_distance(features, distanceType, Dist):

	N = len(features)
	print '  Computing', N, 'X', N, 'distance matrix ...'

	del Dist[:]

	for i in range(N):
		d_temp = []
		#print i
		for j in range(N):
			if distanceType == JACCARD:
				d_temp.append(jaccard_distance(features[i], features[j]))
				#if d_temp[j] > 0:
				#	print d_temp[j]
			else:
				d_temp.append(-dot_distance(features[i], features[j]))

		Dist.append(d_temp)
		#print i
		#print d_temp
		#exit(0)

def group_distance_dict(features1, features2, distanceType, Dist):
	print '  Computing', len(features1), 'X', len(features2), 'distance matrix ...'
	
	del Dist[:]

	for i in range(len(features1)):
		d_temp = {}
		for j in range(len(features2)):
			if distanceType == JACCARD:
				d_temp[j] = jaccard_distance(features1[i], features2[j])
			else:
				d_temp[j] = -dot_distance(features1[i], features2[j])

		Dist.append(d_temp)


def group_distance(features1, features2, distanceType, Dist):
	print '  Computing', len(features1), 'X', len(features2), 'distance matrix ...'
	
	del Dist[:]

	for i in range(len(features1)):
		d_temp = []
		for j in range(len(features2)):
			if distanceType == JACCARD:
				d_temp.append(jaccard_distance(features1[i], features2[j]))
			else:
				d_temp.append(-dot_distance(features1[i], features2[j]))

		Dist.append(d_temp)

 

if __name__=='__main__':
	tmpList = [1,2,3]
	testList(tmpList)

	print (tmpList)
