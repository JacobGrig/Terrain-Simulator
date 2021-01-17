#include "City.h"

void City::construct_building_with_fee (
    BuildingID building_type,
    Point point
) //throw (BurstingBudget, InvalidArgument)
{
    int price = BuildingsInfo::property_info.at (building_type).get_price ();
    Size size = BuildingsInfo::property_info.at (building_type).get_min_size ();
    if (this->attributes.budget < price + LocalityConstants::cost_of_builders * size.area ())
    {
        throw BurstingBudget ("construct building");
    }
    else
    {
        PointOnMap point_on_map = PointOnMap (point.abscissa, point.ordinate, building_type);
        Building building = Building (
            building_type.get_building_type (), 
            point.abscissa, 
            point.ordinate, 
            size.length, 
            size.width
        );
        this->add_building_to_map (building);
        this->attributes.budget -= price + LocalityConstants::cost_of_builders * size.area();
    }
}

void City::destroy_building_with_fee (
    Point point
) //throw (BurstingBudget, InvalidArgument)
{
    int abscissa = point.abscissa;
    int ordinate = point.ordinate;

    PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];

    Size size = BuildingsInfo::property_info.at (point_on_map.get_building_type ()).get_min_size ();

    if (this->attributes.budget < LocalityConstants::cost_of_builders * size.area ())
    {
        throw BurstingBudget ("destroy building");
    }
    else
    {
        this->delete_building_from_map (point);
        this->attributes.budget -= LocalityConstants::cost_of_builders * size.area();
    }
}

void City::update_building_with_fee (
    Point point,
    BuildingAttributeTypes attribute_type,
    int value,
    bool up_or_down
) //throw (BurstingBudget, MaxValue, InvalidArgument)
{
    int abscissa = point.abscissa;
    int ordinate = point.ordinate;

    PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];
    if (point_on_map.get_building_type () == BuildingID ())
    {
        throw InvalidArgument ("building", "City::update_building_with_fee ()");
    }
    Building &building = this->points_on_buildings[point_on_map];

    if (building.update_attribute (attribute_type, value, up_or_down))
    {
        if (this->attributes.budget >= value * CityConstants::cost_per_attribute)
        {
            update_budget_with_concrete_value (-value * CityConstants::cost_per_attribute);
        }
        else
        {
            building.update_attribute (attribute_type, value, !up_or_down);
            throw BurstingBudget ("update the building");
        }
    }
    else
    {
        throw MaxValue ("building's attribute");
    }
}

void City::expand_area_with_fee (
    int number_of_rows
) //throw (BurstingBudget, UnavailableOperation, std::bad_alloc)
{
    if (number_of_rows < this->accessible_expansion)
    {
        if (this->attributes.budget > number_of_rows * CityConstants::ñost_per_row)
        {
            this->rewidth (this->get_size().width + number_of_rows);
            this->update_budget_with_concrete_value (-number_of_rows * CityConstants::ñost_per_row);
        }
        else
        {
            throw BurstingBudget ("expand the area");
        }
    }
    else
    {
        throw UnavailableOperation ("expand the area");
    }
}

void City::insure_with_fee (
    Point point
) //throw (MaxValue, UnavailableOperation, BurstingBudget, std::bad_alloc)
{
    std::set<Point> addresses;
    PointOnMap point_on_map_of_building = this->terrain_map[point.ordinate][point.abscissa];
    Building &our_building = points_on_buildings[point_on_map_of_building];
    if (our_building.is_insured())
    {
        throw MaxValue ("the building is already insured");
    }
    if (this->attributes.budget < BuildingsConstants::cost_of_insurance * BuildingsInfo::property_info.at (our_building).get_price ())
    {
        throw BurstingBudget ("insure");
    }
    try
    {
        addresses = this->get_addresses (BuildingID (insurance_company));
    }
    catch (std::out_of_range)
    {
        throw UnavailableOperation ("no insurance");
    }
    for (
        std::set<Point>::const_iterator iter = addresses.cbegin ();
        iter != addresses.cend ();
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
    throw UnavailableOperation ("no places in insurance");
}