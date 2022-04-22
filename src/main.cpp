#include "PortfolioCashFlows.h"

#include <memory>
#include <future>

std::vector<std::vector<std::string>> readPolicyData () {

    std::string line;
    std::string PRODUCT_TYPE, POLICY_NUMBER, GENDER, PAYMENT_MODE, DATE_OF_COMMENCEMENT, DATE_OF_BIRTH, POLICY_TERM, SUM_ASSURED, PREMIUM;
    std::ifstream samplePoliciesCsv ("../data/Policy_Data.csv");

    std::vector<std::string> policy;
    std::vector<std::vector<std::string>> policies;

    if (samplePoliciesCsv.is_open()) {

        getline(samplePoliciesCsv, line); // Skip the first line. 

        while (getline( samplePoliciesCsv, line) ) {

            replace(line.begin(), line.end(), ',', ' ' );

            std::istringstream linestream(line);

            while (linestream >> PRODUCT_TYPE >> POLICY_NUMBER >> GENDER >> PAYMENT_MODE  >> DATE_OF_COMMENCEMENT >> DATE_OF_BIRTH >> POLICY_TERM  >> SUM_ASSURED >> PREMIUM) {

                policy.push_back(PRODUCT_TYPE);
                policy.push_back(POLICY_NUMBER);
                policy.push_back(GENDER);
                policy.push_back(PAYMENT_MODE);
                policy.push_back(DATE_OF_COMMENCEMENT);
                policy.push_back(DATE_OF_BIRTH);
                policy.push_back(POLICY_TERM);
                policy.push_back (SUM_ASSURED);
                policy.push_back(PREMIUM);
                
                policies.emplace_back(policy);
                policy.clear();
            }
        }
    }
    return policies;
};


int main()
{
//-------------------------------------------------------------------------------------------------//
    std::shared_ptr portfolioCashFlows = std::make_shared<PortfolioCashFlows>();
    std::vector<std::future<void>> futures;

    std::string dateOfValuation;
    std::cout << "Enter Date of Valuation (YYYY-MM-DD)" << std::endl;
    std::cin >> dateOfValuation; 

    RunType runType;
    std::string runTypeConsoleInput;
    std::cout << "Enter Run Type: Base or Stress" << std::endl;
    std::cin >> runTypeConsoleInput;
    if (runTypeConsoleInput == "Base") 
    {
        runType = RunType::Base;
    } else
    {
        runType = RunType::Stress;
    };

    Valuation *valuation = new Valuation(from_simple_string(dateOfValuation), runType);
    MortalityRates *mortalityRates = new MortalityRates(*valuation);
    LapseRates *lapseRates = new LapseRates(*valuation);

    std::vector<std::vector<std::string>> policyData = readPolicyData();
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct Policy objects and push into vector of Policy objects in the PortfolioCashFlows object using std::async
    std::for_each(policyData.begin(), policyData.end(), [&portfolioCashFlows, &valuation, &futures] (std::vector<std::string> &data) {
        
        Policy *policy = new Policy(*valuation, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackPolicy, portfolioCashFlows, std::move(*policy)));
    });
    // wait for threads to finish
    std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct TimeStepProjection objects and push into vector of TimeStepProjection objects in the PortfolioCashFlows object using std::async
   std::for_each(portfolioCashFlows->getPolicyVector().begin(), portfolioCashFlows->getPolicyVector().end(), [&portfolioCashFlows,&futures] (Policy &policy) {

       TimeStepProjection *timeStepProjection = new TimeStepProjection(policy);
       futures.emplace_back(std::async(&PortfolioCashFlows::pushBackTimeStepProjection, portfolioCashFlows, std::move(*timeStepProjection)));
   } );

   // wait for threads to finish
   std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the TimeStepProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getTimeStepProjectionVector().begin(), portfolioCashFlows->getTimeStepProjectionVector().end(), [&futures] (TimeStepProjection &timeStepProjection) {
        futures.emplace_back(std::async(&TimeStepProjection::run, std::ref(timeStepProjection)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct DecrementsProjection objects, and push into vector of DecrementsProjection objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getTimeStepProjectionVector().begin(), portfolioCashFlows->getTimeStepProjectionVector().end(), [&portfolioCashFlows, &futures, &mortalityRates, &lapseRates] (TimeStepProjection &timeStepProjection) {
        DecrementsProjection *decrementsProjection = new DecrementsProjection(timeStepProjection, *mortalityRates, *lapseRates);
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackDecrementsProjection, portfolioCashFlows, std::move(*decrementsProjection)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the DecrementsProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getDecrementsProjectionVector().begin(), portfolioCashFlows->getDecrementsProjectionVector().end(), [&futures] (DecrementsProjection &decrementsProjection) {
        futures.emplace_back(std::async(&DecrementsProjection::run, std::ref(decrementsProjection)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct CashFlowsProjection objects, and push into vector of CashFlowsProjection objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getDecrementsProjectionVector().begin(), portfolioCashFlows->getDecrementsProjectionVector().end(), [&portfolioCashFlows,&futures] (DecrementsProjection &decrementsProjection) {
        CashFlowsProjection *cashFlowsProjection = new CashFlowsProjection(decrementsProjection);
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackCashFlowsProjection, portfolioCashFlows, std::move(*cashFlowsProjection)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the CashFlowsProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getCashFlowsProjectionVector().begin(), portfolioCashFlows->getCashFlowsProjectionVector().end(), [&futures] (CashFlowsProjection &cashFlowsProjection) {
        futures.emplace_back(std::async(&CashFlowsProjection::run, std::ref(cashFlowsProjection)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct CashFlowsProjectionByValuationYear objects, and push into vector of CashFlowsProjectionByValuationYear objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getCashFlowsProjectionVector().begin(), portfolioCashFlows->getCashFlowsProjectionVector().end(), [&portfolioCashFlows,&futures] (CashFlowsProjection &cashFlowsProjection) {
        CashFlowsProjectionByValuationYear *cashFlowsProjectionByValuationYear = new CashFlowsProjectionByValuationYear(cashFlowsProjection);
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackCashFlowsProjectionByValuationYear, portfolioCashFlows, std::move(*cashFlowsProjectionByValuationYear)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the CashFlowsProjectionByValuationYear vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getCashFlowsProjectionByValuationYearVector().begin(), portfolioCashFlows->getCashFlowsProjectionByValuationYearVector().end(), [&futures] (CashFlowsProjectionByValuationYear &cashFlowsProjectionByValuationYear) {
        futures.emplace_back(std::async(&CashFlowsProjectionByValuationYear::run, std::ref(cashFlowsProjectionByValuationYear)));
    });

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // invoke the run function of the PortfolioCashFlows object.
    portfolioCashFlows->run();




    std::cout << "Finished" << std::endl;

   

    return 0;
//-------------------------------------------------------------------------------------------------//
}