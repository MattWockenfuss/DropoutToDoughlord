#include "GameData.h"

#include <string>

namespace TaxAccountant {
    int count = 0;
}

namespace DigitalMarketingSpecialist {
    int count = 0;
}

namespace EmployeeRelationsManager {
    int count = 0;
}

namespace Region {
    //apparently we dont need a constructor for this struct because strings are already initialized, and this is merely an 'aggregate'
    Data regions[9] = {
        {"North America", ""},
        {"South America", ""},
        {"Europe", ""},
        {"Africa", ""},
        {"Middle East", ""},
        {"Asia", ""},
        {"Japan", ""},
        {"Oceania", ""},
        {"Caribbean", ""}
    };
}

namespace LogisticsEngineer {
    int count = 0;
}

namespace SoftwareDeveloper {
    int count = 0;
}

namespace RoboticSystemsAdministrator {
    int count = 0;
}