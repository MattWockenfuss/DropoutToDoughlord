#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "utils.h"
#include "Location.h"
#include "ShopLocation.h"
#include "GameData.h"

using namespace std;

class PizzaCompany {
    public:
        int GAMESTATE = 0;

        PizzaCompany();
        void recalculateHeadcount();
        void readLocationsData();
        void printBeginningCutscene();
        void createFirstLocation();
        void toNextWeek();
        void buildAtNewLocation(string region);
        void ownedLocationScreen();
        void ActionMenu();
        void viewLocations();
        void ownedLocationsInRegion(string region);
        void viewRegions();
        void unownedLocationScreen();
        void printHUD();
        void headcountScreen();
        void costOfGoodsSoldScreen();
        void maintenanceScreen();
        void inventoryReport();
        void revenueReport();
        void printOrgChart();
        void openACorporateOffice();
        void visitCorporate();
        void talkToCOO();
        void talkToCFO();
        void talkToCMO();
        void talkToCTO();
        void talkToCLO();
        void manageBoard();
        void manageCorporateEmployees();

        double managerBuySuppliesThreshold = 100000.00; //if you are above it, then managers will buy supplies for their location
        double generalManagerHireThreshold = 200000.00; //if you are above it, then gms will hire when needed
        double generalManagerEquipmentThreshold = 200000.00; //if you are above it, then gms upgrade equipment when needed and u have enough
        int GMsUpgradeFridgeWhenSuppliesBelow = 20;
        bool GMsHireManagers = true;

        //These are global variables, especially used in the HUD
        string CEO;
        string COO = "";
        string CFO = "";
        string CMO = "";
        string CTO = "";
        string CLO = "";
        string shopname;

        double balance = 1211444.11;
        int numberOfLocations = 0;
        int numberOfEmployees = 1;
        int weekNumber = 1;         //The number of weeks that has passed, start at 1

        double lastWeekIncome = 0;
        
        int startExtraMenuOptionsSelect;

        struct corporateOffice {
            int openDate = 0;
            Location *location;
        };
        
        Location *firstLoc, *prevLoc, *currentLoc;
		ShopLocation *first, *prev, *current;
        corporateOffice *corporateLoc = nullptr;
        ShopLocation *lastCurrent, *lastPrevious; //used to store the last screen we were on before proceeding a week
};



PizzaCompany::PizzaCompany(){
    GAMESTATE = 1;
	first = new ShopLocation;
	first -> next = 0;
	prev = first;

    firstLoc = new Location;
	firstLoc -> next = 0;
	prevLoc = firstLoc;
}

void PizzaCompany::recalculateHeadcount(){
    /*
        This function is called at the beginning of every week, and after any changes in employees
        Its purpose is to make sure that the 'numberOfEmployees' matches the actual number of employees
    
        to do this, we first add in all of the employees across all owned stores.
        then we add in corporate employees
        then we add in CSuite
    */

    int total = 0;
    ShopLocation* ptr = first -> next;
    //woah this is cool
    while(ptr != 0){
        total += ptr -> cookCount;
        total += ptr -> cashierCount;
        total += ptr -> dishCount;
        total += ptr -> deliveryGuyCount;
        total += ptr -> managerCount;
        total += ptr -> generalManagerCount;
        ptr = ptr -> next;
    }

    //now we add corporate employees
    total += TaxAccountant::count;
    total += DigitalMarketingSpecialist::count;
    total += EmployeeRelationsManager::count;
    total += LogisticsEngineer::count;
    total += SoftwareDeveloper::count;
    total += RoboticSystemsAdministrator::count;


    // now the directors of operations
    for(int i = 0; i < Region::regionsCount; i++){
        if(!Region::regions[i].director.empty()){
            //if its not empty then we have a director, add it
            total += 1;
        }
    }

    //now we add CSuite Executives
    total += 1;     //for the CEO, ourselves
    if(!COO.empty()) total += 1;
    if(!CFO.empty()) total += 1;
    if(!CMO.empty()) total += 1;
    if(!CTO.empty()) total += 1;
    if(!CLO.empty()) total += 1;

    //thats everyone, our new headcount is <total>
    numberOfEmployees = total;
}


void PizzaCompany::readLocationsData(){
    cout << "Attempting to read locations.csv!" << endl;
    ifstream infile;
	infile.open("locations.csv");


    string line;

    //skip the first row (line) that has our headers in it.
    getline(infile, line);
    int i = 1;

    while(getline(infile, line)){
        currentLoc = new Location;

        //this creates a new stream object from our line, allows us to use extraction operation, 
        //a.k.a getline and >>, we use ',' as delimiter because its a .csv
        stringstream ss(line); 

        getline(ss, currentLoc -> city, ',');
        getline(ss, currentLoc -> state, ',');
        getline(ss, currentLoc -> region, ',');

        string t1 = "";
        string t2 = "";
        string t3 = "";
        string t4 = "";
        string t5 = "";
        string t6 = "";


        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        currentLoc -> baseSliceDemand = stoi(t1);
        currentLoc -> base8CutDemand = stoi(t2);
        currentLoc -> base12CutComboDemand = stoi(t3);

        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        getline(ss, t4, ',');
        currentLoc -> wealth = stoi(t1);
        currentLoc -> buildingCost = stoi(t2);
        currentLoc -> rent = stoi(t3);
        currentLoc -> variance = stoi(t4);

        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        getline(ss, t4, ',');
        currentLoc -> doughPrice= stod(t1);
        currentLoc -> saucePrice= stod(t2);
        currentLoc -> cheesePrice= stod(t3);
        currentLoc -> wingsPrice= stod(t4);

        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        getline(ss, t4, ',');
        getline(ss, t5, ',');
        getline(ss, t6, ',');
        currentLoc -> dishPay = stod(t1);
        currentLoc -> cookPay = stod(t2);
        currentLoc -> cashierPay = stod(t3);
        currentLoc -> deliveryGuyPay = stod(t4);
        currentLoc -> managerPay = stod(t5);
        currentLoc -> generalManagerPay = stod(t6);

        getline(ss, t1, ',');
        getline(ss, t2, ',');
        getline(ss, t3, ',');
        getline(ss, t4, ',');
        currentLoc -> pizzaOvenMaintenance = stod(t1);
        currentLoc -> fryerMaintenance = stod(t2);
        currentLoc -> fridgeMaintenance = stod(t3);
        currentLoc -> robotMaintenance = stod(t4);

        currentLoc -> pizzaOvenUpgradeCost = currentLoc -> pizzaOvenMaintenance * 100;
        currentLoc -> fryerUpgradeCost = currentLoc -> fryerMaintenance * 150;
        currentLoc -> fridgeUpgradeCost = currentLoc -> fridgeMaintenance * 200;
        currentLoc -> robotUpgradeCost = currentLoc -> robotMaintenance * 100;

        getline(ss, t1, ',');
        currentLoc -> taxRate = stod(t1);

        getline(ss, t1);

        //okay so now t1 has the blurb we want to remove the first and last character
        if(t1[0] == '"'){
            t1 = t1.substr(1, t1.size() - 2); //if it starts with a ", remove the first and last characters.
        }
        currentLoc -> blurb = t1;

        currentLoc -> selectNum = i;
        i += 1;

		currentLoc -> next = 0;
		prevLoc -> next = currentLoc;
		prevLoc = currentLoc;
    }
    //okay so we are done looping through the location data.
    startExtraMenuOptionsSelect = i;


    currentLoc = firstLoc -> next;
    // while(currentLoc != 0){
    //     cout << currentLoc -> city << " " 
    //     << currentLoc -> state
    //     << " Region: " << currentLoc -> region  
    //     << " Attractiveness: " << currentLoc -> baseSliceDemand 
    //     << " Wealth: " << currentLoc -> wealth 
    //     << " Rent: " << currentLoc -> rent << endl;

    //     currentLoc = currentLoc -> next;
    // }
    //system("pause");
	infile.close();
}

void PizzaCompany::printBeginningCutscene(){
    system("cls");
    cout << "Oh my God... I completely forgot what a pointer was. ppffftt, when am I ever going to need to know the exact name of the string library! Her tests suck. I" << endl;
    cout << "close my laptop and wish Dr. Baugh a happy holidays with a fake smile.\n'I'll see you next year! Have a good one!' I spew, quickly walking out of the room.\n" << endl;
    cout << "'Her tests are so unfair' - I sigh to myself.\n'Who?' asked a familiar voice. \nI bumped into Sarah. The polo to Dr. Baugh's Marco. The stereotypical know-it-all, Her";
    cout << " voice is the only one heard in \nHale 309 at 9:14 AM. Just when I thought I could forget that while we were in elementary school together, Sarah's\nwriting nested linked lists while";
    cout << " I'm in the corner stacking blocks.\n\n'Dr. Baugh' I sighed even lower in response to Sarah. Wondering if she got a 96 or 98.\n'Her tests are NOT hard at all, it's practically a piece of cake. Besides, shes so nice!' ";
    cout << "Sarah replied, giving me the 'oh your so dumb look'." << endl;

    cout << endl;
    cout << "Maybe this is why Apple invented AirPods with noise cancelling." << endl; 
    system("pause");
    system("cls");

    cout << "'I don't know' I guess I'm just not that good at programming, I'm not sure I even passed the class, guess I'll wait and see.' " << endl;
    cout << "'Do you need the class for your degree?' Sarah asked, already knowing the answer." << endl;
    cout << "'No'. I said, 'I'm a business major, just taking it for fun.'" << endl;
    cout << "'Oh! Maybe we'll see each other next semester, I'm taking a business class thats required for computer science next semester. I think my degree audit says Digital Marketing." << endl;
    cout << "Yeah! Digital Marketing, Monday-Wednesday-Fridays, at 11.' Sarah gleefully said." << endl;
    cout << "'Shit' I thought. 'Actually yeah I'll be in that class. That'll be cool', I said. " << endl;
    cout << "'That's gonna be so fun! We could study together too! Maybe you'll actually ace the final!' She responded. " << endl;
    cout << "'Yeah. maybe...'\n\n";
    cout << "A few seconds go by, you guys keep walking, about to part ways." << endl;
    cout << "'Hey, I gotta get going, I'll see you next semester?' I asked her." << endl;
    cout << "'You bet! Have a good break! I'll see ya!'" << endl;
    cout << "'You too! I respond.\n\n";
    cout << "I pass the UPMC Events center on my way towards the University Campus Entrance." << endl;
    system("pause");
    system("cls");
    cout << "I keep walking..." << endl;
    system("pause");
    system("cls");

    cout << "As I approach the red light that splits University with the Moon shopping center I realize how hungry I am. I'm deciding if I want Pizza Fiesta, Mike & Tony's, or Burger King." << endl;
    cout << "Honestly the toughest question I've had all day. Harder than reading files into linked lists. I push the pedestrian button." << endl;
    system("pause");
    system("cls");

    cout << "The pedestrian light gives me that look so I know its time. I start to cross when all of a sudden. Out of the corner of my eye I see a tinted out dodge challenger barrelling towards" << endl;
    cout << "me. I immediately run to the sidewalk. He slams on the break, and I can't seem him, but can only imagine he thinks I'm the problem. At least I'm perfectly fine," << endl;
    cout << "angrily looking at this guy, wondering what he was thinking." << endl;
    cout << "Filled with disbelief that I was mere inches from never getting to try Pizza Fiesta, as I've been to burger king more times than Advanced C++." << endl;
    cout << "I keep walking." << endl;
    system("pause");
    system("cls");

    cout << "Just passing Excel Dental, I see two guys outside Pizza Fiesta arguing. It look's pretty serious. As I approach I hear one" << endl;
    cout << "of them yelling about money. 'Great'. I thought. One of them is an older guy, the other younger, only a few years older than me. The young guy exclaims" << endl;
    cout << "'Look im tired of this! I'm done with you telling me how to do everything! I quit!' and he storms off. I arrived by this point and the old guy looks at me-" << endl;
    cout << "'What do you want some pizza or something.' 'Actually kinda, but it looks like you guys are closed' I replied. \n'Oh nonsense, I old Rick has a pizza shop, and a hungry customer,";
    cout << "we ain't closed!' He says." << endl;
    cout << "We end up sitting down inside together, sharing a beautiful, stuffed crust, pepperoni and cheese pizza. In a wood fired oven just long enough for the pepperoni to curl." << endl;
    system("pause");
    system("cls");
    
    cout << "'Whats your name kid?' He asks me." << endl;
    //CEO = "Matt";
    cin >> CEO;

    cout << "'So " << CEO << ". Whats a young man like you do for work?' He asks me." << endl;
    cout << "'Im a business student actually.'." << endl;
    system("pause");
    cout << "\n";
    cout << "'Really? Business huh? So you think you know a lot about business " << CEO << "?' He posits." << endl;
    cout << "'Not yet, still learning, still inexperienced.'" << endl;
    system("pause");
    cout << "\n";
    cout << "'You know..' Rick starts." << endl;
    cout << "'My son doesn't want to run this shop any more. I've been running it for 58 years, and him for 17, *cough* give or take.'" << endl;
    cout << "'It's time I retire, " << CEO << ".'" << endl;
    system("pause");
    cout << "\n";
    cout << "'What are you gonna do with the shop?' I ask, genuinely curious." << endl;
    cout << "'Well... I always dreamed that my son would take it over, but that ship sailed *cough*'." << endl;
    system("pause");
    system("cls");
    cout << "...";
    system("pause");
    system("cls");
    cout << "'" << CEO << ", you like pizza dont you business student? How would you like to take over my shop? free of charge!' He chuckles." << endl;
    cout << endl;
    cout << "Oh my God... This is the crazist day of my life..." << endl;
    cout << endl;
    cout << "'Absolutely!' I shout, and we shake hands." << endl;
    system("pause");
    system("cls");

    cout << "You spend almost every ounce of your life savings remodelling..." << endl;
    system("pause");

    cout << "\nYou got new chairs, (-$2,100.00)\n";
    system("pause");

    cout << "\nnew menus, (-$322.88)\n";
    system("pause");

    cout << "\nclean plates, (-$931.76)\n";
    system("pause");

    cout << "\na new 'state of the art' Oven, (-$3,250.00)\n";
    system("pause");

    cout << "\na mop bucket, (-$87.99 WHAT!)\n";
    system("pause");

    cout << "\nEverything!\n";
    system("pause");

    cout << "\nYou even had to pay 'backtaxes' (Thanks old man Rick)., (-$3,500.00)\n";
    system("pause");
    
    cout << "\nIt's finally all done, and your ready for your first week of business." << endl;

    cout << "What is the name for your new shop?" << endl;
    cin.ignore(10000, '\n');
    getline(cin, shopname); //this lets us get the entire line, not separated by spaces

    createFirstLocation();

    cout << shopname << " is now open for business! Just across the street from (your now former university) Robert Morris University in " 
    << Utils::getLocationString(current -> location -> city, current -> location -> state) << "\n\n";

    cout << "You have 3 different items on the menu, a single slice for the on the go student, an 8-cut pizza, and finally, a 12-cut pizza and wings combo." << endl;
    cout << "Its time to set your prices! Remember, too low and you might not make enought money, and too high, well, customers might stop coming!\n\n";

    string p = "\tThe current price of a Slice of Pizza is " +Utils::getMoneyString(current -> slicePrice) + ", and you want to change it to $";
    double newPrice = Utils::sanitizeDoubleInput("\t", p);
    current -> slicePrice = max(min(newPrice, MenuItems::SliceBasePrice * 10), 0.0);


    p = "\tThe current price of an 8 Cut Pizza is " +Utils::getMoneyString(current -> Cut8Price) + ", and you want to change it to $";
    newPrice = Utils::sanitizeDoubleInput("\t", p);
    current -> Cut8Price = max(min(newPrice, MenuItems::Cut8BasePrice * 10), 0.0);

    p = "\tThe current price of a 12 Cut Pizza and Wings Combo is " +Utils::getMoneyString(current -> Cut12ComboPrice) + ", and you want to change it to $";
    newPrice = Utils::sanitizeDoubleInput("\t", p);
    current -> Cut12ComboPrice = max(min(newPrice, MenuItems::Cut12BasePrice * 10), 0.0);


    cout << "\n\n\nLets see how the first week goes!\n";
    system("pause");
    system("cls");
}

void PizzaCompany::createFirstLocation(){
    current = new ShopLocation;

    //alright they are creating their first location!

    //we want to loop through the list of locations and find Pittsburgh
    prevLoc = firstLoc;
    currentLoc = firstLoc -> next;
    while(currentLoc != 0){
        if(currentLoc -> city == "Pittsburgh") break;
        prevLoc = currentLoc;
        currentLoc = currentLoc -> next;
    }

    current -> location = currentLoc;
    current -> location -> alreadyOwned = true;
    current -> isRented = true;

    current -> maxSupplies = current -> fridgeLevel * EquipmentBase::maxSuppliesPerFridgeLevel;

    current -> DoughStock = current -> maxSupplies;
    current -> SauceStock = current -> maxSupplies;
    current -> CheeseStock = current -> maxSupplies;
    current -> WingsStock = current -> maxSupplies;
    current -> openDate = 1;
    numberOfLocations++;
    Region::regions[0].locations++;  //north america

    current -> next = 0;
    prev -> next = current;
    prev = first;
}

void PizzaCompany::toNextWeek(){
    
    system("cls");
    printHUD();
    cout << "                                         " << shopname << " Weekly Report   (" << Utils::getYearString(weekNumber) << ")" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;

    prev = first;
    current = first -> next;
    

    double weeklyTotalRevenue = 0;
    double managersBoughtSupplies = 0;
    weekNumber += 1;

    while (current != 0){
        //Set the Flags
        current -> notEnoughCooks = false;
        current -> notEnoughCashiers = false;
        current -> notEnoughDishwashers = false;
        current -> notEnoughOvenSpace = false;
        current -> notEnoughFryerSpace = false;
        current -> notEnoughDough = false;
        current -> notEnoughSauce = false;
        current -> notEnoughCheese = false;
        current -> notEnoughWings = false;

        //Compute Base stats including delivery guy bonus
        int deliveryGuysBonus = (current -> deliveryGuyCount * EmployeesBase::extraOrdersPerDeliveryGuy);
        int baseSlice = current -> location -> baseSliceDemand + deliveryGuysBonus;
        int base8 = current -> location -> base8CutDemand + deliveryGuysBonus;
        int base12 = current -> location -> base12CutComboDemand + deliveryGuysBonus;
        int var = current -> location -> variance;
        
        //Generate Random Sales Figure based on BaseDemand, Delivery Guy Bonus, and Variance
        int slicesBaseSALES = Utils::getRandomInt(baseSlice - var, baseSlice + var);
        int Cut8BaseSALES = Utils::getRandomInt(base8 - var, base8 + var);
        int Cut12BaseSALES = Utils::getRandomInt(base12 - var, base12 + var);
        
        //Define Sales
        int slicesSALES = 0;
        int Cut8SALES = 0;
        int Cut12SALES = 0;

        //Define Markup Percents for each item
        double markupSingle = (current -> slicePrice - MenuItems::SliceBasePrice) / MenuItems::SliceBasePrice;
        double markup8Cut = (current -> Cut8Price - MenuItems::Cut8BasePrice) / MenuItems::Cut8BasePrice;
        double markup12CutCombo = (current -> Cut12ComboPrice - MenuItems::Cut12BasePrice) / MenuItems::Cut12BasePrice;

        //Here we declare and define the purchase probability for each of our menu items based on the wealth of the location
        //The purchase probabilty is between 0 and 1, poor locations have a lower chance to buy based on markup percent
        double ppSingle;
        double pp8Cut;
        double pp12Cut;

        if(current -> location -> wealth >= 87){
            //wealthy location
            ppSingle = (4 / (0.6 * (markupSingle + 12))) + 0.35;
            pp8Cut = (4 / (0.6 * (markup8Cut + 12))) + 0.35;
            pp12Cut = (4 / (0.6 * (markup12CutCombo + 12))) + 0.35;
        }else if(current -> location -> wealth >= 50){
            //middling location
            ppSingle = (1 / (0.17 * (markupSingle + 5.8))) - 0.35;
            pp8Cut = (1 / (0.17 * (markup8Cut + 5.8))) - 0.35;
            pp12Cut = (1 / (0.17 * (markup12CutCombo + 5.8))) - 0.35;
        }else if(current -> location -> wealth >= 14){
            //struggling location
            ppSingle = 1 / (1.57 * (markupSingle + 1.4));
            pp8Cut = 1 / (1.57 * (markup8Cut + 1.4));
            pp12Cut = 1 / (1.57 * (markup12CutCombo + 1.4));
        }else{
            //poor location
            ppSingle = 1 / (3 * (markupSingle + 1.3));
            pp8Cut = 1 / (3 * (markup8Cut + 1.3));
            pp12Cut = 1 / (3 * (markup12CutCombo + 1.3));
        }

        
        //we define the number of tries we have for each of our menu items
        int numOfTries = max({slicesBaseSALES, Cut8BaseSALES, Cut12BaseSALES});
        int sliceTries = 0;
        int cut8Tries = 0;
        int cut12Tries = 0;
        
        double startingDough = current -> DoughStock;
        double startingSauce = current -> SauceStock;
        double startingCheese = current -> CheeseStock;
        double startingWings = current -> WingsStock;



        for(int i = 0; i < numOfTries; i++){
            
            if(sliceTries < slicesBaseSALES){
                double d = Utils::getRandomProbability();
                sliceTries++;
                if(d < ppSingle){
                    //then we have a sale, well given we have everything!

                    //lets define reused variables to make this look nicer
                    int salesSoFar = (slicesSALES + Cut8SALES + Cut12SALES);
                    int availableCooks = (current -> cookCount * current -> ordersPerCook) + (current -> robotLevel * current -> ordersPerRobot);
                    int availableCashiers = (current -> cashierCount * current -> ordersPerCashier);
                    int availableDishwashers = (current -> dishCount * current -> ordersPerDishwasher);
                    bool cantMake = false;

                    //lets check if we have the labor, equipment space and ingredients, then we can make it
                    
                    //equipment
                    if(salesSoFar >= current -> pizzaOvenLevel * EquipmentBase::maxOrdersPerOvenLevel) current -> notEnoughOvenSpace = cantMake = true;
                    
                    //labor
                    if(salesSoFar >= availableCooks) current -> notEnoughCooks = cantMake = true;
                    if(salesSoFar >= availableCashiers) current -> notEnoughCashiers = cantMake = true;
                    if(salesSoFar >= availableDishwashers) current -> notEnoughDishwashers = cantMake = true;
                    
                    //ingredients
                    if(MenuItems::SliceDoughCost > current -> DoughStock) current -> notEnoughDough = cantMake = true;
                    if(MenuItems::SliceSauceCost > current -> SauceStock) current -> notEnoughSauce = cantMake = true;
                    if(MenuItems::SliceCheeseCost > current -> CheeseStock) current -> notEnoughCheese = cantMake = true;
                    
                    if(!cantMake){
                        //then we have everything we need, lets make it!
                        slicesSALES++;
                        current -> DoughStock -= MenuItems::SliceDoughCost;
                        current -> SauceStock -= MenuItems::SliceSauceCost;
                        current -> CheeseStock -= MenuItems::SliceCheeseCost;
                    }
                }
            }
            if(cut8Tries < Cut8BaseSALES){
                double d = Utils::getRandomProbability();
                cut8Tries++;
                if(d < pp8Cut){
                    int salesSoFar = (slicesSALES + Cut8SALES + Cut12SALES);
                    int availableCooks = (current -> cookCount * current -> ordersPerCook) + (current -> robotLevel * current -> ordersPerRobot);
                    int availableCashiers = (current -> cashierCount * current -> ordersPerCashier);
                    int availableDishwashers = (current -> dishCount * current -> ordersPerDishwasher);
                    bool cantMake = false;

                    if(salesSoFar >= current -> pizzaOvenLevel * EquipmentBase::maxOrdersPerOvenLevel) current -> notEnoughOvenSpace = cantMake = true;
                    if(salesSoFar >= availableCooks) current -> notEnoughCooks = cantMake = true;
                    if(salesSoFar >= availableCashiers) current -> notEnoughCashiers = cantMake = true;
                    if(salesSoFar >= availableDishwashers) current -> notEnoughDishwashers = cantMake = true;
                   
                    if(MenuItems::Cut8DoughCost > current -> DoughStock) current -> notEnoughDough = cantMake = true;
                    if(MenuItems::Cut8SauceCost > current -> SauceStock) current -> notEnoughSauce = cantMake = true;
                    if(MenuItems::Cut8CheeseCost > current -> CheeseStock) current -> notEnoughCheese = cantMake = true;
                    
                    if(!cantMake){
                        Cut8SALES++;
                        current -> DoughStock -= MenuItems::Cut8DoughCost;
                        current -> SauceStock -= MenuItems::Cut8SauceCost;
                        current -> CheeseStock -= MenuItems::Cut8CheeseCost;
                    }
                }
            }
            if(cut12Tries < Cut12BaseSALES){
                double d = Utils::getRandomProbability();
                cut12Tries++;
                if(d < pp12Cut){
                    int salesSoFar = (slicesSALES + Cut8SALES + Cut12SALES);
                    int availableCooks = (current -> cookCount * current -> ordersPerCook) + (current -> robotLevel * current -> ordersPerRobot);
                    int availableCashiers = (current -> cashierCount * current -> ordersPerCashier);
                    int availableDishwashers = (current -> dishCount * current -> ordersPerDishwasher);
                    bool cantMake = false;

                    if(salesSoFar >= current -> pizzaOvenLevel * EquipmentBase::maxOrdersPerOvenLevel) current -> notEnoughOvenSpace = cantMake = true;
                    if(Cut12SALES >= current -> fryerLevel * EquipmentBase::maxWingsPerFryerLevel) current -> notEnoughFryerSpace = cantMake = true;

                    if(salesSoFar >= availableCooks) current -> notEnoughCooks = cantMake = true;
                    if(salesSoFar >= availableCashiers) current -> notEnoughCashiers = cantMake = true;
                    if(salesSoFar >= availableDishwashers) current -> notEnoughDishwashers = cantMake = true;
                   
                    if(MenuItems::Cut12DoughCost > current -> DoughStock) current -> notEnoughDough = cantMake = true;
                    if(MenuItems::Cut12SauceCost > current -> SauceStock) current -> notEnoughSauce = cantMake = true;
                    if(MenuItems::Cut12CheeseCost > current -> CheeseStock) current -> notEnoughCheese = cantMake = true;
                    if(MenuItems::Cut12WingsCost > current -> WingsStock) current -> notEnoughWings = cantMake = true;
                    
                    if(!cantMake){
                        Cut12SALES++;
                        current -> DoughStock -= MenuItems::Cut12DoughCost;
                        current -> SauceStock -= MenuItems::Cut12SauceCost;
                        current -> CheeseStock -= MenuItems::Cut12CheeseCost;
                        current -> WingsStock -= MenuItems::Cut12WingsCost;
                    }
                }
            }
        }
        
        //alright, we want to set a fridge space flag
        if(current -> maxSupplies - current -> latestDoughUsed < GMsUpgradeFridgeWhenSuppliesBelow ||
            current -> maxSupplies - current -> latestSauceUsed < GMsUpgradeFridgeWhenSuppliesBelow ||
            current -> maxSupplies - current -> latestCheeseUsed < GMsUpgradeFridgeWhenSuppliesBelow ||
            current -> maxSupplies - current -> latestWingsUsed < GMsUpgradeFridgeWhenSuppliesBelow){
                //if we just used pretty much all of our supplies than
                current -> notEnoughFridgeSpace = true;
            }



        //alright so we have computed our sales. We have factored in the wealth of the area, how much labor we pay for, and how much supplies we have
        //in stock. We also substract the amount of used supplies from our stock pile, we no longer want to pay for them in our balance.
        //we also take into account the availble level of our equipment, like ovens, fryers, and robots

        string status = "";
        if(current -> location -> wealth >= 87){
            //wealthy location
            status = "Wealthy";
        }else if(current -> location -> wealth >= 50){
            //middling location
            status = "Middling";
        }else if(current -> location -> wealth >= 14){
            //struggling location
            status = "Struggling";
        }else{
            //poor location
            status = "Poor";
        }


        
        double weeklyRevenueSlices = current -> slicePrice * slicesSALES;
        double weeklyRevenue8Cut = current -> Cut8Price * Cut8SALES;
        double weeklyRevenue12Cut = current -> Cut12ComboPrice * Cut12SALES;




        //set stats of city
        current -> slicesSold += slicesSALES;
        current -> Cut8Sold += Cut8SALES;
        current -> Cut12Sold += Cut12SALES;
        current -> revenueFromSlices += weeklyRevenueSlices;
        current -> revenueFrom8Cut += weeklyRevenue8Cut;
        current -> revenueFrom12Cut += weeklyRevenue12Cut;
        current -> totalRevenue += (weeklyRevenueSlices + weeklyRevenue8Cut + weeklyRevenue12Cut);


        weeklyTotalRevenue += (weeklyRevenueSlices + weeklyRevenue8Cut + weeklyRevenue12Cut);
        


        cout << current -> location -> city << " " << current -> location -> state << " (" << status << ")" << endl;


        cout << "\t" 
            << left << setw(30) << ""
            << right << setw(9) << "Price ($)"
            << right << setw(12) << "Markup (%)" 
            << right << setw(7) << "Sold"
            << right << setw(12) << "Revenue ($)" 
            << endl;

        cout << "\t" 
            << left << setw(30) << "Single Slice"
            << right << setw(9) << Utils::getMoneyString(current -> slicePrice)
            << right << setw(12) << Utils::getCompoundString(Utils::getDigitDecimal(markupSingle * 100) , "%")
            << right << setw(7) << slicesSALES
            << right << setw(12) << Utils::getMoneyString(weeklyRevenueSlices)
            << endl;

        cout << "\t" 
            << left << setw(30) << "8 Cut Pizza"
            << right << setw(9) << Utils::getMoneyString(current -> Cut8Price)
            << right << setw(12) << Utils::getCompoundString(Utils::getDigitDecimal(markup8Cut * 100) , "%")
            << right << setw(7) << Cut8SALES
            << right << setw(12) << Utils::getMoneyString(weeklyRevenue8Cut)
            << endl;

        cout << "\t" 
            << left << setw(30) << "12 Cut Pizza and Wings Combo"
            << right << setw(9) << Utils::getMoneyString(current -> Cut12ComboPrice)
            << right << setw(12) << Utils::getCompoundString(Utils::getDigitDecimal(markup12CutCombo * 100) , "%")
            << right << setw(7) << Cut12SALES
            << right << setw(12) << Utils::getMoneyString(weeklyRevenue12Cut)
            << endl;

        //next we want to have a little table for the cost of goods sold, dough sauce cheese wings
        current -> latestDoughUsed = MenuItems::SliceDoughCost * slicesSALES + MenuItems::Cut8DoughCost * Cut8SALES + MenuItems::Cut12DoughCost * Cut12SALES;
        current -> latestSauceUsed = MenuItems::SliceSauceCost * slicesSALES + MenuItems::Cut8SauceCost * Cut8SALES + MenuItems::Cut12SauceCost * Cut12SALES;
        current -> latestCheeseUsed = MenuItems::SliceCheeseCost * slicesSALES + MenuItems::Cut8CheeseCost * Cut8SALES + MenuItems::Cut12CheeseCost * Cut12SALES;
        current -> latestWingsUsed = MenuItems::Cut12WingsCost * Cut12SALES;


        double doughcost = current -> location -> doughPrice;
        double saucecost = current -> location -> saucePrice;
        double cheesecost = current -> location -> cheesePrice;
        double wingscost = current -> location -> wingsPrice;

        if(current -> generalManagerCount > 0){
            doughcost   *= (1 - EmployeesBase::GM_COGSavings);
            saucecost   *= (1 - EmployeesBase::GM_COGSavings);
            cheesecost  *= (1 - EmployeesBase::GM_COGSavings);
            wingscost   *= (1 - EmployeesBase::GM_COGSavings);
        }


        double costSingle =     MenuItems::SliceDoughCost * slicesSALES * doughcost +
                                MenuItems::SliceSauceCost * slicesSALES * saucecost +
                                MenuItems::SliceCheeseCost * slicesSALES * cheesecost;

        double cost8Cut =       MenuItems::Cut8DoughCost* Cut8SALES * doughcost +
                                MenuItems::Cut8SauceCost * Cut8SALES * saucecost +
                                MenuItems::Cut8CheeseCost * Cut8SALES * cheesecost;

        double cost12Cut =      MenuItems::Cut12DoughCost * Cut12SALES * doughcost +
                                MenuItems::Cut12SauceCost * Cut12SALES * saucecost +
                                MenuItems::Cut12CheeseCost * Cut12SALES * cheesecost+
                                MenuItems::Cut12WingsCost * Cut12SALES * wingscost;

        current -> latestCostOfGoodsSold = costSingle + cost8Cut + cost12Cut;


        cout << endl << "\tCost of Goods Sold" << endl;
        cout << "\t\t"
            << left << setw(30) << "Ingredient"
            << right << setw(11) << "Dough"
            << right << setw(11) << "Sauce"
            << right << setw(11) << "Cheese"
            << right << setw(11) << "Wings"
            << right << setw(12) << "Cost($)"
            << endl;


        cout << "\t\t" 
            << left << setw(30) << "Price Per Unit ($)"
            << right << setw(11) << Utils::getMoneyString(doughcost)
            << right << setw(11) << Utils::getMoneyString(saucecost)
            << right << setw(11) << Utils::getMoneyString(cheesecost)
            << right << setw(11) << Utils::getMoneyString(wingscost)
            << right << setw(12) << ""
            << endl;

        cout << "\t\t" 
            << left << setw(30) << ""
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;

        cout << "\t\t" 
            << left << setw(30) << "Starting Inventory"
            << right << setw(11) << Utils::get2DigitDecimal(startingDough)
            << right << setw(11) << Utils::get2DigitDecimal(startingSauce)
            << right << setw(11) << Utils::get2DigitDecimal(startingCheese)
            << right << setw(11) << Utils::get2DigitDecimal(startingWings)
            << right << setw(12) << ""
            << right << setw(19) << ""
            << endl;

        cout << "\t\t" 
            << left << setw(30) << "Used"
            << right << setw(11) << Utils::get2DigitDecimal(current -> latestDoughUsed)
            << right << setw(11) << Utils::get2DigitDecimal(current -> latestSauceUsed)
            << right << setw(11) << Utils::get2DigitDecimal(current -> latestCheeseUsed)
            << right << setw(11) << Utils::get2DigitDecimal(current -> latestWingsUsed)
            << right << setw(12) << ""
            << right << setw(19) << ""
            << endl;

        
        string managerBoughtSupplies = "";
        //alright so we just figured out payroll, next we want the manager to buy supplies if they got it
        if(current -> managerCount > 0 && balance >= managerBuySuppliesThreshold){
            //they buy supplies for the location
            managerBoughtSupplies = "Manager Bought Supplies";
            managersBoughtSupplies += current -> latestCostOfGoodsSold;
            current -> DoughStock= current -> maxSupplies;
            current -> SauceStock= current -> maxSupplies;
            current -> CheeseStock = current -> maxSupplies;
            current -> WingsStock= current -> maxSupplies;
            current -> notEnoughDough = false;
            current -> notEnoughSauce = false;
            current -> notEnoughCheese = false;
            current -> notEnoughWings = false;
            current -> weekSuppliesBoughtLast = weekNumber;// We bought supplies this week
        }
        cout << "\t\t" 
            << left << setw(30) << "Ending Inventory"
            << right << setw(11) << Utils::get2DigitDecimal(current -> DoughStock)
            << right << setw(11) << Utils::get2DigitDecimal(current -> SauceStock)
            << right << setw(11) << Utils::get2DigitDecimal(current -> CheeseStock)
            << right << setw(11) << Utils::get2DigitDecimal(current -> WingsStock)
            << right << setw(12) << ""
            << right << setw(19) << ""
            << endl;



        cout << "\t\t" 
            << left << setw(30) << ""
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;


        cout << "\t\t" 
            << left << setw(30) << "Single Slice"
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::SliceDoughCost * slicesSALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::SliceSauceCost * slicesSALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::SliceCheeseCost * slicesSALES)
            << right << setw(11) << ""
            << right << setw(12) << Utils::getMoneyString(costSingle)
            << endl;
        cout << "\t\t" 
            << left << setw(30) << "8 Cut Pizza"
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut8DoughCost * Cut8SALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut8SauceCost * Cut8SALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut8CheeseCost * Cut8SALES)
            << right << setw(11) << ""
            << right << setw(12) << Utils::getMoneyString(cost8Cut)
            << endl;
        cout << "\t\t" 
            << left << setw(30) << "12 Cut Pizza and Wings Combo"
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut12DoughCost * Cut12SALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut12SauceCost * Cut12SALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut12CheeseCost * Cut12SALES)
            << right << setw(11) << Utils::get2DigitDecimal(MenuItems::Cut12WingsCost * Cut12SALES)
            << right << setw(12) << Utils::getMoneyString(cost12Cut)
            << endl;

        //okay we also want to print total used
        cout << "\t\t" 
            << left << setw(30) << ""
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;

        cout << "\t\t" 
            << left << setw(30) << "Cost of Goods Sold ($)"
            << right << setw(11) << Utils::getMoneyString(doughcost * current -> latestDoughUsed)
            << right << setw(11) << Utils::getMoneyString(saucecost * current -> latestSauceUsed)
            << right << setw(11) << Utils::getMoneyString(cheesecost * current -> latestCheeseUsed)
            << right << setw(11) << Utils::getMoneyString(wingscost * current -> latestWingsUsed)
            << right << setw(12) << Utils::getMoneyString(current -> latestCostOfGoodsSold)
            << right << setw(30) << managerBoughtSupplies
            << endl;





        




        cout << endl;
        cout << "\tPayroll and Maintenance" << endl;
                cout << "\t\t"
                << left << setw(20) << ""
                << right << setw(10) << "Headcount"
                << right << setw(18) << "Hourly Wage ($)"
                << right << setw(18) << "Weekly Cost ($)"
                << endl;

        cout << "\t\t"
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << endl;
        
        int headcount = (current -> cookCount + current -> cashierCount + current -> dishCount + current -> deliveryGuyCount + current -> managerCount + current -> generalManagerCount);
        double totalhourly =    current -> latestCostOfLabor =  current -> cookCount * current -> location -> cookPay +
                                current -> cashierCount * current -> location -> cashierPay +
                                current -> dishCount * current -> location -> dishPay +
                                current -> deliveryGuyCount * current -> location -> deliveryGuyPay +
                                current -> managerCount * current -> location -> managerPay +
                                current -> generalManagerCount * current -> location -> generalManagerPay;
        double totalWeeklyPayroll = totalhourly * 40;

        cout << "\t\t"
            << left << setw(20) << "Payroll"
            << right << setw(10) << to_string(headcount)
            << right << setw(18) << Utils::getMoneyString(totalhourly)
            << right << setw(18) << Utils::getMoneyString(totalWeeklyPayroll)
            << endl;
        double totalWeeklyMaintenance = current -> pizzaOvenLevel * current -> location -> pizzaOvenMaintenance + 
                                        current -> fryerLevel * current -> location -> fryerMaintenance +
                                        current -> fridgeLevel * current -> location -> fridgeMaintenance + 
                                        current -> robotLevel * current -> location -> robotMaintenance;
        cout << "\t\t"
            << left << setw(36) << "Maintenance"
            << right << setw(30) << Utils::getMoneyString(totalWeeklyMaintenance)
            << endl;

        current -> latestRevenue = (weeklyRevenueSlices + weeklyRevenue8Cut + weeklyRevenue12Cut);
        current -> latestCostOfLabor = totalWeeklyPayroll;
        current -> latestCostOfMaintenance = totalWeeklyMaintenance;

        //okay so we want to be able to hire employees and upgrade equipment
        if(current -> generalManagerCount > 0 && balance >= generalManagerHireThreshold){
            //then we want to hire if at any bottlenecks
            if(current -> notEnoughCooks){
                current -> cookCount += 1;
                current -> notEnoughCooks = false;
            }
            if(current -> notEnoughCashiers){
                current -> cashierCount += 1;
                current -> notEnoughCashiers = false;
            }
            if(current -> notEnoughDishwashers){
                current -> dishCount += 1;
                current -> notEnoughDishwashers = false;
            }

            //okay so they auto hire people, what about delivery guys? if we are profitable?
            //hire delivery guys if base demands are the bottleneck?
            //if none of the other flags and we are profitable?
            if(!current -> notEnoughCooks && !current -> notEnoughCashiers && !current -> notEnoughDishwashers){
                //okay so none of the labor flags
                //also check fryer oven, and ingredients
                if(!current -> notEnoughOvenSpace && !current -> notEnoughFryerSpace){
                    //finally check what? if we actually have ingredients? should we assume you have a manager if u have a gm
                    //okay so none of the other flags, if we are profitable then hire a delivery guy
                    if(current -> latestRevenue - (current -> latestCostOfGoodsSold + current -> latestCostOfLabor + current -> latestCostOfMaintenance)){
                        //if we are profitable, hire a delivery guy
                        if(current -> deliveryGuyCount < EmployeesBase::maxDeliveryGuys){
                            current -> deliveryGuyCount += 1;
                        }

                        //alright so none of the other flags and we are profitable, if we dont have a manager and we need supplies, hire a manager
                        if(current -> managerCount == 0 && GMsHireManagers){
                            //and we are at half supplies for any supplies
                            if(current -> DoughStock < current -> maxSupplies / 2 || 
                                current -> SauceStock < current -> maxSupplies / 2 ||
                                current -> CheeseStock < current -> maxSupplies / 2 ||
                                current -> WingsStock < current -> maxSupplies / 2){

                                    //alright so we arent missing anyone, we have enough oven space and fryer space, were profitable, 
                                    //if we are running low on supplies and dont have a manager, hire a manager
                                    current -> managerCount += 1;
                            }
                        }
                    }
                }
            }
        }

        if(current -> generalManagerCount > 0 && balance >= generalManagerEquipmentThreshold){
            //okay so should we upgrade our equipment?
            if(current -> notEnoughOvenSpace){
                //then check if we can afford it
                if(balance >= current -> location -> pizzaOvenUpgradeCost){
                    //so we need it, and we can afford it
                        balance -= current -> location -> pizzaOvenUpgradeCost;
                        current -> pizzaOvenLevel++;
                        current -> notEnoughOvenSpace = false;
                }
            }
            if(current -> notEnoughFryerSpace){
                if(balance >= current -> location -> fryerUpgradeCost){
                        balance -= current -> location -> fryerUpgradeCost;
                        current -> fryerLevel++;
                        current -> notEnoughFryerSpace = false;
                }
            }
            if(current -> notEnoughFridgeSpace){
                if(balance >= current -> location -> fridgeUpgradeCost){
                        balance -= current -> location -> fridgeUpgradeCost;
                        current -> fridgeLevel++;
                        current -> notEnoughFridgeSpace = false;
                }
            }
        }

        recalculateHeadcount();
        current -> recalcOrdersPerEmployee();



        prev = current;
        current = current -> next;
    }//END OF FOR EVERY LOCATION
    



    /////////////////////////////////////////////////////////////   Last Week Summary
    cout << endl;
    cout << "\tLast Week Summary" << endl;

    double _tWeeklyRevenue = 0;
    double _tWeeklyCostOfGoodsSold = 0;
    double _tWeeklyLabor = 0;
    double _tWeeklyMaintenance = 0;
    double _tWeeklyRent = 0;
    double _tWeeklyProfit = 0;
    double _tWeeklyTax = 0;
    double _tWeeklyEarnings = 0;


    cout << "\t\t" 
        << left  << setw(25) << "Location"
        << right << setw(15) << "Revenue ($)"
        << right << setw(23) << "Cost of Goods Sold ($)"
        << right << setw(15) << "Payroll ($)"
        << right << setw(19) << "Maintenance ($)"
        << right << setw(13) << "Rent ($)"
        << right << setw(12) << "Profit ($)"
        << right << setw(19) << "Local Taxes ($)"
        << right << setw(19) << "Earnings ($)"
        << endl;

    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    
    prev = first;
    current = first -> next;
    while(current != 0){
        double profit = current -> latestRevenue - (current -> latestCostOfGoodsSold + current -> latestCostOfLabor + current -> latestCostOfMaintenance);
        string rentS = "-";
        string taxS = "-";

        if(current -> isRented){
            double rent = current -> location -> rent;
            rentS = Utils::getALessMoneyString(rent);
            _tWeeklyRent += rent;
            profit -= rent;
        }
        double earnings = profit;
        if(profit > 0){
            taxS = Utils::getALessMoneyString(profit * current -> location -> taxRate);
            _tWeeklyTax += profit * current -> location -> taxRate;
            earnings -= profit * current -> location -> taxRate;
        }
        current -> latestEarnings = earnings;

        cout << "\t\t" 
            << left  << setw(25) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << right << setw(15) << Utils::getMoneyString(current -> latestRevenue)
            << right << setw(23) << Utils::getALessMoneyString(current -> latestCostOfGoodsSold)
            << right << setw(15) << Utils::getALessMoneyString(current -> latestCostOfLabor)
            << right << setw(19) << Utils::getALessMoneyString(current -> latestCostOfMaintenance)
            << right << setw(13) << rentS
            << right << setw(12) << Utils::getMoneyString(profit)
            << right << setw(19) << taxS
            << right << setw(19) << Utils::getMoneyString(earnings)
            << endl;
        




        _tWeeklyRevenue += current -> latestRevenue;
        _tWeeklyCostOfGoodsSold += current -> latestCostOfGoodsSold;
        _tWeeklyLabor += current -> latestCostOfLabor;
        _tWeeklyMaintenance += current -> latestCostOfMaintenance;
        _tWeeklyProfit += profit;
        prev = current;
        current = current -> next;
    }
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "\t\t" 
        << left  << setw(25) << "Totals"
        << right << setw(15) << Utils::getMoneyString(_tWeeklyRevenue)
        << right << setw(23) << Utils::getALessMoneyString(_tWeeklyCostOfGoodsSold)
        << right << setw(15) << Utils::getALessMoneyString(_tWeeklyLabor)
        << right << setw(19) << Utils::getALessMoneyString(_tWeeklyMaintenance)
        << right << setw(13) << Utils::getALessMoneyString(_tWeeklyRent)
        << right << setw(12) << Utils::getMoneyString(_tWeeklyProfit)
        << right << setw(19) << Utils::getALessMoneyString(_tWeeklyTax)
        << right << setw(19) << Utils::getMoneyString(_tWeeklyProfit - _tWeeklyTax)
        << endl;

    cout << endl;
    cout << "\t\t"
        << left  << setw(63) << Utils::getCompoundString("Net Operating Income (", Utils::getCompoundString(Utils::getYearString(weekNumber), ")"))
        << right << setw(15) << ""
        << right << setw(19) << ""
        << right << setw(13) << ""
        << right << setw(12) << Utils::getMoneyString(_tWeeklyProfit)
        << right << setw(19) << ""
        << right << setw(19) << Utils::getMoneyString(_tWeeklyProfit - _tWeeklyTax)
        << endl;


    // cout << "\t\t"
    //     << left  << setw(25) << ""
    //     << right << setw(15) << ""
    //     << right << setw(23) << ""
    //     << right << setw(15) << ""
    //     << right << setw(19) << ""
    //     << right << setw(13) << ""
    //     << right << setw(12) << ""
    //     << right << setw(19) << "Starting Balance"
    //     << right << setw(19) << Utils::getMoneyString(balance)
    //     << endl;

    cout << "\t\t"
        << left  << setw(25) << ""
        << right << setw(15) << ""
        << right << setw(25) << ""
        << right << setw(13) << ""
        << right << setw(19) << ""
        << right << setw(44) << "Earnings NOT Including Costs of Goods Sold"
        << right << setw(19) << Utils::getMoneyString(_tWeeklyProfit - _tWeeklyTax + _tWeeklyCostOfGoodsSold)
        << endl;
        
    lastWeekIncome = _tWeeklyProfit - _tWeeklyTax;
    balance += lastWeekIncome + _tWeeklyCostOfGoodsSold - managersBoughtSupplies;
    //how can we solve this? add back costs of goods sold?



    //alright so we know that the manage bought supplies so lets put a line in here for
    cout << "\t\t"
        << left  << setw(25) << ""
        << right << setw(15) << ""
        << right << setw(25) << ""
        << right << setw(13) << ""
        << right << setw(19) << ""
        << right << setw(44) << "Manager Bought Supplies"
        << right << setw(19) << Utils::getMoneyString(managersBoughtSupplies)
        << endl;

    cout << "\t\t"
        << left  << setw(25) << ""
        << right << setw(15) << ""
        << right << setw(25) << ""
        << right << setw(13) << ""
        << right << setw(19) << ""
        << right << setw(44) << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << right << setw(19) << "~~~~~~~~~~~~~~~~~~~"
        << endl;

    cout << "\t\t"
        << left  << setw(25) << ""
        << right << setw(15) << ""
        << right << setw(25) << ""
        << right << setw(13) << ""
        << right << setw(19) << ""
        << right << setw(13) << ""
        << right << setw(12) << ""
        << right << setw(19) << "Change in Balance"
        << right << setw(19) << Utils::getMoneyString(lastWeekIncome + _tWeeklyCostOfGoodsSold - managersBoughtSupplies)
        << endl;

    cout << "\t\t"
        << left  << setw(25) << ""
        << right << setw(15) << ""
        << right << setw(23) << ""
        << right << setw(15) << ""
        << right << setw(19) << ""
        << right << setw(13) << ""
        << right << setw(12) << ""
        << right << setw(19) << "Ending Balance"
        << right << setw(19) << Utils::getMoneyString(balance)
        << endl;



    cout << endl;
    current = lastCurrent;
    prev = lastPrevious;
    system("pause");
    system("cls");
    if(balance < 0){
        cout << "YOU LOST BECAUSE YOUR BALANCE WAS NEGATIVE" << endl;
    }
}

void PizzaCompany::ActionMenu(){
    printHUD();
	cout << endl << "\t       Action Menu" << endl;
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
	cout << "\t(1)  View Locations" << endl;
    cout << "\t(2)  Proceed to next week." << endl;
    if(numberOfLocations >= MISC::minLocationsForCorporate || numberOfEmployees >= MISC::minEmployeesForCorporate){
        //if we have atleast 4 locations, then we can visit or open the corporate office
        if(corporateLoc == nullptr)
            cout << "\t(3)  (Coming Soon) Open a corporate office!" << endl;
        else
            cout << "\t(3)  Visit our corporate office." << endl;
    }

    cout << "\t(4)  Headcount Report" << endl;
    cout << "\t(5)  Cost of Goods Sold Report" << endl;
    cout << "\t(6)  Maintenance Report" << endl;
    cout << "\t(7)  Inventory Report" << endl;
	cout << "\t(8)  EXIT " << endl;
    int selection =Utils::getPlayerChoice("\t", "\tEnter your selection: ");

    //so now we are sure they entered a number, doesnt mean its the right number
    switch (selection){
    case 1:
        GAMESTATE = 2;
        break;
    case 2:
        lastCurrent = current;
        lastPrevious = prev;
        toNextWeek(); //this leaves the game state the same but we do a week
        break;
    case 3:
        // if(numberOfLocations >= MISC::minLocationsForCorporate || numberOfEmployees >= MISC::minEmployeesForCorporate){
        //     if(corporateLoc == nullptr)
        //         //then we want to open a corporate office screen
        //         GAMESTATE = 11;
        //     else
        //         //then we want to visit the corporate office
        //         GAMESTATE = 12;
        // }
        break;
    case 4:
        GAMESTATE = 8;
        break;
    case 5:
        GAMESTATE = 9;
        break;
    case 6:
        GAMESTATE = 10;
        break;
    case 7:
        GAMESTATE = 20;
        break;
    case 8:
        GAMESTATE = 0;
        break;
    
    default:
        //this handles any integer we are expecting, doesnt change the game state, so this page just gets repeated.
        break;
    }


	cout << endl;
	
}

void PizzaCompany::printHUD(){
    /*
        This functions prints the HUD at the top of every page so the user can see what is going on
    
    */

    cout << left << setw(10) << ""
        << left << setw(3) << " | " 
        << left << setw(10) << "Balance"
        << left << setw(20) << Utils::getMoneyString(balance)
        << left << setw(12) << "Last Week"
        << left << setw(20) << Utils::getMoneyString(lastWeekIncome)
        << left << setw(3) << " | "
        << left << setw(17) << Utils::getCompoundString("CEO ", CEO)
        << left << setw(17) << Utils::getCompoundString("Employees ", to_string(numberOfEmployees))
        << left << setw(12) << Utils::getCompoundString(to_string(numberOfLocations), " Locations ")
        << left << setw(3) << " | "
        << left << setw(26) << Utils::getYearString(weekNumber)
        << left << setw(3) << " | "
        << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << endl;
}


void PizzaCompany::viewLocations(){
    printHUD();
    
    /*
        This function is used when we want to view all of our stores. Now how we view them depends on if we are corporate or not. If we are not
        corporate, then we list all of our locations (all in North America). If we are corporate however, we want to list all of the regions with
        atleast 1 location
    
    
    */

    GAMESTATE = 21;
    return;
    if(corporateLoc == nullptr){
        //then we arent corporate, print just america
        ownedLocationsInRegion("North America");
    }else{
        //then we are corporate, print a list of regions
        GAMESTATE = 21;  //The regions GameState
    }

    

}

void PizzaCompany::viewRegions(){
    /*
        This function is the screen showing all of the owned regions, and accepting input to select them. We are in GAMESTATE 21
    */
    printHUD();
    cout << "\t\t"
        << left << setw(4) << ""
        << left << setw(28) << "Region"
        << left << setw(24) << "Number of Locations"
        << left << setw(30) << "Director"
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    //now we loop through all of them
    for(int i = 0; i < Region::regionsCount; i++){
        string d = "(" + to_string(i + 1) + ")";
        cout << "\t\t"
            << left << setw(4) << d
            << left << setw(28) << Region::regions[i].region
            << left << setw(24) << Region::regions[i].locations
            << left << setw(30) << Region::regions[i].director
            << endl;
    }
    cout << "\n";
    cout << "\t\t(0) Back\n";
    cout << "\t\t(" << (Region::regionsCount + 1) << ") Open a new Location!\n";

    //alright so we listed all of our regions, their director, and number of locations. Next we want to accept input
    int selection = Utils::getPlayerChoice("\t\t", "\t\tPlease enter your selection: ");

    if(selection == 0){
        GAMESTATE = 1; //Go back to homescreen
    }else if(selection == (Region::regionsCount + 1)){
        GAMESTATE = 5;
    }else if(0 < selection && selection <= Region::regionsCount){
        //lets map i to GameState
        //worst written program ive ever seen lmao
        switch (selection){
        case 1: GAMESTATE = 22; break;
        case 2: GAMESTATE = 23; break;
        case 3: GAMESTATE = 24; break;
        case 4: GAMESTATE = 25; break;
        case 5: GAMESTATE = 26; break;
        case 6: GAMESTATE = 27; break;
        case 7: GAMESTATE = 28; break;
        case 8: GAMESTATE = 29; break;
        case 9: GAMESTATE = 30; break;
        
        default:
            GAMESTATE = 1;
            break;
        }
    }




}
void PizzaCompany::ownedLocationsInRegion(string region){
    system("cls");
    printHUD();
    /*
        this function is the screen showing all of the owned locations in a specific region.
        If we are corporate that we see a list of regions, where we pick from the list.
    */
    prev = first;
    current = first -> next;

    cout << "\t\t" 
        << left << setw(38) << "Location"
        << right << setw(50) << ""
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    while(current != 0){
        //cout << "CHECKING " << current -> location -> region << " ==? " << region;
        if(current -> location -> region != region){
            //if the current location is not in the region, skip it
            prev = current;
            current = current -> next;
            continue;
        }
        string flags = "";
        if(current -> notEnoughCooks)       flags += "Not Enough Cooks, ";
        if(current -> notEnoughCashiers)    flags += "Not Enough Cashiers, ";
        if(current -> notEnoughDishwashers) flags += "Not Enough Dishwashers, ";

        if(current -> notEnoughOvenSpace)   flags += "Not Enough Oven Space, ";
        if(current -> notEnoughFryerSpace)  flags += "Not Enough Fryer Space, ";
        if(current -> notEnoughFridgeSpace) flags += "Not Enough Fridge Space, ";

        if(current -> notEnoughDough || current -> notEnoughSauce || current -> notEnoughCheese || current -> notEnoughWings) flags += "Missing ";
        if(current -> notEnoughDough)       flags += "Dough, ";
        if(current -> notEnoughSauce)       flags += "Sauce, ";
        if(current -> notEnoughCheese)      flags += "Cheese, ";
        if(current -> notEnoughWings)       flags += "Wings, ";

        cout << "\t\t" 
            << left << setw(38) << Utils::getCompoundString("(" + to_string(current -> location -> selectNum) + ") ",Utils::getLocationString(current -> location -> city, current -> location -> state)) 
            << left << setw(50) << flags
            << endl;

        prev = current;
        current = current -> next;

    }

    /*
        okay so we just printed all of the owned locations in a specific region, next we want to accept input
        and transfer the user to the particular screen that they desire. Handling input in a decent way.

        The back button is interesting, if we are non corporate, we want to go back to the homescreen. If
        we are corporate however, we want to be transfered back to the regions screen
    */
    int buildLoc = startExtraMenuOptionsSelect;
    int nextWeekBTN = startExtraMenuOptionsSelect + 1;
    int backSel = 0;

    cout << endl;
    cout << "\t\t(" << buildLoc << ") " << "Open a new location!" << endl;
    cout << "\t\t(" << nextWeekBTN << ") " << "Proceed to Next Week!" << endl;
    if(corporateLoc == nullptr) cout << "\t\t(" << backSel << ") " << "Back" << endl;
    else cout << "\t\t(" << backSel << ") " << "Back To Regions" << endl;

    int selection = Utils::getPlayerChoice("\t\t", "\t\tEnter your selection: ");



    if (selection == buildLoc){
        //okay so they want to build a new location and we transfer them to that screen
        //however, we want the region to be passed through so we only render a specific idea.

        //lets just do gamestates
        if(region == "North America")   GAMESTATE = 31;
        else if(region == "South America")   GAMESTATE = 32;
        else if(region == "Europe")          GAMESTATE = 33;
        else if(region == "Africa")          GAMESTATE = 34;
        else if(region == "Middle East")     GAMESTATE = 35;
        else if(region == "Asia")            GAMESTATE = 36;
        else if(region == "Japan")           GAMESTATE = 37;
        else if(region == "Oceania")         GAMESTATE = 38;
        else if(region == "Caribbean")       GAMESTATE = 39;
        else GAMESTATE = 5;

    }else if(selection == backSel){
        //if we arent corporate, go to home screen, if we are, go to regions
        if(corporateLoc == nullptr) GAMESTATE = 1;
        else GAMESTATE = 2;
    }else if(selection == nextWeekBTN){
        lastCurrent = current;
        lastPrevious = prev;
        toNextWeek();
    }else if(0 < selection && selection < startExtraMenuOptionsSelect){
        //then we selected a number that is a location, we want to check that we own it, and its in the current region
        prev = first;
        current = first -> next;
        while(current != nullptr && current -> location -> selectNum != selection){
            prev = current;
            current = current -> next;
        }
        
        if(current == nullptr || current == 0) return;
        //if its not in this region, exit
        if(current -> location -> region != region) return;
        GAMESTATE = 6;
    }



}


void PizzaCompany::buildAtNewLocation(string region = "all"){
    printHUD();
    /* 
        This function starts the diologue for when the user building a new shop at a
        location, they pick to rent or buy, etc...

        first thing we do is loop through all of the locations, for each location, check if their is a shop that is associated with it
        if not, list it, and its build or rent price
        user selects one, we ask if they want to buy the building or rent?
        building pays build price upfront, otherwise pay weekly rent, you can always buy outright later
    */

    cout << "\t\t" 
        << left << setw(8) << ""
        << left << setw(18) << "Region"
        << left << setw(23) << "State/Country"
        << left << setw(23) << "City"
        << left << setw(16) << "Wealth"
        << left << setw(25) << "Estimated Demand"
        << left << setw(16) << "Rent"
        << right << setw(8) << "Tax Rate"
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    prevLoc = firstLoc;
    currentLoc = firstLoc -> next;
    while(currentLoc != 0){

        if(currentLoc -> alreadyOwned){
            prevLoc = currentLoc;
            currentLoc = currentLoc -> next;
            continue;
        }

        //if we are non corporate, and its outside america, dont show it
        // if(corporateLoc == nullptr && currentLoc -> region != "North America"){
        //     prevLoc = currentLoc;
        //     currentLoc = currentLoc -> next;
        //     continue;
        // }
        
        //next we want to check if we are corporate, then we are looking only for a specific section
        //if(corporateLoc != nullptr && region != "all"){
        if(region != "all"){
            //so we are corporate and we are specifying a region, check the region
            if(currentLoc -> region != region){
                /*
                    So if we are corporate and we are specifying a region
                    //and the region doesnt match the current, dont list the current

                */
                prevLoc = currentLoc;
                currentLoc = currentLoc -> next;
                continue;
            }
        }


        string wealthString = "";
        if(currentLoc -> wealth >= 87) wealthString = "Wealthy";
        else if(currentLoc -> wealth >= 50) wealthString = "Middling";
        else if(currentLoc -> wealth >= 14) wealthString = "Struggling";
        else wealthString = "Poor";
        
        int demand = (currentLoc -> baseSliceDemand + currentLoc -> base8CutDemand + currentLoc -> base12CutComboDemand);


        cout << "\t\t" 
            << left << setw(8) << Utils::getCompoundString(Utils::getCompoundString("(", to_string(currentLoc -> selectNum)), ")")
            << left << setw(18) << currentLoc -> region
            << left << setw(23) << currentLoc -> state
            << left << setw(23) << currentLoc -> city
            // << left << setw(40) << Utils::getLocationString(currentLoc -> city, currentLoc -> state)
            << left << setw(16) << wealthString
            << left << setw(25) << demand
            << left << setw(16) << Utils::getMoneyString(currentLoc -> rent)
            << right << setw(8) << Utils::getCompoundString(Utils::getDigitDecimal(currentLoc -> taxRate * 100), "%")
            << endl;

        prevLoc = currentLoc;
        currentLoc = currentLoc -> next;
    }

    cout << endl;
    cout << "\t\t" 
        << left << setw(8) << "(0)"
        << left << setw(40) << "Back To Owned Locations"
        << left << setw(16) << ""
        << left << setw(25) << ""
        << left << setw(16) << ""
        << right << setw(8) << ""
        << endl;

    cout << endl << endl;
    int selection = Utils::getPlayerChoice("\t\t", "\t\tPlease enter the Desired Location: ");

    if(selection == 0){
        //then we want to go back
        GAMESTATE = 2;
    }else if (0 < selection && selection < startExtraMenuOptionsSelect){
        /*
            alright so they selected a number that is a location
            check and see if they location is already bought or
            if we are non corporate and its outside the Americas
        */
        
        prevLoc = firstLoc;
        currentLoc = firstLoc -> next;
        while(currentLoc -> selectNum != selection){
            prevLoc = currentLoc;
            currentLoc = currentLoc -> next;
        }
        
        if(currentLoc -> alreadyOwned){
            cout << endl << "\t\tWe Already own that location!" << endl;
            system("pause");
            return;
        }

        // if(corporateLoc == nullptr && currentLoc -> region != "North America"){
        //     //they tried to access a location outside america before corporate
        //     return;
        // }
        //currentLoc was the location typed in, check if its in the region, if not skip, reloading the page
        //corporateLoc != nullptr && 
        if(region != "all"){
            if(currentLoc -> region != region){
                return;
            }
        }

        //so we dont already own it, and we can buy it, its currently stored in currentLoc, lets switch the gamestate over
        GAMESTATE = 7;
    }
}
void PizzaCompany::ownedLocationScreen(){
    printHUD();
    cout << "\t~~~~~~~~~~~~~~~~~~~~~~~ " << shopname << " in " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " ~~~~~~~~~~~~~~~~~~~~~ " << endl;
    cout << endl << endl;
    cout << "\t\t" 
        << left << setw(30) << "Available Menu Options"
        << right << setw(15) << "Price"
        << right << setw(18) << "Estimated Demand"
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    
    string deliver = "";
    if(current -> deliveryGuyCount > 0) deliver = "(+" + to_string(current -> deliveryGuyCount * EmployeesBase::extraOrdersPerDeliveryGuy) + ")";
    cout << "\t\t"
        << left << setw(30) << "Single Slice"
        << right << setw(15) << Utils::getMoneyString(current -> slicePrice)
        << right << setw(18) << current -> location -> baseSliceDemand << deliver
        << endl;
    cout << "\t\t"
        << left << setw(30) << "8 Cut Pizza"
        << right << setw(15) << Utils::getMoneyString(current -> Cut8Price)
        << right << setw(18) << current -> location -> base8CutDemand << deliver
        << endl;
    cout << "\t\t"
        << left << setw(30) << "12 Cut Pizza Combo with Wings"
        << right << setw(15) << Utils::getMoneyString(current -> Cut12ComboPrice)
        << right << setw(18) << current -> location -> base12CutComboDemand << deliver
        << endl;

    cout << endl << endl;
    cout << "\t" << Utils::wrapText(current -> location -> blurb) << endl;

    //alright now we want to print the wealth, and what are rent is
    string wealthSen = "";
    if(current -> location -> wealth >= 87){
        wealthSen = " is considered Wealthy. It's people will care little about the price of our pizzas.";
    }else if(current -> location -> wealth >= 50){
        wealthSen = " is Middling. Prices matter in this town, but a craving for pizza will make anyone part with their cash.";
    }else if(current -> location -> wealth >= 14){
        wealthSen = " is Struggling. The locals will not buy our pizza if it's outside of their budget.";
    }else {
        wealthSen = " is very poor. They will only consider our pizza if it's far below base price.";
    }

    string buildingCosts = " We could buy a shop here for " +Utils::getMoneyString(current -> location -> buildingCost) + ", or we could rent for " +Utils::getMoneyString(current -> location -> rent);
    string taxCosts = " Obviously, we have to pay the local governor for some reason. The local tax rate here is " + Utils::get2DigitDecimal(current -> location -> taxRate * 100) + "% of our operating income.";
    string laborCosts = " I estimate cook pay to cost " +Utils::getMoneyString(current -> location -> cookPay) + " per employee per hour. And as the laws of physics still apply,";
    string ingredients = " the employees also need ingredients to make the pizzas. I made a table for you.";
    string maintenance = "Rent and Ingredients are the only costs of doing business. We also have to pay the maintenance costs for our equipment.";
    
    cout << endl;
    cout << "\t" << Utils::wrapText(Utils::getCompoundString(Utils::getLocationString(current -> location -> city, current -> location -> state),Utils::getCompoundString(wealthSen,Utils::getCompoundString(buildingCosts,Utils::getCompoundString(Utils::getCompoundString(taxCosts, laborCosts), ingredients))))) << endl;
    cout << endl;

    string sup = "Local Cost";
    if(current -> generalManagerCount > 0) sup = "Local Cost (-" + Utils::getDigitDecimal(EmployeesBase::GM_COGSavings * 100) +"%)";

    cout << "\t\t"
        << left << setw(20) << "Ingredient"
        << left << setw(20) << sup
        << left << setw(12) << "In Stock"
        << left << setw(17) << "Max Supplies"
        << endl;
    cout << "\t\t"
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << endl;
        
    cout << "\t\t"
        << left << setw(20) << "Dough"
        << left << setw(20) << Utils::getMoneyString(current -> location -> doughPrice * (1 - (current -> generalManagerCount * EmployeesBase::GM_COGSavings)))
        << left << setw(12) << Utils::get2DigitDecimal(current -> DoughStock)
        << left << setw(17) << Utils::get2DigitDecimal(current -> maxSupplies)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Sauce"
        << left << setw(20) << Utils::getMoneyString(current -> location -> saucePrice * (1 - (current -> generalManagerCount * EmployeesBase::GM_COGSavings)))
        << left << setw(12) << Utils::get2DigitDecimal(current -> SauceStock)
        << left << setw(17) << Utils::get2DigitDecimal(current -> maxSupplies)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Cheese"
        << left << setw(20) << Utils::getMoneyString(current -> location -> cheesePrice * (1 - (current -> generalManagerCount * EmployeesBase::GM_COGSavings)))
        << left << setw(12) << Utils::get2DigitDecimal(current -> CheeseStock)
        << left << setw(17) << Utils::get2DigitDecimal(current -> maxSupplies)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Wings"
        << left << setw(20) << Utils::getMoneyString(current -> location -> wingsPrice * (1 - (current -> generalManagerCount * EmployeesBase::GM_COGSavings)))
        << left << setw(12) << Utils::get2DigitDecimal(current -> WingsStock)
        << left << setw(17) << Utils::get2DigitDecimal(current -> maxSupplies)
        << endl;

    cout << endl;
    
    if(current -> isRented){
        cout << "\tWe are currently renting this location for $" << current -> location -> rent << " per week." << endl;
    }else{
        cout << "\tWe own this location." << endl;
    }

    cout << endl;

    cout << "\t\tPayroll" << endl << endl;
            cout << "\t\t"
            << left << setw(20) << "Job"
            << right << setw(10) << "Headcount"
            << right << setw(18) << "Hourly Wage ($)"
            << right << setw(18) << "Weekly Cost ($)"
            << right << setw(17) << "Maximum Orders"
            << endl;

    cout << "\t\t"
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    << endl;

    cout << "\t\t"
        << left << setw(20) << "Cook"
        << right << setw(10) << to_string(current -> cookCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> cookPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> cookPay * 40)
        << right << setw(17) << to_string(current -> cookCount * current -> ordersPerCook)
        << endl;

    cout << "\t\t"
        << left << setw(20) << "Cashier"
        << right << setw(10) << to_string(current -> cashierCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> cashierPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> cashierPay * 40)
        << right << setw(17) << to_string(current -> cashierCount * current -> ordersPerCashier)
        << endl;

    cout << "\t\t"
        << left << setw(20) << "Dishwasher"
        << right << setw(10) << to_string(current -> dishCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> dishPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> dishPay * 40)
        << right << setw(17) << to_string(current -> dishCount * current -> ordersPerDishwasher)
        << endl;

    cout << "\t\t"
        << left << setw(20) << "Delivery Driver"
        << right << setw(10) << to_string(current -> deliveryGuyCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> deliveryGuyPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> deliveryGuyPay * 40)
        << right << setw(17) << ""
        << endl;

    cout << "\t\t"
        << left << setw(20) << "Manager"
        << right << setw(10) << to_string(current -> managerCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> managerPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> managerPay * 40)
        << right << setw(17) << ""
        << endl;

    cout << "\t\t"
        << left << setw(20) << "General Manager"
        << right << setw(10) << to_string(current -> generalManagerCount)
        << right << setw(18) << Utils::getMoneyString(current -> location -> generalManagerPay)
        << right << setw(18) << Utils::getMoneyString(1 * current -> location -> generalManagerPay * 40)
        << right << setw(17) << ""
        << endl;

    cout << "\t\t"
    << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    << endl;
    
    int headcount = (current -> cookCount + current -> cashierCount + current -> dishCount + current -> deliveryGuyCount + current -> managerCount + current -> generalManagerCount);
    double totalhourly =    current -> latestCostOfLabor =  current -> cookCount * current -> location -> cookPay +
                            current -> cashierCount * current -> location -> cashierPay +
                            current -> dishCount * current -> location -> dishPay +
                            current -> deliveryGuyCount * current -> location -> deliveryGuyPay +
                            current -> managerCount * current -> location -> managerPay +
                            current -> generalManagerCount * current -> location -> generalManagerPay;
    double totalWeekly = totalhourly * 40;
    int maxOrders = min(min(current -> cookCount * current -> ordersPerCook, current -> cashierCount * current -> ordersPerCashier), current -> dishCount * current -> ordersPerDishwasher);

    cout << "\t\t"
        << left << setw(20) << "Total"
        << right << setw(10) << to_string(headcount)
        << right << setw(18) << Utils::getMoneyString(totalhourly)
        << right << setw(18) << Utils::getMoneyString(totalWeekly)
        << right << setw(17) << to_string(maxOrders)
        << endl;

    cout << endl;
    cout << Utils::wrapText(maintenance) << endl;
    cout << endl;

    cout << "\tEquipment" << endl << endl;
    //for each piece of equipment, if the level is 0 dont display it.
    if(current -> pizzaOvenLevel >= 1){
        cout << "\tPizza Oven " << current -> pizzaOvenLevel << endl
            << "\t\tCosts " + Utils::getMoneyString(current -> location -> pizzaOvenMaintenance * current -> pizzaOvenLevel) + " to maintain" << endl
            << "\t\t+" + to_string(EquipmentBase::maxOrdersPerOvenLevel * current -> pizzaOvenLevel) + " max pizzas per week" << endl
            << "\t\t+" + to_string(EquipmentBase::maxEmployeeOrdersPerOvenLevel * current -> pizzaOvenLevel) + " max pizzas per employee per week" << endl
            << "\t\t+" + to_string(EquipmentBase::maxRobotOrdersPerOvenLevel * current -> pizzaOvenLevel) + " max pizzas per robot per week" << endl << endl;
    }
    if(current -> fryerLevel >= 1){
        cout << "\tFryer " << current -> fryerLevel << endl
            << "\t\tCosts " + Utils::getMoneyString(current -> location -> fryerMaintenance * current -> fryerLevel) + " to maintain" << endl
            << "\t\t+" + to_string(EquipmentBase::maxWingsPerFryerLevel * current -> fryerLevel) + " max wings per week" << endl << endl;
    }
    if(current -> fridgeLevel >= 1){
        cout << "\tFridge " << current -> fridgeLevel << endl
            << "\t\tCosts " + Utils::getMoneyString(current -> location -> fridgeMaintenance * current -> fridgeLevel) + " to maintain" << endl
            << "\t\t+" + Utils::get2DigitDecimal(EquipmentBase::maxSuppliesPerFridgeLevel * current -> fridgeLevel) + " max supplies in stockpile\n\n";
    }
    if(current -> robotLevel >= 1){
        cout << "\tRobot " << current -> robotLevel << endl
            << "\t\tCosts " + Utils::getMoneyString(current -> location -> robotMaintenance * current -> robotLevel) + " to maintain" << endl
            << "\t\t+" + to_string(EquipmentBase::maxOrdersPerRobotLevel * current -> robotLevel) + " max pizzas per week in labor\n\n";
    }

    cout << endl;
    //alright lets have a little paragraph about what the maximum number of orders we can handle is.
    //so what do we want to mention? oven space, fryer space, labor, then ingredients

    cout << "\t\tIt's important to keep in mind that our shop can only make so many pizzas based on " << endl
        << "\tour equipment and available labor. Based on the current level, I estimate that our Pizza" << endl 
        << "\tOven can make " + to_string(current -> pizzaOvenLevel * EquipmentBase::maxOrdersPerOvenLevel) + " Orders per week total, across all three of our menu options. For our 12 Cut" << endl
        << "\tPizza and Wings Combo, Our Fryer can make about " + to_string(current -> fryerLevel * EquipmentBase::maxWingsPerFryerLevel) + " orders of wings per week based on its size." << endl
        << "\tOf course, this is assuming we have the available labor, of which I estimate our " + to_string(current -> cookCount) + " cook(s) " << endl
        << "\tcan make upto " + to_string(current -> cookCount * current -> ordersPerCook) + " orders per week. To top it all of, we need to be sure we have the necessary " << endl
        << "\tingredients to make all of these pizzas.";




    cout << endl;
    cout << endl;

    //alright now we can print our menu options
    cout << "\t" << "Menu" << endl;
    cout << "\t" << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "\t(1) Adjust Local Prices" << endl;
    cout << "\t(2) Buy Supplies" << endl;
    cout << "\t(3) Manage Employees" << endl;
    cout << "\t(5) Upgrade Equipment" << endl;
    if(current -> isRented)
    cout << "\t(6) Buy the Building (" << Utils::getMoneyString(current -> location -> buildingCost) << ")" << endl;

    if(current -> isRented) cout << "\t(7) Close Location" << endl;
    else cout << "\t(7) Close Location (Sell Building)" << endl;

    cout << "\t(8) Proceed to Next Week" << endl;
    cout << "\t(0) Back To Owned Locations" << endl;

    //cout << current -> location -> variance << endl;

    //printMap();
    //printSquares();
    int selection =Utils::getPlayerChoice("\t", "\tPlease enter your selection: ");

    cout << endl;
    //ill fix this later, this is disgusting
    int employees = (current -> cookCount + current -> cashierCount + current -> dishCount + current -> deliveryGuyCount + current -> managerCount + current -> generalManagerCount);
    string buildingPara = "\tYou currently pay " + Utils::getMoneyString(current -> location -> rent) + " in rent per week to the landlord. They are offering you the opportunity to purchase the building outright for " +Utils::getMoneyString(current -> location -> buildingCost) + ". Do you wish to proceed? (Y/N)";

    string upgradeEquipment =   "\tThe Pizza Oven in " +Utils::getLocationString(current -> location -> city, current -> location -> state) + " is the heart of the restaurant, turning dough, sauce and cheese into profit.\n";
    upgradeEquipment +=         "\t\t Costs " +Utils::getMoneyString(current -> location -> pizzaOvenUpgradeCost) + " to upgrade and " + Utils::getMoneyString(current -> location -> pizzaOvenMaintenance) + " to maintain\n";
    upgradeEquipment +=         "\t\t+" + to_string(EquipmentBase::maxOrdersPerOvenLevel) + " max pizzas per week\n";
    upgradeEquipment +=         "\t\t+" + to_string(EquipmentBase::maxEmployeeOrdersPerOvenLevel) + " max pizzas per employee per week\n";
    upgradeEquipment +=         "\t\t+" + to_string(EquipmentBase::maxRobotOrdersPerOvenLevel) + " max pizzas per robot per week\n\n";
    upgradeEquipment +=         "\tThe Fryer in " +Utils::getLocationString(current -> location -> city, current -> location -> state) + " keeps customers happy with crispy sides-perfect for boosting combo sales and overall revenue.\n";
    upgradeEquipment +=         "\t\t Costs " +Utils::getMoneyString(current -> location -> fryerUpgradeCost) + " to upgrade and " + Utils::getMoneyString(current -> location -> fryerMaintenance) + " to maintain\n";
    upgradeEquipment +=         "\t\t+" + to_string(EquipmentBase::maxWingsPerFryerLevel) + " max wings per week\n\n";
    upgradeEquipment +=         "\tThe Fridge in " +Utils::getLocationString(current -> location -> city, current -> location -> state) + " safeguards our ingredients, keeping everything fresh so your pizzas taste good and don't poison anyone.\n";
    upgradeEquipment +=         "\t\t Costs " +Utils::getMoneyString(current -> location -> fridgeUpgradeCost) + " to upgrade and " + Utils::getMoneyString(current -> location -> fridgeMaintenance) + " to maintain\n";
    upgradeEquipment +=         "\t\t+" + Utils::get2DigitDecimal(EquipmentBase::maxSuppliesPerFridgeLevel) + " max supplies in stockpile\n\n";
    upgradeEquipment +=         "\tThe Robots in " +Utils::getLocationString(current -> location -> city, current -> location -> state) + " is the path to permanent profitability, capable of everything an employee can do, just not as well, and not nearly as expensive.\n";
    upgradeEquipment +=         "\t\t Costs " +Utils::getMoneyString(current -> location -> robotUpgradeCost) + " to upgrade and " + Utils::getMoneyString(current -> location -> robotMaintenance) + " to maintain\n";
    upgradeEquipment +=         "\t\t+" + to_string(EquipmentBase::maxOrdersPerRobotLevel) + " max pizzas per week in labor\n\n";

    switch (selection){
        case 1: {
            //okay so the user wants to change the local prices.
            string p = "\tThe current price of a Slice of Pizza is " +Utils::getMoneyString(current -> slicePrice) + ", and you want to change it to $";
            double newPrice = Utils::sanitizeDoubleInput("\t", p);
            current -> slicePrice = max(min(newPrice, MenuItems::SliceBasePrice * 10), 0.0);


            p = "\tThe current price of an 8 Cut Pizza is " +Utils::getMoneyString(current -> Cut8Price) + ", and you want to change it to $";
            newPrice = Utils::sanitizeDoubleInput("\t", p);
            current -> Cut8Price = max(min(newPrice, MenuItems::Cut8BasePrice * 10), 0.0);

            p = "\tThe current price of a 12 Cut Pizza and Wings Combo is " +Utils::getMoneyString(current -> Cut12ComboPrice) + ", and you want to change it to $";
            newPrice = Utils::sanitizeDoubleInput("\t", p);
            current -> Cut12ComboPrice = max(min(newPrice, MenuItems::Cut12BasePrice * 10), 0.0);
            break; //takes us back to the main menu
        }
        case 2: {
            //total cost of filling supplies
            double costOfFilling =  (current -> maxSupplies - current -> DoughStock) * (current -> location -> doughPrice) +
                                    (current -> maxSupplies - current -> SauceStock) * (current -> location -> saucePrice) +
                                    (current -> maxSupplies - current -> CheeseStock) * (current -> location -> cheesePrice) +
                                    (current -> maxSupplies - current -> WingsStock) * (current -> location -> wingsPrice);
            if(current -> generalManagerCount > 0) costOfFilling *= (1 - EmployeesBase::GM_COGSavings);

            string supplyPara = "\tBecause " + current -> location -> city + " has a level " + to_string(current -> fridgeLevel) + " Fridge, we can store up to " + Utils::get2DigitDecimal(current -> maxSupplies) + " of each of our ingredients. Should I buy as many supplies as we can hold for a total of " +Utils::getMoneyString(costOfFilling) + ", or do you want to choose the amount manually? Type Y for maximum, N for a specific number.";
            
            //okay so the user wants to be able to buy supplies. they need to know how much they have, and the local prices
            cout << endl;
            cout << Utils::wrapText(supplyPara) << endl;
            cout << "\t";
            char in;
            cin >> in;

            if(in == 'Y' || in == 'y'){
                //then buy max
                balance -= costOfFilling;
                current -> DoughStock = current -> maxSupplies;
                current -> SauceStock = current -> maxSupplies;
                current -> CheeseStock = current -> maxSupplies;
                current -> WingsStock = current -> maxSupplies;
                current -> weekSuppliesBoughtLast = weekNumber;// We bought supplies this week
            }else{
                //i dont love this but it is what it is
                double doughcost = current -> location -> doughPrice;
                double saucecost = current -> location -> saucePrice;
                double cheesecost = current -> location -> cheesePrice;
                double wingscost = current -> location -> wingsPrice;
                if(current -> generalManagerCount > 0){
                    doughcost *= (1 - EmployeesBase::GM_COGSavings);
                    saucecost *= (1 - EmployeesBase::GM_COGSavings);
                    cheesecost *= (1 - EmployeesBase::GM_COGSavings);
                    wingscost *= (1 - EmployeesBase::GM_COGSavings);
                }

                cout << "\tCurrently, dough costs " << Utils::getMoneyString(doughcost) << " per unit, and the stock pile is sitting at " 
                << Utils::get2DigitDecimal(current -> maxSupplies - current -> DoughStock) + " below full, or " << Utils::get2DigitDecimal(current -> DoughStock) 
                << " of " << Utils::get2DigitDecimal(current -> maxSupplies) << "." << endl;
                cout << "\tHow much would you like to buy? ";

                double input;
                cin >> input;

                if(input <= current -> maxSupplies - current -> DoughStock){
                    //then we can fit this amount
                    if(balance > input * doughcost){
                        //then we have enough money
                        balance -= input * doughcost;
                        current -> DoughStock += input;
                    }else {cout << "\tYou dont have enough money!" << endl;}
                }else {cout << "\tYou dont have enough room in the fridge!" << endl;}

                cout << "\tCurrently, sauce costs " << Utils::getMoneyString(saucecost) << " per unit, and the stock pile is sitting at " 
                << Utils::get2DigitDecimal(current -> maxSupplies - current -> SauceStock) + " below full, or " << Utils::get2DigitDecimal(current -> SauceStock) 
                << " of " << Utils::get2DigitDecimal(current -> maxSupplies) << "." << endl;
                cout << "\tHow much would you like to buy? ";
                cin >> input;
                if(input <= current -> maxSupplies - current -> SauceStock){
                    //then we can fit this amount
                    if(balance > input * saucecost){
                        //then we have enough money
                        balance -= input * saucecost;
                        current -> SauceStock += input;
                    }else {cout << "\tYou dont have enough money!" << endl;}
                }else {cout << "\tYou dont have enough room in the fridge!" << endl;}

                cout << "\tCurrently, cheese costs " << Utils::getMoneyString(cheesecost) << " per unit, and the stock pile is sitting at " 
                << Utils::get2DigitDecimal(current -> maxSupplies - current -> CheeseStock) + " below full, or " << Utils::get2DigitDecimal(current -> CheeseStock) 
                << " of " << Utils::get2DigitDecimal(current -> maxSupplies) << "." << endl;
                cout << "\tHow much would you like to buy? ";
                cin >> input;
                if(input <= current -> maxSupplies - current -> CheeseStock){
                    //then we can fit this amount
                    if(balance > input * cheesecost){
                        //then we have enough money
                        balance -= input * cheesecost;
                        current -> CheeseStock += input;
                    }else {cout << "\tYou dont have enough money!" << endl;}
                }else {cout << "\tYou dont have enough room in the fridge!" << endl;}

                cout << "\tCurrently, wings costs " << Utils::getMoneyString(wingscost) << " per unit, and the stock pile is sitting at " 
                << Utils::get2DigitDecimal(current -> maxSupplies - current -> WingsStock) + " below full, or " << Utils::get2DigitDecimal(current -> WingsStock) 
                << " of " << Utils::get2DigitDecimal(current -> maxSupplies) << "." << endl;
                cout << "\tHow much would you like to buy? ";
                cin >> input;
                if(input <= current -> maxSupplies - current -> WingsStock){
                    //then we can fit this amount
                    if(balance > input * wingscost){
                        //then we have enough money
                        balance -= input * wingscost;
                        current -> WingsStock += input;
                    }else {cout << "\tYou dont have enough money!" << endl;}
                }else {cout << "\tYou dont have enough room in the fridge!" << endl;}
                //alright so now we can buy supplies
            }
            break;
        }
        case 3: {
            system("cls");
            printHUD();
            
            //okay so we want a table with current head count, wage, totals , tell them what each employee does, and then ask which type
            cout << "\t\t"
            << left << setw(20) << "Job"
            << right << setw(10) << "Headcount"
            << right << setw(18) << "Hourly Wage ($)"
            << right << setw(18) << "Weekly Cost ($)"
            << right << setw(17) << "Maximum Orders"
            << endl;

            cout << "\t\t"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;

            cout << "\t\t"
                << left << setw(20) << "Cook"
                << right << setw(10) << to_string(current -> cookCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> cookPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> cookPay * 40)
                << right << setw(17) << to_string(current -> ordersPerCook)
                << endl;

            cout << "\t\t"
                << left << setw(20) << "Cashier"
                << right << setw(10) << to_string(current -> cashierCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> cashierPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> cashierPay * 40)
                << right << setw(17) << to_string(current -> ordersPerCashier)
                << endl;

            cout << "\t\t"
                << left << setw(20) << "Dishwasher"
                << right << setw(10) << to_string(current -> dishCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> dishPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> dishPay * 40)
                << right << setw(17) << to_string(current -> ordersPerDishwasher)
                << endl;

            cout << "\t\t"
                << left << setw(20) << "Delivery Driver"
                << right << setw(10) << to_string(current -> deliveryGuyCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> deliveryGuyPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> deliveryGuyPay * 40)
                << right << setw(17) << ""
                << endl;

            cout << "\t\t"
                << left << setw(20) << "Manager"
                << right << setw(10) << to_string(current -> managerCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> managerPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> managerPay * 40)
                << right << setw(17) << ""
                << endl;

            cout << "\t\t"
                << left << setw(20) << "General Manager"
                << right << setw(10) << to_string(current -> generalManagerCount)
                << right << setw(18) << Utils::getMoneyString(current -> location -> generalManagerPay)
                << right << setw(18) << Utils::getMoneyString(1 * current -> location -> generalManagerPay * 40)
                << right << setw(17) << ""
                << endl;

            cout << "\t\t"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;
            
            int headcount = (current -> cookCount + current -> cashierCount + current -> dishCount + current -> deliveryGuyCount + current -> managerCount + current -> generalManagerCount);
            double totalhourly =    current -> latestCostOfLabor =  current -> cookCount * current -> location -> cookPay +
                                    current -> cashierCount * current -> location -> cashierPay +
                                    current -> dishCount * current -> location -> dishPay +
                                    current -> deliveryGuyCount * current -> location -> deliveryGuyPay +
                                    current -> managerCount * current -> location -> managerPay +
                                    current -> generalManagerCount * current -> location -> generalManagerPay;
            double totalWeekly = totalhourly * 40;

            cout << "\t\t"
                << left << setw(20) << "Total"
                << right << setw(10) << to_string(headcount)
                << right << setw(18) << Utils::getMoneyString(totalhourly)
                << right << setw(18) << Utils::getMoneyString(totalWeekly)
                << right << setw(17) << to_string(current -> ordersPerCook)
                << endl;
            
            //now lets have a paragraph explaining what each type of employee does, 
            cout << endl;
            cout << "\t(1) Cooks are the backbone of our restaurant, and we can't operate without them. Based on our" << endl 
                << "\tlevel of equipment and management every cook can make on average " << current -> ordersPerCook << " orders per week. With" << endl
                << "\t" << to_string(current -> cookCount) << " Cook(s), we estimate a maximum of " << to_string(current -> cookCount * current -> ordersPerCook) 
                << " orders per week." << endl 
                << "\tIn " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> cookPay) << " per hour.";

            cout << endl << endl;
            cout << "\t(2) Cashiers are the face of our company, handling orders, hate mail, and complaints, with a" << endl 
                << "\tsmile. Based on our level of equipment and management every cashier can handle on average" << endl 
                << "\t" << current -> ordersPerCashier << " orders per week. With " << to_string(current -> cookCount) << " Cashiers(s), we estimate a maximum of "
                << to_string(current -> cashierCount * current -> ordersPerCashier) << " orders per week." << endl
                << "\tIn " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> cashierPay) << " per hour.";
                                
            cout << endl << endl;
            cout << "\t(3) Dishwashers make sure every spatula, knife and pizza pan is ready for the cooks. Based on " << endl 
                << "\tour level of equipment and management every dishwasher can wash on average '" << current -> ordersPerDishwasher << " orders per week'" << endl 
                <<"\tworth of dishes. With " << to_string(current -> cookCount) << " Dishwasher(s), we estimate a maximum of '" << to_string(current -> cookCount * current -> ordersPerCook) 
                << " orders per week' worth of dishes." << endl
                << "\tIn " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> dishPay) << " per hour.";
                                
            cout << endl << endl;
            cout << "\t(4) Delivery Guys are part of the reason that " << shopname << " is a household name. They deliver orders" << endl 
                << "\tto our customers so they don't have to leave the couch! On average, each delivery guy adds about " << endl
                << "\t" << to_string(EmployeesBase::extraOrdersPerDeliveryGuy) << " extra demand to each of our menu items. We can only have upto " << EmployeesBase::maxDeliveryGuys << " per location however. " << endl 
                <<"\tThey usually work for mostly tips, so in " 
                << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> deliveryGuyPay) << " per hour.";

            cout << endl << endl;
            cout << "\t(5) Managers make sure operations are running smoothly, and frankly, make our job much easier. Due" << endl 
                << "\tto their supervision, cooks make " << to_string(EmployeesBase::extraCookOrdersPerManager) << " extra orders per week. They also buy supplies as long as we are" << endl 
                << "\tabove a threshold of " << Utils::getMoneyString(managerBuySuppliesThreshold) << " (Configurable). A maximum of " << EmployeesBase::maxManagers << " per Location" << endl 
                << "\tIn " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> managerPay) << " per hour.";

            cout << endl << endl;
            cout << "\t(6) General Managers are the managers' managers. Due to their supervision, everyone can handle " << to_string(EmployeesBase::extraOrdersPerGM) << endl 
                << "\textra orders per week, and a " << Utils::getDigitDecimal(EmployeesBase::GM_COGSavings * 100) << "% Cost of Goods Sold discount. They will also automatically hire" << endl 
                << "\tand fire employees, if above " << Utils::getMoneyString(generalManagerHireThreshold) << " (Configurable). You can only have " << EmployeesBase::maxGMs << " per location. " << endl 
                << "\tIn " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " they make " 
                << Utils::getMoneyString(current -> location -> generalManagerPay) << " per hour.";
            
            cout << endl << endl;

            //okay now we need to know if they want to hire or fire?
            cout << "\t(0 For Back) Do you want to hire(y) or fire(n)? ";
            char input;
            cin >> input;
            if(input == '0') break;
            string who = "\t(0 For Back) Who would you like to hire? ";
            if(input != 'Y' && input != 'y') who = "\t(0 For Back) Who would you like to fire? ";

            int selection = Utils::getPlayerChoice("\t", who);
            
            //alright so we just said a number, switch case
            string job = "";
            switch (selection){
            case 1:
                //then they want a cook
                job = "Cooks";
                break;
            case 2:
                //then they want a cashier
                job = "Cashiers";
                break;
            case 3:
                //then they want a dishwasher
                job = "Dishwashers";
                break;
            case 4:
                //then they want a delivery guy
                job = "Delivery Guys";
                break;
            case 5:
                //a manager
                job = "Managers";
                break;
            case 6:
                //a general manager
                job = "General Managers";
                break;
            default:
                //then lets return to the selection screen
                break;
            }
            if(job == "") break;




            string q = "\tHow many " + job + " would you like to hire? ";
            if(input != 'Y' && input != 'y') q = "\tHow many " + job + " would you like to fire? ";
            int number =Utils::getPlayerChoice("\t", q);
            if(number == 0) break; //then send them back
            
            //okay so we know if they want to hire  or  fire
            //we know which position
            //we know how many

            //now do our checks
            if(input == 'Y' || input == 'y'){
                //we are hiring people
                if(selection == 4 && (number + current -> deliveryGuyCount) > EmployeesBase::maxDeliveryGuys){
                    cout << endl << endl << endl << "\tWe can't have " << to_string(number + current -> deliveryGuyCount) << " delivery guys! Max of " << to_string(EmployeesBase::maxDeliveryGuys) << "." << endl;
                    system("pause");
                    break;
                }
                if(selection == 5 && (number + current -> managerCount) > EmployeesBase::maxManagers){
                    cout << endl << endl << endl << "\tWe can't have " << to_string(number + current -> managerCount) << " managers! Max of " << to_string(EmployeesBase::maxManagers) << "." << endl;
                    system("pause");
                    break;
                }
                if(selection == 6 && (number + current -> generalManagerCount) > EmployeesBase::maxGMs){
                    cout << endl << endl << endl << "\tWe can't have " << to_string(number + current -> generalManagerCount) << " General Managers! Max of " << to_string(EmployeesBase::maxGMs) << "." << endl;
                    system("pause");
                    break;
                }

                //alright so there are no other errors, hire x number of employees for each job
                if(selection == 1) current -> cookCount += number;
                if(selection == 2) current -> cashierCount += number;
                if(selection == 3) current -> dishCount += number;
                if(selection == 4) current -> deliveryGuyCount += number;
                if(selection == 5) current -> managerCount += number;
                if(selection == 6) current -> generalManagerCount += number;
            }else{
                //so we are subtracting from headcount, we have to check if we can fire that many, if not then do max and throw an error
                if(selection == 1 && number > current -> cookCount){
                    //then fire all of them and print error
                    current -> cookCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " cooks to fire, firing them all..." << endl;
                    system("pause");
                }else if(selection == 2 && number > current -> cashierCount){
                    //then fire all of them and print error
                    current -> cashierCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " cashiers to fire, firing them all..." << endl;
                    system("pause");
                }else if(selection == 3 && number > current -> dishCount){
                    //then fire all of them and print error
                    current -> dishCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " dishwashers to fire, firing them all..." << endl;
                    system("pause");
                }else if(selection == 4 && number > current -> deliveryGuyCount){
                    //then fire all of them and print error
                    current -> deliveryGuyCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " delivery guys to fire, firing them all..." << endl;
                    system("pause");
                }else if(selection == 5 && number > current -> managerCount){
                    //then fire all of them and print error
                    current -> managerCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " managers to fire, firing them all..." << endl;
                    system("pause");
                }else if(selection == 6 && number > current -> generalManagerCount){
                    //then fire all of them and print error
                    current -> generalManagerCount = 0;
                    cout << endl << endl << endl << "\tWe don't have " << number << " general managers to fire, firing them all..." << endl;
                    system("pause");
                }else{
                    //alright so if none of those were true, then we can and should fire x number of employees
                    if(selection == 1) current -> cookCount -= number;
                    if(selection == 2) current -> cashierCount -= number;
                    if(selection == 3) current -> dishCount -= number;
                    if(selection == 4) current -> deliveryGuyCount -= number;
                    if(selection == 5) current -> managerCount -= number;
                    if(selection == 6) current -> generalManagerCount -= number;
                }
            }
            current -> recalcOrdersPerEmployee();
            recalculateHeadcount();
            break;
        }
        case 4: {
            //

            break;
        }
        case 5: {
            //alright so the user wants to upgrade their equipment. This includes the oven, fryer, fridge and robot.
            //first spit out a blurb about each one, then how much it costs at each level, then ask if they would like to upgrade
            cout << upgradeEquipment << endl;
            cout << endl;
            cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "\t(1) Pizza Oven" << endl;
            cout << "\t(2) Fryer" << endl;
            cout << "\t(3) Fridge" << endl;
            cout << "\t(4) Robot" << endl;
            
            int num3;
            cout << "\t";
            cin >> num3;
            switch (num3){
                case 1:
                    //if they have the money, do it
                    if(balance >= current -> location -> pizzaOvenUpgradeCost){
                        balance -= current -> location -> pizzaOvenUpgradeCost;
                        current -> pizzaOvenLevel++;
                    }
                    break;
                case 2:
                    if(balance >= current -> location -> fryerUpgradeCost){
                        balance -= current -> location -> fryerUpgradeCost;
                        current -> fryerLevel++;
                        
                    }
                    break;
                case 3:
                    if(balance >= current -> location -> fridgeUpgradeCost){
                        balance -= current -> location -> fridgeUpgradeCost;
                        current -> fridgeLevel++;
                        current -> maxSupplies = (current -> fridgeLevel * EquipmentBase::maxSuppliesPerFridgeLevel);
                    }
                    break;
                case 4:
                    if(balance >= current -> location -> robotUpgradeCost){
                        balance -= current -> location -> robotUpgradeCost;
                        current -> robotLevel++;
                    }
                    break;
                default:
                    break;
            }

            current -> recalcOrdersPerEmployee();

            break;
        }
        case 6: {
            //okay so they typed in 6, still make sure they are renting, if not break
            if(!current -> isRented) break;
            //okay so they arent renting, 
            cout << Utils::wrapText(buildingPara) << "  ";
            char in;
            cin >> in;

            if(in == 'Y' || in =='y'){
                if(balance >= current -> location -> buildingCost){
                    current -> isRented = false;
                    balance -= current -> location -> buildingCost;
                    return;
                }
                cout << endl << endl;
                cout << "You dont have enough money!" << endl;
            }
            break; //if they say no, break to prevent from rolling down.
        }
        case 7: {
            if(current -> isRented){
                //then they are renting the location, we want to close the location
                cout << "\t\tWe are renting in " << Utils::getLocationString(current -> location -> city, current -> location -> state) << " for " << Utils::getMoneyString(current -> location -> rent) << " per week. Last week it made " << Utils::getMoneyString(current -> latestRevenue) << " in" << endl
                    << "\tand has " << employees << " employees. You can choose to close this location, a good choice if its unprofitable." << endl
                    << "\tDo you wish to proceed? (Y/N)";
            }else {
                //then they arent, close the location, but also sell the building
                cout << "\t\tWe own this store, located in " << Utils::getLocationString(current -> location -> city, current -> location -> state) << ". Last week it made " << Utils::getMoneyString(current -> latestRevenue) << " in revenue" << endl
                    << "\tand has " << employees << " employees. You can choose to sell this building for " << Utils::getDigitDecimal(MISC::buildingSellValue * 100) << "% of cost, or " << Utils::getMoneyString(current -> location -> buildingCost * MISC::buildingSellValue) << "." << endl
                    << "\tDo you wish to proceed? (Y/N)";
            }

            char in;
            cin >> in;
            //they are either renting or they bought it, we want to sell if they said yes
            if(!(in == 'Y' || in == 'y')){
                //then they said no
                break;
            }

            //okay they want to close the location
            current -> cookCount = 0;
            current -> cashierCount = 0;
            current -> dishCount = 0;
            current -> deliveryGuyCount = 0;
            current -> managerCount = 0;
            current -> generalManagerCount = 0;

            recalculateHeadcount();

            numberOfLocations--;
            

            //set the location to unowned
            current -> location -> alreadyOwned = false;
            
            //if they bought the location, then they get 80% of building cost
            if(!current -> isRented) balance += (current -> location -> buildingCost * MISC::buildingSellValue);

            //then we update the linked list so we are no longer pointing to this location, freeing memory
            prev -> next = current -> next;
            delete current;
            current = prev -> next;
            GAMESTATE = 2;
            
            break;
        }
        case 8: {
            lastCurrent = current;
            lastPrevious = prev;
            toNextWeek();
            break;
        }
        case 0: {
            //takes us back to viewing locations
            GAMESTATE = 2;
            break;
        }
        default: {
            break;
        }
    }
}
void PizzaCompany::unownedLocationScreen(){
    printHUD();
    cout << "\t~~~~~~~~~~~~~~~~~~~~~~~ " << shopname << " in " << Utils::getLocationString(currentLoc -> city, currentLoc -> state) << " ~~~~~~~~~~~~~~~~~~~~~ " << endl;

    cout << endl << endl;
    cout << "\t\t" 
        << left << setw(30) << "Available Menu Options"
        << right << setw(18) << "Estimated Demand"
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    
    cout << "\t\t"
        << left << setw(30) << "Single Slice"
        << right << setw(18) << currentLoc -> baseSliceDemand
        << endl;
    cout << "\t\t"
        << left << setw(30) << "8 Cut Pizza"
        << right << setw(18) << currentLoc -> base8CutDemand
        << endl;
    cout << "\t\t"
        << left << setw(30) << "12 Cut Pizza Combo with Wings"
        << right << setw(18) << currentLoc -> base12CutComboDemand
        << endl;

    cout << endl << endl;
    cout << "\t" << Utils::wrapText(currentLoc -> blurb) << endl;

    //alright now we want to print the wealth, and what are rent is
    string wealthSen = "";
    if(currentLoc -> wealth >= 87){
        wealthSen = " is considered Wealthy. It's people will care little about the price of our pizzas.";
    }else if(currentLoc -> wealth >= 50){
        wealthSen = " is Middling. Prices matter in this town, but a craving for pizza will make anyone part with their cash.";
    }else if(currentLoc -> wealth >= 14){
        wealthSen = " is Struggling. The locals will not buy our pizza if it's outside of their budget.";
    }else {
        wealthSen = " is very poor. They will only consider our pizza if it's far below base price.";
    }

    string buildingCosts = " We could buy a shop here for " + Utils::getMoneyString(currentLoc -> buildingCost) + ", or we could rent for " + Utils::getMoneyString(currentLoc -> rent);
    string taxCosts = " Obviously, we have to pay the local governor for some reason. The local tax rate here is " + Utils::get2DigitDecimal(currentLoc -> taxRate * 100) + "% of our operating income.";
    string laborCosts = " I estimate cook pay to cost " + Utils::getMoneyString(currentLoc -> cookPay) + " per employee per hour. And as the laws of physics still apply,";
    string ingredients = " the employees also need ingredients to make the pizzas. I made a table for you.";
    string maintenance = "Rent and Ingredients are the only costs of doing business. We also have to pay the maintenance costs for our equipment.";
    
    cout << endl;
    cout << Utils::wrapText(Utils::getCompoundString(Utils::getLocationString(currentLoc -> city, currentLoc -> state), Utils::getCompoundString(wealthSen, Utils::getCompoundString(buildingCosts,Utils::getCompoundString(Utils::getCompoundString(taxCosts, laborCosts), ingredients))))) << endl;
    cout << endl;

    cout << "\t\t"
        << left << setw(20) << "Ingredient"
        << left << setw(14) << "Local Cost ($)"
        << endl;
    cout << "\t\t"
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << endl;
        
    cout << "\t\t"
        << left << setw(20) << "Dough"
        << right << setw(14) << Utils::getMoneyString(currentLoc -> doughPrice)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Sauce"
        << right << setw(14) << Utils::getMoneyString(currentLoc -> saucePrice)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Cheese"
        << right << setw(14) << Utils::getMoneyString(currentLoc -> cheesePrice)
        << endl;
    cout << "\t\t"
        << left << setw(20) << "Wings"
        << right << setw(14) << Utils::getMoneyString(currentLoc -> wingsPrice)
        << endl;

    cout << endl;


    cout << "\t\t"
        << left << setw(17) << "Equipment"
        << left << setw(18) << "Upgrade Cost ($)"
        << left << setw(24) << "Maintenance Cost ($)"
        << endl;
    cout << "\t\t"
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        << endl;
        
    cout << "\t\t"
        << left << setw(17) << "Pizza Oven"
        << left << setw(18) << Utils::getMoneyString(currentLoc -> pizzaOvenUpgradeCost)
        << left << setw(24) << Utils::getMoneyString(currentLoc -> pizzaOvenMaintenance)
        << endl;
    cout << "\t\t"
        << left << setw(17) << "Fryer"
        << left << setw(18) << Utils::getMoneyString(currentLoc -> fryerUpgradeCost)
        << left << setw(24) << Utils::getMoneyString(currentLoc -> fryerMaintenance)
        << endl;
    cout << "\t\t"
        << left << setw(17) << "Fridge"
        << left << setw(18) << Utils::getMoneyString(currentLoc -> fridgeUpgradeCost)
        << left << setw(24) << Utils::getMoneyString(currentLoc -> fridgeMaintenance)
        << endl;
    cout << "\t\t"
        << left << setw(17) << "Robot"
        << left << setw(18) << Utils::getMoneyString(currentLoc -> robotUpgradeCost)
        << left << setw(24) << Utils::getMoneyString(currentLoc -> robotMaintenance)
        << endl;

        
    cout << endl;
    cout << "\t\tNow you must determine if you wish to buy or rent. If you buy" << endl 
        << "\t\tyou do not have to pay the weekly rent, however you must pay" << endl
        << "\t\tfor the large cost upfront. It is worth mentioning, that" << endl 
        <<"\t\tif you rent, you can alway buy the location later." << endl;
    cout << endl;
    cout << "\t\tWould you like to buy or rent?" << endl;
    cout << "\t\t(1) Buy    (" << Utils::getMoneyString(currentLoc -> buildingCost) << ")" << endl;
    cout << "\t\t(2) Rent   (" << Utils::getMoneyString(currentLoc -> rent) << " a week)" << endl;
    cout << "\t\t(0) Back To Buy Locations" << endl;
    cout << "\t\t(3) Back To Locations" << endl;

    int selection =Utils::getPlayerChoice("\t\t", "\t\tPlease select the desired option: ");

    switch (selection){
        case 1:
            //then they wanna buy!

            //fix this later, this is disgusting


            if(balance < currentLoc -> buildingCost){
                //then we can't afford it!
                cout << endl << endl;
                cout << "\t\tYou cant afford this building! it costs " << Utils::getMoneyString(currentLoc -> buildingCost) << endl;
                //then we will come back to this screen
                system("pause");
                break;
            }

            //  Make sure we start at the end of the list
            prev = first;
            current = first -> next;
            while (current != 0){
                prev = current;
                current = current -> next;
            }

            balance -= currentLoc -> buildingCost;

            
            

            current = new ShopLocation;
            current -> next = 0;
            prev -> next = current;

            numberOfLocations++;

            current -> location = currentLoc;
            current -> isRented = false;
            currentLoc -> alreadyOwned = true;

            current -> maxSupplies = current -> fridgeLevel * EquipmentBase::maxSuppliesPerFridgeLevel;
            current -> recalcOrdersPerEmployee();

            current -> DoughStock = 0;
            current -> SauceStock = 0;
            current -> CheeseStock = 0;
            current -> WingsStock = 0;
            current -> openDate = weekNumber;
            
            for(int i = 0; i < Region::regionsCount; i++){
                if(current -> location -> region == Region::regions[i].region){
                    //Then this region got a new location!
                    Region::regions[i].locations++;
                }
            }

            current -> next = 0;
            prev -> next = current;
            prev = current;

            //okay so we just bought the location, lets go to the owned location screen, and that is where current is pointing right now!
            GAMESTATE = 6;
            break;
        case 2:
            //then they wanna rent!

            //  Make sure we start at the end of the list
            prev = first;
            current = first -> next;
            while (current != 0){
                prev = current;
                current = current -> next;
            }

            current = new ShopLocation;

            numberOfLocations++;

            current -> location = currentLoc;
            current -> isRented = true;
            currentLoc -> alreadyOwned = true;

            current -> maxSupplies = current -> fridgeLevel * EquipmentBase::maxSuppliesPerFridgeLevel;
            current -> recalcOrdersPerEmployee();

            current -> DoughStock = 0;
            current -> SauceStock = 0;
            current -> CheeseStock = 0;
            current -> WingsStock = 0;
            current -> openDate = weekNumber;

            for(int i = 0; i < Region::regionsCount; i++){
                if(current -> location -> region == Region::regions[i].region){
                    //Then this region got a new location!
                    Region::regions[i].locations++;
                }
            }


            current -> next = 0;
            prev -> next = current;
            prev = current;

            //okay so we just started renting the location, lets go to the owned location screen, and that is where current is pointing right now!
            GAMESTATE = 6;

            break;
        case 0:
            //then they want to go back to buy locations
            GAMESTATE = 5;
            break;
        case 3:
            //then they want to go back to locations
            GAMESTATE = 2;
            break;
        default:
            //do nothing, they will come back to this screen if unhandled, currentLoc is still the same
            break;
    }
}


void PizzaCompany::headcountScreen(){
    recalculateHeadcount();
    printHUD();

    //how would i go about sorting the elements in a linked list?
    //well, compile an array of their headcounts and select nums. 
    //bubble / insertion sort by headcount
    //loop through the list displaying when selectnum is right.
    
    cout << "\t\t" 
        << left  << setw(28) << "Location"
        << left  << setw(28) << "Date Opened"
        << left << setw(8) << "| Dish."
        << left << setw(6) << "Cook"
        << left << setw(6) << "Cash."
        << left << setw(6) << "Deli."
        << left << setw(6) << "Mana."
        << left << setw(6) << "GMs"
        << " | "
        << left << setw(10) << "Headcount"
        << left << setw(17) << "Weekly Cost ($)"
        << left << setw(17) << "Profit ($)"
        << right << setw(15) << "Profit / Head"
        << " | "
        
        << endl;

    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    prev = first;
    current = first -> next;

    double latestRevenue = 0;

    int totalDish = 0;
    int totalCook = 0;
    int totalCashier = 0;
    int totalDelivery = 0;
    int totalManager = 0;
    int totalGM = 0;
    int totalTotal = 0;
    int totalWeeklyWage = 0;
    double totalLatestEarnings = 0;

    while(current != 0){
        int totalHeadcount = current -> dishCount + current -> cookCount + current -> cashierCount + current -> deliveryGuyCount + current -> managerCount + current -> generalManagerCount;
        int weeklyWage =    (current -> cookCount * current -> location -> cookPay +
                            current -> cashierCount * current -> location -> cashierPay +
                            current -> dishCount * current -> location -> dishPay +
                            current -> deliveryGuyCount * current -> location -> deliveryGuyPay +
                            current -> managerCount * current -> location -> managerPay +
                            current -> generalManagerCount * current -> location -> generalManagerPay) * 40;

        totalDish += current -> dishCount;
        totalCook += current -> cookCount;
        totalCashier += current -> cashierCount;
        totalDelivery += current -> deliveryGuyCount;
        totalManager += current -> managerCount;
        totalGM += current -> generalManagerCount;
        totalTotal += totalHeadcount;
        totalWeeklyWage += weeklyWage;
        totalLatestEarnings += current -> latestEarnings;

        latestRevenue += current -> latestRevenue;

        cout << "\t\t" 
            << left  << setw(28) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << left  << setw(28) << Utils::getYearString(current -> openDate)
            << left << setw(8) << Utils::getCompoundString("| ", to_string(current -> dishCount))
            << left << setw(6) << current -> cookCount
            << left << setw(6) << current -> cashierCount
            << left << setw(6) << current -> deliveryGuyCount
            << left << setw(6) << current -> managerCount
            << left << setw(6) << current -> generalManagerCount
            << " | "
            << left << setw(10) << totalHeadcount
            << left << setw(17) << Utils::getMoneyString(weeklyWage)
            << left << setw(17) << Utils::getMoneyString(current -> latestEarnings)
            << right << setw(15) << Utils::getMoneyString(current -> latestEarnings / totalHeadcount)
            << " | "
            << endl;

        prev = current;
        current = current -> next;
    }
    cout << endl; //extra space
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    
    cout << "\t\t" 
        << left  << setw(28) << "Total"
        << left  << setw(28) << "-"
        << left << setw(8) << Utils::getCompoundString("| ", to_string(totalDish))
        << left << setw(6) << totalCook
        << left << setw(6) << totalCashier
        << left << setw(6) << totalDelivery
        << left << setw(6) << totalManager
        << left << setw(6) << totalGM
        << " | "
        << left << setw(10) << totalTotal
        << left << setw(17) << Utils::getMoneyString(totalWeeklyWage)
        << left << setw(17) << Utils::getMoneyString(totalLatestEarnings)
        << right << setw(15) << Utils::getMoneyString(totalLatestEarnings / totalTotal)
        << " | "
        
        << endl;






    GAMESTATE = 1;
    system("pause");
}
void PizzaCompany::costOfGoodsSoldScreen(){
    printHUD();

    //this menu details the cost of goods sold in all locations, maybe 2 pages? i dont love that but
    cout << "  " 
        << left  << setw(20) << ""
        << "| "
        << left << setw(7) << ""
        << left << setw(14) << "  Price ($)"
        << left << setw(7) << ""
        << "| "
        << left << setw(40) << "        Cost of Last Week ($)"
        << "| "
        << left << setw(10) << ""
        << "| "
        << left << setw(35) << "       Cost Per Order ($)"
        << "| "
        << endl;

    cout << "  " 
        << left  << setw(20) << "Location"
        << "| "
        << left << setw(7) << "Dough"
        << left << setw(7) << "Sauce"
        << left << setw(7) << "Chee."
        << left << setw(7) << "Wings"
        << "| "
        << left << setw(10) << "Dough"
        << left << setw(10) << "Sauce"
        << left << setw(10) << "Chee."
        << left << setw(10) << "Wings"
        << "| "
        << left << setw(10) << "Total ($)"
        << "| "
        << left << setw(10) << "Single"
        << left << setw(10) << "8 Cut"
        << left << setw(15) << "12 Cut + Wings"
        << "| "
        << endl;

    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    prev = first;
    current = first -> next;


    double costSingle = 0;
    double cost8Cut = 0;
    double cost12Cut = 0;



    while(current != 0){
        costSingle =    MenuItems::SliceDoughCost * current -> location -> doughPrice +
                        MenuItems::SliceSauceCost * current -> location -> saucePrice +
                        MenuItems::SliceCheeseCost * current -> location -> cheesePrice;
        cost8Cut =      MenuItems::Cut8DoughCost * current -> location -> doughPrice +
                        MenuItems::Cut8SauceCost* current -> location -> saucePrice +
                        MenuItems::Cut8CheeseCost * current -> location -> cheesePrice;
        cost12Cut =     MenuItems::Cut12DoughCost * current -> location -> doughPrice +
                        MenuItems::Cut12SauceCost * current -> location -> saucePrice +
                        MenuItems::Cut12CheeseCost* current -> location -> cheesePrice +
                        MenuItems::Cut12WingsCost * current -> location -> wingsPrice;
                
        cout << "  " 
            << left  << setw(20) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << "| "
            << left << setw(7) << Utils::getMoneyString(current -> location -> doughPrice)
            << left << setw(7) << Utils::getMoneyString(current -> location -> saucePrice)
            << left << setw(7) << Utils::getMoneyString(current -> location -> cheesePrice)
            << left << setw(7) << Utils::getMoneyString(current -> location -> wingsPrice)
            << "| "
            << left << setw(10) << Utils::getMoneyString(current -> latestDoughUsed * current -> location -> doughPrice)
            << left << setw(10) << Utils::getMoneyString(current -> latestSauceUsed * current -> location -> saucePrice)
            << left << setw(10) << Utils::getMoneyString(current -> latestCheeseUsed * current -> location -> cheesePrice)
            << left << setw(10) << Utils::getMoneyString(current -> latestWingsUsed * current -> location -> wingsPrice)
            << "| "
            << left << setw(10) << Utils::getMoneyString(current -> latestCostOfGoodsSold)
            << "| "
            << left << setw(10) << Utils::getMoneyString(costSingle)
            << left << setw(10) << Utils::getMoneyString(cost8Cut)
            << left << setw(15) << Utils::getMoneyString(cost12Cut)
            << "| "
            << endl;

        prev = current;
        current = current -> next;
    }
    cout << endl; //extra space
    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;






    GAMESTATE = 1;
    system("pause");
}
void PizzaCompany::maintenanceScreen(){
    printHUD();

    //this screen displays the current levels of our equipment in each location, their levels, maintenance costs, total costs, profit, and 
    cout << "  " 
        << left  << setw(20) << ""
        << "| "
        << left << setw(6) << ""
        << left << setw(13) << "     Level"
        << left << setw(7) << ""
        << "| "
        << left << setw(32) << "     Maintenance Cost ($)"
        << "| "
        << left << setw(12) << ""
        << "| "
        << left << setw(12) << ""
        << "| "
        << endl;

    cout << "  " 
        << left  << setw(20) << "Location"
        << "| "
        << left << setw(6) << "Oven"
        << left << setw(6) << "Fryer"
        << left << setw(7) << "Fridge"
        << left << setw(7) << "Robot"
        << "| "
        << left << setw(8) << "Oven"
        << left << setw(8) << "Fryer"
        << left << setw(8) << "Fridge"
        << left << setw(8) << "Robot"
        << "| "
        << left << setw(12) << "Total ($)"
        << "| "
        << left << setw(12) << "Profit ($)"
        << "| "
        << endl;

    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    prev = first;
    current = first -> next;

    int numOfShops = 0;

    int totalOvens = 0;
    int totalFryers = 0;
    int totalFridges = 0;
    int totalRobots = 0;

    double totalMaintenanceOvens = 0;
    double totalMaintenanceFryers = 0;
    double totalMaintenanceFridges = 0;
    double totalMaintenanceRobots = 0;

    double totalCostOfMaintenance = 0;
    double totalEarnings = 0;


    while(current != 0){
        double costOfMaintenance =  current -> pizzaOvenLevel * current -> location -> pizzaOvenMaintenance + 
                                    current -> fryerLevel * current -> location -> fryerMaintenance + 
                                    current -> fridgeLevel * current -> location -> fridgeMaintenance + 
                                    current -> robotLevel * current -> location -> robotMaintenance;

        numOfShops++;
        totalOvens += current -> pizzaOvenLevel;
        totalFryers += current -> fryerLevel;
        totalFridges += current -> fridgeLevel;
        totalRobots += current -> robotLevel;

        totalMaintenanceOvens += current -> pizzaOvenLevel * current -> location -> pizzaOvenMaintenance;
        totalMaintenanceFryers += current -> fryerLevel * current -> location -> fryerMaintenance;
        totalMaintenanceFridges += current -> fridgeLevel * current -> location -> fridgeMaintenance;
        totalMaintenanceRobots += current -> robotLevel * current -> location -> robotMaintenance;
        
        totalCostOfMaintenance += costOfMaintenance;
        totalEarnings += current -> latestEarnings;


        cout << "  " 
            << left  << setw(20) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << "| "
            << left << setw(6) << current -> pizzaOvenLevel
            << left << setw(6) << current -> fryerLevel
            << left << setw(7) << current -> fridgeLevel
            << left << setw(7) << current -> robotLevel
            << "| "
            << left << setw(8) << Utils::getMoneyString(current -> pizzaOvenLevel * current -> location -> pizzaOvenMaintenance)
            << left << setw(8) << Utils::getMoneyString(current -> fryerLevel * current -> location -> fryerMaintenance)
            << left << setw(8) << Utils::getMoneyString(current -> fridgeLevel * current -> location -> fridgeMaintenance)
            << left << setw(8) << Utils::getMoneyString(current -> robotLevel * current -> location -> robotMaintenance)
            << "| "
            << left << setw(12) << Utils::getMoneyString(costOfMaintenance)
            << "| "
            << left << setw(12) << Utils::getMoneyString(current -> latestEarnings)
            << "| "
            << endl;

        prev = current;
        current = current -> next;
    }
    cout << endl; //extra space
    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    //alright now we need total and average, average first
    cout << "  " 
        << left  << setw(20) << "Average"
        << "| "
        << left << setw(6) << Utils::get2DigitDecimal(totalOvens / numOfShops)
        << left << setw(6) << Utils::get2DigitDecimal(totalFryers / numOfShops)
        << left << setw(7) << Utils::get2DigitDecimal(totalFridges/ numOfShops)
        << left << setw(7) << Utils::get2DigitDecimal(totalRobots/ numOfShops)
        << "| "
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceOvens / numOfShops)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceFryers / numOfShops)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceFridges / numOfShops)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceRobots / numOfShops)
        << "| "
        << left << setw(12) << Utils::getMoneyString(totalCostOfMaintenance / numOfShops)
        << "| "
        << left << setw(12) << Utils::getMoneyString(totalEarnings / numOfShops)
        << "| "
        << endl;

    cout << "  " 
        << left  << setw(20) << "Total"
        << "| "
        << left << setw(6) << Utils::get2DigitDecimal(totalOvens)
        << left << setw(6) << Utils::get2DigitDecimal(totalFryers)
        << left << setw(7) << Utils::get2DigitDecimal(totalFridges)
        << left << setw(7) << Utils::get2DigitDecimal(totalRobots)
        << "| "
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceOvens)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceFryers)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceFridges)
        << left << setw(8) << Utils::getMoneyString(totalMaintenanceRobots)
        << "| "
        << left << setw(12) << Utils::getMoneyString(totalCostOfMaintenance)
        << "| "
        << left << setw(12) << Utils::getMoneyString(totalEarnings)
        << "| "
        << endl;





    GAMESTATE = 1;
    system("pause");
}
void PizzaCompany::inventoryReport(){
    printHUD();
    //this report details the total supplies in each location, last time supplies was bought, the max, and last used
    cout << "  " 
        << left  << setw(20) << ""
        << "| "
        << left << setw(11) << ""
        << left << setw(22) << "Used Last Week"
        << left << setw(11) << ""
        << "  "
        << left << setw(14) << ""
        << "| "
        << left << setw(11) << ""
        << left << setw(22) << "Current Stockpile"
        << left << setw(11) << ""
        << "| "
        << left << setw(20) << ""
        << "| "
        << endl;

    cout << "  " 
        << left  << setw(20) << "Location"
        << "| "
        << left << setw(11) << "Dough"
        << left << setw(11) << "Sauce"
        << left << setw(11) << "Cheese"
        << left << setw(11) << "Wings"
        << "| "
        << left << setw(14) << "Max Supplies"
        << "| "
        << left << setw(11) << "Dough"
        << left << setw(11) << "Sauce"
        << left << setw(11) << "Cheese"
        << left << setw(11) << "Wings"
        << "| "
        << left << setw(20) << "Supplies Bought"
        << "| "
        << endl;

    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    prev = first;
    current = first -> next;

    int numOfShops = 0;
    double totalDoughUsed = 0;
    double totalSauceUsed = 0;
    double totalCheeseUsed = 0;
    double totalWingsUsed = 0;
    double totalMaxSupplies = 0;
    double totalDoughStock = 0;
    double totalSauceStock = 0;
    double totalCheeseStock = 0;
    double totalWingsStock = 0;

    while(current != 0){
        //so if supplies last bought it negative one, print nothing, otherwise, date
        string date = "";
        if(current -> weekSuppliesBoughtLast != -1) date = Utils::getYearString(current -> weekSuppliesBoughtLast);

        numOfShops += 1;
        totalDoughUsed += current -> latestDoughUsed;
        totalSauceUsed += current -> latestSauceUsed;
        totalCheeseUsed += current -> latestCheeseUsed;
        totalWingsUsed += current -> latestWingsUsed;
        totalMaxSupplies += current -> maxSupplies;
        totalDoughStock += current -> DoughStock;
        totalSauceStock += current -> SauceStock;
        totalCheeseStock += current -> CheeseStock;
        totalWingsStock += current -> WingsStock;

        cout << "  " 
            << left  << setw(20) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << "| "
            << left << setw(11) << Utils::get2DigitDecimal(current -> latestDoughUsed)
            << left << setw(11) << Utils::get2DigitDecimal(current -> latestSauceUsed)
            << left << setw(11) << Utils::get2DigitDecimal(current -> latestCheeseUsed)
            << left << setw(11) << Utils::get2DigitDecimal(current -> latestWingsUsed)
            << "| "
            << left << setw(14) << Utils::get2DigitDecimal(current -> maxSupplies)
            << "| "
            << left << setw(11) << Utils::get2DigitDecimal(current -> DoughStock)
            << left << setw(11) << Utils::get2DigitDecimal(current -> SauceStock)
            << left << setw(11) << Utils::get2DigitDecimal(current -> CheeseStock)
            << left << setw(11) << Utils::get2DigitDecimal(current -> WingsStock)
            << "| "
            << left << setw(20) << date
            << "| "
            << endl;

        prev = current;
        current = current -> next;
    }
    cout << endl; //extra space
    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    //alright now we need total and average, average first
    cout << "  " 
        << left  << setw(20) << "Average"
        << "| "
        << left << setw(11) << Utils::get2DigitDecimal(totalDoughUsed / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalSauceUsed / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalCheeseUsed / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalWingsUsed / numOfShops)
        << "| "
        << left << setw(14) << Utils::get2DigitDecimal(totalMaxSupplies)
        << "| "
        << left << setw(11) << Utils::get2DigitDecimal(totalDoughStock / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalSauceStock / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalCheeseStock / numOfShops)
        << left << setw(11) << Utils::get2DigitDecimal(totalWingsStock / numOfShops)
        << "| "
        << left << setw(20) << ""
        << "| "
        << endl;

    cout << "  " 
        << left  << setw(20) << "Total"
        << "| "
        << left << setw(11) << Utils::get2DigitDecimal(totalDoughUsed)
        << left << setw(11) << Utils::get2DigitDecimal(totalSauceUsed)
        << left << setw(11) << Utils::get2DigitDecimal(totalCheeseUsed)
        << left << setw(11) << Utils::get2DigitDecimal(totalWingsUsed)
        << "| "
        << left << setw(14) << Utils::get2DigitDecimal(totalMaxSupplies)
        << "| "
        << left << setw(11) << Utils::get2DigitDecimal(totalDoughStock)
        << left << setw(11) << Utils::get2DigitDecimal(totalSauceStock)
        << left << setw(11) << Utils::get2DigitDecimal(totalCheeseStock)
        << left << setw(11) << Utils::get2DigitDecimal(totalWingsStock)
        << "| "
        << left << setw(20) << ""
        << "| "
        << endl;





    GAMESTATE = 1;
    system("pause");
}
void PizzaCompany::revenueReport(){
    printHUD();

    /*
        The revenue report prints revenues for all locations, both all time stats and last week
        the total revenues 
    
    */
    cout << "  " 
        << left  << setw(20) << "Location"
        << " | "
        << left << setw(11) << ""
        << left << setw(11) << ""
        << left << setw(11) << ""
        << left << setw(11) << ""
        << " | "
        << left << setw(14) << ""
        << " | "
        << left << setw(11) << ""
        << left << setw(22) << "Current Stockpile"
        << left << setw(11) << ""
        << " | "
        << left << setw(20) << ""
        << " | "
        << endl;


}



void PizzaCompany::printOrgChart(){
    //this function prints the org chart of the organization, to be used on any screen
    cout << left << "+------------------------------+" << endl;
    cout << left << "|                              |" << endl;
    cout << left << "+------------------------------+" << endl;
    cout << left << "CEO " << CEO << endl;
    cout << left << "COO " << COO << endl;
    cout << left << "CFO " << CFO << endl;
    cout << left << "CMO " << CMO << endl;
    cout << left << "CTO " << CTO << endl;
    cout << left << "CLO " << CLO << endl;
}
void PizzaCompany::openACorporateOffice(){
    printHUD();
    //alright so when opening a corporate office, what do we care about? what makes a corporate office unique?
    //so the cost if 10x the building cost of the location's building cost
    //however that location gets double base demand
    //once they buy a corporate office, they can start to open stores internationally, they can hire CSuite and Corporate employees
    //etc...

    //alright so, give a little blurb, then list possible locations, note u can only corporatize in America, because u wont be able to buy internation until after, and u cant move it

    cout 
    << "\t\t\tOpening a Corporate Office is one of the most important decisions to make. Having one allows you to hire corporate employees in various roles, from C-Suite \n"
    << "\t\tExecutives to marketing managers, accountants, directors of operations and even engineers. These employees allow our organization to accomplish a wider variety of tasks,\n"
    << "\t\tlike open international locations all around the world, compile reports for headcount and maintenance costs, and even start international ad campaigns with popular\n"
    << "\t\tathletes. Where you open your corporate office matters. For starters, more expensive areas cost more to build, however the shop in that location will double its base demand!\n"
    << "\n\t\t It is worth noting that the location of our corporate office does NOT Influence corporate wages, you also will NOT be able to move the office once open!\n";

    cout << endl << endl;
    
    prev = first;
    current = first -> next;

    cout << "\t\t" 
        << left << setw(8) << ""
        << left << setw(30) << "Location"
        << left << setw(30) << "Cost to Open ($)"
        << endl;
    cout << "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;



    while(current != 0){
        cout << "\t\t" 
            << left << setw(8) << Utils::getCompoundString("(" + to_string(current -> location -> selectNum), ") ") 
            << left << setw(30) << Utils::getLocationString(current -> location -> city, current -> location -> state)
            << left << setw(30) << Utils::getMoneyString(current -> location -> buildingCost * MISC::corporateBuildingMultiplier)
            << endl;
        
        prev = current;
        current = current -> next;
    }
    cout << "\t\t" 
        << left << setw(8) << "(0)" 
        << left << setw(30) << "Back"
        << left << setw(30) << ""
        << endl;

    cout << endl;
    //Alright so we just printed the list of owned locations, all of these are in the united states, assuming they can only buy in the United States until then
    //next we want to sanitize input, and handling appropriately

    int selection = Utils::getPlayerChoice("\t\t", "\t\tPlease enter the location for your new corporate headquarters! (0 For Back)  ");
    //okay now we want to handle input, so check if its in bounds, 

    if(selection == 0){
        //then we want to go back to action menu
        GAMESTATE = 1;
    }else if(0 < selection && selection < startExtraMenuOptionsSelect){
        //so then we picked a location, we want to check if its valid, i.e, that we own it
        prev = first;
        current = first -> next;
        while(current != nullptr && current -> location -> selectNum != selection){
            prev = current;
            current = current -> next;
        }

        if(current == nullptr || current == 0) return; //if its null or we made it to end of list, just return, essentially reloading this page and they can pick again
        //otherwise we want to buy this location assuming we have enough money
        //first lets check if they have enough money, remember the location is current -> location

        if(balance <= (current -> location -> buildingCost * MISC::corporateBuildingMultiplier)){
            //then they dont have enought money
            cout << endl << endl;
            cout << "\t\tYou don't have enough money to open the corporate office here!" << endl;
            system("pause");
            return;
            //return is nice because this will just reset the page as the gamestate hasnt changed
        }

        //okay so they have enough money, lets double check with them
        
        cout << endl << endl;
        cout << "\t\tAre you sure you want to open the Corporate Headquarters in " << current -> location -> city << "? (Y / N)" << endl;
        char input;
        cout << "\t\t";
        cin >> input;
        if(input != 'Y' && input != 'y'){
            //so we said no, just return, reloading the page
            return;
        }

        //okay so we have enough money and the player said they are sure, lets open a corporate location in this city
        balance -= (current -> location -> buildingCost * MISC::corporateBuildingMultiplier);
        corporateLoc = new corporateOffice;
        corporateLoc -> location = current -> location;
        corporateLoc -> openDate = weekNumber;
        //okay so we have a new corporate office in the correct location
        //next we set GAMESTATE to to the newly opened corporate office!
        GAMESTATE = 12;
    }
}
void PizzaCompany::visitCorporate(){
    printHUD();

    cout << "\t~~~~~~~~~~~~~~~~~~~~~~~ " << shopname << "'s Corporate Headquarters in " << Utils::getLocationString(corporateLoc -> location -> city, corporateLoc -> location -> state) << " ~~~~~~~~~~~~~~~~~~~~~ " << endl;
    cout << endl << endl;

    cout << "\t\t" << endl;
    printOrgChart();


    //if possible, we want to be able to talk to the appropriate executives, each one lets us to certain actions

    cout << "\n";
    if(!COO.empty()) cout << "\t\t(3) Talk to our COO, " << COO << "\n";
    if(!CFO.empty()) cout << "\t\t(4) Talk to our CFO, " << CFO << "\n";
    if(!CMO.empty()) cout << "\t\t(5) Talk to our CMO, " << CMO << "\n";
    if(!CTO.empty()) cout << "\t\t(6) Talk to our CTO, " << CTO << "\n";
    if(!CLO.empty()) cout << "\t\t(7) Talk to our CLO, " << CLO << "\n";
    cout << "\t\t(8) Manage Board of Directors\n";
    cout << "\t\t(9) Manage Employees\n";
    cout << "\n";
    cout << "\t\t(1) Proceed to next week\n";
    cout << "\t\t(2) View Owned Locations\n";
    cout << "\t\t(0) Back\n";

    int selection =Utils::getPlayerChoice("\t\t", "\t\tPlease enter your choice: ");
    
    if(selection == 0){ GAMESTATE = 1;}
    else if(selection == 1){
        lastCurrent = current;
        lastPrevious = prev;
        toNextWeek();
    }else if(selection == 2){
        GAMESTATE = 2;
    }else if(selection == 8){
        GAMESTATE = 18;
    }else if(selection == 9){
        GAMESTATE = 19;
    }else if(selection == 3){
        if(!COO.empty()) GAMESTATE = 13;
    }else if(selection == 4){
        if(!CFO.empty()) GAMESTATE = 14;
    }else if(selection == 5){
        if(!CMO.empty()) GAMESTATE = 15;
    }else if(selection == 6){
        if(!CTO.empty()) GAMESTATE = 16;
    }else if(selection == 7){
        if(!CLO.empty()) GAMESTATE = 17;
    }

}

void PizzaCompany::manageBoard(){
    printHUD();
    cout << endl << endl;
    //here we want to hire company executives
    //sign on bonus of 50k and 120$ an hour for all of them?

    //first we want a little blurb about all of them, whom we currently have, etc...
    cout << "\t\t(1) The (COO) Chief Operating Officer handles most of the daily operations of the business, including the purchase of supplies, setting of prices, and opening of new\n";
    cout << "\t\tstores. A COO will compile reports of the daily operations for you like 'Cost of Goods Sold' Reports, Maintenance Reports, and so on. They will also allow you to hire\n";
    cout << "\t\tLogistics Engineers out of the corporate hq, as well as Directors of Operation for every region we are in. \n";
    cout << "\t\t\t" << Utils::getMoneyString(MISC::executiveSignOnBonus) << " sign-on bonus, and " << Utils::getMoneyString(MISC::executiveSignOnBonus) << " per hour.\n";

    cout << "\n\t\t(2) The (CFO) Chief Financial Officer handles the preparation of financial reports, like the Prices Report. They also allow to take our loans for the company, and even\n";
    cout << "\t\tinvest into the Stock Market, leveraging our capital. They can hire Tax Accountants, decreasing our Taxes payable in all locations. \n";
    cout << "\t\t\t" << Utils::getMoneyString(MISC::executiveSignOnBonus) << " sign-on bonus, and " << Utils::getMoneyString(MISC::executiveSignOnBonus) << " per hour.\n";

    cout << "\n\t\t(3) The (CMO) Chief Marketing Officer is responsible for increasing market attraction for our goods and services. They accomplish this through a variety of means, like\n";
    cout << "\t\tpurchasing Ads on TV, renting billboards, or even doing brand deals with famous celebrities! They can hire digital marketing strategists, capable of reaching potential\n";
    cout << "\t\tcustomers online. \n";
    cout << "\t\t\t" << Utils::getMoneyString(MISC::executiveSignOnBonus) << " sign-on bonus, and " << Utils::getMoneyString(MISC::executiveSignOnBonus) << " per hour.\n";

    cout << "\n\t\t(4) The (CTO) Chief Technology Officer researches innovative ways for our company to grow and developer in the forever-changing landscape. Hiring a CTO will allow us\n";
    cout << "\t\tto purchase Ziosks and Robots in our locations worldwide, potentially saving on labor. They allow us to hire Robotic System Administrators, increasing our technologies\n";
    cout << "\t\teffectiveness, and Software Developers, whom can build us apps and websites, increasing engagement.\n";
    cout << "\t\t\t" << Utils::getMoneyString(MISC::executiveSignOnBonus) << " sign-on bonus, and " << Utils::getMoneyString(MISC::executiveSignOnBonus) << " per hour.\n";

    cout << "\n\t\t(5) The (CLO) Chief Legal Officer is our lianson with local governments in all of our locations around the world. They handle settlements with customers, disputes\n";
    cout << "\t\twith employees and other entities, and even legal infractions. Due to their experience, they also have quite a few 'contacts' that might be able to help us save a few\n";
    cout << "\t\tnickels in taxes, or even find reasons to close our competitors, increasing demand.\n";
    cout << "\t\t\t" << Utils::getMoneyString(MISC::executiveSignOnBonus) << " sign-on bonus, and " << Utils::getMoneyString(MISC::executiveSignOnBonus) << " per hour.\n";

    cout << "\n\n";
    cout << "\t\tWould you like to hire(y), or fire(n) (0 For Back): ";
    char input;
    cin >> input;
    if(input == '0'){
        GAMESTATE = 12;
        return;
    }

    //okay so they said hire or fire

    int selection =Utils::getPlayerChoice("\t\t", "\t\tPlease Enter your selection(0 For Back): ");
    
    if(selection == 0){
        GAMESTATE = 12;
    }

    if(input == 'Y' || input == 'y'){
        if(selection == 1){
            //we if we have one, then we want to reroll the name, and dont take any money, if we do have one, and not enough money, do nothing, 
            if(!COO.empty()){
                //then we have one already, reroll the name
                COO = Utils::getRandomName();
                GAMESTATE = 12;
            }else{
                //okay so we don't currently have a COO, but the player wants to buy one
                //check if they have enough money, if so but one, 
                if(balance >= MISC::executiveSignOnBonus){
                    //then we can afford one, do it
                    balance -= MISC::executiveSignOnBonus;
                    COO = Utils::getRandomName();
                    GAMESTATE = 12;
                }
            }
        }else if(selection == 2){
                if(!CFO.empty()){
                    CFO = Utils::getRandomName();
                    GAMESTATE = 12;
                }else{
                    if(balance >= MISC::executiveSignOnBonus){
                        balance -= MISC::executiveSignOnBonus;
                        CFO = Utils::getRandomName();
                        GAMESTATE = 12;
                    }
                }
        }else if(selection == 3){
                if(!CMO.empty()){
                    CMO = Utils::getRandomName();
                    GAMESTATE = 12;
                }else{
                    if(balance >= MISC::executiveSignOnBonus){
                        balance -= MISC::executiveSignOnBonus;
                        CMO = Utils::getRandomName();
                        GAMESTATE = 12;
                    }
                }
        }else if(selection == 4){
                if(!CTO.empty()){
                    CTO = Utils::getRandomName();
                    GAMESTATE = 12;
                }else{
                    if(balance >= MISC::executiveSignOnBonus){
                        balance -= MISC::executiveSignOnBonus;
                        CTO = Utils::getRandomName();
                        GAMESTATE = 12;
                    }
                }
        }else if(selection == 5){
                if(!CLO.empty()){
                    CLO = Utils::getRandomName();
                    GAMESTATE = 12;
                }else{
                    if(balance >= MISC::executiveSignOnBonus){
                        balance -= MISC::executiveSignOnBonus;
                        CLO = Utils::getRandomName();
                        GAMESTATE = 12;
                    }
                }
        }
    }else {
        //then we want to fire, whatever selected, clear
        if(selection == 1) COO = "";
        if(selection == 2) CFO = "";
        if(selection == 3) CMO = "";
        if(selection == 4) CTO = "";
        if(selection == 5) CLO = "";
        GAMESTATE = 12;
    }

    recalculateHeadcount();

    
    

}
void PizzaCompany::manageCorporateEmployees(){
    printHUD();

    //lets print a chart of how much they are paid, their max, and who they report too

    cout << "\t\t\t"
        << left << setw(30) << "Job Title"
        << left << setw(25) << "Currently Employed"
        << left << setw(16) << "Hourly Rate"
        << left << setw(16) << "Maximum of"
        << left << setw(16) << "Reports to"
        << endl;
    cout << "\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    cout << "\t\t\t"
        << left << setw(30) << "Tax Accountant"
        << left << setw(25) << TaxAccountant::count
        << left << setw(16) << Utils::getMoneyString(TaxAccountant::pay)
        << left << setw(16) << TaxAccountant::max
        << left << setw(16) << Utils::getCompoundString("CFO ", CFO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Digital Marketing Specialist"
        << left << setw(25) << DigitalMarketingSpecialist::count
        << left << setw(16) << Utils::getMoneyString(DigitalMarketingSpecialist::pay)
        << left << setw(16) << DigitalMarketingSpecialist::max
        << left << setw(16) << Utils::getCompoundString("CMO ", CMO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Employee Relations Manager"
        << left << setw(25) << EmployeeRelationsManager::count
        << left << setw(16) << Utils::getMoneyString(EmployeeRelationsManager::pay)
        << left << setw(16) << EmployeeRelationsManager::max
        << left << setw(16) << Utils::getCompoundString("CEO ", CEO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Director Of Operations"
        << left << setw(25) << "numOfDirectorsOfOperations"
        << left << setw(16) << Utils::getMoneyString(DirectorOfOperations::pay)
        << left << setw(16) << "1 Per Region"
        << left << setw(16) << Utils::getCompoundString("COO ", COO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Logistics Engineer"
        << left << setw(25) << LogisticsEngineer::count
        << left << setw(16) << Utils::getMoneyString(LogisticsEngineer::pay)
        << left << setw(16) << LogisticsEngineer::max
        << left << setw(16) << Utils::getCompoundString("COO ", COO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Software Developer"
        << left << setw(25) << SoftwareDeveloper::count
        << left << setw(16) << Utils::getMoneyString(SoftwareDeveloper::pay)
        << left << setw(16) << SoftwareDeveloper::max
        << left << setw(16) << Utils::getCompoundString("CTO ", CTO)
        << endl;
    cout << "\t\t\t"
        << left << setw(30) << "Robotic Systems Adminstrator"
        << left << setw(25) << RoboticSystemsAdministrator::count
        << left << setw(16) << Utils::getMoneyString(RoboticSystemsAdministrator::pay)
        << left << setw(16) << RoboticSystemsAdministrator::max
        << left << setw(16) << Utils::getCompoundString("CTO ", CTO)
        << endl;

    cout << "\n\n";

    cout << "\n\t(1) The Tax Accountant moves some money around, and finds loop wholes in the tax code for our locations. They report to the CFO. \n";
    cout << "\t\t-" << Utils::getDigitDecimal(TaxAccountant::taxReliefPer * 100) <<"% Taxes in all locations per, Max of " << TaxAccountant::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(TaxAccountant::pay) << " per hour.\n";
    
    cout << "\n\t(2) The Digital Marketing Strategist promotes our brand online, increasing our presence in the market and attracting more customers. They report to the CMO\n";
    cout << "\t\t" << DigitalMarketingSpecialist::extraDemandPer <<" extra demand for all menu items in all locations. Max of " << DigitalMarketingSpecialist::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(DigitalMarketingSpecialist::pay) << " per hour.\n";

    cout << "\n\t(3) The Employee Relations Manager protects the companies's assets and reign in payroll expenses. They report to you, the CEO \n";
    cout << "\t\t-" << Utils::getDigitDecimal(EmployeeRelationsManager::payrollReliefPer * 100) <<"% Payroll Expense for all Non-Corporate Employees in all locations Per. Max of " << EmployeeRelationsManager::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(EmployeeRelationsManager::pay) << " per hour.\n";
        
    cout << "\n\t(4) The Director of Operations write reports and handle day to day operations. They oversee GMs in their region. They report to the COO \n";
    cout << "\t\t-" << Utils::getDigitDecimal(DirectorOfOperations::COGSSavings * 100) <<"% Cost of Goods Sold in all locations in their Region. Max of 1 Per Region\n";
    cout << "\t\t" << Utils::getMoneyString(DirectorOfOperations::pay) << " per hour.\n";

    cout << "\n\t(5) The Logistics Engineer handle distribution of everything needed to make a store run, including ingredients. They Report to the COO\n";
    cout << "\t\t-" << Utils::getDigitDecimal(LogisticsEngineer::COGSSavings * 100) <<"% Cost of Goods Sold Per in All Locations, Max of " << LogisticsEngineer::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(LogisticsEngineer::pay) << " per hour.\n";
        
    cout << "\n\t(6) The Software Developer creates interactive Apps and Websites, letting us reach more customers, hence increasing demand. They report to the CTO \n";
    cout << "\t\t" << SoftwareDeveloper::ExtraDemandPer << " extra demand for all menu items in all locations. Max of " << SoftwareDeveloper::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(SoftwareDeveloper::pay) << " per hour.\n";

    cout << "\n\t(7) The Robotic Systems Administrator manages our robotic systems, keeping up with technological change. They report to the CTO\n";
    cout << "\t\t" << RoboticSystemsAdministrator::extraOrdersPer << " extra Orders Per Robot and Ziosk in all locations. Max of " << RoboticSystemsAdministrator::max << "\n";
    cout << "\t\t" << Utils::getMoneyString(RoboticSystemsAdministrator::pay) << " per hour.\n";

    cout << "\n\n";
    char input;
    cout << "\t\tWould you like to hire(y) or fire(n) (0 For Back): ";
    cin >> input;
    //if input isnt y or n, then redo the page, if its 0, go back to corporate office
    if(input != 'Y' && input != 'y' && input != 'N' && input != 'n'){
        if(input == '0') GAMESTATE = 12;
        return;
    }

    //okay so they want to hire or fire,
    string hf = "hire";
    string jobtitle = "";
    if(input == 'N' || input == 'n') hf = "fire";

    string prompt = "\t\tWho would you like to " + hf + "? (0 For Back): ";

    int selection =Utils::getPlayerChoice("\t\t", prompt);
    switch (selection){
        case 1: jobtitle = "Tax Accountant(s)"; break;
        case 2: jobtitle = "Digital Marketing Specialist(s)"; break;
        case 3: jobtitle = "Employee Relations Manager(s)"; break;
        case 4: jobtitle = "Director Of Operation(s)"; break;
        case 5: jobtitle = "Logistics Engineer(s)"; break;
        case 6: jobtitle = "Software Developer(s)"; break;
        case 7: jobtitle = "Robotic System Administrator(s)"; break;
        default:
            GAMESTATE = 12;
            return;
    }

    //so we know who they want to hire/fire, check if we have their boss
    if(selection == 1 && CFO.empty()){
        cout << "\t\tWe cannot hire a Tax Accountant without a CFO!\n";
        system("pause");
        return;//dont change the gamestate, restart diologue
    }else if(selection == 2 && CMO.empty()){
        cout << "\t\tWe cannot hire a Digital Marketer without a CMO!\n";
        system("pause");
        return;
    }else if(selection == 4 && COO.empty()){
        cout << "\t\tWe cannot hire a Director of Operations without a COO!\n";
        system("pause");
        return;
    }else if(selection == 5 && COO.empty()){
        cout << "\t\tWe cannot hire a Logistics Engineer without a COO!\n";
        system("pause");
        return;
    }else if(selection == 6 && CTO.empty()){
        cout << "\t\tWe cannot hire a Software Developer without a CTO!\n";
        system("pause");
        return;
    }else if(selection == 7 && CTO.empty()){
        cout << "\t\tWe cannot hire a Robotic Systems Administrator without a CTO!\n";
        system("pause");
        return;
    }


    prompt = "\t\tHow many " + jobtitle + " would you like to " + hf + "? (0 For Back): ";
    int changeInHeadCount = Utils::getPlayerChoice("\t\t", prompt);


    switch (selection){
    case 1://then tax accountant
        //okay so they want to either hire or fire x accountants,
        if(input == 'Y' || input == 'y'){
            //hire x accounts, check if we hit max
            // 0 1 2 3 
            TaxAccountant::count += changeInHeadCount;
            if(TaxAccountant::count > TaxAccountant::max){
                TaxAccountant::count = TaxAccountant::max;
                cout << "\t\tHey boss! We can't have more than " << TaxAccountant::max << " Tax Accountants. It's okay I fixed it. We currently have " << TaxAccountant::count << " on payroll.\n";
                system("pause");
            }
        }else{
            //then they said fire
            //fire x accounts, cant be below 0
            TaxAccountant::count -= changeInHeadCount;
            if(TaxAccountant::count < 0){
                TaxAccountant::count = 0;
                cout << "\t\tHey boss! We can't have negative Tax Accountants?. It's okay I fixed it. We currently have " << TaxAccountant::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    case 2://then Digital Marketer
        if(input == 'Y' || input == 'y'){
            DigitalMarketingSpecialist::count += changeInHeadCount;
            if(DigitalMarketingSpecialist::count > DigitalMarketingSpecialist::max){
                DigitalMarketingSpecialist::count = DigitalMarketingSpecialist::max;
                cout << "\t\tHey boss! We can't have more than " << DigitalMarketingSpecialist::max << " Digital Marketing Strategists. It's okay I fixed it. We currently have " << DigitalMarketingSpecialist::count << " on payroll.\n";
                system("pause");
            }
        }else{
            DigitalMarketingSpecialist::count -= changeInHeadCount;
            if(DigitalMarketingSpecialist::count < 0){
                DigitalMarketingSpecialist::count = 0;
                cout << "\t\tHey boss! We can't have negative Digital Marketing Strategists?. It's okay I fixed it. We currently have " << DigitalMarketingSpecialist::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    case 3://then Employee Relations Manager
        if(input == 'Y' || input == 'y'){
            EmployeeRelationsManager::count += changeInHeadCount;
            if(EmployeeRelationsManager::count > EmployeeRelationsManager::max){
                EmployeeRelationsManager::count = EmployeeRelationsManager::max;
                cout << "\t\tHey boss! We can't have more than " << EmployeeRelationsManager::max << " Employee Relations Managers. It's okay I fixed it. We currently have " << EmployeeRelationsManager::count << " on payroll.\n";
                system("pause");
            }
        }else{
            EmployeeRelationsManager::count -= changeInHeadCount;
            if(EmployeeRelationsManager::count < 0){
                EmployeeRelationsManager::count = 0;
                cout << "\t\tHey boss! We can't have negative Employee Relations Managers?. It's okay I fixed it. We currently have " << EmployeeRelationsManager::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    case 4://then Director of Operations
        break;
    case 5://then Logistics Engineer
        if(input == 'Y' || input == 'y'){
            LogisticsEngineer::count += changeInHeadCount;
            if(LogisticsEngineer::count > LogisticsEngineer::max){
                LogisticsEngineer::count = LogisticsEngineer::max;
                cout << "\t\tHey boss! We can't have more than " << LogisticsEngineer::max << " Logistics Engineers. It's okay I fixed it. We currently have " << LogisticsEngineer::count << " on payroll.\n";
                system("pause");
            }
        }else{
            LogisticsEngineer::count -= changeInHeadCount;
            if(LogisticsEngineer::count < 0){
                LogisticsEngineer::count = 0;
                cout << "\t\tHey boss! We can't have negative Logistics Engineers. It's okay I fixed it. We currently have " << LogisticsEngineer::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    case 6://then Software Developer
        if(input == 'Y' || input == 'y'){
            SoftwareDeveloper::count += changeInHeadCount;
            if(SoftwareDeveloper::count > SoftwareDeveloper::max){
                SoftwareDeveloper::count = SoftwareDeveloper::max;
                cout << "\t\tHey boss! We can't have more than " << SoftwareDeveloper::max << " Software Developers. It's okay I fixed it. We currently have " << SoftwareDeveloper::count << " on payroll.\n";
                system("pause");
            }
        }else{
            SoftwareDeveloper::count -= changeInHeadCount;
            if(SoftwareDeveloper::count < 0){
                SoftwareDeveloper::count = 0;
                cout << "\t\tHey boss! We can't have negative Software Developers?. It's okay I fixed it. We currently have " << SoftwareDeveloper::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    case 7://then Robot Systems Admin
        if(input == 'Y' || input == 'y'){
            RoboticSystemsAdministrator::count += changeInHeadCount;
            if(RoboticSystemsAdministrator::count > RoboticSystemsAdministrator::max){
                RoboticSystemsAdministrator::count = RoboticSystemsAdministrator::max;
                cout << "\t\tHey boss! We can't have more than " << RoboticSystemsAdministrator::max << " Robotic Systems Admins. It's okay I fixed it. We currently have " << RoboticSystemsAdministrator::count << " on payroll.\n";
                system("pause");
            }
        }else{
            RoboticSystemsAdministrator::count -= changeInHeadCount;
            if(RoboticSystemsAdministrator::count < 0){
                RoboticSystemsAdministrator::count = 0;
                cout << "\t\tHey boss! We can't have negative Robotic Systems Admins?. It's okay I fixed it. We currently have " << RoboticSystemsAdministrator::count << " on payroll.\n";
                system("pause");
            }
        }
        break;
    default:
        GAMESTATE = 12;
        return;
    }

    recalculateHeadcount();
    GAMESTATE = 12;
    return;

}


void PizzaCompany::talkToCOO(){
    printHUD();
    printOrgChart();
}
void PizzaCompany::talkToCFO(){
    printHUD();
    printOrgChart();
}
void PizzaCompany::talkToCMO(){
    printHUD();
    printOrgChart();
}
void PizzaCompany::talkToCTO(){
    printHUD();
    printOrgChart();
}
void PizzaCompany::talkToCLO(){
    printHUD();
    printOrgChart();
}

int main(){
    PizzaCompany pizza;
    pizza.readLocationsData();
    pizza.printBeginningCutscene();

	while (pizza.GAMESTATE != 0) {
		if (pizza.GAMESTATE == 1)
			pizza.ActionMenu();
        else if (pizza.GAMESTATE == 2)
            pizza.viewLocations();
        else if (pizza.GAMESTATE == 21)
			pizza.viewRegions();

        else if (pizza.GAMESTATE == 22)
			pizza.ownedLocationsInRegion("North America");
        else if (pizza.GAMESTATE == 23)
			pizza.ownedLocationsInRegion("South America");
        else if (pizza.GAMESTATE == 24)
			pizza.ownedLocationsInRegion("Europe");
        else if (pizza.GAMESTATE == 25)
			pizza.ownedLocationsInRegion("Africa");
        else if (pizza.GAMESTATE == 26)
			pizza.ownedLocationsInRegion("Middle East");
        else if (pizza.GAMESTATE == 27)
			pizza.ownedLocationsInRegion("Asia");
        else if (pizza.GAMESTATE == 28)
			pizza.ownedLocationsInRegion("Japan");
        else if (pizza.GAMESTATE == 29)
			pizza.ownedLocationsInRegion("Oceania");
        else if (pizza.GAMESTATE == 30)
			pizza.ownedLocationsInRegion("Caribbean");

        else if (pizza.GAMESTATE == 31)
			pizza.buildAtNewLocation("North America");
        else if (pizza.GAMESTATE == 32)
			pizza.buildAtNewLocation("South America");
        else if (pizza.GAMESTATE == 33)
			pizza.buildAtNewLocation("Europe");
        else if (pizza.GAMESTATE == 34)
			pizza.buildAtNewLocation("Africa");
        else if (pizza.GAMESTATE == 35)
			pizza.buildAtNewLocation("Middle East");
        else if (pizza.GAMESTATE == 36)
			pizza.buildAtNewLocation("Asia");
        else if (pizza.GAMESTATE == 37)
			pizza.buildAtNewLocation("Japan");
        else if (pizza.GAMESTATE == 38)
			pizza.buildAtNewLocation("Oceania");
        else if (pizza.GAMESTATE == 39)
			pizza.buildAtNewLocation("Caribbean");

        else if (pizza.GAMESTATE == 5)
			pizza.buildAtNewLocation();//set when all

        else if (pizza.GAMESTATE == 3)
            ;
        else if (pizza.GAMESTATE == 4)
			int x = 4;

        else if (pizza.GAMESTATE == 6)
            //shows where current is pointing!
			pizza.ownedLocationScreen();
        else if (pizza.GAMESTATE == 7)
            //shows where currentLOC is pointing!
			pizza.unownedLocationScreen();





        else if (pizza.GAMESTATE == 8)
			pizza.headcountScreen();
        else if (pizza.GAMESTATE == 9)
			pizza.costOfGoodsSoldScreen();
        else if (pizza.GAMESTATE == 10)
			pizza.maintenanceScreen();

        //open or view corporate offices

        else if (pizza.GAMESTATE == 11) //OPEN A CORPORATE OFFICE
			pizza.openACorporateOffice();
        else if (pizza.GAMESTATE == 12) //VISIT THE CURRENT CORPORATE OFFICE
			pizza.visitCorporate();


        //talk to the C-Suite Executives

        else if (pizza.GAMESTATE == 13)
			pizza.talkToCOO();
        else if (pizza.GAMESTATE == 14)
			pizza.talkToCFO();
        else if (pizza.GAMESTATE == 15)
			pizza.talkToCMO();
        else if (pizza.GAMESTATE == 16)
			pizza.talkToCTO();
        else if (pizza.GAMESTATE == 17)
			pizza.talkToCLO();
        
        //hire corporate employees

        else if (pizza.GAMESTATE == 18)
			pizza.manageBoard();
        else if (pizza.GAMESTATE == 19)
			pizza.manageCorporateEmployees();
        else if (pizza.GAMESTATE == 20)
			pizza.inventoryReport();


        system("cls");
	}
	return 0;
}