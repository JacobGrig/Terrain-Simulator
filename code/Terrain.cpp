#include "Terrain.h"

const Size Terrain::get_size () const noexcept
{
    return size;
}

Terrain::Terrain (
    int length,
    int width,
    double tax
) //throw (std::bad_alloc) 
    : terrain_map (
        width,
        std::vector<PointOnMap> (
            length,
            PointOnMap ()
        )
    ),
    size (
        length,
        width
    ),
    tax (tax)
{
    this->month = 0;
}

bool Terrain::check_before_build (
    Point point,
    Size size
) const noexcept
{
    int right_limit = point.abscissa + size.length;
    int low_limit = point.ordinate + size.width;
    if (this->get_size ().length < right_limit ||
        this->get_size ().width < low_limit)
    {
        return false;
    }
    for (int i = point.abscissa; i < right_limit; ++i)
    {
        for (int j = point.ordinate; j < low_limit; ++j)
        {
            PointOnMap point_on_map = this->terrain_map[j][i];
            if (point_on_map.get_building_type() != BuildingID())
            {
                return false;
            }
        }
    }
    return true;
}

void Terrain::construct_building (
    Building building
) //throw (InvalidArgument)
{
    Size size = this->get_size ();
    PointOnMap point_on_map(
        building.get_point ().abscissa,
        building.get_point ().ordinate,
        BuildingID (building.get_building_type ())
    );
    int max_abscissa = building.get_point ().abscissa + building.get_size ().length;
    int max_ordinate = building.get_point ().ordinate + building.get_size ().width;

    if ((!check_before_build (building.get_point (), building.get_size ())) &&
        building.get_building_type () != no_building)
    {
        throw InvalidArgument ("other buildings interfere");
    }
    else
    {
        for (
            int ordinate = building.get_point ().ordinate;
            ordinate < max_ordinate;
            ++ordinate
        )
        {
            for (
                int abscissa = building.get_point ().abscissa;
                abscissa < max_abscissa;
                ++abscissa
                )
            {
                PointOnMap &local_point = terrain_map[ordinate][abscissa];
                if (local_point.get_building_type () != BuildingID () &&
                    building.get_building_type () != no_building)
                {
                    throw InvalidArgument ("building");
                }
                else
                {
                    local_point = point_on_map;
                }
            }
        }
    }
}

void Terrain::destroy_building (
    Building building
) //throw (InvalidArgument)
{
    Building no_building (
        no_building,
        building.get_point().abscissa,
        building.get_point().ordinate,
        building.get_size().length,
        building.get_size().width
    );
    this->construct_building (no_building);
}

std::ostream& operator<< (
    std::ostream &out, 
    const Terrain &terrain
) noexcept
{
    try
    {
        out << ' ';
        for (int i = 0; i < terrain.size.length; i++)
        {
            out << '=';
        }
        out << ' ' << std::endl;
        for (int i = 0; i < terrain.size.width; i++)
        {
            out << '|';
            for (int j = 0; j < terrain.size.length; j++)
            {
                const PointOnMap &local_point = terrain.terrain_map[i][j];
                if (local_point.get_building_type() == BuildingID())
                {
                    out << ' ';
                }
                else
                {
                    if (i - local_point.ordinate == 1)
                    {
                        unsigned dif_abs = j - local_point.abscissa;
                        const std::string &name =
                            BuildingsInfo::property_info.at(
                                local_point.get_building_type()
                            ).get_name();
                        if (!dif_abs || dif_abs > name.length())
                        {
                            out << '*';
                        }
                        else
                        {
                            out << name[dif_abs - 1];
                        }
                    }
                    else
                    {
                        out << '*';
                    }
                }
            }
            out << '|' << std::endl;
        }
        out << ' ';
        for (int i = 0; i < terrain.size.length; i++)
        {
            out << '=';
        }
        out << ' ' << std::endl;
        out << "Size is the following: " << terrain.size << "." << std::endl;
        out << "Income tax is equal to " << terrain.tax << "." << std::endl;
        return out;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

void Terrain::resize (
    int new_length,
    int new_width
) //throw (std::bad_alloc)
{
    int old_width = size.width;

    size.length = new_length;
    size.width = new_width;
    terrain_map.resize (
        new_width, 
        std::vector<PointOnMap> (
            new_length, 
            PointOnMap ()
        )
    );

    int width = new_width;

    for (int i = 0; i < min (old_width, width); i++)
    {
        terrain_map[i].resize (
            new_length, 
            PointOnMap ()
        );
    }
}

Building Terrain::get_building (
    Point point
) const
{
    PointOnMap point_on_map = this->terrain_map[point.ordinate][point.abscissa];
    if (point_on_map.get_building_type() == BuildingID ())
    {
        throw InvalidArgument("no_building");
    }
    return points_on_buildings.at(point_on_map);
}