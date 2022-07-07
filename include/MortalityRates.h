#ifndef MORTALITY_H
#define MORTALITY_H

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "Valuation.h"
/* 
The purpose of this class is to encapsulate the base mortality rate table (yearly),
hold the applicable loading on the base rates (which differes based on whether base or stress runType from Valuation Class), 
and to hold the independent monthly loading-applied mortality rates.
*/

class MortalityRates
{
public:
    // constructor
    MortalityRates(Valuation &valuation);

    // destructor
    ~MortalityRates() { std::cout << "MortalityRates object destructed" << std::endl; };

    // getters
    const std::unordered_map<int, std::vector<double>> &getBaseMortalityRatesHashTableYearly() { return _baseMortalityRatesYearlyHashTable; };
    const std::unordered_map<int, std::vector<double>> &getLoadedMortalityRatesHashTableMonthly() { return _loadedMortalityRatesMonthlyHashTable; };


private: 
    Valuation &_valuation;
    
    std::unordered_map<int, std::vector<double>> _baseMortalityRatesYearlyHashTable;
    float _applicableLoadingOnBaseMortalityRatesYearly;
    std::unordered_map<int, std::vector<double>> _loadedMortalityRatesMonthlyHashTable;
    
    // setters
    std::unordered_map<int, std::vector<double>> createBaseMortalityRatesHashTableYearly(); // used in the initializer list. Needs to be overrident in subsequent classes.
    std::unordered_map<int, std::vector<double>> createLoadedMortalityRatesHashTableMonthly(); // used in the constructor body. Depends on the run type (Valuation.getRunType())
};



#endif