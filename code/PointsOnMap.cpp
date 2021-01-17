#include "PointsOnMap.h"

PointOnMap::PointOnMap (
    int abscissa,
    int ordinate,
    BuildingID building_type
) noexcept : Point (                                        // point on the top left corner of the building
        abscissa,
        ordinate
    )
{
    this->building_type = building_type;
}

bool PointOnMap::operator== (
    const PointOnMap &point_on_map
) const noexcept
{
    return (this->abscissa == point_on_map.abscissa &&
            this->ordinate == point_on_map.ordinate &&
            this->building_type == point_on_map.building_type);
}

bool PointOnMap::operator< (
    const PointOnMap &point_on_map
) const noexcept
{
    return (this->ordinate < point_on_map.ordinate ||
            this->ordinate == point_on_map.ordinate &&
            this->abscissa < point_on_map.abscissa ||
            this->ordinate == point_on_map.ordinate &&
            this->abscissa == point_on_map.abscissa &&
            this->building_type < point_on_map.building_type);
}

std::ostream& operator<< (
    std::ostream& out, 
    const PointOnMap &point_on_map
) noexcept
{
    try
    {
        return out << "Point is (" << point_on_map.abscissa << ", "
            << point_on_map.ordinate << "), building type is \""
            << BuildingsInfo::property_info.at(point_on_map.building_type).get_name() << "\".";
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}