#include "Desert.h"

void Desert::update_divine () noexcept
{
    this->divine_power += get_random (
        DesertConstants::high_limit_of_divine_addition,
        DesertConstants::low_limit_of_divine_addition
    );
}

void Desert::add_building_to_map (
    Building building
) //throw (InvalidArgument, std::bad_alloc)
{
    this->construct_building (building);
    PointOnMap point_on_map (
        building.get_point ().abscissa,
        building.get_point ().ordinate,
        BuildingID (building.get_building_type ())
    );
    this->points_on_buildings[point_on_map] = building;
    this->buildings_on_points[building].insert(point_on_map);
}

void Desert::send_catalysm_with_divine_power (
    Cataclysm cataclysm
) //throw (BurstingDivine)
{
    if (cataclysm.get_level () * cataclysm.get_radius () * DesertConstants::divine_power_per_unit > this->divine_power)
    {
        throw BurstingDivine ("send cataclysm");
    }
    this->divine_power -= cataclysm.get_level() * cataclysm.get_radius() * DesertConstants::divine_power_per_unit;
    send_cataclysm (cataclysm);
}

void Desert::add_random_building_to_map () //throw (InvalidArgument)
{
    Building building = get_random_building (this->get_size ());
    add_building_to_map (building);
}

void Desert::delete_building_from_map (
    Point point
) //throw (InvalidArgument)
{
    PointOnMap point_on_map = terrain_map[point.ordinate][point.abscissa];
    Building building = points_on_buildings[point_on_map];
    this->destroy_building (building);
    this->points_on_buildings.erase (point_on_map);
    this->buildings_on_points[building].erase (point_on_map);
    if (this->buildings_on_points[building].empty ())
    {
        this->buildings_on_points.erase (building);
    }
}

void Desert::expand_area(
	int number_of_rows
) //throw (std::bad_alloc)
{
	this->rewidth(this->get_size().width + number_of_rows);
}

void Desert::send_cataclysm (
    Cataclysm cataclysm
) noexcept
{
    Point center = cataclysm.get_point ();
    int level = cataclysm.get_level ();
    int radius = cataclysm.get_radius ();
    Size size = this->get_size ();
    CataclysmTypes cataclysm_type = cataclysm.get_cataclysm_type ();

    int left_limit = max (0, center.abscissa - radius);
    int right_limit = min (size.length, center.abscissa + radius);
    int low_limit = max (0, center.ordinate - radius);
    int high_limit = min (size.width, center.ordinate + radius);

    for (int ordinate = low_limit; ordinate < high_limit; ++ordinate)
    {
        for (int abscissa = left_limit; abscissa < right_limit; ++abscissa)
        {
            if (Point (abscissa, ordinate).is_in_circle (center, radius))
            {
                PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];

                if (point_on_map.get_building_type () != BuildingID ())
                {
                    Building &building = this->points_on_buildings[point_on_map];
                    int error = -1;
                    int &attribute =
                        (cataclysm_type == earthquake) ?
                            building.attributes.earthquake_resistance :
                        (cataclysm_type == flood) ?
                            building.attributes.moisture_resistance :
                        (cataclysm_type == hurricane) ?
                            building.attributes.strength :
                        (cataclysm_type == fire) ?
                            building.attributes.fire_resistance : error;
                    attribute -= level;
                    if (attribute < 0)
                    {
						std::cout << "Destructed building: " << std::endl << building << std::endl;
                        this->delete_building_from_map (building.get_point ());
                    }
                }
            }
        }
    }
}

void Desert::live_month () noexcept
{
    ++this->month;
    for (
        int i = 0;
        i < DesertConstants::max_number_of_constructed_buildings;
        ++i
    )
    {
        try
        {
            add_random_building_to_map ();
        }
        catch (InvalidArgument &)
        {
            ;
        }
    }
    update_divine ();
	expand_area (get_random (TerrainConstants::max_number_of_extensions));
}

std::ostream& operator<< (
    std::ostream &out,
    const Desert &desert
) noexcept
{
    try
    {
        const Terrain &desert_as_terrain = desert;
        out << desert_as_terrain << std::endl;
        return out << "Divine Power = " << desert.divine_power << std::endl;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}