#!/usr/local/bin/python3

import numpy as np
import numpy.random as npr
import sys

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print(f'usage: python {sys.argv[0]} vertices [file_name]')
		exit(1)

	rng = npr.default_rng()
	n = int(sys.argv[1])
	file_name = sys.argv[2] if len(sys.argv)>2 else 'in'
	f = open(file_name,'w')

	print(n,file=f)
	for _ in range(n):
		ws = rng.beta(0.5,0.5,n)*1000
		print(*[int(i) for i in ws],file=f,sep=' ',end='\n');

	f.close()
