#pragma once

#include <memory>

#include "City.h"
#include "UnlimitedCity.h"
#include "Desert.h"
#include "Interface.h"

class UserInteraction
{
public:
    virtual void start () = 0;
    virtual void request_handler () = 0;
	virtual ~UserInteraction () = default;
};

class UserInteractionCity : public UserInteraction
{
    City city;
public:
    UserInteractionCity (City &city) noexcept { this->city = city; }
    void start () override; //throw (UnavailableOperation) ;
    void request_handler () override; //throw (UnavailableOperation) ;
};

class UserInteractionUnlimitedCity : public UserInteraction
{
    UnlimitedCity unlimited_city;
public:
    UserInteractionUnlimitedCity (UnlimitedCity &unlimited_city) 
    {
        this->unlimited_city = unlimited_city; 
    }
    void start () override; //throw (UnavailableOperation) ;
    void request_handler () override; //throw (UnavailableOperation) ;
};

class UserInteractionDesert : public UserInteraction
{
    Desert desert;
public:
    UserInteractionDesert (Desert &desert)
    {
        this->desert = desert;
    }
    void start () override; //throw (UnavailableOperation) ;
    void request_handler () override; //throw (UnavailableOperation) ;
};

void start (); //throw (UnavailableOperation);
void create_terrain (); //throw (UnavailableOperation);
void exit () noexcept;