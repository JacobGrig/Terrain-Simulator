#include "Randomness.h"

int get_random (
    int right_limit,
    int left_limit
) noexcept
{
    if (left_limit > right_limit)
    {
        return get_random (left_limit, right_limit);
    }
    else if (left_limit == right_limit)
    {
        return left_limit;
    }
    else
    {
        return left_limit + rand () % (right_limit - left_limit);
    }
}

Point get_random_point (
    Size size
) noexcept
{
    return Point (get_random (size.length), get_random (size.width));
}

CataclysmID get_random_cataclysm_id () noexcept
{
    return CataclysmID (get_random (cataclysm_types_number));
}

Cataclysm get_random_cataclysm (
    Size size
) noexcept
{
    return Cataclysm (
        get_random (CataclysmsConstants::max_level),
        get_random (CataclysmsConstants::max_radius),
        get_random_point (size),
        get_random_cataclysm_id ()
    );
}

BuildingTypes get_random_building_type () noexcept
{
    return static_cast<BuildingTypes> (get_random (number_of_building_types, 1));
}

Building get_random_building (
    Size size
) noexcept
{
    BuildingTypes building_type = get_random_building_type ();
    Size size_of_building = BuildingsInfo::property_info.at (BuildingID (building_type)).get_min_size ();
    return Building (
        building_type,
        get_random (size.length - size_of_building.length),
        get_random (size.width - size_of_building.width),
        size_of_building.length,
        size_of_building.width,
        get_random (BuildingsConstants::max_value_of_attributes),
        get_random (BuildingsConstants::max_value_of_attributes),
        get_random (BuildingsConstants::max_value_of_attributes),
        get_random (BuildingsConstants::max_value_of_attributes)
    );
}

bool decide (
    double probability
) noexcept
{
    return (get_random (100) / 100.0 < probability);
}

double get_probability (
    double right_limit,
    double left_limit
) noexcept
{
    return static_cast<double> (
        get_random (
            static_cast<int> (right_limit * 100),
            static_cast<int> (left_limit * 100)
        )
    ) / 100;
}