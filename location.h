#ifndef LOCATION_H
#define LOCATION_H

#include <string>

struct Location {
    bool alreadyOwned = false;
    int selectNum;

    std::string city;
    std::string state;
    std::string region;
    std::string blurb;
    
    int baseSliceDemand;
    int base8CutDemand;
    int base12CutComboDemand;
    int wealth;
    int buildingCost;
    int rent;
    int variance;
    
    double doughPrice;
    double saucePrice;
    double cheesePrice;
    double wingsPrice;

    double dishPay;
    double cookPay;
    double cashierPay;
    double deliveryGuyPay;
    double managerPay;
    double generalManagerPay;

    double pizzaOvenMaintenance;
    double fryerMaintenance;
    double fridgeMaintenance;
    double robotMaintenance;

    double taxRate;

    double pizzaOvenUpgradeCost;
    double fryerUpgradeCost;
    double fridgeUpgradeCost;
    double robotUpgradeCost;

    Location *next = nullptr;
};



#endif