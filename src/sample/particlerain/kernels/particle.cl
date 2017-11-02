#include    <components/particle.script.hh>
#include    <components/obstacle.script.hh>

void kmain(u32, u32,
           segment<BugEngine::ParticleRain::Particle> particles,
           segment<const BugEngine::ParticleRain::Obstacle> obstacles)
{
    be_forceuse(particles);
    be_forceuse(obstacles);
}
