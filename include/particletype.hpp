#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
#include <iostream>

class ParticleType {
 public:
  ParticleType(const char* Name, const double Mass, const int Charge);
  const char* GetName()const;
  const double GetMass()const;
  const int GetCharge()const;
  virtual const double GetWidth()const;
  virtual void Print();

 private:
  const char* fName;
  const double fMass;
  const int fCharge;
};
#endif