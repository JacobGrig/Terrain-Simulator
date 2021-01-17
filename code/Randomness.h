#pragma once

#include <cstdlib>
#include <ctime>

#include "Geometry.h"
#include "Cataclysms.h"
#include "Buildings.h"

int get_random (int right_limit = RAND_MAX, int left_limit = 0) noexcept;
Point get_random_point (Size size = Size ()) noexcept;
CataclysmID get_random_cataclysm_id () noexcept;
Cataclysm get_random_cataclysm (Size size = Size ()) noexcept;
BuildingTypes get_random_building_type () noexcept;
Building get_random_building (Size size = Size ()) noexcept;
bool decide (double probability) noexcept;
double get_probability (double right_limit = 1.0, double left_limit = 0.0) noexcept;