#include "resonancetype.hpp"

const double ResonanceType::GetWidth()const { return fWidth; };
ResonanceType::ResonanceType(const char* Name, const double Mass,
                             const int Charge, const double Width)
    : ParticleType::ParticleType(Name, Mass, Charge), fWidth(Width) {};

void ResonanceType::Print()  {
  ParticleType::Print();
  std::cout << "Width: " << fWidth<<std::endl;
};