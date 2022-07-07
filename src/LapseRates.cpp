#include "LapseRates.h"

// constructor
LapseRates::LapseRates(Valuation &valuation) : 
_valuation(valuation),
_baseLapseRatesYearlyHashTable(createBaseLapseRatesHashTableYearly()), 
_applicableLoadingOnBaseLapseRatesYearly(0.95),
_loadedLapseRatesMonthlyHashTable(createLoadedLapseRatesHashTableMonthly()) 
{
    std::cout << "LapseRates object constructed" << std::endl;
}

// setters
std::unordered_map<int, std::vector<double>> LapseRates::createBaseLapseRatesHashTableYearly() 
{
    std::unordered_map<int, std::vector<double>> LapseRatesHashTableYearly; 

    int age;
    double maleRates;
    double femaleRates;

    std::string line;

    std::ifstream LapseRates("../data/Lapse_Rates.csv");

    if (LapseRates.is_open()) 
    {
        while (getline (LapseRates, line ) ) 
        {
            std::replace (line.begin(), line.end(), ',', ' ');

            std::istringstream linestream(line);

            while (linestream >> age >> maleRates >> femaleRates) 
            {
                LapseRatesHashTableYearly[age] = {maleRates,femaleRates};

                //std::cout << age << "     " << maleRates << "    " << femaleRates << std::endl;
               // std::cout << LapseRatesHashTableYearly[age][maleRates] << " " << LapseRatesHashTableYearly[age][femaleRates] << std::endl;
            }
        }
    }
    return LapseRatesHashTableYearly;
};

std::unordered_map<int, std::vector<double>> LapseRates::createLoadedLapseRatesHashTableMonthly()
{
    std::unordered_map<int, std::vector<double>> LapseRatesHashTableMonthly = this->_baseLapseRatesYearlyHashTable;

    float runTypeLoading;

    if (this->_valuation.getRunType() == RunType::Base)
    {
        runTypeLoading = 1.0;
    } 
    else
    {
        runTypeLoading = 1.1;
    }

    for (auto &rates : LapseRatesHashTableMonthly)
    {
        rates.second[0] = 1 - std::pow( 1 - rates.second[0] * this->_applicableLoadingOnBaseLapseRatesYearly * runTypeLoading, 1.0/12.0);
        rates.second[1] = 1 - std::pow( 1 - rates.second[1] * this->_applicableLoadingOnBaseLapseRatesYearly * runTypeLoading, 1.0/12.0);
        
        //std::cout << rates.first << "    " << rates.second[0] << "     " << rates.second[1] << std::endl;
    }

    return LapseRatesHashTableMonthly;    
};