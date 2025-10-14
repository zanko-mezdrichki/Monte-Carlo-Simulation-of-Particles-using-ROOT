#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include "particle.hpp"
#include <iostream>

void mainmodule() {
  Particle::AddParticleType("P+", 0.13957, 1, 0);    // 0
  Particle::AddParticleType("P-", 0.13957, -1, 0);   // 1
  Particle::AddParticleType("K+", 0.49367, 1, 0);    // 2
  Particle::AddParticleType("K-", 0.49367, -1, 0);   // 3
  Particle::AddParticleType("Pr+", 0.93827, 1, 0);   // 4
  Particle::AddParticleType("Pr-", 0.93827, -1, 0);  // 5
  Particle::AddParticleType("K*", 0.89166, 0, 0.050); // 6

  gRandom->SetSeed();

  const int events = 100000;
  Int_t N = 120;
  Double_t mean = 1.;
  Particle EventParticles[N];

  TH1F *particletype = new TH1F("ParticleType", "Particle Type", 7, 0, 7);
  TH1F *phi = new TH1F("Phi", "Phi", 1000, 0, 2 * TMath::Pi());
  TH1F *theta = new TH1F("Theta", "Theta", 1000, 0, TMath::Pi());
  TH1F *impulse = new TH1F("Pulse", "Pulse", 1000, 0, 10);
  TH1F *transverseimpulse = new TH1F("TransversePulse", "Transverse Pulse", 1000, 0, 10);
  TH1F *energy = new TH1F("Energy", "Energy", 1000, 0, 10);
  TH1F *invariantmassall = new TH1F("InvariantMassAll", "Invariant Mass of All Particle Pairs", 1000, 0, 10);
  TH1F *invariantMassOppositeCharge = new TH1F("InvariantMassOppositeCharge", "Invariant Mass Opposite Charge", 1000, 0, 5);
  TH1F *invariantMassSameCharge = new TH1F("InvariantMassSameCharge", "Invariant Mass Same Charge", 1000, 0, 5);
  TH1F *invariantMassPionKaonOpposite = new TH1F("InvariantMassPionKaonOpposite", "Invariant Mass Pion+/Kaon- or Pion-/Kaon+", 1000, 0, 5);
  TH1F *invariantMassPionKaonSame = new TH1F("InvariantMassPionKaonSame", "Invariant Mass Pion+/Kaon+ or Pion-/Kaon-", 1000, 0, 5);
  TH1F *invariantMassKTrue = new TH1F("invariantMassKTrue", "Invariant Mass Kaon True", 1000, 0, 2);

  particletype->Sumw2();
  phi->Sumw2();
  theta->Sumw2();
  impulse->Sumw2();
  transverseimpulse->Sumw2();
  energy->Sumw2();
  invariantmassall->Sumw2();
  invariantMassOppositeCharge->Sumw2();
  invariantMassSameCharge->Sumw2();
  invariantMassPionKaonOpposite->Sumw2();
  invariantMassPionKaonSame->Sumw2();

  for (Int_t i = 0; i < events; i++) {
    int max_part = 100;
    for (Int_t j = 0; j < 100; j++) {
      double seed = gRandom->Uniform(0., 1.);
      double Phi = gRandom->Uniform(0., 2 * TMath::Pi());
      double Theta = gRandom->Uniform(0., TMath::Pi());
      double pulse = gRandom->Exp(mean);
      double px = pulse * sin(Theta) * cos(Phi);
      double py = pulse * sin(Theta) * sin(Phi);
      double pz = pulse * cos(Theta);

      EventParticles[j].SetP(px, py, pz);

      if (seed <= 0.4) {
        EventParticles[j].SetIndex("P+");
        particletype->Fill(0);
      } else if (seed > 0.4 && seed <= 0.8) {
        EventParticles[j].SetIndex("P-");
        particletype->Fill(1);
      } else if (seed > 0.8 && seed <= 0.85) {
        EventParticles[j].SetIndex("K+");
        particletype->Fill(2);
      } else if (seed > 0.85 && seed <= 0.90) {
        EventParticles[j].SetIndex("K-");
        particletype->Fill(3);
      } else if (seed > 0.90 && seed <= 0.945) {
        EventParticles[j].SetIndex("Pr+");
        particletype->Fill(4);
      } else if (seed > 0.945 && seed <= 0.99) {
        EventParticles[j].SetIndex("Pr-");
        particletype->Fill(5);
      } else {
        EventParticles[j].SetIndex("K*");
        particletype->Fill(6);
      }

      if (EventParticles[j].GetIndex() == 6 && max_part + 2 <= N) {
        double x = gRandom->Rndm();
        if (x < 0.5) {
          EventParticles[max_part].SetIndex(0);      // Pione +
          EventParticles[max_part + 1].SetIndex(3);  // Kaone -
        } else {
          EventParticles[max_part].SetIndex(1);      // Pione -
          EventParticles[max_part + 1].SetIndex(2);  // Kaone +
        }
        EventParticles[j].Decay2body(EventParticles[max_part], EventParticles[max_part + 1]);
        max_part += 2;
        double invMass = EventParticles[max_part - 2].GetInvMass(EventParticles[max_part - 1]);
        invariantMassKTrue->Fill(invMass);
      }

      impulse->Fill(pulse);
      phi->Fill(Phi);
      theta->Fill(Theta);
      double Energy = EventParticles[j].GetEnergy();
      energy->Fill(Energy);
      double tp = sqrt(pow(px, 2) + pow(py, 2));
      transverseimpulse->Fill(tp);
    }

    for (int k = 0; k < max_part; k++) {
      for (Int_t n = k + 1; n < max_part; n++) {
        double invMass = EventParticles[k].GetInvMass(EventParticles[n]);
        invariantmassall->Fill(invMass);

        if (EventParticles[k].GetCharge() * EventParticles[n].GetCharge() < 0) {
          invariantMassOppositeCharge->Fill(invMass);
        } else if (EventParticles[k].GetCharge() * EventParticles[n].GetCharge() > 0) {
          invariantMassSameCharge->Fill(invMass);
        }

        int idx1 = EventParticles[k].GetIndex();
        int idx2 = EventParticles[n].GetIndex();
        if ((idx1 == 0 && idx2 == 3) || (idx1 == 1 && idx2 == 2)) {  // Pion+/Kaon- or Pion-/Kaon+
          invariantMassPionKaonOpposite->Fill(invMass);
        } else if ((idx1 == 0 && idx2 == 2) || (idx1 == 1 && idx2 == 3)) {  // Pion+/Kaon+ or Pion-/Kaon-
          invariantMassPionKaonSame->Fill(invMass);
        }
      }
    }
  }

  TFile *file = new TFile("grafici.root", "RECREATE");
  particletype->Write();
  phi->Write();
  theta->Write();
  impulse->Write();
  transverseimpulse->Write();
  energy->Write();
  invariantmassall->Write();
  invariantMassOppositeCharge->Write();
  invariantMassSameCharge->Write();
  invariantMassPionKaonOpposite->Write();
  invariantMassPionKaonSame->Write();
  invariantMassKTrue->Write();
  file->Close();

  TCanvas *c1 = new TCanvas("c1", "Particle Type");
  particletype->Draw();
  TCanvas *c2 = new TCanvas("c2", "Pulse");
  impulse->Draw();
  TCanvas *c4 = new TCanvas("c4", "Energy");
  energy->Draw();
  TCanvas *c5 = new TCanvas("c5", "Trasverse impulse");
  transverseimpulse->Draw();
  TCanvas *c6 = new TCanvas("c6", "Phi");
  phi->Draw();
  TCanvas *c7 = new TCanvas("c7", "Theta");
  theta->Draw();
  TCanvas *c8 = new TCanvas("c8", "InvMass All");
  invariantmassall->Draw();
  TCanvas *c9 = new TCanvas("c9", "InvMass Opposite");
  invariantMassOppositeCharge->Draw();
  TCanvas *c10 = new TCanvas("c10", "InvMass Same");
  invariantMassSameCharge->Draw();
  TCanvas *c11 = new TCanvas("c11", "InvMass PionKaonOpposite");
  invariantMassPionKaonOpposite->Draw();
  TCanvas *c12 = new TCanvas("c12", "InvMass PionKaonSame");
  invariantMassPionKaonSame->Draw();
  TCanvas *c13 = new TCanvas("c13", "InvMass KaonTrue");
  invariantMassKTrue->Draw();
}