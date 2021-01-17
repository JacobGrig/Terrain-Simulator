#include "Locality.h"

LocalityAttributes::LocalityAttributes (
    long budget,
    long population,
    double security,
    double education,
    double health,
    double food,
    double welfare,
    double ecology,
    double spirituality, 
    double contentment
) noexcept
{
    this->budget = budget;
    this->population = population;
    this->security = security;
    this->education = education;
    this->health = health;
    this->food = food;
    this->welfare = welfare;
    this->ecology = ecology;
    this->spirituality = spirituality;
    this->contentment = contentment;
}

void small_change (
    double &number,
    double factor
) noexcept
{
    number = get_random (
        static_cast<int> (number * (1 + factor) + 1),
        static_cast<int> (number * (1 - factor))
    );
}

std::ostream& operator<< (
    std::ostream &out,
    const LocalityAttributes &attributes
) noexcept
{
    try
    {
        out << "budget = " << attributes.budget << std::endl;
        out << "contentment = " << attributes.contentment << std::endl;
        out << "ecology = " << attributes.ecology << std::endl;
        out << "education = " << attributes.education << std::endl;
        out << "food = " << attributes.food << std::endl;
        out << "health = " << attributes.health << std::endl;
        out << "population = " << attributes.population << std::endl;
        out << "security = " << attributes.security << std::endl;
        out << "spirituality = " << attributes.spirituality << std::endl;
        out << "welfare = " << attributes.welfare << std::endl;
        return out;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

void LocalityAttributes::get_changes (
    double factor
) noexcept
{
    small_change (this->ecology, factor);
    small_change (this->education, factor);
    small_change (this->food, factor);
    small_change (this->health, factor);
    small_change (this->security, factor);
    small_change (this->spirituality, factor);
    small_change (this->welfare, factor);
}

Locality::Locality (
    long budget,
    long population,
    double security,
    double education,
    double health,
    double food,
    double welfare,
    double ecology,
    double spirituality,
    double contentment
) noexcept : attributes (
        budget,
        population,
        security,
        education,
        health,
        food,
        welfare,
        ecology,
        spirituality,
        contentment
    )
{
    Size size = this->get_size ();
    for (int ordinate = 0; ordinate < size.width; ++ordinate) {
        for (int abscissa = 0; abscissa < size.length; ++abscissa) {
            this->points_on_buildings[PointOnMap(abscissa, ordinate)] = Building ();
        }
    }
    this->affordable_insurance = 0;
    this->accessible_expansion = 0;
}

void Locality::add_building_to_map (
    Building building
) //throw (InvalidArgument, std::bad_alloc)
{
    this->construct_building (building);
    PointOnMap point_on_map (
        building.get_point ().abscissa,
        building.get_point ().ordinate,
        BuildingID(building.get_building_type ())
    );
    this->points_on_buildings[point_on_map] = building;
    this->buildings_on_points[building].insert(point_on_map);
    switch (building.get_building_type())
    {
    case city_center:
        this->accessible_expansion += TerrainConstants::expansion_width;
        break;
    case house:
        this->capacity += BuildingsConstants::capacity_of_house;
        break;
    case hospital:
        this->update_health(true);
        break;
    case parkland:
        break;
    case police:
    case court:
        this->update_security(true);
        break;
    case housing_office:
        break;
    case market:
        this->update_food(true);
        break;
    case church:
        this->update_spirituality(true);
        break;
    case school:
        this->update_education(true);
        break;
    case bank:
        break;
    case insurance_company:
        this->affordable_insurance += LocalityConstants::max_number_of_insured_buildings;
        break;
    case natural_zone:
        this->update_ecology(true);
        break;
    case no_building:
        throw InvalidArgument ("no_building", "Locality::add_building_to_map ()");
        break;
    default:
        throw InvalidArgument ("default", "Locality::add_building_to_map ()");
        break;
    }
}

void Locality::delete_building_from_map (
    Point point
) //throw (InvalidArgument)
{
    PointOnMap point_on_map = terrain_map[point.ordinate][point.abscissa];
    Building building = points_on_buildings[point_on_map];
    this->destroy_building(building);
    this->points_on_buildings.erase(point_on_map);
    this->buildings_on_points[building].erase(point_on_map);
    if (this->buildings_on_points[building].empty())
    {
        this->buildings_on_points.erase(building);
    }
    switch (building.get_building_type())
    {
    case city_center:
        this->accessible_expansion -= TerrainConstants::expansion_width;
        break;
    case house:
        this->capacity -= BuildingsConstants::capacity_of_house;
        break;
    case hospital:
        this->update_health(false);
        break;
    case parkland:
        break;
    case police:
    case court:
        this->update_security(false);
        break;
    case housing_office:
        break;
    case market:
        this->update_food(false);
        break;
    case church:
        this->update_spirituality(false);
        break;
    case school:
        this->update_education(false);
        break;
    case bank:
        break;
    case insurance_company:
        for (
            std::set<Point>::const_iterator iter = building.get_insured_buildings().cbegin();
            iter != building.get_insured_buildings().cend();
            ++iter
            )
        {
            int abscissa = (*iter).abscissa;
            int ordinate = (*iter).ordinate;
            PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];
            Building &local_building = points_on_buildings[point_on_map];
            local_building.set_default_point_of_insurance();
            local_building.set_uninsured();
            ++this->affordable_insurance;
        }
        this->affordable_insurance -= LocalityConstants::max_number_of_insured_buildings;
        break;
    case natural_zone:
        this->update_ecology(false);
        break;
    case no_building:
        break;
    default:
        throw InvalidArgument ("default", "Locality::delete_building_from_map");
        break;
    }
    if (building.is_insured ())
    {
        Point point = building.get_point_of_insurance ();
        PointOnMap point_on_map = terrain_map[point.ordinate][point.abscissa];
        Building &local_building = points_on_buildings[point_on_map];
        local_building.delete_from_insured(building.get_point());
        ++this->affordable_insurance;
    }
}

const std::set<Point> &Locality::get_addresses (
    BuildingID building_id
) const //throw (std::out_of_range)
{
    return (this->buildings_on_points).at(building_id);
}

void Locality::send_cataclysm (
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
            if (Point(abscissa, ordinate).is_in_circle(center, radius))
            {
                PointOnMap point_on_map = this->terrain_map[ordinate][abscissa];
                if (point_on_map.get_building_type () != BuildingID ())
                {
                    Building &building = this->points_on_buildings[point_on_map];
                    if (building.is_insured())
                    {
                        BuildingID building_type = BuildingID(building.get_building_type ());
                        long insurance_payment = static_cast<long> (
                            BuildingsConstants::factor_of_insurance *
                            BuildingsInfo::property_info.at (building_type).get_price ()
                        );
                        this->update_budget_with_concrete_value (insurance_payment);

                        Point point = building.get_point_of_insurance ();
                        PointOnMap point_on_map = terrain_map[point.ordinate][point.abscissa];
                        Building &local_building = points_on_buildings[point_on_map];
                        this->uninsure (building, local_building);
                    }
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
                        this->delete_building_from_map(building.get_point());
                    }
                }
            }
        }
    }
}

void Locality::insure (
    Building &insured_building,
    Building &insurance_building
) //throw (InvalidArgument, UnavailableOperation, std::bad_alloc)
{
    if (insurance_building.get_building_type() != insurance_company)
    {
        throw InvalidArgument ("insurance_building", "Locality::insure ()");
    }
    else if (insurance_building.get_insured_buildings().size() >= LocalityConstants::max_number_of_insured_buildings)
    {
        throw UnavailableOperation ("insure in this insurance company");
    }
    else
    {
        insured_building.set_insured ();
        insured_building.set_point_of_insurance (insurance_building.get_point());

        insurance_building.add_to_insured (insured_building.get_point());
    }
}

void Locality::uninsure (
    Building &insured_building,
    Building &insurance_building
) //throw (InvalidArgument)
{
    if (insurance_building.get_building_type() != insurance_company)
    {
        throw InvalidArgument ("insurance_building", "Locality::uninsure ()");
    }
    else if (insurance_building.get_insured_buildings().size() == 0)
    {
        throw InvalidArgument ("insured_building", "Locality::uninsure ()");
    }
    else
    {
        insured_building.set_uninsured ();
        insured_building.set_default_point_of_insurance ();

        insurance_building.delete_from_insured (insured_building.get_point ());
    }
}

void Locality::send_random_cataclysm () noexcept
{
    Cataclysm cataclysm = get_random_cataclysm ();
	try
	{
		std::cout << cataclysm;
	}
	catch (std::ios_base::failure &)
	{
		;
	}
    this->send_cataclysm (cataclysm);
}

void Locality::send_or_not_send_random_cataclysm (
    double probability
) noexcept
{
    if (decide (probability))
    {
        this->send_random_cataclysm ();
    }
}

std::ostream& operator<< (
    std::ostream &out,
    const std::set<Point>& points_on_map
) noexcept
{
    try
    {
        std::set<Point>::const_iterator iter;
        for (iter = points_on_map.cbegin(); iter != points_on_map.cend(); ++iter) {
            out << "    " << *iter << std::endl;
        }
        return out;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}

void Locality::set_contentment () noexcept
{
    this->attributes.contentment =
        this->attributes.ecology +
        this->attributes.education +
        this->attributes.food +
        this->attributes.health +
        this->attributes.security +
        this->attributes.spirituality;
}

void Locality::set_population () noexcept
{
	this->attributes.population = static_cast<long> (
		this->capacity *
		this->attributes.contentment / LocalityConstants::max_value_of_factors
    );
}

void Locality::set_ecology () noexcept
{
    try
    {
        this->attributes.ecology =
            this->get_addresses(BuildingID(natural_zone)).size() *
            BuildingsConstants::density_of_nature;
    }
    catch (std::out_of_range &)
    {
        this->attributes.ecology = 0;
    }
    if (this->attributes.ecology > LocalityConstants::max_value_of_factors)
    {
        this->attributes.ecology = LocalityConstants::max_value_of_factors;
    }
}

void Locality::set_education () noexcept
{
    try
    {
        this->attributes.education =
            this->get_addresses(BuildingID(school)).size() *
            BuildingsConstants::knowledge_density;
    }
    catch (std::out_of_range &)
    {
        this->attributes.education = 0;
    }
    if (this->attributes.education > LocalityConstants::max_value_of_factors)
    {
        this->attributes.education = LocalityConstants::max_value_of_factors;
    }
}

void Locality::set_food () noexcept
{
    try
    {
        this->attributes.food =
            this->get_addresses(BuildingID(market)).size() *
            BuildingsConstants::food_density;
    }
    catch (std::out_of_range &)
    {
        this->attributes.food = 0;
    }
    if (this->attributes.food > LocalityConstants::max_value_of_factors)
    {
        this->attributes.food = LocalityConstants::max_value_of_factors;
    }
}

void Locality::set_health () noexcept
{
    try
    {
        this->attributes.health =
            this->get_addresses (BuildingID(hospital)).size() *
            BuildingsConstants::density_of_doctors;
    }
    catch (std::out_of_range &)
    {
        this->attributes.health = 0;
    }
    if (this->attributes.health > LocalityConstants::max_value_of_factors)
    {
        this->attributes.health = LocalityConstants::max_value_of_factors;
    }
}

void Locality::set_security () noexcept
{
    try
    {
        this->attributes.security = (
            this->get_addresses(BuildingID(police)).size() +
            this->get_addresses(BuildingID(court)).size()
        ) * BuildingsConstants::fairness;
    }
    catch (std::out_of_range &)
    {
        try
        {
            this->attributes.security = (
                this->get_addresses (BuildingID (police)).size()
            ) * BuildingsConstants::fairness;
        }
        catch (std::out_of_range &)
        {
            this->attributes.security = 0;
        }
    }
    if (this->attributes.security > LocalityConstants::max_value_of_factors)
    {
        this->attributes.security = LocalityConstants::max_value_of_factors;
    }
}

void Locality::set_spirituality() noexcept
{
    try
    {
        this->attributes.spirituality =
            this->get_addresses(BuildingID(church)).size() *
            BuildingsConstants::density_of_spirituality;
    }
    catch (std::out_of_range &)
    {
        this->attributes.spirituality = 0;
    }
    if (this->attributes.spirituality > LocalityConstants::max_value_of_factors)
    {
        this->attributes.spirituality = LocalityConstants::max_value_of_factors;
    }
}

void Locality::update_ecology (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::density_of_nature : -BuildingsConstants::density_of_nature;
    double &factor = this->attributes.ecology;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::update_education (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::knowledge_density : -BuildingsConstants::knowledge_density;
    double &factor = this->attributes.education;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::update_food (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::food_density : -BuildingsConstants::food_density;
    double &factor = this->attributes.food;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::update_health (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::density_of_doctors : -BuildingsConstants::density_of_doctors;
    double &factor = this->attributes.health;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::update_security (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::fairness : -BuildingsConstants::fairness;
    double &factor = this->attributes.security;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::update_spirituality (
    bool up_or_down
) noexcept
{
    int value = (up_or_down) ? BuildingsConstants::density_of_spirituality : -BuildingsConstants::density_of_spirituality;
    double &factor = this->attributes.spirituality;
    if (factor + value > LocalityConstants::max_value_of_factors)
    {
        factor = LocalityConstants::max_value_of_factors;
    }
    else if (factor + value < 0)
    {
        factor = 0;
    }
    else
    {
        factor += value;
    }
}

void Locality::set_welfare () noexcept
{
    int result_welfare = 0;
    std::unordered_map<BuildingID, BuildingTypeInfo, std::hash<int>>::const_iterator iter;
    for (
        iter = BuildingsInfo::property_info.cbegin();
        iter != BuildingsInfo::property_info.cend();
        iter++
    )
    {
        if ((*iter).first != BuildingID (house) && (*iter).first != BuildingID ())
        {
            try
            {
                result_welfare +=
                    this->get_addresses((*iter).first).size() *
                    (*iter).second.get_min_size().area();
            }
            catch (std::out_of_range &)
            {
                ;
            }
        }
    }
    this->attributes.welfare = result_welfare * LocalityConstants::wage_density;
}

void Locality::update_budget_in_general () noexcept
{
    int parkland_size = 0;
    int housing_size = 0;
    int bank_size = 0;
    try
    {
        parkland_size = this->get_addresses(BuildingID(parkland)).size();
    }
    catch (std::out_of_range &)
    {
        ;
    }
    try
    {
        housing_size = this->get_addresses(BuildingID(housing_office)).size();
    }
    catch (std::out_of_range &)
    {
        ;
    }
    try
    {
        bank_size = this->get_addresses(BuildingID(bank)).size();
    }
    catch (std::out_of_range &)
    {
        ;
    }
    this->attributes.budget += static_cast<long> (
        tax *
        this->attributes.welfare *
        this->attributes.population
      + parkland_size * BuildingsConstants::parking_charge *
            get_probability (BuildingsConstants::right_limit_of_banking, BuildingsConstants::left_limit_of_banking)
      + housing_size * BuildingsConstants::ñost_of_housing_services *
            get_probability (BuildingsConstants::right_limit_of_banking, BuildingsConstants::left_limit_of_banking)
      + bank_size * BuildingsConstants::amount_of_loans *
            get_probability (BuildingsConstants::right_limit_of_banking, BuildingsConstants::left_limit_of_banking)
    );
}

void Locality::update_budget_with_concrete_value (
    int value
) //throw (BurstingBudget)
{
    if (this->attributes.budget + value < 0)
    {
        throw BurstingBudget ();
    }
    this->attributes.budget += value;
}

void Locality::live_month () noexcept
{
    ++this->month;
    this->send_or_not_send_random_cataclysm (CataclysmsConstants::cataclysm_probability);
    this->update_budget_in_general ();
    this->attributes.get_changes (LocalityConstants::changing_factor);
    if (month % 12 == 0)
    {
        this->set_ecology ();
        this->set_education ();
        this->set_food ();
        this->set_health ();
        this->set_security ();
        this->set_spirituality ();
    }
    this->set_welfare ();
    this->set_contentment ();
    this->set_population ();
}

std::ostream& operator<< (
    std::ostream &out,
    const Locality &locality
) noexcept
{
    try
    {
        const Terrain &locality_as_terrain = locality;
        out << locality_as_terrain << std::endl;
        out << "budget = " << locality.attributes.budget << std::endl;
        out << "contentment = " << locality.attributes.contentment << std::endl;
        out << "ecology = " << locality.attributes.ecology << std::endl;
        out << "education = " << locality.attributes.education << std::endl;
        out << "food = " << locality.attributes.food << std::endl;
        out << "health = " << locality.attributes.health << std::endl;
        out << "population = " << locality.attributes.population << std::endl;
        out << "security = " << locality.attributes.security << std::endl;
        out << "spirituality = " << locality.attributes.spirituality << std::endl;
        out << "welfare = " << locality.attributes.welfare << std::endl;
        return out << std::endl;
    }
    catch (std::ios_base::failure &)
    {
        return out;
    }
}