import random
import sys

level1 = ['e','r','r','r','r','r','r','b','b','b','b','b','b','w','w']
level2 = ['e','rxxxxxx','r','r','r','r','r','b','b','b','b','w','w','y','y']
level3 = ['e','r','r','r','r','b','b','b','b','w','w','y','y','g','g']
level4 = ['e',  'r','r','r','r','b','b','w','w','y','y','g','g','p','p']

if sys.argv[1] == '1':
    array = level1
elif sys.argv[1] == '2':
    array = level2
elif sys.argv[1] == '3':
    array = level3
elif sys.argv[1] == '4':
    array = level4
else:
    array = level1
with open('level'+sys.argv[1]+'.txt', 'w') as f:
    for i in range(int(sys.argv[2])):
        random.shuffle(array)
        for c in array:
            f.write(c+' ')
        f.write('\n')
    f.close()