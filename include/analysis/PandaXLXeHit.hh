#ifndef PANDAXLXEHIT_H
#define PANDAXLXEHIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>

#include <string>

using std::string;

class PandaXLXeHit : public G4VHit {
public:
  PandaXLXeHit ();
  ~PandaXLXeHit ();

  const PandaXLXeHit & operator=(const PandaXLXeHit &right);

  int operator==(const PandaXLXeHit & right) const;

  inline void * operator new(size_t);

  inline void operator delete(void *aHit);

  void Draw() {;}
  void Print() {;}

  int getTrackId() const;
  int getParentId() const;
  double getEnergy() const;
  double getX() const;
  double getY() const;
  double getZ() const;
  double getT() const;
  const string & getType() const;
  const string & getCreatorProcess() const;
  const string & getDepositionProcess() const;
  const string & getParent() const;
  
  void setTrackId(int id);
  void setParentId(int id);
  void setEnergy(double energy);
  void setX(double x);
  void setY(double y);
  void setZ(double z);
  void setT(double t);
  void setType(const string &type);
  void setCreatorProcess(const string &process);
  void setDepositionProcess(const string& process);
  void setParent(const string &parent);

private:
  int _trackId;
  int _parentId;
  double _energy;
  double _x;
  double _y;
  double _z;
  double _t;
  string _type;
  string _creatorProcess;
  string _depositionProcess;
  string _parent;
};

typedef G4THitsCollection<PandaXLXeHit> PandaXLXeHitsCollection;

extern G4Allocator<PandaXLXeHit> * pandaXLXeHitAllocator;

inline void * PandaXLXeHit::operator new(size_t)
{
  if (!pandaXLXeHitAllocator)
    pandaXLXeHitAllocator = new G4Allocator<PandaXLXeHit>;
  return (void *) pandaXLXeHitAllocator->MallocSingle();
}

inline void PandaXLXeHit::operator delete(void * aHit)
{
  pandaXLXeHitAllocator->FreeSingle((PandaXLXeHit*)aHit);
}
#endif// PANDAXLXEHIT_H
