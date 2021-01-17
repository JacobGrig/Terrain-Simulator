#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <ios>
#include <stdexcept>
#include <new>

#include "Terrain.h"
#include "Randomness.h"
#include "Errors.h"

namespace LocalityConstants
{
    constexpr int cost_of_builders = 100;
    constexpr double changing_factor = 0.2;
    constexpr int max_number_of_insured_buildings = 15;
    constexpr int max_value_of_factors = 100;
    constexpr int wage_density = 100;
}

void small_change (double &number, double factor) noexcept;

class LocalityAttributes
{
public:
    long budget;
    long population;

    double security;
    double education;
    double health;
    double food;
    double welfare;
    double ecology;
    double spirituality;
    double contentment;

    explicit LocalityAttributes (
        long budget = 1000000,
        long population = 0,
        double security = 0,
        double education = 0,
        double health = 0,
        double food = 0,
        double welfare = 0,
        double ecology = 0,
        double spirituality = 0,
        double contentment = 0
    ) noexcept;

    void get_changes (double factor) noexcept;
};

class Locality : public Terrain
{
protected:
    LocalityAttributes attributes;
    int capacity;
	int affordable_insurance;
	int accessible_expansion;
	int parkland_area;

    std::unordered_map<BuildingID, std::set<Point>, std::hash<int>> buildings_on_points;
    void send_cataclysm (Cataclysm cataclysm) noexcept override final;
    void send_random_cataclysm () noexcept;
	void insure (Building &insured_building, Building &insurance_building);
	//throw (InvalidArgument, UnavailableOperation, std::bad_alloc);
	void uninsure (Building &insured_building, Building &insurance_building); //throw (InvalidArgument);
	void send_or_not_send_random_cataclysm (double probability) noexcept;
	void set_contentment() noexcept;
	void set_population() noexcept;

	void set_ecology() noexcept;
	void set_education() noexcept;
	void set_food() noexcept;
	void set_health() noexcept;
	void set_security() noexcept;
	void set_spirituality() noexcept;

	void update_ecology(bool up_or_down) noexcept;
	void update_education(bool up_or_down) noexcept;
	void update_food(bool up_or_down) noexcept;
	void update_health(bool up_or_down) noexcept;
	void update_security(bool up_or_down) noexcept;
	void update_spirituality(bool up_or_down) noexcept;

	void set_welfare() noexcept;

	void update_budget_in_general () noexcept;
	void update_budget_with_concrete_value(int value); //throw (BurstingBudget);
public:
    explicit Locality (
        long budget = 1000000,
        long population = 0,
        double security = 0,
        double education = 0,
        double health = 0,
        double food = 0,
        double welfare = 0,
        double ecology = 0,
        double spirituality = 0,
        double contentment = 0
    ) noexcept;
	void add_building_to_map (Building building) override final; //throw (InvalidArgument, std::bad_alloc) ;
	void delete_building_from_map (Point point) override final; //throw (InvalidArgument) ;
    const std::set<Point>& get_addresses (BuildingID building_id) const; //throw (std::out_of_range);

    void live_month () noexcept override final;
    friend std::ostream& operator<< (std::ostream &out, const Locality &locality) noexcept;
};