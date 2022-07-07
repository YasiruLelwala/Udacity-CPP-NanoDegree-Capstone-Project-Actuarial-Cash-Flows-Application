#include "MortalityRates.h"

// constructor
MortalityRates::MortalityRates(Valuation &valuation) : 
_valuation(valuation),
_baseMortalityRatesYearlyHashTable(createBaseMortalityRatesHashTableYearly()),
_applicableLoadingOnBaseMortalityRatesYearly(0.95), 
_loadedMortalityRatesMonthlyHashTable(createLoadedMortalityRatesHashTableMonthly()) 
{
    std::cout << "MortalityRates object constructed" << std::endl;
};

// setters
std::unordered_map<int, std::vector<double>> MortalityRates::createBaseMortalityRatesHashTableYearly() 
{
    std::unordered_map<int, std::vector<double>> mortalityRatesHashTableYearly; 

    int age;
    double maleRates;
    double femaleRates;

    std::string line;

    std::ifstream mortalityRates("../data/Mortality_Rates.csv");

    if (mortalityRates.is_open()) 
    {
        while (getline (mortalityRates, line ) ) 
        {
            std::replace (line.begin(), line.end(), ',', ' ');

            std::istringstream linestream(line);

            while (linestream >> age >> maleRates >> femaleRates) 
            {
                mortalityRatesHashTableYearly[age] = {maleRates,femaleRates};

                //std::cout << age << "     " << maleRates << "    " << femaleRates << std::endl;
                //std::cout << mortalityRatesHashTableYearly[age][maleRates] << " " << mortalityRatesHashTableYearly[age][femaleRates] << std::endl;
            }
        }
    }
    return mortalityRatesHashTableYearly;
};

std::unordered_map<int, std::vector<double>> MortalityRates::createLoadedMortalityRatesHashTableMonthly()
{
    std::unordered_map<int, std::vector<double>> mortalityRatesHashTableMonthly = this->_baseMortalityRatesYearlyHashTable;

    float runTypeLoading;

    if (this->_valuation.getRunType() == RunType::Base)
    {
        runTypeLoading = 1.0;
    } 
    else
    {
        runTypeLoading = 1.1;
    }



    for (auto &rates : mortalityRatesHashTableMonthly)
    {
        rates.second[0] = 1 - std::pow( 1 - rates.second[0] * this->_applicableLoadingOnBaseMortalityRatesYearly * runTypeLoading, 1.0/12.0);
        rates.second[1] = 1 - std::pow( 1 - rates.second[1] * this->_applicableLoadingOnBaseMortalityRatesYearly * runTypeLoading, 1.0/12.0);
        
        //std::cout << rates.first << "    " << rates.second[0] << "     " << rates.second[1] << std::endl;
    }

    return mortalityRatesHashTableMonthly;    
};