#include "Cataclysms.h"

CataclysmID::CataclysmID (
    CataclysmTypes cataclysm_type
) noexcept
{
    this->cataclysm_type = cataclysm_type;
}

CataclysmID::CataclysmID (
    int type
) noexcept
{
    this->cataclysm_type = static_cast<CataclysmTypes> (type);
}

CataclysmTypes CataclysmID::get_cataclysm_type () const noexcept
{
    return this->cataclysm_type;
}

Cataclysm::Cataclysm (
    int level,
    int radius,
    Point point,
    CataclysmID cataclysm_id
) noexcept : CataclysmID (cataclysm_id)
{
    this->level = level;
    this->radius = radius;
    this->point = point;
}

std::ostream& operator<< (
    std::ostream &out,
    const Cataclysm& cataclysm
) noexcept
{
    try
    {
        out << "The level of cataclysm is equals to " << cataclysm.get_level() << "." << std::endl;
        out << "The radius of cataclysm is equals to " << cataclysm.get_radius() << "." << std::endl;
        out << "The point of cataclysm is " << cataclysm.get_point() << "." << std::endl;
        const CataclysmTypes type = cataclysm.get_cataclysm_type();
        out << "The type of cataclysm is \""
            << ((type == earthquake) ? "EARTHQUAKE" :
				(type == flood) ? "FLOOD" :
                (type == hurricane) ? "HURRICANE" :
                (type == fire) ? "FIRE" : "ERROR") << "\"." << std::endl;
        return out;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}