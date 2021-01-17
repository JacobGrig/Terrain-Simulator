#pragma once

#include <map>
#include <unordered_map>
#include <set>
#include <functional>
#include <algorithm>

#include "Locality.h"
#include "Randomness.h"
#include "Errors.h"

namespace CityConstants 
{
    constexpr long ñost_per_row = 1000000;
    constexpr long cost_per_attribute = 10000;
}

class City : public Locality
{
public:
    void construct_building_with_fee (BuildingID building_type, Point point); //throw (BurstingBudget, InvalidArgument);
    void destroy_building_with_fee (Point point); //throw (BurstingBudget, InvalidArgument);

    void update_building_with_fee (
        Point point,
        BuildingAttributeTypes attribute_type,
        int value,
        bool up_or_down
    ); //throw (BurstingBudget, MaxValue, InvalidArgument);

    void expand_area_with_fee (int number_of_rows = 1); //throw (BurstingBudget, UnavailableOperation, std::bad_alloc);
    void insure_with_fee (Point point); //throw (MaxValue, UnavailableOperation, BurstingBudget, std::bad_alloc);
};