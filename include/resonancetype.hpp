#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "particletype.hpp"

class ResonanceType : public ParticleType {
    public:
    ResonanceType(const char* Name, const double Mass, const int Charge, const double Width);
    const double GetWidth()const override ;
    void Print() override;
    
    private:
    const double fWidth;
};

#endif