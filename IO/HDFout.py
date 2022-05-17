import h5py
import numpy as np
import ParticleSys as ps
import matplotlib.pyplot as plt
wall=ps.Wall()
gp=h5py.File("HDF5Data.hdf5")["output_data"]
pl=[]
for i in gp:
    pl.append(ps.ParticleSystem(False,wall,datagroup=gp[i],rlist=[0.2 for i in range(2000)],mlist=[1 for i in range(2000)]))#*读取数据
pl.sort()
#*这里已经将数据输出排好序
#todo可以进行相关操作

