import h5py
import numpy as np
import ParticleSys as ps
import matplotlib.pyplot as plt
plt.style.use('elegant.mplstyle')
l1=np.array([ps.Particle(30+np.linspace(0,400,1000)[i],
                        10+np.linspace(0,400,1000)[i],
                        100,100,0.2,1) for i in range(1000)])
l2=np.array([ps.Particle(5+np.linspace(0,400,1000)[i],
                        20+np.linspace(0,400,1000)[i],
                        100,100,0.2,1) for i in range(1000)])//初始化点
al=np.hstack([l1,l2])
wall=ps.Wall()
f=h5py.File("HDF5Data.h5","w")
g=f.create_group("input_data")
pcs=ps.ParticleSystem(True,wall,al,g)
ax=plt.subplot(111)
pcs.show(ax)
pcs.in_write_hdf5(0.05,30,0.2)
plt.show()#*展示点分布
f.flush()
f.close()



