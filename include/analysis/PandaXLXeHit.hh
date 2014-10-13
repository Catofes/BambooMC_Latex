#ifndef PANDAXLXEHIT_H
#define PANDAXLXEHIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>

#include <string>

using std::string;

class PandaXLXeHit : public G4VHit {
public:
  PandaXLXeHit ();
  ~PandaXLXeHit ();

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
  const string & getProcess() const;
  const string & getParent() const;
  
  void setTrackId(int id);
  void setParentId(int id);
  void setEnergy(double energy);
  void setX(double x);
  void setY(double y);
  void setZ(double z);
  void setT(double t);
  void setType(const string &type);
  void setProcess(const string &process);
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
  string _process;
  string _parent;
};

typedef G4THitsCollection<PandaXLXeHit> PandaXLXeHitsCollection;

#endif// PANDAXLXEHIT_H
