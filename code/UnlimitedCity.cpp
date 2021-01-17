#include "UnlimitedCity.h"

void UnlimitedCity::construct_building_without_fee (
    BuildingID building_type,
    Point point
) //throw (InvalidArgument)
{
    Size size = BuildingsInfo::property_info.at (building_type).get_min_size ();
    Building building = Building (
        building_type.get_building_type (),
        point.abscissa,
        point.ordinate,
        size.length,
        size.width
    );
    this->add_building_to_map (building);
}

void UnlimitedCity::destroy_building_without_fee (
    Point point
) //throw (InvalidArgument)
{
    this->delete_building_from_map (point);
}

void UnlimitedCity::update_building_without_fee (
    Point point,
    BuildingAttributeTypes attribute_type,
    int value,
    bool up_or_down
) //throw (MaxValue)
{
    int abscissa = point.abscissa;
    int ordinate = point.ordinate;

    PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];
    if (point_on_map.get_building_type () == BuildingID ())
    {
        throw InvalidArgument ("building", "UnlimitedCity::update_building_without_fee()");
    }
    Building &building = this->points_on_buildings[point_on_map];

    if (!building.update_attribute (attribute_type, value, up_or_down))
    {
        throw MaxValue ("building's attribute");
    }
}

void UnlimitedCity::expand_area_without_fee (
    int number_of_rows
) //throw (std::bad_alloc)
{
    this->rewidth (this->get_size ().width + number_of_rows);
}

void UnlimitedCity::insure_without_fee (
    Point point
) //throw (MaxValue, UnavailableOperation, std::bad_alloc)
{
    std::set<Point> addresses;
    PointOnMap point_on_map_of_building = this->terrain_map[point.ordinate][point.abscissa];
    Building &our_building = points_on_buildings[point_on_map_of_building];
    if (our_building.is_insured())
    {
        throw MaxValue ("the building is already insured");
    }
    try
    {
        addresses = this->get_addresses(BuildingID(insurance_company));
    }
    catch (std::out_of_range)
    {
        throw UnavailableOperation("no insurance");
    }
    for (
        std::set<Point>::const_iterator iter = addresses.cbegin();
        iter != addresses.cend();
        ++iter
        )
    {
        int abscissa = (*iter).abscissa;
        int ordinate = (*iter).ordinate;

        PointOnMap point_on_map_of_insurance_building = this->terrain_map[ordinate][abscissa];
        Building &insurance_building = points_on_buildings[point_on_map_of_insurance_building];

        if (insurance_building.get_insured_buildings().size() < LocalityConstants::max_number_of_insured_buildings)
        {
            insure (our_building, insurance_building);
            return;
        }
    }
    throw UnavailableOperation("no places in insurance");
}