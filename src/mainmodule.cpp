#include "particle.hpp"
#include "TMath.h"
#include "TRandom.h"
int main() {
  Particle::AddParticleType((const char*)'pi+', 0.13957, 1, 0);
  Particle::AddParticleType((const char*)'pi-', 0.13957, -1, 0);
  Particle::AddParticleType((const char*)'Ka+', 0.49367, 1, 0);
  Particle::AddParticleType((const char*)'Ka-', 0.49367, -1, 0);
  Particle::AddParticleType((const char*)'pr+', 0.93827, 1, 0);
  Particle::AddParticleType((const char*)'pr-', 0.93827, -1, 0);
  Particle::AddParticleType((const char*)'K*+', 0.89166, 0, 0.050);
  gRandom->SetSeed();
  const int events = 100000;
  Int_t N = 120;
  Double_t mean = 1.;
  Particle* EventParticles[N];
  for (Int_t i = 0; i < events; i++) {
    for (Int_t j = 0; j < 100; j++) {
      double Phi = gRandom->Uniform(0., 2 * TMath::Pi());
      double Theta =  gRandom->Uniform(0., TMath::Pi());
      double pulse =  gRandom->Exp(mean);

      double px = pulse * sin(Theta) * cos(Phi);
      double py = pulse * sin(Theta) * sin(Phi);
      double pz = pulse * cos(Theta);

      EventParticles[j].SetP(px, py, pz);
    }
  }
}