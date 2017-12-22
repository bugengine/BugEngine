#include    <kernel/input/input.hh>
#include    <components/particle.script.hh>
#include    <components/obstacle.script.hh>

static void kmain(u32, u32,
                  be_segments(Particle) particles,
                  be_segments(const Obstacle) obstacles)
{
    (void)(particles);
    (void)(obstacles);
}
