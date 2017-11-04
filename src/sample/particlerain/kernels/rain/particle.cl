#include    <components/particle.script.hh>
#include    <components/obstacle.script.hh>


void kmain(u32, u32,
           segments<BugEngine::ParticleRain::Particle> particles,
           segments<const BugEngine::ParticleRain::Obstacle> obstacles)
{
    be_forceuse(particles);
    be_forceuse(obstacles);
}
