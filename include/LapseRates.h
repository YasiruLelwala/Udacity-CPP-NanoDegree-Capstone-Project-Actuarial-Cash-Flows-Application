#ifndef LAPSE_RATES_H
#define LAPSE_RATES_H

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
The purpose of this class is to encapsulate the base lapse rate table (yearly),
hold the applicable loading on the base rates (which differes based on whether base or stress runType from Valuation Class), 
and to hold the independent monthly loading-applied lapse rates.
*/

class LapseRates
{
public:
    // constructor
    LapseRates(Valuation &valuation);

    // getters
    const std::unordered_map<int, std::vector<double>> &getBaseLapseRatesHashTableYearly() { return _baseLapseRatesYearlyHashTable; };
    const std::unordered_map<int, std::vector<double>> &getLoadedLapseRatesHashTableMonthly() { return _loadedLapseRatesMonthlyHashTable; };


private:
    Valuation &_valuation;
    
    std::unordered_map<int, std::vector<double>> _baseLapseRatesYearlyHashTable;
    float _applicableLoadingOnBaseLapseRatesYearly;
    std::unordered_map<int, std::vector<double>> _loadedLapseRatesMonthlyHashTable;
    
    // setters
    std::unordered_map<int, std::vector<double>> createBaseLapseRatesHashTableYearly(); // used in the initializer list. Needs to be overrident in subsequent classes.
    std::unordered_map<int, std::vector<double>> createLoadedLapseRatesHashTableMonthly(); // used in the constructor body
};


#endif