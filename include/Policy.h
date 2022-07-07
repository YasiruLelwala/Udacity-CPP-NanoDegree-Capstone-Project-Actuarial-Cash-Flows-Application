#ifndef POLICY_H
#define POLICY_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <fstream>

#include "Valuation.h"
/* 
The purpose of this class is to instantiate 
a Policy object from the In-Force data,
and add further attributes such as the date of maturity,
duration in-force/outstanding. 
*/

class Policy
{
public:
    // constructor
    Policy
    (
        Valuation &valuation,

        // from In-Force data
        std::string productType,
        std::string policyNumber,
        std::string gender,
        std::string paymentMode,
        std::string dateOfCommencement,
        std::string dateOfBirth,
        std::string policyTerm,
        std::string sumAssured,
        std::string premium 
    );

    // destructor
    ~Policy() { std::cout << "Policy object with id = " << _id << " destructed" << std::endl; };

    // print methods
    void printPolicyAttributes();

    // getters
    // returns a reference to a const. Reference to const means that reference itself could not modify the referred object.
    int getID() { return _id; }
    Valuation &getValuation() { return _valuation; }; 
    const std::string &getProductType() { return _productType; };
    const std::string &getPolicyNumber() { return _policyNumber; };
    const date &getDateOfCommencement() { return _dateOfCommencement; };
    const date &getDateOfBirth() { return _dateOfBirth; };
    const std::string &getGender() { return _gender; };
    const int &getPolicyTerm() { return _policyTerm; };
    const double &getSumAssured() { return _sumAssured; };
    const std::string &getPaymentMode() { return _paymentMode; };
    const double &getPremium() { return _premium; };
    const date &getDateOfMaturity() { return _dateOfMaturity; };
    const int &getDurationInForce() { return _durationInForce; };
    const int &getDurationOutstanding() { return _durationOutstanding; }; 
    const std::vector<greg_month> &getPremiumDueMonths() { return _premiumDueMonths; };

private:
    int _id;
    static int idCnt;

    Valuation &_valuation; // used to get the date of valuation.

    // attributes from data
    std::string _productType;
    std::string _policyNumber;
    date _dateOfCommencement;
    date _dateOfBirth;
    std::string _gender;
    int _policyTerm;
    double _sumAssured;
    std::string _paymentMode;
    double _premium;

    // attributes to be set
    date _dateOfMaturity;
    int _durationInForce;
    int _durationOutstanding;
    std::vector<greg_month> _premiumDueMonths;

    // setters
    date setDateOfMaturity(date dateOfCommencement, int policyTerm);
    int setDurationInForce(date dateOfCommencement);
    int setDurationOutstanding(date dateOfCommencement, int policyTerm);
    std::vector<greg_month> setPremiumDueMonths(date dateOfCommencement, std::string paymentMode);
};

#endif