a = """
	2,
	1,
	3,
	1,
	1,
	1,
	4,
	2,
	7,
	2,
	3,
	2,
	8,
	3,
	5,
	3,
	7,
	3,
	6,
	4,
	1,
	4,
	5,
	4,
	7,
	5,
	1,
	5,
	3,
	5,
	4,
	6,
	6,
	6,
	8,
	6,
	2,
	1,
	4,
	1,
	3,
	1,
	4,
	2,
	8,
	2,
	7,
	2,
	8,
	3,
	6,
	3,
	5,
	3,
	6,
	4,
	2,
	4,
	1,
	4,
	7,
	5,
	5,
	5,
	1,
	5,
	4,
	6,
	2,
	6,
	6,
	6,
"""

code = map(lambda x: str(int(x)-1) if len(x) > 0 else '', a.replace('\n', '').replace('\t', '').split(','))
print(",".join(code))