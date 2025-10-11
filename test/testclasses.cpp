#include "particle.hpp"
int main() {
  char name{'p'};
  double mass{3.1415};
  int charge{1};
  double width{5};
  ParticleType particle1(&name, mass, charge);
  ResonanceType particle2(&name, mass, charge, width);

  std::cout << '\n' << "test dei getters:" << '\n';

  std::cout << particle1.GetName() << '\n';
  std::cout << particle1.GetMass() << '\n';
  std::cout << particle1.GetCharge() << '\n';
  std::cout << particle2.GetWidth() << '\n';

  std::cout << '\n' << "test dei print:" << '\n';

  particle1.Print();
  particle2.Print();

  std::cout << '\n' << "test con un array a puntatori:" << '\n';

  ParticleType* test[2];
  test[0] = new ParticleType(&name, mass, charge);
  test[1] = new ResonanceType(&name, mass, charge, width);
  for (int i = 0; i < 2; ++i) {
    test[i]->Print();
  }

  char nome3 = 'e';
  double massa3 = 5.0;
  int charge3 = 2;
  double larghezza3 = 0.0;

  char nome4 = 'm';
  double massa4 = 4.0;
  int charge4 = -1;
  double larghezza4 = 0.2;

  char nome31 = 'p';
  Particle particle3(&nome31, 0, 0, 0);

  particle3.AddParticleType(&nome3, massa3, charge3,
                            larghezza3);  // Aggiunta particella normale
  particle3.AddParticleType(
      &nome4, massa4, charge4,
      larghezza4);  // Aggiunta particella con larghezza (risonanza)

  std::cout << "\nParticelle aggiunte:\n";
  particle3.PrintData();

  return 0;
}