# Using readlines()
file1 = open('ft_nm.out', 'r')
ft_nm = file1.readlines()
file2 = open('nm.out', 'r')
nm = file2.readlines()
 
the_file = open('diff', 'w')

for i in range(len(ft_nm)):
	if ft_nm[i] != nm[i]:
		the_file.write(ft_nm[i])
		the_file.write(nm[i])
