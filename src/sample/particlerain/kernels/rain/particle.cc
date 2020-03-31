#include <kernel/input/input.hh>
#include <components/obstacle.script.hh>
#include <components/particle.script.hh>

void kmain(u32 index, u32 total,
           Kernel::segments<Particle> particles,
           Kernel::segments<Obstacle> obstacles)
{
    be_forceuse(index);
    be_forceuse(total);
    be_forceuse(particles);
    be_forceuse(obstacles);
}
