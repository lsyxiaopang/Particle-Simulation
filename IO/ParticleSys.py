import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import h5py
from sympy import cse

class Particle:
    """ 该类表示一个粒子
    """    
    def __init__(self,xc:float,yc:float,vx:float,vy:float,r=0.2,m=1.0,crash_times=0) -> None:
        """__init__ 粒子的初始化

        Parameters
        ----------
        xc : float
            表示x坐标
        yc : float
            表示y坐标
        vx : float
            x方向速度
        vy : float
            y方向速度
        crash_times : int, optional
            碰撞次数(初始为0)
        r : float
            粒子半径
        m : float
            _description_
        crash_times : int, optional
            _description_, by default 0
        """              
        self.x=xc
        self.y=yc
        self.vx=vx
        self.vy=vy
        self.r=r
        self.m=m
        self.crash_times=crash_times
    @property
    def data_frame(self)->pd.DataFrame:
        return pd.DataFrame(self.__dict__,index=[0])


class Wall:
    def __init__(self,Acx=0,Acy=0,Bcx=500,Bcy=500,crash_times=0) -> None:
        """__init__ 边界的初始化

        Parameters
        ----------
        Acx : int, optional
            左下角点x坐标, by default 0
        Acy : int, optional
            左下角点y坐标, by default 0
        Bcx : int, optional
            右上角点x坐标, by default 500
        Bcy : int, optional
            右上角点y坐标, by default 500
        crash_times : int, optional
            碰撞次数,初始为0
        """        
        self.Ac=np.array([Acx,Acy])
        self.Bc=np.array([Bcx,Bcy])
        self.crash_times=crash_times
    @property
    def xlim(self)->list:
        """xlim 获得x范围

        Returns
        -------
        list shape(2)
            x的最小和最大值
        """           
        return [self.Ac[0],self.Bc[0]]
    @property
    def ylim(self)->list:
        """ylim 获得y范围

        Returns
        -------
        list shape(2)
            y的最小和最大值
        """  
        return [self.Ac[1],self.Bc[1]]

class ParticleSystem:
    def __init__(self,inp:bool,wall:Wall,particle_array=None,datagroup=None,rlist=None,mlist=None) -> None:
        """__init__ 初始化一个粒子系统

        Parameters
        ----------
        inp : bool
            区分是输入还是输出
        wall : Wall
            墙壁
        particle_array : np.array
            (输入)粒子列表  
        datagroup : h5py.Group
            (输入/输出)数据
        rlist : np.array
            (输出)半径列表
        mlist : np.array
            (输出)质量列表
        """        
        if inp:
            self.particle_array=particle_array
            self.wall=wall
            self.time=0
            self.outdatagroup=datagroup
        else:
            xset:h5py.Dataset=datagroup["x"]
            yset:h5py.Dataset=datagroup["y"]
            vxset:h5py.Dataset=datagroup["vx"]
            vyset:h5py.Dataset=datagroup["vy"]
            cset:h5py.Dataset=datagroup["particle crash times"]
            kl=[]
            for i in range(len(xset)):
                kl.append(Particle(xset[i],yset[i],vxset[i],vyset[i],rlist[i],mlist[i],cset[i]))
            self.time=datagroup.attrs["time"]
            self.wall=wall
            self.wall.crash_times=datagroup.attrs["crash times"]
            self.particle_array=np.array([Particle(xset[i],yset[i],vxset[i],vyset[i],rlist[i],mlist[i],cset[i]) for i in range(len(xset))])

        self.make_DataFrame()
        
    def make_DataFrame(self)->pd.DataFrame:
        """make_DataFrame 将数据转换成数据表

        Returns
        -------
        pd.DataFrame
            数据表
        """        
        op=pd.DataFrame([])
        for i in self.particle_array:
            op=pd.concat([op,i.data_frame],axis=0)
        self.pDataFrame=op
        return op
    def show(self,axes:plt.Axes)->None:
        """show 展示粒子在空间中分布

        Parameters
        ----------
        axes : plt.Axes
            绘图
        """        
        axes.scatter(self.pDataFrame["x"],self.pDataFrame["y"],self.pDataFrame["r"])
        axes.set_xlim(self.wall.xlim)
        axes.set_ylim(self.wall.ylim)
    def in_write_hdf5(self,outstep:float,all_time:float,d:float):
        """in_write_hdf5 将输入数据写入hdf5中

        Parameters
        ----------
        outstep : float
            输出的时间步
        all_time : float
            总共计算的时间
        d : float
            特征长度
        """        
        self.outdatagroup["x"]=self.pDataFrame["x"]
        self.outdatagroup["y"]=self.pDataFrame["y"]
        self.outdatagroup["vx"]=self.pDataFrame["vx"]
        self.outdatagroup["vy"]=self.pDataFrame["vy"]
        self.outdatagroup["r"]=self.pDataFrame["r"]
        self.outdatagroup["m"]=self.pDataFrame["m"]
        self.outdatagroup.attrs["out_put_step"]=outstep
        self.outdatagroup.attrs["time_out"]=all_time
        self.outdatagroup.attrs["d"]=d
        self.outdatagroup.attrs["count"]=len(self.particle_array)
    def __lt__(self,other):
        return self.time<other.time
    def __gt__(self,other):
        return self.time>other.time
    
