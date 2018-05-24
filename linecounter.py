import os

okpat = ["cpp", "h", "sh"]

def check(file):
	# Makefile

	if file == "Makefile" or file == "makefile":
		return True
	
	s = file.split('.')
	if len(s) == 1:
		return True

	# only core spider counts	
	if s[1] == "py":
		if "spider" in s[0]:
			print file
			return True	
		else:
			return False

	# other files		
	if s[1] in okpat:
		print file
		return True

	return False
	
def counter(path):
	files = os.listdir(path)

	tot_line = 0
	tot_size = 0

	for file in files:
		if not check(file):
			continue
		if not os.path.isdir(path + "/" + file):
			f = open(path + "/" + file)
			iter_f = iter(f)
			for line in iter_f:
				tot_line += 1
				tot_size += len(line)
		else:
			a, b = counter(path + "/" + file)
			tot_line += a
			tot_size += b

	return tot_line, tot_size

path = "."
ra, rb = counter(path)
print "total line: %d\ntotal size: %d Bytes (%.3f KB)" % (ra, rb, rb / 1024.0)
