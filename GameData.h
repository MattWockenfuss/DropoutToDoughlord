#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>

/*
    I dont necessarily need to store these variables in namespaces, in fact, since they are constant and referenced throughout the project
    it might be smarter not to. However I'm going to as it appears to bad practice to let many variables float around in the global namespace
*/
namespace MenuItems {
    static const double SliceBasePrice      = 3.00;
    static const double Cut8BasePrice       = 8.50;
    static const double Cut12BasePrice      = 13.99;

    static const double SliceDoughCost      = 0.125;
    static const double SliceSauceCost      = 0.125;
    static const double SliceCheeseCost     = 0.125;

    static const double Cut8DoughCost       = 1.00;
    static const double Cut8SauceCost       = 1.00;
    static const double Cut8CheeseCost      = 1.00;

    static const double Cut12DoughCost      = 1.50;
    static const double Cut12SauceCost      = 1.50;
    static const double Cut12CheeseCost     = 2.00;
    static const double Cut12WingsCost      = 1.00;
}

namespace EmployeesBase {
    /*
        These define base attributes for the different employees in a location
    */
    static const int ordersPerCook              = 150;
    static const int ordersPerCashier           = 750;
    static const int ordersPerDishwasher        = 1650;

    static const int extraOrdersPerDeliveryGuy  = 20;
    static const int maxDeliveryGuys            = 2;

    static const int extraCookOrdersPerManager  = 20;
    static const int maxManagers                = 2;

    static const int extraOrdersPerGM           = 50;
    static const double GM_COGSavings           = 0.075;
    static const int maxGMs                     = 1;

}

namespace Region {
    struct Data {
        std::string region;
        std::string director;
        int locations = 0;
    };
    static const int regionsCount = 9;
    extern Data regions[9];
}

namespace EquipmentBase {
    //okay so this stuff is per level of equipment
    static const int maxOrdersPerOvenLevel          = 1250;
    static const int maxEmployeeOrdersPerOvenLevel  = 30;
    static const int maxRobotOrdersPerOvenLevel     = 60;

    static const int maxWingsPerFryerLevel          = 60;
    static const double maxSuppliesPerFridgeLevel   = 1400;
    //this is calculated along with employee orders, so we can replace labor
    static const int maxOrdersPerRobotLevel         = 100;
    
}

namespace TaxAccountant {
    extern int count;
    static const int max                                    = 2;
    static const double taxReliefPer                        = 0.025;
    static const double pay                                 = 35.00;
}

namespace DigitalMarketingSpecialist {
    extern int count;
    static const int max                                    = 6;
    static const int extraDemandPer                         = 25;
    static const double pay                                 = 23.00;
}

namespace EmployeeRelationsManager {
    extern int count;
    static const int max                                    = 5;
    static const double payrollReliefPer                    = 0.01;
    static const double pay                                 = 32.00;
}

namespace DirectorOfOperations {
    extern int count;
    static const double COGSSavings                         = 0.075;
    static const double pay                                 = 45.00;
}

namespace LogisticsEngineer {
    extern int count;
    static const int max                                    = 5;
    static const double COGSSavings                         = 0.01;
    static const double pay                                 = 34.00;
}

namespace SoftwareDeveloper {
    extern int count;
    static const int max                                    = 2;
    static const int ExtraDemandPer                         = 20;
    static const double pay                                 = 35.00;
}

namespace RoboticSystemsAdministrator {
    extern int count;
    static const int max                                    = 3;
    static const int extraOrdersPer                         = 125;
    static const double pay                                 = 40.00;
}


namespace MISC {
    static const double buildingSellValue                   = 0.8;
    static const int minLocationsForCorporate               = 8;        //you need 8 Locations OR 120 Employees is the Logic
    static const int minEmployeesForCorporate               = 0;
    static const double corporateBuildingMultiplier         = 10;
    static const double executiveSignOnBonus                = 50000;
    static const double executiveHourlyPay                  = 120;
}

//inline constexpr std::array<int, 5> XP_LEVELS = {0, 100, 300, 700, 1500};
#endif