#pragma once

#include <new>

#include "Terrain.h"
#include "Randomness.h"

namespace DesertConstants
{
    constexpr int primary_divine = 15000;
    constexpr int low_limit_of_divine_addition = 1000;
    constexpr int high_limit_of_divine_addition = 10000;
    constexpr unsigned max_number_of_constructed_buildings = 10;
    constexpr int divine_power_per_unit = 100;
}

class Desert : public Terrain
{
    int divine_power;

    void add_building_to_map (Building building); //throw (InvalidArgument, std::bad_alloc) override;
    void delete_building_from_map (Point point); //throw (InvalidArgument) override;

    void update_divine () noexcept;
    void send_cataclysm (Cataclysm cataclysm) noexcept override;
	void expand_area (int number_of_rows);
public:
    void send_catalysm_with_divine_power (Cataclysm cataclysm); //throw (BurstingDivine);
    Desert () noexcept : divine_power (DesertConstants::primary_divine) { ; }
    void add_random_building_to_map (); //throw (InvalidArgument);

    void live_month () noexcept override;
    friend std::ostream& operator<< (std::ostream &out, const Desert &desert) noexcept;
};