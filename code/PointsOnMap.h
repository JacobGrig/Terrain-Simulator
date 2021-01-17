#pragma once

#include "Buildings.h"

class PointOnMap : public Point
{
    BuildingID building_type;
public:
    explicit PointOnMap (
        int abscissa = 0, 
        int ordinate = 0, 
        BuildingID building_type = BuildingID ()
    ) noexcept;
    BuildingID get_building_type () const noexcept { return building_type; }
    bool operator== (const PointOnMap &point_on_map) const noexcept;
    bool operator< (const PointOnMap &point_on_map) const noexcept;
    friend std::ostream& operator<< (std::ostream& out, const PointOnMap &point_on_map) noexcept;
};