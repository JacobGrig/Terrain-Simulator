#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <unordered_map>
#include <map>
#include <new>
#include <ios>

#include "PointsOnMap.h"
#include "Cataclysms.h"
#include "Errors.h"

namespace TerrainConstants
{
    constexpr int expansion_width = 15;
	constexpr int max_number_of_extensions = 50;
}

class Terrain
{
    Size size;
protected:
    std::vector<std::vector<PointOnMap>> terrain_map;
    std::map<PointOnMap, Building> points_on_buildings;
    std::unordered_map<BuildingID, std::set<Point>, std::hash<int>> buildings_on_points;
    double tax;
    int month;
    bool check_before_build (Point point, Size size) const noexcept;
    void construct_building (Building building); //throw (InvalidArgument);
    void destroy_building (Building building); //throw (InvalidArgument);
    virtual void add_building_to_map (Building building) = 0;
    virtual void delete_building_from_map (Point point) = 0;
    virtual void send_cataclysm (Cataclysm cataclysm) = 0;
    virtual void live_month () = 0;
	void resize(int new_length = 100, int new_width = 100); //throw (std::bad_alloc);
public:
    const Size get_size () const noexcept;
    explicit Terrain (int length = 50, int width = 50, double tax = 0.2); //throw (std::bad_alloc);
	Building get_building (Point point) const; //throw (InvalidArgument);
    friend std::ostream& operator<< (std::ostream &out, const Terrain &terrain) noexcept;
	void rewidth(int new_width) //throw (std::bad_alloc) 
	{
		this->resize(this->get_size().length, new_width);
	}
	virtual ~Terrain () = default;
};