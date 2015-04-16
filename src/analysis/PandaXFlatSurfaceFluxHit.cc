#include "analysis/PandaXFlatSurfaceFluxHit.hh"

G4Allocator<PandaXFlatSurfaceFluxHit> * pandaXFlatSurfaceFluxHitAllocator = new G4Allocator<PandaXFlatSurfaceFluxHit>;

PandaXFlatSurfaceFluxHit::PandaXFlatSurfaceFluxHit()
  : _energy(0), _surface(0)
{
}

PandaXFlatSurfaceFluxHit::~PandaXFlatSurfaceFluxHit()
{
}

const PandaXFlatSurfaceFluxHit & PandaXFlatSurfaceFluxHit::operator=(const PandaXFlatSurfaceFluxHit & right)
{
  _energy = right._energy;
  _momentum = right._momentum;
  _surface = right._surface;
  return * this;
}

int PandaXFlatSurfaceFluxHit::operator==(const PandaXFlatSurfaceFluxHit &right) const
{
  return ((this == &right)?1:0);
}

double PandaXFlatSurfaceFluxHit::getEnergy() const
{
  return _energy;
}

G4ThreeVector PandaXFlatSurfaceFluxHit::getMomentum() const
{
  return _momentum;
}

int PandaXFlatSurfaceFluxHit::getSurface() const
{
  return _surface;
}

void PandaXFlatSurfaceFluxHit::setEnergy(double energy)
{
  _energy = energy;
}

void PandaXFlatSurfaceFluxHit::setMomentum(G4ThreeVector & momentum)
{
  _momentum = momentum;
}

void PandaXFlatSurfaceFluxHit::setSurface(int s)
{
  _surface = s;
}
