#include "DecrementsProjection.h"

// constructor
DecrementsProjection::DecrementsProjection(TimeStepProjection &timeStepProjection, MortalityRates &mortalityRates, LapseRates &lapseRates) :_timeStepProjection(timeStepProjection), _mortalityRates(mortalityRates), _lapseRates(lapseRates) 
{
    std::cout << "DecrementsProjection object with id = " << this->getTimeStepProjection().getPolicy().getID() << " destructed" << std::endl;
};


// methods
void DecrementsProjection::run()
{
    this->_independentLoadedMortalityRatesMonthly = projectIndependentLoadedMortalityRatesMonthly();
    this->_independentLoadedLapseRatesMonthly = projectIndependentLoadedLapseRatesMonthly();
    this->_decrementsContainer = projectDecrements();
}

// print methods
void DecrementsProjection::printDecrements()
{
    std::ofstream myFile("../results/decrementsProjection.csv");

    myFile 
    << "TIME_STEP" << "," 
    << "DATE_BOM" << ","
    << "AGE_NEAREST" << "," 
    << "VALUATION_YEAR" << ","
    << "POLICY_YEAR" << "," 
    << "INDEPENDENT_DEATH_RATES" << "," 
    << "INDEPENDENT_LAPSE_RATES" << ","
    << "SURVIVING_POPULATION_BOM" << ","  
    << "DEPENDENT_DEATH_OUTGO" << "," 
    << "DEPENDENT_LAPSE_OUTGO" << "," 
    << "SURVIVING_POPULATION_EOM" << std::endl;

    for (int i = 0; i < this->getTimeStepProjection().getTimeStep().size(); i++)
    {
        myFile 
        << this->getTimeStepProjection().getTimeStep()[i] << "," 
        << this->getTimeStepProjection().getDate()[i] << ","
        << this->getTimeStepProjection().getAgeNearest()[i] << "," 
        << this->getTimeStepProjection().getValuationYear()[i] << "," 
        << this->getTimeStepProjection().getPolicyYear()[i] << "," 
        << this->getIndependentLoadedMortalityRatesMonthly()[i] << "," 
        << this->getIndependentLoadedLapseRatesMonthly()[i] << "," 
        << this->getSurvivingPopulationBOM()[i] << "," 
        << this->getDependentDeathOutgo()[i] << "," 
        << this->getDependentLapseOutgo()[i] << "," 
        << this->getSurvivingPopulationEOM()[i] << std::endl;      
    }
    
    myFile.close();
}


// methods
std::vector<double> DecrementsProjection::projectIndependentLoadedMortalityRatesMonthly()
{
    const std::unordered_map<int, std::vector<double>> &loadedMortalityRatesMonthlyHashTable = this->_mortalityRates.getLoadedMortalityRatesHashTableMonthly();

    const std::vector<int> &ageNearest = this->_timeStepProjection.getAgeNearest();

    std::vector<double> independentLoadedMortalityRatesMonthly(ageNearest.size());

    double independentLoadedMortalityRate;

    // determining gender in order to pick the correct column from the hash table.
    int index;
    if (this->getTimeStepProjection().getPolicy().getGender() == "M")
    {
        index = 0;
    }
    else
    {
        index = 1;
    }

    for (int i = 0; i < ageNearest.size(); i++)
    {
        independentLoadedMortalityRate = loadedMortalityRatesMonthlyHashTable.at(ageNearest[i])[index];
        independentLoadedMortalityRatesMonthly[i] = independentLoadedMortalityRate;
    }

    return independentLoadedMortalityRatesMonthly;
}

std::vector<double> DecrementsProjection::projectIndependentLoadedLapseRatesMonthly()
{
    const std::unordered_map<int, std::vector<double>> &loadedLapseRatesMonthlyHashTable = this->_lapseRates.getLoadedLapseRatesHashTableMonthly();

    const std::vector<int> &policyYear = this->_timeStepProjection.getPolicyYear();

    std::vector<double> independentLoadedLapseRatesMonthly(policyYear.size());

    double independentLoadedLapseRate;

    // determining gender in order to pick the correct column from the hash table.
    int index;
    if (this->getTimeStepProjection().getPolicy().getGender() == "M")
    {
        index = 0;
    }
    else
    {
        index = 1;
    }

    for (int i = 0; i < policyYear.size(); i++)
    {
        independentLoadedLapseRate = loadedLapseRatesMonthlyHashTable.at(policyYear[i])[index];
        independentLoadedLapseRatesMonthly[i] = independentLoadedLapseRate;
    }
    
    return independentLoadedLapseRatesMonthly;
}

std::vector<std::vector<double>> DecrementsProjection::projectDecrements()
{
    // independent loaded monthly rates
    const std::vector<double> &independentLoadedMortalityRatesMonthly = this->_independentLoadedMortalityRatesMonthly;
    const std::vector<double> &independentLoadedLapseRatesMonthly = this->_independentLoadedLapseRatesMonthly;

    // initializing empty vectors of specified size
    std::vector<double> survivingPopulationBOM(independentLoadedMortalityRatesMonthly.size());
    std::vector<double> dependentDeathOutgo(independentLoadedMortalityRatesMonthly.size());
    std::vector<double> dependentLapseOutgo(independentLoadedMortalityRatesMonthly.size());
    std::vector<double> dependentSurrenderOutgo(independentLoadedMortalityRatesMonthly.size());
    std::vector<double> survivingPopulationEOM(independentLoadedMortalityRatesMonthly.size());

    // first Row of Decrements Projection (Dependent Rates)
    survivingPopulationBOM[0] = 1.0;
    dependentDeathOutgo[0] = survivingPopulationBOM[0] * independentLoadedMortalityRatesMonthly[0];
    dependentLapseOutgo[0] = ( survivingPopulationBOM[0] - dependentDeathOutgo[0] ) * independentLoadedLapseRatesMonthly[0];
    survivingPopulationEOM[0] = survivingPopulationBOM[0] - dependentDeathOutgo[0] - dependentLapseOutgo[0];

    // remaining rows
    for (int i = 1; i < independentLoadedMortalityRatesMonthly.size(); i++)
    {
        survivingPopulationBOM[i] = survivingPopulationEOM[i-1];
        dependentDeathOutgo[i] = survivingPopulationBOM[i] * independentLoadedMortalityRatesMonthly[i];
        dependentLapseOutgo[i] = ( survivingPopulationBOM[i] - dependentDeathOutgo[i] ) * independentLoadedLapseRatesMonthly[i];
        survivingPopulationEOM[i] = survivingPopulationBOM[i] - dependentDeathOutgo[i] - dependentLapseOutgo[i];

    }

    std::vector<std::vector<double>> decrements {survivingPopulationBOM, dependentDeathOutgo, 
    dependentLapseOutgo, survivingPopulationEOM};

    return decrements;   
}