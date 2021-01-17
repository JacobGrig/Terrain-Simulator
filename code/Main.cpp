#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Geometry.h"
#include "Buildings.h"
#include "PointsOnMap.h"
#include "Terrain.h"
#include "Locality.h"
#include "City.h"
#include "UnlimitedCity.h"
#include "Desert.h"
#include "Randomness.h"
#include "Cataclysms.h"
#include "UserInteraction.h"

int main () 
{
    srand (static_cast<unsigned> (time (NULL)));
    try
    {
        start();
    }
    catch (...)
    {
        std::cout << std::endl << "Unknown Error!" << std::endl;
    }
    system ("pause");
}