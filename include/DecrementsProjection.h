#ifndef DECREMENTS_PROJECTION_H
#define DECREMENTS_PROJECTION_H

#include "TimeStepProjection.h"
#include "MortalityRates.h"
#include "LapseRates.h"

/*
The purpose of this class is to project, and store the 
dependent death and lapse probabilities. 
*/


class DecrementsProjection
{
public:
    // constructor
    DecrementsProjection(TimeStepProjection &timeStepProjection, MortalityRates &mortalityRates, LapseRates &lapseRates);

    // destructor
    ~DecrementsProjection() { std::cout << "DecrementsProjection object with id = " << this->getTimeStepProjection().getPolicy().getID() << " destructed" << std::endl; }
    // methods
    void run();

    // print methods
    void printDecrements();
    
    // getters
    TimeStepProjection &getTimeStepProjection() { return _timeStepProjection; };
    MortalityRates &getMortalityRates() { return _mortalityRates; };
    LapseRates &getLapseRates() { return _lapseRates; };
    const std::vector<double> &getIndependentLoadedMortalityRatesMonthly() { return _independentLoadedMortalityRatesMonthly; };    
    const std::vector<double> &getIndependentLoadedLapseRatesMonthly() { return _independentLoadedLapseRatesMonthly; };
    const std::vector<double> &getSurvivingPopulationBOM() { return _decrementsContainer[0]; };
    const std::vector<double> &getDependentDeathOutgo() { return _decrementsContainer[1]; };
    const std::vector<double> &getDependentLapseOutgo() {return _decrementsContainer[2]; };
    const std::vector<double> &getSurvivingPopulationEOM() { return _decrementsContainer[3]; };

private:
    // members
    TimeStepProjection &_timeStepProjection;
    MortalityRates &_mortalityRates;
    LapseRates &_lapseRates;

    std::vector<double> _independentLoadedMortalityRatesMonthly;
    std::vector<double> _independentLoadedLapseRatesMonthly;

    std::vector<std::vector<double>> _decrementsContainer;

protected:
    // methods
    std::vector<double> projectIndependentLoadedMortalityRatesMonthly();
    std::vector<double> projectIndependentLoadedLapseRatesMonthly();

    std::vector<std::vector<double>> projectDecrements(); // can be overriden in subsequent classes. 
};


#endif