from tools import *



class classifier:
	def __init__(self, bfs, dist):
		self.features = bfs
		self.Dist = dist
		self.N = len(self.Dist)

	def __init__(self, dist):
		self.Dist = dist
		self.N = len(self.Dist)


	def setCluster(self, k):
		self.cutoffType = BY_NUMBER
		self.numCluster = k
		self.threshold = 10 * MIN_DIST

	def setThreshold(self, t):
		self.cutoffType = BY_THRESHOLD
		self.threshold = t
		self.numCluster = 1


	def populate_distance(self):

		for i in range(self.N):
			E_temp = self.Dist[i]
			V_temp = {}

			for key in E_temp:
				j = key
				D_temp = (j, E_temp[key])


if __name__=='__main__':
	print 'hello world'
