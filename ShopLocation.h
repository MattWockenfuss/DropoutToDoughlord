#ifndef SHOP_LOCATION_H
#define SHOP_LOCATION_H

#include <string>
#include "Location.h"
#include "GameData.h"

struct ShopLocation {
    Location *location;
    
    //these are some flags that are triggered 
    bool notEnoughCooks = false;
    bool notEnoughCashiers = false;
    bool notEnoughDishwashers = false;

    bool notEnoughOvenSpace = false;
    bool notEnoughFryerSpace = false;
    bool notEnoughFridgeSpace = false;

    bool notEnoughDough = false;
    bool notEnoughSauce = false;
    bool notEnoughCheese = false;
    bool notEnoughWings = false;

    int openDate = 0;
    bool isRented;

    double slicePrice = 3.99;
    double Cut8Price = 12.50;
    double Cut12ComboPrice = 24.50;

    int pizzaOvenLevel = 1;
    int fryerLevel = 1;
    int fridgeLevel = 1;
    int robotLevel = 0;

    double DoughStock = 0;
    double SauceStock = 0;
    double CheeseStock = 0;
    double WingsStock = 0;

    double maxSupplies = 0;

    int dishCount = 0;
    int cookCount = 0;
    int cashierCount = 0;
    int deliveryGuyCount = 0;
    int managerCount = 0;
    int generalManagerCount = 0;


    int ordersPerCook;
    int ordersPerRobot;         //The robot acts exactly like a cook
    int ordersPerCashier;
    int ordersPerDishwasher;

    void recalcOrdersPerEmployee(){
        /*
            This function recalculates the amount of orders the location gets per cook, robot, cashier and dishwasher
            it is called at the beginning of every week (for fun) and everytime one of the variables changes
        */

        ordersPerCook = EmployeesBase::ordersPerCook                + (pizzaOvenLevel * EquipmentBase::maxEmployeeOrdersPerOvenLevel)
                                                                    + (managerCount * EmployeesBase::extraCookOrdersPerManager)
                                                                    + (generalManagerCount * EmployeesBase::extraOrdersPerGM);

        ordersPerRobot = EquipmentBase::maxOrdersPerRobotLevel      + (pizzaOvenLevel * EquipmentBase::maxRobotOrdersPerOvenLevel)
                                                                    + (managerCount * EmployeesBase::extraCookOrdersPerManager)
                                                                    + (generalManagerCount * EmployeesBase::extraOrdersPerGM);
                                                                    
        ordersPerCashier = EmployeesBase::ordersPerCashier          + (generalManagerCount * EmployeesBase::extraOrdersPerGM);

        ordersPerDishwasher = EmployeesBase::ordersPerDishwasher    + (generalManagerCount * EmployeesBase::extraOrdersPerGM);
    }

    //Last Week's Stats
    double latestEarnings = 0;
    double latestRevenue = 0;
    double latestCostOfGoodsSold = 0;
    double latestCostOfLabor = 0;
    double latestCostOfMaintenance = 0;

    double latestDoughUsed = 0;
    double latestSauceUsed = 0;
    double latestCheeseUsed = 0;
    double latestWingsUsed = 0;

    //The last time supplies were bought, if -1, then never
    int weekSuppliesBoughtLast = -1;

    //These are long term stats of the current location, would love to use
    int slicesSold = 0;
    int Cut8Sold = 0;
    int Cut12Sold = 0;
    double revenueFromSlices = 0;
    double revenueFrom8Cut = 0;
    double revenueFrom12Cut = 0;
    double totalRevenue = 0;

    ShopLocation *next;
};



#endif