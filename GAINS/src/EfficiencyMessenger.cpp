#include "EfficiencyMessenger.h"
#include "RunManager.h"
#include <sstream>
#include <fstream>
#include "G4SystemOfUnits.hh"
#include <G4UImanager.hh>

EfficiencyMessenger::EfficiencyMessenger(RunManager* runManager)
	: m_runManager(runManager)
{
	m_runSinglePeaksCmd = new G4UIcommand("/GAINS/runSinglePeaks", this);
	m_runSinglePeaksCmd->SetGuidance("Simulate runs where single photons are emitted.");
	m_runSinglePeaksCmd->SetGuidance("Argument 1 - input file containing peak energies.");
	m_runSinglePeaksCmd->SetGuidance("Argument 2 - output directory.");
	m_runSinglePeaksCmd->SetGuidance("Argument 3 - number of events per run.");
	G4UIparameter* p1 = new G4UIparameter("inputFile", 's', false);
	p1->SetDefaultValue("***NULL***");
	m_runSinglePeaksCmd->SetParameter(p1);
	G4UIparameter* p2 = new G4UIparameter("outputDirectory", 's', false);
	p2->SetDefaultValue("***NULL***");
	m_runSinglePeaksCmd->SetParameter(p2);
	G4UIparameter* p3 = new G4UIparameter("numberOfEvent", 'i', false);
	p3->SetParameterRange("numberOfEvent >= 0");
	m_runSinglePeaksCmd->SetParameter(p3);
}

EfficiencyMessenger::~EfficiencyMessenger()
{
	delete m_runSinglePeaksCmd;
}

void EfficiencyMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
	if (command == m_runSinglePeaksCmd) {
		//Read parameters
		std::stringstream ss(newValues);
		G4String inputFile;
		G4String outputDirectory;
		G4int numEvents;
		ss >> inputFile >> outputDirectory >> numEvents;

		std::ifstream in(inputFile);
		std::vector<double> energies;

		//Read energies
		G4double energy;
		while (in >> energy)
			energies.push_back(energy * keV);
		in.close();

		//For each energy, run simulation
		for (double energy : energies) {
			G4double energyInKeV = energy / keV;
			G4String outputFileName = outputDirectory
				+ "/GAINS_" + std::to_string(static_cast<G4int>(energyInKeV))
				+ "keV.root";

			//Set initial energy and output location
			G4UImanager* UImanager = G4UImanager::GetUIpointer();
			UImanager->ApplyCommand("/gps/energy " + std::to_string(energyInKeV) + " keV");
			UImanager->ApplyCommand("/analysis/setFileName " + outputFileName);

			m_runManager->BeamOn(numEvents);
		}
	}
}

G4String EfficiencyMessenger::GetCurrentValue(G4UIcommand* command)
{
	return G4String();
}
