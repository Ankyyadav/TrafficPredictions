# -*- coding: utf-8 -*-
"""
Created on Wed Mar  4 12:38:30 2020

@author: Shraddha Jain
"""

# -*- coding: utf-8 -*-
"""
Created on Mon Mar  2 15:43:12 2020

@author: Shraddha Jain
"""
from pylab import*
import math
import numpy as np
import matplotlib.pyplot as plt
from random import randrange
from random import sample
num_cars = 10 ##number of cars initially
L = 100
num_cells = (int)(L/7.5)
tot_time = 30
Vmax = 5
prob = 0.3
c=0
pos_time = np.zeros([tot_time,num_cells])
flag=0
num = np.linspace(0,num_cells-1,num_cells,dtype=int)
num = num.tolist()
alpha = 0.5
beta = 0.8
flag = 0
for i in range(num_cars):
    rdm = sample(num,1)
    num.remove(rdm[0])
    pos_time[0,rdm[0]] = rdm[0]+1
    

vel = []

for i in range(num_cars): ##assigning initial velocities
    R = randrange(0,Vmax+1,1)
    vel.append(R)
    
###########################################################
def pos_to_vel(T,P):
    e = 0
    for i in range(num_cells):
        if(pos_time[T,P]!=0):
            
            
###########################################################

##print(vel[0])
#print(vel)#.tolist())
#print(pos_time)## prints initial position

################function begins#############################
def add_remove(t1): ## function to add or remove cars with probability alpha

    if(pos_time[t1,0]==0):## if the first cell is empty at a given time t
        rd1 = np.random.random() 
        if(rd1<alpha):
            pos_time[t1,0] = 1 ##first position
            vel.insert(0,randrange(0,Vmax+1,1)) ##assigning velocity to the newly added car
            
    if(pos_time[t1,num_cells-1] !=0):# if the last cell is occupied, remove.
        rd2 = np.random.random()
        if(rd2<beta):
            pos_time[t1,num_cells-1] = 0 ##removal of the car
            vel.pop(len(vel)-1) ## removing the velocity
    
    return pos_time
##################function ends############################    

##################function begins##########################
def pos_next(t2,p2):### function to find the position of the next car
    flag = 0
    for i in range(p2+1,num_cells):
        if(pos_time[t2,i]!=0):
            b = pos_time[t2,i]
            flag = 1
            break
        
    if(flag==1):
        return b
    else:
        return 0 ##indication that there are no cars in front of this car
##################function ends###########################

for t in range(tot_time-1):
    i=0
    for p in range(num_cells):
        if(p==num_cells-1 and pos_time[t,p]!=0):
            break
        else:
            if(pos_time[t,p]!=0):
                p1 = pos_time[t,p]
                p2 = pos_next(t,p)
                if(p2!=0):
                    d = abs(p2-p1)
                    
                if(vel[i]<Vmax): ## first condition
                    vel[i] = vel[i]+1

                if(p2!=0): 
                    if(d<=vel[i]): ## second condition
                        vel[i] = d-1

                if(vel[i]>0): ## third condition - randomization
                    r = np.random.random()
                    if(r<prob):
                        vel[i] = vel[i]-1
                n = (int)(pos_time[t,p] + vel[i])
                if(n<=num_cells):
                    pos_time[t+1,n-1] = n
                i = i+1
    pos_time = add_remove(t+1)

print(pos_time)

