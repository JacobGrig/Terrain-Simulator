#include "UserInteraction.h"

using namespace std;

void start () //throw (UnavailableOperation)
{
    int operation = -1;
    string begin_string = 
        "\nHello, my dear! Do you want to create a terrain?\n"
        "\n"
        "Choose one of the next items:\n"
        "\n"
        "    1. Yes, of course! Create the best terrain for me, please!\n"
        "    2. No, why do I need all this? Exit the program and do not touch me anymore...\n"
        "\n"
        "    Entry: ";
    string repeat_string = 
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "    1. Create the best terrain for me, please!\n"
        "    2. Exit the program and do not touch me anymore...\n"
        "\n"
        "    Entry: ";
    help_with_interface (begin_string, repeat_string, operation, 1, 2);
    switch (operation) {
    case 1:
        create_terrain();
        break;
    case 2:
        exit ();
        break;
    default:
        throw UnavailableOperation ("code " + to_string (operation));
    }
}

void create_terrain () //throw (UnavailableOperation)
{
    int operation = -1;
    string begin_string =
        "\nSo, you chose to create a terrain. This is the right decision,\n"
        "it characterizes you as wise and sensitive human,\n"
        "and rest assured that it's still very useful. Let's start!\n"
        "\n"
        "Choose the most suitable type of terrain for yourself: \n"
        "\n"
        "    1. City!\n"
        "    2. City with unlimited budget!\n"
        "    3. Desert (God' mode)!\n"
        "\n"
        "    Entry: ";

    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "    1. City!\n"
        "    2. City with unlimited budget!\n"
        "    3. Desert (God' mode)!\n"
        "\n"
        "    Entry: ";
    help_with_interface (begin_string, repeat_string, operation, 1, 3);
    shared_ptr<UserInteraction> user_terrain;
    switch (operation)
    {
    case 1:
        user_terrain = make_shared<UserInteractionCity> (City ());
        break;
    case 2:
        user_terrain = make_shared<UserInteractionUnlimitedCity> (UnlimitedCity ());
        break;
    case 3:
        user_terrain = make_shared<UserInteractionDesert> (Desert ());
        break;
    default:
        throw UnavailableOperation ("terrain type");
        break;
    }
    user_terrain->start ();
}

void exit () noexcept
{
    cout << "\nHow sad that you're leaving us so soon... We are waiting for you eagerly\n"
            "the next time and wish you to get ready for the nearest exam session perfectly well!\n\n";
}

void UserInteractionCity::start () //throw (UnavailableOperation)
{
    int operation = -1;
    Size size_of_city_center = BuildingsInfo::property_info.at (BuildingID(city_center)).get_min_size ();

    Point point_of_city_center (
        (city.get_size().length - size_of_city_center.length) / 2,
        (city.get_size().width - size_of_city_center.width) / 2
    );

    Building building (
        city_center,
        point_of_city_center.abscissa,
        point_of_city_center.ordinate,
        size_of_city_center.length,
        size_of_city_center.width
    );

    this->city.add_building_to_map (building);

    string begin_string =
        "\nYou chose the \"city\" mode. Congratulations, \n"
        "in the near future you will plunge into the atmosphere \n"
        "of urban development. Imagine that you are the mayor of\n"
        "the city. Now it consists only of the city center \n"
        "in which your office is located, and no one in your \n"
        "city lives, but as more buildings are built and the city \n"
        "develops, more and more people will fill it. Your task is \n"
        "to build a successful and rich city. So, let's begin! \n"
        "\n"
        "Now I give you time to build your city. Remember that \n"
        "our people are very demanding, and in the absence of the \n"
        "building they need, they will not settle in our city. \n"
        "\n"
        "The following list of factors will help you find your bearings: \n"
        "    1. Security\n"
        "    2. Education\n"
        "    3. Health\n"
        "    4. Food\n"
        "    5. Ecology\n"
        "    6. Spirituality\n"
        "\n"
        "Here is a list of buildings that you can build right now, \n"
        "the price each of them: \n" + BuildingsInfo::property_prices + 
        "\n"
        "\n    Select one of them: ";
    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "List of buildings that you can build right now, \n"
        "the price and size of each of them: \n" + BuildingsInfo::property_prices +
        "\n"
        "\n    Select one of them: ";
    help_with_interface (begin_string, repeat_string, operation, 1, 13);
    BuildingID building_type = BuildingsInfo::property_codes.at(operation);
    cout << "\nThe size of the selected building is as follows:\n"
        << BuildingsInfo::property_info.at (building_type).get_min_size ();

    int abscissa, ordinate;

    begin_string =
        "\nNow select the point on the map: \n"
        "    1. x = ";
    repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "Select the point on the map: \n"
        "    1. x = ";
    help_with_interface (begin_string, repeat_string, abscissa, 0, this->city.get_size ().length);
    begin_string =
        "    2. y = ";
    repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "Select the point on the map: \n"
        "    2. y = ";
    help_with_interface (begin_string, repeat_string, ordinate, 0, this->city.get_size ().width);
    try
    {
        this->city.construct_building_with_fee (building_type, Point (abscissa, ordinate));
    }
    catch (GeneralException &exception)
    {
        cout << endl << exception.what () << endl;
    }
    catch (...)
    {
        cout << "\nUnknown error in the function UserInteractionCity::start ()";
    }
    cout << "\nNow you know how you can manage your city, \n"
        << "on this your training is over, enjoy!\n";
    this->request_handler ();
}

void UserInteractionCity::request_handler () //throw (UnavailableOperation)
{
    int operation = -1;
    int abscissa = -1;
    int ordinate = -1;

    string begin_string =
        "\nSelect one of the next items:\n"
        "\n"
        "    1. Print all information about my city.\n"
        "    2. Construct building.\n"
        "    3. Destroy building.\n"
        "    4. Insure building.\n"
        "    5. Expand the territory of the city.\n"
        "    6. Update building.\n"
        "    7. Print all information about building.\n"
        "    8. Go for next month.\n"
        "    9. Exit the program. \n"
        "\n"
        "    Entry: ";
    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
		"Select one of the next items:\n"
        "\n"
        "    1. Print all information about my city.\n"
        "    2. Construct building.\n"
        "    3. Destroy building.\n"
        "    4. Insure building.\n"
        "    5. Expand the territory of the city.\n"
        "    6. Update building.\n"
        "    7. Print all information about building.\n"
        "    8. Go for next month.\n"
        "    9. Exit the program.\n"
        "\n"
        "    Entry: ";
    help_with_interface (begin_string, repeat_string, operation, 1, 9);

    BuildingID building_type;
    set<Point> addresses;
    BuildingAttributeTypes attribute_type;
    int value;

    switch (operation)
    {
    case 1:
        cout << this->city;
        break;
    case 2:
        begin_string =
            "\nHere is a list of buildings that you can build right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can build right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        cout << "\nThe size of the selected building is as follows:\n"
            << BuildingsInfo::property_info.at(building_type).get_min_size() << endl;

        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->city.get_size().width);
        try
        {
            this->city.construct_building_with_fee (building_type, Point (abscissa, ordinate));
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 3:
        begin_string =
            "\nHere is a list of building types that you can destroy right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "The cost of the destruction of the building consists only of the cost of the builders.\n"
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can destroy right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
            )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        try
        {
            this->city.destroy_building_with_fee(Point(abscissa, ordinate));
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 4:
        begin_string =
            "\nHere is a list of building types that you can insure right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "The cost of insurance is " + to_string (BuildingsConstants::cost_of_insurance) + " of the value of the building."
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can insure right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface (begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->city.get_addresses (building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin ();
            iter != addresses.cend ();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        try
        {
            this->city.insure_with_fee (Point (abscissa, ordinate));
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what ();
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
		break;
    case 5:
        begin_string =
            "\nEnter the number of lines to expand the city to (from 1 to " +
            to_string (TerrainConstants::max_number_of_extensions) + "): ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Enter the number of lines to expand the city to (from 1 to " +
            to_string (TerrainConstants::max_number_of_extensions) + "): ";
        help_with_interface (begin_string, repeat_string, operation, 1, TerrainConstants::max_number_of_extensions);
        try
        {
            this->city.expand_area_with_fee (operation);
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
        break;
    case 6:
        begin_string =
            "\nHere is a list of building types that you can update right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can update right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        begin_string =
            "\nNow you must select the type of update:\n"
            "    1. Earthquake resistance.\n"
            "    2. Moisture resistance. \n"
            "    3. Strength.\n"
            "    4. Fire resistance.\n"
            "\n"
            "    Entry: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the type of update:\n"
            "    1. Earthquake resistance.\n"
            "    2. Moisture resistance. \n"
            "    3. Strength.\n"
            "    4. Fire resistance.\n"
            "\n"
            "    Entry: ";
        help_with_interface (begin_string, repeat_string, operation, 1, 4);
        attribute_type = BuildingsInfo::property_attributes.at (operation);
        begin_string =
            "\nEnter the number of points to which you want to improve the attribute.\n"
            "(from 1 to " + to_string (BuildingsConstants::max_value_of_attributes) + "): ";
        repeat_string = 
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Enter the number of points to which you want to improve the attribute.\n"
            "(from 1 to " + to_string(BuildingsConstants::max_value_of_attributes) + "): ";
        help_with_interface (begin_string, repeat_string, value, 1, BuildingsConstants::max_value_of_attributes);
        try 
        {
            this->city.update_building_with_fee (Point (abscissa, ordinate), attribute_type, value, true);
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 7:
        begin_string =
            "\nHere is a list of building types that you can print right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can print right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
            )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        try
        {
            cout << this->city.get_building (Point (abscissa, ordinate));
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
        break;
    case 8:
        this->city.live_month ();
        break;
    case 9:
        exit ();
        return;
    default:
        throw UnavailableOperation ("request_handler");
    }
    this->request_handler();
}

void UserInteractionUnlimitedCity::start () //throw (UnavailableOperation)
{
    int operation = -1;
    Size size_of_city_center = BuildingsInfo::property_info.at(BuildingID(city_center)).get_min_size();

    Point point_of_city_center(
        (unlimited_city.get_size().length - size_of_city_center.length) / 2,
        (unlimited_city.get_size().width - size_of_city_center.width) / 2
    );

    Building building (
        city_center,
        point_of_city_center.abscissa,
        point_of_city_center.ordinate,
        size_of_city_center.length,
        size_of_city_center.width
    );

    this->unlimited_city.add_building_to_map(building);

    string begin_string =
        "\nYou chose the \"unlimited city\" mode. Congratulations, \n"
        "in the near future you will plunge into the atmosphere \n"
        "of urban development. Imagine that you are the mayor of\n"
        "the city. Now it consists only of the city center \n"
        "in which your office is located, and no one in your \n"
        "city lives, but as more buildings are built and the city \n"
        "develops, more and more people will fill it. Your task is \n"
        "to build a successful and rich city. So, let's begin! \n"
        "\n"
        "Now I give you time to build your city. Remember that \n"
        "our people are very demanding, and in the absence of the \n"
        "building they need, they will not settle in our city. \n"
        "\n"
        "The following list of factors will help you find your bearings: \n"
        "    1. Security\n"
        "    2. Education\n"
        "    3. Health\n"
        "    4. Food\n"
        "    5. Ecology\n"
        "    6. Spirituality\n"
        "\n"
        "Here is a list of buildings that you can build right now, \n"
        "the price of each of them, but you have unlimited budget, so \n"
        "do not pay attention to it: \n" + BuildingsInfo::property_prices +
        "\n"
        "\n    Select one of them: ";
    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "List of buildings that you can build right now, \n"
        "the price of each of them, but you have unlimited budget, so \n"
        "do not pay attention to it: \n" + BuildingsInfo::property_prices +
        "\n"
        "\n    Select one of them: ";
    help_with_interface(begin_string, repeat_string, operation, 1, 13);
    BuildingID building_type = BuildingsInfo::property_codes.at(operation);
    cout << "\nThe size of the selected building is as follows:\n"
        << BuildingsInfo::property_info.at(building_type).get_min_size();

    int abscissa, ordinate;

    begin_string =
        "\nNow select the point on the map: \n"
        "    1. x = ";
    repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "Select the point on the map: \n"
        "    1. x = ";
    help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
    begin_string =
        "    2. y = ";
    repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
        "Select the point on the map: \n"
        "    2. y = ";
    help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
    try
    {
        this->unlimited_city.construct_building_without_fee(building_type, Point(abscissa, ordinate));
    }
    catch (GeneralException &exception)
    {
        cout << endl << exception.what () << endl;
    }
    catch (...)
    {
        cout << "\nUnknown error in the function UserInteractionCity::start ()";
    }
    cout << "\nNow you know how you can manage your unlimited city, \n"
        << "on this your training is over, enjoy!\n";
    this->request_handler();
}

void UserInteractionUnlimitedCity::request_handler () //throw (UnavailableOperation)
{
    int operation = -1;
    int abscissa = -1;
    int ordinate = -1;

    string begin_string =
        "\nSelect one of the next items:\n"
        "\n"
        "    1. Print all information about my unlimited city.\n"
        "    2. Construct building.\n"
        "    3. Destroy building.\n"
        "    4. Insure building.\n"
        "    5. Expand the territory of the unlimited city.\n"
        "    6. Update building.\n"
        "    7. Print all information about building.\n"
        "    8. Go for next month.\n"
        "    9. Exit the program. \n"
        "\n"
        "    Entry: ";
    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "\n"
		"Select one of the next items:\n"
        "\n"
        "    1. Print all information about my unlimited city.\n"
        "    2. Construct building.\n"
        "    3. Destroy building.\n"
        "    4. Insure building.\n"
        "    5. Expand the territory of the unlimited city.\n"
        "    6. Update building.\n"
        "    7. Print all information about building.\n"
        "    8. Go for next month.\n"
        "    9. Exit the program.\n"
        "\n"
        "    Entry: ";
    help_with_interface(begin_string, repeat_string, operation, 1, 9);

    BuildingID building_type;
    set<Point> addresses;
    BuildingAttributeTypes attribute_type;
    int value;

    switch (operation)
    {
    case 1:
        cout << this->unlimited_city;
        break;
    case 2:
        begin_string =
            "\nHere is a list of buildings that you can build right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can build right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        cout << "\nThe size of the selected building is as follows:\n"
            << BuildingsInfo::property_info.at(building_type).get_min_size() << endl;

        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
        try
        {
            this->unlimited_city.construct_building_without_fee(building_type, Point(abscissa, ordinate));
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 3:
        begin_string =
            "\nHere is a list of building types that you can destroy right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can destroy right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->unlimited_city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        this->unlimited_city.destroy_building_without_fee (Point (abscissa, ordinate));
        break;
    case 4:
        begin_string =
            "\nHere is a list of building types that you can insure right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can insure right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->unlimited_city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        try
        {
            this->unlimited_city.insure_without_fee (Point (abscissa, ordinate));
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
		break;
    case 5:
        begin_string =
            "\nEnter the number of lines to expand the city to (from 1 to " +
            to_string(TerrainConstants::max_number_of_extensions) + "): ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Enter the number of lines to expand the city to (from 1 to " +
            to_string(TerrainConstants::max_number_of_extensions) + "): ";
        help_with_interface(begin_string, repeat_string, operation, 1, TerrainConstants::max_number_of_extensions);
        try
        {
            this->unlimited_city.expand_area_without_fee (operation);
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
        break;
    case 6:
		begin_string =
			"\nHere is a list of building types that you can update right now, \n"
			"the price of each of them: \n" + BuildingsInfo::property_prices +
			"\n"
			"\n    Select one of them: ";
		repeat_string =
			"\nOh, it seems that you entered a non-existent item... Please try again!\n"
			"\n"
			"List of buildings that you can update right now, \n"
			"the price of each of them: \n" + BuildingsInfo::property_prices +
			"\n"
			"\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->unlimited_city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
        if (!addresses.count(Point(abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        begin_string =
            "\nNow you must select the type of update:\n"
            "    1. Earthquake resistance.\n"
            "    2. Moisture resistance. \n"
            "    3. Strength.\n"
            "    4. Fire resistance.\n"
            "\n"
            "    Entry: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the type of update:\n"
            "    1. Earthquake resistance.\n"
            "    2. Moisture resistance. \n"
            "    3. Strength.\n"
            "    4. Fire resistance.\n"
            "\n"
            "    Entry: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 4);
        attribute_type = BuildingsInfo::property_attributes.at(operation);
        begin_string =
            "\nEnter the number of points to which you want to improve the attribute.\n"
            "(from 1 to " + to_string(BuildingsConstants::max_value_of_attributes) + "): ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Enter the number of points to which you want to improve the attribute.\n"
            "(from 1 to " + to_string(BuildingsConstants::max_value_of_attributes) + "): ";
        help_with_interface(begin_string, repeat_string, value, 1, BuildingsConstants::max_value_of_attributes);
        try
        {
            this->unlimited_city.update_building_without_fee(Point(abscissa, ordinate), attribute_type, value, true);
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 7:
        begin_string =
            "\nHere is a list of building types that you can print right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "List of buildings that you can print right now, \n"
            "the price of each of them: \n" + BuildingsInfo::property_prices +
            "\n"
            "\n    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 13);
        building_type = BuildingsInfo::property_codes.at(operation);
        try
        {
            addresses = this->unlimited_city.get_addresses(building_type);
        }
        catch (std::out_of_range &)
        {
            cout << "\nAt the moment, there are no buildings of this type in the city.\n";
            break;
        }
        cout << "\nHere is a list of addresses of buildings of this type:\n";
        for (
            set<Point>::const_iterator iter = addresses.cbegin();
            iter != addresses.cend();
            ++iter
        )
        {
            cout << "    " << *iter << endl;
        }
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->unlimited_city.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->unlimited_city.get_size().width);
        if (!addresses.count (Point (abscissa, ordinate)))
        {
            cout << "\nThis point does not belong to the building of the chosen type.\n";
            break;
        }
        try
        {
            cout << this->unlimited_city.get_building (Point (abscissa, ordinate));
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()\n";
        }
        break;
    case 8:
        this->unlimited_city.live_month();
        break;
    case 9:
        exit();
        return;
    default:
        throw UnavailableOperation ("UserInteractionUnlimitedCity::request_handler ()");
    }
    this->request_handler ();
}

void UserInteractionDesert::start () //throw (UnavailableOperation)
{
    int operation = -1;

    cout <<
        "\nYou chose the \"desert (God)\" mode. Congratulations, \n"
        "in the near future you will plunge into the atmosphere \n"
        "of destruction. Imagine that you are the God of\n"
        "the desert. Now it is empty (which desert should be). \n"
        "Your task is to eradicate all the emerging buildings on the map.\n"
        "to des a successful and rich city. So, let's begin! \n"
        "\n";
    this->desert.live_month ();
    this->request_handler ();
}

void UserInteractionDesert::request_handler () //throw (UnavailableOperation)
{
    int operation = -1;
    int level = -1;
    int radius = -1;
    int abscissa = -1;
    int ordinate = -1;
    int error = -1;

    string begin_string =
        "\nSelect one of the next items:\n"
        "\n"
        "    1. Print all information about my desert.\n"
        "    2. Send cataclysm.\n"
        "    3. Go for the next month.\n"
        "    4. Exit the program.\n"
        "\n"
        "    Entry: ";
    string repeat_string =
        "\nOh, it seems that you entered a non-existent item... Please try again!\n"
        "Select one of the next items:\n"
        "\n"
        "    1. Print all information about my desert.\n"
        "    2. Send cataclysm.\n"
        "    3. Go for the next month.\n"
        "    4. Exit the program.\n"
        "\n"
        "    Entry: ";
    help_with_interface (begin_string, repeat_string, operation, 1, 4);
    CataclysmTypes cataclysm_type;
    switch (operation)
    {
    case 1:
        cout << this->desert;
        break;
    case 2:
        begin_string =
            "\nHere is a list of the types of cataclysms that we can send.\n"
            "\n"
            "    1. Earthquake.\n"
            "    2. Flood.\n"
            "    3. Hurricane.\n"
            "    4. Fire.\n"
            "\n"
            "    Select one of them: ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "\nList of the types of cataclysms that we can send.\n"
            "\n"
            "    1. Earthquake.\n"
            "    2. Flood.\n"
            "    3. Hurricane.\n"
            "    4. Fire.\n"
            "\n"
            "    Select one of them: ";
        help_with_interface(begin_string, repeat_string, operation, 1, 4);

        cataclysm_type =
            (operation == 1) ? earthquake :
            (operation == 2) ? flood :
            (operation == 3) ? hurricane :
            (operation == 4) ? fire : cataclysm_types_number;

        begin_string =
            "\nNow select the attributes of the cataclysm: \n"
            "    1. level (from 1 to " + to_string(CataclysmsConstants::max_level) + ") = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the attributes of the cataclysm: \n"
            "    1. level (from 1 to " + to_string (CataclysmsConstants::max_level) + ") = ";
        help_with_interface(begin_string, repeat_string, level, 1, CataclysmsConstants::max_level);
        begin_string =
            "    2. radius (from 1 to " + to_string (CataclysmsConstants::max_radius) + ") = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the attributes of the cataclysm: \n"
            "    2. radius (from 1 to " + to_string(CataclysmsConstants::max_radius) + ") = ";
        help_with_interface(begin_string, repeat_string, radius, 1, CataclysmsConstants::max_radius);
        begin_string =
            "\nNow select the point on the map: \n"
            "    1. x = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    1. x = ";
        help_with_interface(begin_string, repeat_string, abscissa, 0, this->desert.get_size().length);
        begin_string =
            "    2. y = ";
        repeat_string =
            "\nOh, it seems that you entered a non-existent item... Please try again!\n"
            "\n"
            "Select the point on the map: \n"
            "    2. y = ";
        help_with_interface(begin_string, repeat_string, ordinate, 0, this->desert.get_size().width);
        try
        {
            this->desert.send_catalysm_with_divine_power (
                Cataclysm (
                    level,
                    radius,
                    Point (
                        abscissa,
                        ordinate
                    ),
                    CataclysmID (cataclysm_type)
                )
            );
        }
        catch (GeneralException &exception)
        {
            cout << endl << exception.what () << endl;
        }
        catch (...)
        {
            cout << "\nUnknown error in the function UserInteractionCity::start ()";
        }
        break;
    case 3:
        this->desert.live_month ();
        break;
    case 4:
        exit();
        return;
    default:
        throw UnavailableOperation ("UserInteractionUnlimitedCity::request_handler ()");
    }
    this->request_handler();
}