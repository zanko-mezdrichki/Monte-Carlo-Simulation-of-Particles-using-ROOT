#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include <iostream>

void analisi4() {
  // Apri il file ROOT contenente i grafici
  TFile *file = TFile::Open("grafici.root", "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Errore nell'aprire il file ROOT!" << std::endl;
    return;
  }

  // Recupera gli istogrammi dal file
  TH1F *particletype = (TH1F *)file->Get("ParticleType");
  TH1F *phi = (TH1F *)file->Get("Phi");
  TH1F *theta = (TH1F *)file->Get("Theta");
  TH1F *impulse = (TH1F *)file->Get("Pulse");
  TH1F *transverseimpulse = (TH1F *)file->Get("TransversePulse");
  TH1F *energy = (TH1F *)file->Get("Energy");
  TH1F *invariantmassall = (TH1F *)file->Get("InvariantMassAll");
  TH1F *invariantMassOppositeCharge = (TH1F *)file->Get("InvariantMassOppositeCharge");
  TH1F *invariantMassSameCharge = (TH1F *)file->Get("InvariantMassSameCharge");
  TH1F *invariantMassPionKaonOpposite = (TH1F *)file->Get("InvariantMassPionKaonOpposite");
  TH1F *invariantMassPionKaonSame = (TH1F *)file->Get("InvariantMassPionKaonSame");
  TH1F *invariantMassKTrue = (TH1F *)file->Get("invariantMassKTrue");

  // Fit di invariantMassKTrue e stampa valori
  TF1 *invariantmassKTrue_fit = new TF1("invariantmassKTrue_fit", "[0]*TMath::Gaus(x, [1], [2])", 0, 2);
  invariantmassKTrue_fit->SetParameter(1, 0.89166);
  invariantmassKTrue_fit->SetParameter(2, 0.050);
  invariantMassKTrue->Fit(invariantmassKTrue_fit, "Q");
  invariantMassKTrue->SetTitle("Invariant mass of the decays of K*; Invariant mass of the particle [GeV/c^2]; Occurrences");

  std::cout << "Media (massa): " << invariantmassKTrue_fit->GetParameter(1)
            << ", Sigma (larghezza): " << invariantmassKTrue_fit->GetParameter(2)
            << " ampiezza: " << invariantmassKTrue_fit->GetParameter(0) << std::endl;
  std::cout << "Chi2/NDF: " << invariantmassKTrue_fit->GetChisquare() / invariantmassKTrue_fit->GetNDF() << std::endl;
  std::cout << "Probabilità: " << invariantmassKTrue_fit->GetProb() << std::endl;

  for (int i = 0; i < 3; ++i) {
    std::cout << "parameter " << i << " -> " << invariantmassKTrue_fit->GetParameter(i) 
              << " +/- " << invariantmassKTrue_fit->GetParError(i) << std::endl;
  }

  // Creazione istogrammi differenza
  TH1F *invariantMassDiffAll = (TH1F *)invariantMassOppositeCharge->Clone("invariantMassOppositeCharge");
  invariantMassDiffAll->Add(invariantMassSameCharge, -1);

  TH1F *invariantMassDiffPionKaon = (TH1F *)invariantMassPionKaonOpposite->Clone("invariantMassDiffPionKaon");
  invariantMassDiffPionKaon->Add(invariantMassPionKaonSame, -1);

  // Fit di invariantmassAll e stampa valori
  TF1 *invariantmassall_fit = new TF1("invariantmassall_fit", "[0]*TMath::Gaus(x, [1], [2])", 0, 2);
  invariantmassall_fit->SetParameter(1, 0.89166);
  invariantmassall_fit->SetParameter(2, 0.050);
  invariantMassDiffAll->Fit(invariantmassall_fit, "Q");
  invariantMassDiffAll->SetTitle("Difference between inv mass of particles of same and opposite charge; Invariant mass of the particle [GeV/c^2]; Difference in occurrences");

  std::cout << "Fit gaussiano per K* (differenza di masse invariante tutte le particelle):" << std::endl;
  std::cout << "Media (massa): " << invariantmassall_fit->GetParameter(1)
            << ", Sigma (larghezza): " << invariantmassall_fit->GetParameter(2)
            << " ampiezza: " << invariantmassall_fit->GetParameter(0) << std::endl;
  std::cout << "Chi2/NDF: " << invariantmassall_fit->GetChisquare() / invariantmassall_fit->GetNDF() << std::endl;
  std::cout << "Probabilità: " << invariantmassall_fit->GetProb() << std::endl;

  for (int i = 0; i < 3; ++i) {
    std::cout << "parameter " << i << " -> " << invariantmassall_fit->GetParameter(i) 
              << " +/- " << invariantmassall_fit->GetParError(i) << std::endl;
  }

  // Fit di invariantmassPK e stampa valori
  TF1 *invariantmassPK_fit = new TF1("invariantmassPK_fit", "[0]*TMath::Gaus(x, [1], [2])", 0, 2);
  invariantmassPK_fit->SetParameter(1, 0.89166);
  invariantmassPK_fit->SetParameter(2, 0.050);
  invariantMassDiffPionKaon->Fit(invariantmassPK_fit, "Q");
  invariantMassDiffPionKaon->SetTitle("Difference between inv mass of Pions and Kaons of same and opposite charge; Invariant mass of the particle [GeV/c^2]; Difference in occurrences");

  std::cout << "Fit gaussiano per K* (differenza di masse invariante pion-kaon):" << std::endl;
  std::cout << "Media (massa): " << invariantmassPK_fit->GetParameter(1)
            << ", Sigma (larghezza): " << invariantmassPK_fit->GetParameter(2)
            << " ampiezza: " << invariantmassPK_fit->GetParameter(0) << std::endl;
  std::cout << "Chi2/NDF: " << invariantmassPK_fit->GetChisquare() / invariantmassPK_fit->GetNDF() << std::endl;
  std::cout << "Probabilità: " << invariantmassPK_fit->GetProb() << std::endl;

  for (int i = 0; i < 3; ++i) {
    std::cout << "parameter " << i << " -> " << invariantmassPK_fit->GetParameter(i) 
              << " +/- " << invariantmassPK_fit->GetParError(i) << std::endl;
  }

  // Salvataggio grafici massa invariante
  TCanvas *c1 = new TCanvas("c1", "Invariant Mass", 1200, 900);
  c1->Divide(2, 2);
  c1->cd(1);
  invariantMassDiffAll->Draw("SAME");
  c1->cd(2);
  invariantMassDiffPionKaon->Draw("SAME");
  c1->cd(3);
  invariantMassKTrue->Draw("SAME");
  c1->SaveAs("InvMassDifference_fit.pdf");

  // Gestione altri grafici
  // titoli per grafici che lo richiedono
  phi->SetTitle("Polar angular distribution; Angle phi [rad]; Occurrences");
  theta->SetTitle("Azimuth angular distribution; Theta angle [rad]; Occurrences");
  impulse->SetTitle("Pulse modulus; pulse modulus [GeV/c]; Occurrences");

  std::cout << "HISTOGRAMS' ENTRIES: (for each histogram)" << std::endl;
  std::cout << "entries for particletype: " << particletype->GetEntries() << std::endl;
  std::cout << "entries for phi: " << phi->GetEntries() << std::endl;
  std::cout << "entries for theta: " << theta->GetEntries() << std::endl;
  std::cout << "entries for impulse: " << impulse->GetEntries() << std::endl;
  std::cout << "entries for transverseimpulse: " << transverseimpulse->GetEntries() << std::endl;
  std::cout << "entries for energy: " << energy->GetEntries() << std::endl;
  std::cout << "entries for invariantmassall: " << invariantmassall->GetEntries() << std::endl;
  std::cout << "entries for invariantMassOppositeCharge: " << invariantMassOppositeCharge->GetEntries() << std::endl;
  std::cout << "entries for invariantMassSameCharge: " << invariantMassSameCharge->GetEntries() << std::endl;
  std::cout << "entries for invariantMassPionKaonOpposite: " << invariantMassPionKaonOpposite->GetEntries() << std::endl;
  std::cout << "entries for invariantMassPionKaonSame: " << invariantMassPionKaonSame->GetEntries() << std::endl;
  std::cout << "entries for invariantMassDecayk: " << invariantMassKTrue->GetEntries() << std::endl;

  // Print delle proporzioni delle particelle ed errori
  std::cout << "PARTICLES PROPORTIONS (number of particles of each type):" << std::endl;
  std::cout << "Positive Pions: " << (particletype->GetBinContent(1)) << " +/- " << particletype->GetBinError(1) << std::endl;
  std::cout << "Negative Pions: " << (particletype->GetBinContent(2)) << " +/- " << particletype->GetBinError(2) << std::endl;
  std::cout << "Positive Kaons: " << (particletype->GetBinContent(3)) << " +/- " << particletype->GetBinError(3) << std::endl;
  std::cout << "Negative Kaons: " << (particletype->GetBinContent(4)) << " +/- " << particletype->GetBinError(4) << std::endl;
  std::cout << "Positive Protons: " << (particletype->GetBinContent(5)) << " +/- " << particletype->GetBinError(5) << std::endl;
  std::cout << "Negative Protons: " << (particletype->GetBinContent(6)) << " +/- " << particletype->GetBinError(6) << std::endl;
  std::cout << "Resonance K*: " << (particletype->GetBinContent(7)) << " +/- " << particletype->GetBinError(7) << std::endl;

  // Fit di phi e stampa valori
  TF1 *phi_fit = new TF1("phi_fit", "[0]", 0, 2 * TMath::Pi());
  phi->Fit("phi_fit", "RQ0");
  std::cout << "PHI -- Uniform fit over phi histogram" << std::endl;
  std::cout << "Uniform fit parameter: " << phi_fit->GetParameter(0) << " +/- " << phi_fit->GetParError(0) << std::endl;
  std::cout << "ChiSquare: " << phi_fit->GetChisquare() << std::endl;
  std::cout << "ChiSquare/NDF: " << phi_fit->GetChisquare() / phi_fit->GetNDF() << std::endl;
  std::cout << "NDF: " << phi_fit->GetNDF() << std::endl;
  std::cout << "Fit probability: " << phi_fit->GetProb() * 100 << "%" << std::endl;

  // Fit di theta e stampa valori
  TF1 *theta_fit = new TF1("theta_fit", "[0]", 0, TMath::Pi());
  theta->Fit("theta_fit", "RQ0");
  std::cout << "THETA -- Uniform fit over theta histogram" << std::endl;
  std::cout << "Uniform fit parameter: " << theta_fit->GetParameter(0) << " +/- " << theta_fit->GetParError(0) << std::endl;
  std::cout << "ChiSquare: " << theta_fit->GetChisquare() << std::endl;
  std::cout << "NDF: " << theta_fit->GetNDF() << std::endl;
  std::cout << "ChiSquare/NDF: " << theta_fit->GetChisquare() / theta_fit->GetNDF() << std::endl;
  std::cout << "Fit probability: " << theta_fit->GetProb() * 100 << "%" << std::endl;

  // Fit di impulse e stampa valori
  TF1 *impulse_fit = new TF1("impulse_fit", "[0]*exp(-[1]*x)", 0.1, 10);
  impulse->Fit("impulse_fit", "RQ0");
  std::cout << "IMPULSE VALUE -- Exponential fit over impulse value histogram " << std::endl;
  std::cout << "Exponential fit parameter: " << impulse_fit->GetParameter(1) << " +/- " << impulse_fit->GetParError(1) << std::endl;
  std::cout << "ChiSquare: " << impulse_fit->GetChisquare() << std::endl;
  std::cout << "NDF: " << impulse_fit->GetNDF() << std::endl;
  std::cout << "ChiSquare/NDF: " << impulse_fit->GetChisquare() / impulse_fit->GetNDF() << std::endl;
  std::cout << "Fit probability: " << impulse_fit->GetProb() * 100 << "%" << std::endl;

  // Canvas delle distribuzioni angolari e modulo dell'impulso
  TCanvas *c2 = new TCanvas("c2", "Fit Histograms", 800, 600);
  c2->Divide(2, 2);
  c2->cd(1);
  particletype->Draw();
  c2->cd(2);
  phi->Draw();
  phi_fit->Draw("SAME");
  c2->cd(3);
  theta->Draw();
  theta_fit->Draw("SAME");
  c2->cd(4);
  impulse->Draw();
  impulse_fit->Draw("SAME");
  c2->SaveAs("type_angles_impulse_fit.pdf");

  // Canvas dei grafici rimanenti
  TCanvas *c3 = new TCanvas("c3", "Other graphs", 800, 600);
  c3->Divide(2);
  c3->cd(1);
  transverseimpulse->Draw();
  c3->cd(2);
  energy->Draw();
  c3->SaveAs("other.pdf");

  // Canvas massa invariante
  TCanvas *c4 = new TCanvas("c4", "Invariant mass", 800, 600);
  c4->Divide(2, 3);
  c4->cd(1);
  invariantmassall->Draw();
  c4->cd(2);
  invariantMassOppositeCharge->Draw();
  c4->cd(3);
  invariantMassSameCharge->Draw();
  c4->cd(4);
  invariantMassPionKaonOpposite->Draw();
  c4->cd(5);
  invariantMassPionKaonSame->Draw();
  c4->SaveAs("invMass.pdf");

  // Chiudi il file ROOT
  file->Close();
}