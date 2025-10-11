#include "particle.hpp"
#include <cstdlib>  //for RAND_MAX
ParticleType* Particle::Array[Particle::fMaxNumParticleType];
int Particle::fNParticleType = 0;
int Particle::FindParticle(const char* NamePart) {
  for (int i = 0; i < fNParticleType; ++i) {
    if (NamePart == Array[i]->GetName()) {
      return i;
    }
  }
  std::cout << "Particle not found!" << std::endl;
  return -1;
}
Particle::Particle(const char* NamePart, double Px, double Py, double Pz)
    : fIndex(FindParticle(NamePart)), fPx(Px), fPy(Py), fPz(Pz) {}
const double Particle::GetPx() const { return fPx; }
const double Particle::GetPy() const { return fPy; }
const double Particle::GetPz() const { return fPz; }
const int Particle::GetIndex() const { return fIndex; }
const double Particle::GetMass() const { return Array[fIndex]->GetMass(); }
const double Particle::GetEnergy() const {
  return {sqrt(pow(Array[fIndex]->GetMass(), 2) + pow(fPx, 2) + pow(fPy, 2) +
               pow(fPz, 2))};
}
const double Particle::GetInvMass(Particle& P) const {
  return sqrt(pow(GetEnergy() + P.GetEnergy(), 2) -
              ((pow(fPx + P.GetPx(), 2) + pow(fPy + P.GetPy(), 2) +
                pow(fPz + P.GetPz(), 2))));
}
void Particle::AddParticleType(const char* Name, double const Mass,
                               int const Charge, double const Width) {
  if (FindParticle(Name) == -1 && fNParticleType != fMaxNumParticleType) {
    if (Width > 0) {
      Array[fNParticleType] = new ResonanceType(Name, Mass, Charge, Width);
      fNParticleType++;

    } else {
      Array[fNParticleType] = new ParticleType(Name, Mass, Charge);
      fNParticleType++;
    }
  } else {
    std::cout << "Particle already in the array!" << std::endl;
  }
}
void Particle::SetIndex(int const index) {
  if (index >= 0 && index < fNParticleType) {
    fIndex = index;
  } else {
    std::cout << "Indice fuori dai limiti dell'array!\n";
  }
}

void Particle::SetIndex(const char* name) {
  int index = FindParticle(name);
  if (index >= 0) {
    fIndex = index;
  } else {
    std::cout << "Nome della particella non trovato!\n";
  }
}

void Particle::PrintData() {
  for (int i = 0; i < fNParticleType; i++) {
    Array[i]->Print();
  }
};
void Particle::PrintProperties() {
  std::cout << "Particella" << Array[fIndex]->GetName() << "numero " << fIndex
            << ", vettore impulso: " << "( " << fPx << ", " << fPy << ", "
            << fPz << " )" << '\n';
}

void Particle::SetP(double Px, double Py, double Pz) {
  fPx = Px;
  fPy = Py;
  fPz = Pz;
}
int Particle::Decay2body(Particle& dau1, Particle& dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();
  if (fIndex > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += Array[fIndex]->GetWidth() * y1;
  }
  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz) {
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}