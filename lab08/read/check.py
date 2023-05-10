f = open("result1.txt", 'r')
fp = open("truth.txt", 'r')

line1 = f.readlines()
line2 = fp.readlines()

check = True
for i in range(len(line2)):
    if(line1[i] != line2[i]):
        check = False 
        print(line1[i])
        print(line2[i])
        break

print(check)


f.close()