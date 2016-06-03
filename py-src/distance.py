from tools import *

def jaccard_index(a, b):
	num = (a & b).count()
	den = (a | b).count()

	if abs(den) < 1e-10:
		return 0.0

	return num/den

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
			else:
				d_temp[j] = -dot_distance(features[i], features[j])

		Dist.append(d_temp)

def all_pair_distance(features, distanceType, Dist):

	N = len(features)
	print '  Computing', N, 'X', N, 'distance matrix ...'

	del Dist[:]

	for i in range(N):
		d_temp = []
		for j in range(N):
			if distanceType == JACCARD:
				d_temp.append(jaccard_distance(features[i], features[j]))
			else:
				d_temp.append(-dot_distance(features[i], features[j]))

		Dist.append(d_temp)

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