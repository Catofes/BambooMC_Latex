#include "analysis/PandaXLXeHit.hh"

PandaXLXeHit::PandaXLXeHit()
  : _trackId(-1), _parentId(-1), _energy(0), _x(0), _y(0), _z(0),
    _type("unknown"), _process("unknown"), _parent("")
{
}

PandaXLXeHit::~PandaXLXeHit()
{
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

const string & PandaXLXeHit::getProcess() const
{
  return _process;
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

void PandaXLXeHit::setProcess(const string &process)
{
  _process = process;
}

void PandaXLXeHit::setParent(const string &parent)
{
  _parent = parent;
}
