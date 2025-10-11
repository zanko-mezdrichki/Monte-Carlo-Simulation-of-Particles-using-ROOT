#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "resonancetype.hpp"
#include <cmath>

class Particle {
 public:
  Particle(const char* NamePart, double Px, double Py, double Pz);
  const double GetPx() const;
  const double GetPy() const;
  const double GetPz() const;
  const int GetIndex() const;
  const double GetMass() const;
  const double GetEnergy() const;
  const double GetInvMass(Particle& P) const;
  static void AddParticleType(const char* Name, double const Mass,
                              int const Charge, double const Width);
  void SetIndex(int const index);
  void SetIndex(const char*  name);
  void PrintData();
  void PrintProperties();

  void SetP(double Px, double Py, double Pz);
  int Decay2body(Particle &dau1,Particle &dau2) const;
  void Boost(double bx, double by, double bz);

 private:
  static const int fMaxNumParticleType = 10;
  static ParticleType* Array[fMaxNumParticleType];
  static int fNParticleType;
  int fIndex;
  double fPx = 0;
  double fPy = 0;
  double fPz = 0;
  static int FindParticle(const char* NamePart);
};

#endif