#include "Buildings.h"

using std::to_string;

BuildingID::BuildingID (
    BuildingTypes building_type
) noexcept
{
    this->building_type = building_type;
}

bool BuildingID::operator== (
    const BuildingID &building_id
) const noexcept
{
    return (this->building_type == building_id.building_type);
}

bool BuildingID::operator < (
    const BuildingID &building_id
) const noexcept
{
    return (this->building_type < building_id.building_type);
}

BuildingTypes BuildingID::get_building_type () const noexcept
{
    return this->building_type;
}

std::unordered_map<BuildingID, BuildingTypeInfo, std::hash<int>> BuildingsInfo::property_init () noexcept
{
    typedef BuildingTypeInfo BTI;
    std::unordered_map<BuildingID, BTI, std::hash<int>> property_info;
    property_info[BuildingID (bank)]              = BTI(100000, "BANK",              Size (6, 3));
    property_info[BuildingID (church)]             = BTI(50000, "CHURCH",            Size (8, 3));
    property_info[BuildingID (city_center)]      = BTI(1500000, "CITY-CENTER",       Size (13, 3));
    property_info[BuildingID (court)]              = BTI(50000, "COURT",             Size (7, 3));
    property_info[BuildingID (hospital)]          = BTI(100000, "HOSPITAL",          Size (10, 3));
    property_info[BuildingID (house)]              = BTI(10000, "HOUSE",             Size (7, 3));
    property_info[BuildingID (housing_office)]    = BTI(100000, "HOUSING-OFFICE",    Size (16, 3));
    property_info[BuildingID (insurance_company)] = BTI(200000, "INSURANCE-COMPANY", Size (19, 3));
    property_info[BuildingID (market)]             = BTI(70000, "MARKET",            Size (8, 3));
    property_info[BuildingID (natural_zone)]      = BTI(100000, "NATURAL-ZONE",      Size (14, 3));
    property_info[BuildingID (no_building)]            = BTI(0, "",                  Size (0, 0));
    property_info[BuildingID (parkland)]         = BTI(1000000, "PARKLAND",          Size (10, 3));
    property_info[BuildingID (police)]            = BTI(100000, "POLICE",            Size (8, 3));
    property_info[BuildingID (school)]            = BTI(200000, "SCHOOL",            Size (8, 3));
    return property_info;
}

const std::unordered_map<
    BuildingID, 
    BuildingTypeInfo, 
    std::hash<int>
> BuildingsInfo::property_info = BuildingsInfo::property_init ();

const std::string BuildingsInfo::property_prices = 
        "    1. City center: " + to_string (BuildingsInfo::property_info.at (BuildingID (city_center)).get_price ()) + 
        "\n    2. House: " + to_string (BuildingsInfo::property_info.at (BuildingID (house)).get_price ()) +
        "\n    3. Hospital: " + to_string (BuildingsInfo::property_info.at (BuildingID (hospital)).get_price ()) +
        "\n    4. Parkland: " + to_string (BuildingsInfo::property_info.at (BuildingID (parkland)).get_price ()) +
        "\n    5. Police: " + to_string (BuildingsInfo::property_info.at (BuildingID (police)).get_price ()) +
        "\n    6. Court: " + to_string (BuildingsInfo::property_info.at (BuildingID (court)).get_price ()) +
        "\n    7. Housing office: " + to_string (BuildingsInfo::property_info.at (BuildingID (housing_office)).get_price ()) +
        "\n    8. Market: " + to_string (BuildingsInfo::property_info.at (BuildingID (market)).get_price ()) +
        "\n    9. Church: " + to_string (BuildingsInfo::property_info.at (BuildingID (church)).get_price ()) +
        "\n    10. School: " + to_string (BuildingsInfo::property_info.at (BuildingID (school)).get_price ()) +
        "\n    11. Bank: " + to_string (BuildingsInfo::property_info.at (BuildingID (bank)).get_price ()) +
        "\n    12. Insurance company: " + to_string (BuildingsInfo::property_info.at (BuildingID (insurance_company)).get_price ()) +
        "\n    13. Natural zone: " + to_string (BuildingsInfo::property_info.at (BuildingID (natural_zone)).get_price ());

std::unordered_map<int, BuildingID> BuildingsInfo::property_codes_init() noexcept
{
    std::unordered_map<int, BuildingID> property_codes;
    property_codes[1] = BuildingID (city_center);
    property_codes[2] = BuildingID (house);
    property_codes[3] = BuildingID (hospital);
    property_codes[4] = BuildingID (parkland);
    property_codes[5] = BuildingID (police);
    property_codes[6] = BuildingID (court);
    property_codes[7] = BuildingID (housing_office);
    property_codes[8] = BuildingID (market);
    property_codes[9] = BuildingID (church);
    property_codes[10] = BuildingID (school);
    property_codes[11] = BuildingID (bank);
    property_codes[12] = BuildingID (insurance_company);
    property_codes[13] = BuildingID (natural_zone);
    return property_codes;
}

const std::unordered_map<int, BuildingID> BuildingsInfo::property_codes = BuildingsInfo::property_codes_init ();

std::unordered_map<int, BuildingAttributeTypes> BuildingsInfo::property_attributes_init () noexcept
{
    std::unordered_map<int, BuildingAttributeTypes> property_attributes;
    property_attributes[1] = earthquake_resistance;
    property_attributes[2] = moisture_resistance;
    property_attributes[3] = strength;
    property_attributes[4] = fire_resistance;
    return property_attributes;
}

const std::unordered_map<int, BuildingAttributeTypes> BuildingsInfo::property_attributes 
    = BuildingsInfo::property_attributes_init();

BuildingAttributes::BuildingAttributes (
    int earthquake_resistance,
    int moisture_resistance,
    int strength,
    int fire_resistance,
    bool insurance
) noexcept
{
    this->earthquake_resistance = earthquake_resistance;
    this->moisture_resistance = moisture_resistance;
    this->strength = strength;
    this->fire_resistance = fire_resistance;
    this->insurance = insurance;
}

Building::Building (
    BuildingTypes building_type,
    int abscissa,
    int ordinate,
    int length,
    int width,
    int earthquake_resistance,
    int moisture_resistance,
    int strength,
    int fire_resistance
) noexcept : BuildingID (
        building_type
    ),
    point (
        abscissa, 
        ordinate
    ),
    size (
        length, 
        width
    ),
    attributes (
        earthquake_resistance,
        moisture_resistance,
        strength,
        fire_resistance,
        false
    )
{
    this->point = point;
    this->size = size;
    this->insurance_building = Point ();
}

bool Building::update_attribute (
    BuildingAttributeTypes attribute_type, 
    int value, 
    bool up_or_down
) noexcept
{
    int res_value = (up_or_down) ? value : -value;
    int error = -1;
    int &attribute =
        (attribute_type == earthquake_resistance) ? this->attributes.earthquake_resistance :
        (attribute_type == moisture_resistance) ? this->attributes.moisture_resistance :
        (attribute_type == strength) ? this->attributes.strength :
        (attribute_type == fire_resistance) ? this->attributes.fire_resistance : error;
    if (attribute + res_value > BuildingsConstants::max_value_of_attributes)
    {
        return false;
    }
    else if (attribute + res_value < 0)
    {
        attribute = 0;
        return true;
    }
    attribute += res_value;
    return true;
}

std::ostream& operator<< (
    std::ostream &out,
    const BuildingAttributes &attributes
) noexcept
{
    try
    {
        return out << std::endl
            << "earthquake resistant is equals to " << attributes.earthquake_resistance << std::endl
            << "fire resistance is equals to " << attributes.fire_resistance << std::endl
            << "moisture resistant is equals to " << attributes.moisture_resistance << std::endl
            << "strength is equals to " << attributes.strength << std::endl
            << "building is " << ((!attributes.insurance) ? "not " : "") << "insured" << std::endl;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

void Building::add_to_insured (
    Point point
) //throw (std::bad_alloc)
{
    try
    {
        this->insured_buildings.insert (point);
    }
    catch (std::bad_alloc &)
    {
        ;
    }
}

std::ostream& operator<< (
    std::ostream &out,
    const Building &building
) noexcept
{
    try
    {
        out << "\nThe type of building is \""
            << BuildingsInfo::property_info.at (BuildingID(building.building_type)).get_name() << "\"."<< std::endl;
        out << "Attributes of building are the following: " << building.attributes << std::endl;
        out << "The point of building is the following: " << building.get_point() << "." << std::endl;
        out << "The size of building is the following: " << building.get_size() << "." << std::endl;
        std::set<Point> insured_buildings = building.insured_buildings;
        std::set<Point>::const_iterator iter;
        if (!insured_buildings.empty()) {
            out << "Insured buildings:" << std::endl;
            for (iter = insured_buildings.begin(); iter != insured_buildings.end(); ++iter)
            {
                out << "    " << *iter << std::endl;
            }
        }
        return out;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}