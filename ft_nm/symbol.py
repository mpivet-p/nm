# Using readlines()
file1 = open('ft_nm.out', 'r')
ft_nm = file1.readlines()
file2 = open('nm.out', 'r')
nm = file2.readlines()
 
for i in range(len(ft_nm)):
	if ft_nm[i] != nm[i]:
		print(ft_nm[i].strip())
		print(nm[i].strip())
