#include "analysis/PandaXLXeHit.hh"

G4Allocator<PandaXLXeHit> * pandaXLXeHitAllocator = new G4Allocator<PandaXLXeHit>;

PandaXLXeHit::PandaXLXeHit()
  : _trackId(-1), _parentId(-1), _energy(0), _x(0), _y(0), _z(0),
    _type("unknown"), _creatorProcess("unknown"), _depositionProcess("unknown"), _parent("none")
{
}

PandaXLXeHit::~PandaXLXeHit()
{
}

const PandaXLXeHit & PandaXLXeHit::operator=(const PandaXLXeHit & right)
{
  _trackId = right._trackId;
  _parentId = right._parentId;
  _energy = right._energy;
  _x = right._x;
  _y = right._y;
  _z = right._z;
  _t = right._t;
  _type = right._type;
  _creatorProcess = right._creatorProcess;
  _depositionProcess = right._depositionProcess;
  _parent = right._parent;
  return * this;
}

int PandaXLXeHit::operator==(const PandaXLXeHit &right) const
{
  return ((this == &right)?1:0);
}

int PandaXLXeHit::getTrackId () const
{
  return _trackId;
}

int PandaXLXeHit::getParentId () const
{
  return _parentId;
}

double PandaXLXeHit::getEnergy() const
{
  return _energy;
}

double PandaXLXeHit::getX() const
{
  return _x;
}

double PandaXLXeHit::getY() const
{
  return _y;
}

double PandaXLXeHit::getZ() const
{
  return _z;
}

double PandaXLXeHit::getT() const
{
  return _t;
}

const string & PandaXLXeHit::getType() const
{
  return _type;
}

const string & PandaXLXeHit::getCreatorProcess() const
{
  return _creatorProcess;
}

const string & PandaXLXeHit::getDepositionProcess() const
{
  return _depositionProcess;
}

const string & PandaXLXeHit::getParent() const
{
  return _parent;
}

void PandaXLXeHit::setTrackId (int id)
{
  _trackId = id;
}

void PandaXLXeHit::setParentId (int id)
{
  _parentId = id;
}
  
void PandaXLXeHit::setEnergy(double energy)
{
  _energy = energy;
}

void PandaXLXeHit::setX(double x)
{
  _x = x;
}

void PandaXLXeHit::setY(double y)
{
  _y = y;
}

void PandaXLXeHit::setZ(double z)
{
  _z = z;
}

void PandaXLXeHit::setT(double t)
{
  _t = t;
}

void PandaXLXeHit::setType(const string &type)
{
  _type = type;
}

void PandaXLXeHit::setCreatorProcess(const string &process)
{
  _creatorProcess = process;
}

void PandaXLXeHit::setDepositionProcess(const string &process)
{
  _depositionProcess = process;
}

void PandaXLXeHit::setParent(const string &parent)
{
  _parent = parent;
}
