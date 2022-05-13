

#include "ParticleSimulation.h"


#include"IOHelper.h"
#include"vec.h"
#include"particle.h"
#include"particlesystem.h"

void probe_func(ParticleSystem* ps)
{
    printf("%0.4f\n", ps->get_time());
}

int main(int argc,char** argv)
{
    //Particle a(vec(50,50),vec(-100,10),1,1);
    //Particle b(vec(60, 60), vec(-1, -1), 1, 1);
    //Particle* plist = new Particle[40000];
    //for (int i = 0; i < 200; i++)
    //{
    //    for (int j = 0; j < 200; j++)
    //    {
    //        Particle* thp = plist + i*200+j;
    //        thp->coordinate = vec(5 + 0.5 * i, 1 + 0.5 * j);
    //        thp->velocity = vec(100,-100);
    //        thp->radius = 0.2;
    //    }
    //}

    //Wall sw(vec(0, 0), vec(500, 500));
    //ParticleSystem ps(plist, 40000, &sw, 0.2);
    //ps.auto_run(100,*probe_func);
    //std::ofstream out1("out1.txt");
    //std::ofstream out2("out2.txt");
    //for (int i = 0; i < 2000; i++)
    //{
    //    out1 << plist[i].velocity.mod() << " ";
    //    out2 << plist[i].coordinate.x << " " << plist[i].coordinate.y<<"\n";
    //}
    //    
    //a.run_step(5);
    //Wall wall(vec(0,0),vec(500,500));
    //wall.check_handle_crash(a, 5);
    IOHelper ios(argv[1]);
    ios.init_particle();
    ios.auto_run();
    return 0;
}


