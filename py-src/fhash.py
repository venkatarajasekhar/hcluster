from bitarray import bitarray

def jaccard_index(a, b):
	num = (a & b).count()
	den = (a | b).count()

	if abs(den) < 1e-10:
		return 0.0

	return num/den


if __name__=='__main__':
	a = bitarray('101010')
	b = bitarray('010101')

	print jaccard_index(a, b)