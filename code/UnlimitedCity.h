#pragma once

#include <new>

#include "Locality.h"
#include "Errors.h"

class UnlimitedCity : public Locality
{
public:
    void construct_building_without_fee (BuildingID building_type, Point point); //throw (InvalidArgument);
    void destroy_building_without_fee (Point point); //throw (InvalidArgument);

    void update_building_without_fee (
        Point point,
        BuildingAttributeTypes attribute_type,
        int value,
        bool up_or_down
    ); //throw (MaxValue);

    void expand_area_without_fee (int number_of_rows = 1); //throw (std::bad_alloc);
    void insure_without_fee (Point point); //throw (MaxValue, UnavailableOperation, std::bad_alloc);
};