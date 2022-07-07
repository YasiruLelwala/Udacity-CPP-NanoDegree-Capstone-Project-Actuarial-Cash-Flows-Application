#include "CashFlowsProjectionByValuationYear.h"

// constructor
CashFlowsProjectionByValuationYear::CashFlowsProjectionByValuationYear(CashFlowsProjection &CashFlowsProjection) : _cashFlowsProjection(CashFlowsProjection) 
{
    std::cout << "CashFlowsProjectionByValuationYear object with id = " << this->getCashFlowsProjection().getDecrementsProjection().getTimeStepProjection().getPolicy().getID() << " constructed" << std::endl;
}

// methods
void CashFlowsProjectionByValuationYear::run()
{
    this->_cashFlowsByValuationYear = createCashFlowsByValuationYear();
}

// print methods
// void GroupedCashFlowsProjection::printCashFlowsMultiMap()
// {
//     std::ofstream myFile("../results/cashFlowsMultiMap.csv");

//     myFile 
//     << "VALUATION_YEAR" << "," 
//     << "EXPECTED_PREMIUM" << ","
//     << "DEATH_OUTGO" << std::endl;
    
//     const std::multimap<int,std::vector<double>> &cashFlowsMonthly = this->getCashFlowsMonthly();

    
//     for (auto i = cashFlowsMonthly.begin(); i != cashFlowsMonthly.end(); i++)
//     {
//         myFile << i->first << "," 
//         << i->second[0] <<  "," 
//         << i->second[1] << ","
//         << i->second[2] << std::endl;

//     }

//     myFile.close();
// }

void CashFlowsProjectionByValuationYear::printCashFlowsByValuationYear()
{
    std::ofstream myFile("../results/cashFlowsByValuationYear.csv");

    myFile 
    << "VALUATION_YEAR" << "," 
    << "EXPECTED_PREMIUM" << ","
    << "DEATH_OUTGO" << std::endl;
    
    const std::map<int,std::vector<double>> &cashFlowsByValuationYear = this->getCashFlowsByValuationYear();

    for (auto i = cashFlowsByValuationYear.begin(); i != cashFlowsByValuationYear.end(); i++)
    {
        myFile << i->first << "," 
        << i->second[0] <<  "," 
        << i->second[1] << std::endl;
        

    }
    myFile.close();
}


// methods
std::multimap<int,std::vector<double>> CashFlowsProjectionByValuationYear::createMultiMapFromCashFlowsProjectionObject()
{
    const std::vector<int> &valuationYear = this->getCashFlowsProjection().getDecrementsProjection().getTimeStepProjection().getValuationYear();
    
    // get all the cash flows vectors from the cashflows object.
    const std::vector<double> &expectedPremium = this->getCashFlowsProjection().getExpectedPremium();
    const std::vector<double> &expectedDeathOutgo = this->getCashFlowsProjection().getExpectedDeathOutgo();

    std::multimap<int,std::vector<double>> cashFlowsMonthly;

    for (int i = 0; i < expectedPremium.size(); i ++)
    {
        cashFlowsMonthly.insert
        (
            std::pair<int,std::vector<double>>
            (
                valuationYear[i], 
                {
                    expectedPremium[i],
                    expectedDeathOutgo[i],
                    
                }
            )
        );
    }
    return cashFlowsMonthly;
}

std::map<int,std::vector<double>> CashFlowsProjectionByValuationYear::createCashFlowsByValuationYear()
{
    std::multimap<int,std::vector<double>> cashFlowsMonthly = CashFlowsProjectionByValuationYear::createMultiMapFromCashFlowsProjectionObject();

    std::map<int,std::vector<double>> cashFlowsByValuationYear;

    auto your_sum = [](const std::vector<double> &cont1, const std::vector<double> &cont2)
    {
        std::vector<double> result;
        //const auto smaller = std::min(cont1.size(), cont2.size());
        for ( size_t i = 0 ; i < cont1.size(); i++)
        {
              result.push_back(cont1[i]+cont2[i]);  
        }
        return result;
    };

    std::for_each(cashFlowsMonthly.begin(), cashFlowsMonthly.end(), [&cashFlowsByValuationYear,&your_sum] (auto const & i) 
        {
            //  map[i.first] += i.second; // caution  map[i.first] create an empty key_type 
            if ( cashFlowsByValuationYear.count(i.first) > 0)
            {
                cashFlowsByValuationYear.at(i.first) = your_sum(i.second, cashFlowsByValuationYear.at(i.first));
            }
            else
            {
                cashFlowsByValuationYear.insert({i.first, i.second});
            }
        }
    );

    return cashFlowsByValuationYear;
}