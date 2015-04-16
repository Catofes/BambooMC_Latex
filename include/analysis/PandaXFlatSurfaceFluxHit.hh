#ifndef PANDAXFLATSURFACEFLUXHIT_H
#define PANDAXFLATSURFACEFLUXHIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

#include <string>

using std::string;

class PandaXFlatSurfaceFluxHit : public G4VHit {
public:
  PandaXFlatSurfaceFluxHit ();
  ~PandaXFlatSurfaceFluxHit ();

  const PandaXFlatSurfaceFluxHit & operator=(const PandaXFlatSurfaceFluxHit &right);

  int operator==(const PandaXFlatSurfaceFluxHit & right) const;

  inline void * operator new(size_t);

  inline void operator delete(void *aHit);

  void Draw() {;}
  void Print() {;}

  double getEnergy() const;
  G4ThreeVector getMomentum() const;
  int getSurface () const;

  void setEnergy(double energy);
  void setMomentum(G4ThreeVector & momentum);
  void setSurface(int s);

private:
  double _energy;
  G4ThreeVector _momentum;
  int _surface;
};

typedef G4THitsCollection<PandaXFlatSurfaceFluxHit> PandaXFlatSurfaceFluxHitsCollection;

extern G4Allocator<PandaXFlatSurfaceFluxHit> * pandaXFlatSurfaceFluxHitAllocator;

inline void * PandaXFlatSurfaceFluxHit::operator new(size_t)
{
  if (!pandaXFlatSurfaceFluxHitAllocator)
    pandaXFlatSurfaceFluxHitAllocator = new G4Allocator<PandaXFlatSurfaceFluxHit>;
  return (void *) pandaXFlatSurfaceFluxHitAllocator->MallocSingle();
}

inline void PandaXFlatSurfaceFluxHit::operator delete(void * aHit)
{
  pandaXFlatSurfaceFluxHitAllocator->FreeSingle((PandaXFlatSurfaceFluxHit*)aHit);
}
#endif// PANDAXSPACEHIT_H
