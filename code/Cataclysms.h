#pragma once

#include <ostream>
#include <ios>

#include "Geometry.h"

namespace CataclysmsConstants
{
    constexpr int max_level = 10;
    constexpr int max_radius = 25;
    constexpr double cataclysm_probability = 0.5;
}

enum CataclysmTypes
{
    earthquake,
    flood,
    hurricane,
    fire,
    cataclysm_types_number
};

class CataclysmID
{
protected:
    CataclysmTypes cataclysm_type;
public:
    CataclysmID (CataclysmTypes cataclysm_type = earthquake) noexcept;
    CataclysmID (int type) noexcept;
    CataclysmTypes get_cataclysm_type () const noexcept;
};

class Cataclysm : public CataclysmID
{
    int level;
    int radius;
    Point point;
public:
    Cataclysm (
        int level = 1,
        int radius = 1,
        Point point = Point (),
        CataclysmID cataclysm_type = CataclysmID (earthquake)
    ) noexcept;
    int get_level() const noexcept { return this->level; }
    int get_radius () const noexcept { return this->radius; }
    Point get_point () const noexcept { return this->point; }
    friend std::ostream& operator<< (std::ostream &out, const Cataclysm& cataclysm) noexcept;
};