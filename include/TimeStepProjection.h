#ifndef TIME_STEP_PROJECTION_H
#define TIME_STEP_PROJECTION_H

#include "Policy.h"
/* 
The purpose of this class is to project, and store :
1. timeStep - vector of integers from 1 to the durationOutstanding.
2. date - vector of monthly dates starting from the dateOfValuation and ending with dateOfValuation + months = last element of timeStep
3. premiumIndicator - vector of ones or zeros depending on whether a premium payment is expected on that date.
4. policyYear - vector of integers; at each date, we calculate the the time from dateOfCommencement to that date (i.e. the policyYear)
5. valuationYear - similar to policyYear, except we calculate the length of time from dateOfValuation.
6. ageNearest - vector of integers; at each date, we calculate the age using the dateOfBirth.
*/
class TimeStepProjection
{
public:
    // constructor
    TimeStepProjection(Policy &policy);

    // destructor
    ~TimeStepProjection() { std::cout << "TimeStepProjection object with id = " << this->getPolicy().getID() << " destructed" << std::endl; };

    // methods
    void run();

    // print methods
    void printTimeStep();

    // getters
    Policy &getPolicy() { return _policy; };
    
    const std::vector<int> &getTimeStep() { return _timeStep; };
    const std::vector<date> &getDate() { return _dateBOM; };
    const std::vector<bool> &getPremiumIndicator() { return _premiumIndicator; };
    const std::vector<int> &getPolicyYear() { return _policyYear; };
    const std::vector<int> &getValuationYear() { return _valuationYear; };
    const std::vector<int> &getAgeNearest() { return _ageNearest; };

private:
    // members
    Policy &_policy;
    
    std::vector<int> _timeStep;
    std::vector<date> _dateBOM;
    std::vector<bool> _premiumIndicator;
    std::vector<int> _policyYear;
    std::vector<int> _valuationYear;
    std::vector<int> _ageNearest;

protected:
    // methods
    std::vector<int> projectTimeStep(); // simply a sequence of integers from 1 to duration outstanding (Policy::getDurationOutstanding())
    std::vector<date> projectDate(); 
    std::vector<bool> projectPremiumIndicator();
    std::vector<int> projectPolicyYear();
    std::vector<int> projectValuationYear();
    std::vector<int> projectAgeNearest();    
};




#endif
