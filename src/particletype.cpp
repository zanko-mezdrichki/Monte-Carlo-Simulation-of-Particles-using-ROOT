#include "particletype.hpp"
const char* ParticleType::GetName() const { return fName; }
const double ParticleType::GetMass() const { return fMass; }
const int ParticleType::GetCharge() const { return fCharge; }
const double ParticleType::GetWidth() const { return 0; }

ParticleType::ParticleType(const char* Name, const double Mass,
                           const int Charge)
    : fName(Name), fMass(Mass), fCharge(Charge) {};

void ParticleType::Print() {
  std::cout << "Name: " << fName << std::endl;
  std::cout << "Mass: " << fMass << std::endl;
  std::cout << "Charge: " << fCharge << std::endl;
}