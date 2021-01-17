#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <ostream>
#include <algorithm>
#include <functional>
#include <exception>

#include "Geometry.h"

namespace BuildingsConstants 
{
    constexpr int capacity_of_house = 500;
    constexpr int density_of_workplaces = 100;
    constexpr int density_of_nature = 10;
    constexpr int knowledge_density = 5;
    constexpr int food_density = 5;
    constexpr int density_of_doctors = 7;
    constexpr int fairness = 2;
    constexpr int density_of_spirituality = 5;
    constexpr int parking_charge = 10000;
    constexpr int ñost_of_housing_services = 10000;
    constexpr double right_limit_of_banking = 1.0;
    constexpr double left_limit_of_banking = 0.5;
    constexpr double factor_of_insurance = 3;
    constexpr double cost_of_insurance = 0.5;
    constexpr int amount_of_loans = 10000;
    constexpr int max_value_of_attributes = 100;
}

enum BuildingTypes
{
    no_building,
    city_center,
    house,
    hospital,
    parkland,
    police,
    court,
    housing_office,
    market,
    church,
    school,
    bank,
    insurance_company,
    natural_zone,
    number_of_building_types
};

class BuildingID
{
protected:
    BuildingTypes building_type;
public:
    explicit BuildingID (BuildingTypes building_type = no_building) noexcept;
    operator int () const noexcept { return building_type; }
    bool operator== (const BuildingID &building_id) const noexcept;
    bool operator< (const BuildingID &building_id) const noexcept;              // neñessery for map
    BuildingTypes get_building_type () const noexcept;
};

class BuildingTypeInfo
{
    long price;
    std::string name;
    Size min_size;
public:
    explicit BuildingTypeInfo (
        long price = 10000,
        std::string name = "HOUSE",
        Size min_size = Size (1, 1)
    ) noexcept : price (price),
        name (name),
        min_size (min_size)
    {
        ;
    }
    long get_price () const noexcept { return this->price; }
    const std::string& get_name () const noexcept { return this->name; }
    const Size& get_min_size () const noexcept { return this->min_size; }
};

enum BuildingAttributeTypes
{
    earthquake_resistance,
    moisture_resistance,
    strength,
    fire_resistance
};

class BuildingsInfo 
{
public:
    static std::unordered_map<BuildingID, BuildingTypeInfo, std::hash<int>> property_init () noexcept;
    static const std::unordered_map<BuildingID, BuildingTypeInfo, std::hash<int>> property_info;
    static const std::string property_prices;
    static std::unordered_map<int, BuildingID> property_codes_init () noexcept;
    static const std::unordered_map<int, BuildingID> property_codes;
    static std::unordered_map<int, BuildingAttributeTypes> property_attributes_init () noexcept;
    static const std::unordered_map<int, BuildingAttributeTypes> property_attributes;

};

class BuildingAttributes
{
public:
    int earthquake_resistance;
    int moisture_resistance;
    int strength;
    int fire_resistance;
    bool insurance;
    explicit BuildingAttributes (
        int earthquake_resistance = 10,
        int moisture_resistance = 10,
        int strength = 10,
        int fire_resistance = 10,
        bool insurance = false
    ) noexcept;
    friend std::ostream& operator<< (std::ostream &out, const BuildingAttributes &attributes) noexcept;
};

class Building : public BuildingID
{
    Point point;
    Size size;
    std::set<Point> insured_buildings;
    Point insurance_building;
public:
    BuildingAttributes attributes;
    explicit Building (
        BuildingTypes building_type = no_building,
        int abscissa = 0,
        int ordinate = 0,
        int length = 0,
        int width = 0,
        int earthquake_resistance = 0,
        int moisture_resistance = 0,
        int strength = 0,
        int fire_resistance = 0
    ) noexcept;
    bool update_attribute (BuildingAttributeTypes attribute_type, int value, bool up_or_down) noexcept;
    const Point& get_point () const noexcept { return this->point; }
    const Point& get_point_of_insurance () const noexcept { return this->insurance_building; }
    const Size& get_size () const noexcept { return this->size; }
    const std::set<Point>& get_insured_buildings () const noexcept { return insured_buildings; }
    bool is_insured () const noexcept { return this->attributes.insurance; }
    void set_insured () noexcept { this->attributes.insurance = true; }
    void set_uninsured () noexcept { this->attributes.insurance = false; }
    void add_to_insured (Point point); //throw (std::bad_alloc);
    void delete_from_insured (Point point) noexcept { this->insured_buildings.erase (point); }
    // If there is no such element in the set, the function erase () returns zero.
    void set_point_of_insurance (Point point) noexcept { this->insurance_building = point; }
    void set_default_point_of_insurance () noexcept { set_point_of_insurance (Point ()); }
    friend std::ostream& operator<< (std::ostream &out, const Building &building) noexcept;
};