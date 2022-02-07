// Datastructures.cc
//
// Student name: Kasperi Kouri
// Student email: kasperi.kouri@tuni.fi
// Student number: H292044

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
    clear_all();
}

unsigned int Datastructures::town_count()
{
    return towns.size();
}

void Datastructures::clear_all()
{
    // Deletes all the stored towns
    for (auto it = towns.begin(); it != towns.end(); it++)
    {
        delete it->second;
    }
    towns.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    // Adds all the info to the town struct.
    // Also calculates distance between origin and given coordinates
    if (towns.find(id) == towns.end())
    {
        Town* new_town = new Town{id, name, coord, tax, distance(coord, {0,0})};
        towns.insert({id, new_town});
        return true;
    }
    return false;
}

Name Datastructures::get_town_name(TownID id)
{
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return NO_NAME;
    }
    return town_pair->second->name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return NO_COORD;
    }
    return town_pair->second->coordinates;
}

int Datastructures::get_town_tax(TownID id)
{
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return NO_VALUE;
    }
    return town_pair->second->tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> all_towns;
    all_towns.reserve(towns.size());

    std::transform(towns.begin(), towns.end(), std::back_inserter(all_towns),
                   [](const auto& town_pair){return town_pair.first;});

    return all_towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> town_names = {};

    for (const auto& town_pair : towns)
    {
        if (town_pair.second->name == name)
        {
            town_names.push_back(town_pair.first);
        }
    }
    return town_names;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return false;
    }
    town_pair->second->name = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // Create a vector for sorting and reserve memory for it
    std::vector<Town*> town_pointers;
    town_pointers.reserve(towns.size());

    // Push back Town* pointers to the vector
    std::transform(towns.begin(), towns.end(), std::back_inserter(town_pointers),
                   [](const auto& town_pair){return town_pair.second;});

    // Sort using lambda comparison function
    std::sort(town_pointers.begin(), town_pointers.end(), [](Town* a, Town* b)
              {return a->name < b->name;});

    // Create a return vector containing ids and reserve memory for it
    std::vector<TownID> alphabetical;
    alphabetical.reserve(town_pointers.size());

    // Push back the sorted ids to the return vector
    std::transform(town_pointers.begin(), town_pointers.end(), std::back_inserter(alphabetical),
                   [](const auto& town_pointer){return town_pointer->id;});

    return alphabetical;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Create a vector for sorting and reserve memory for it
    std::vector<Town*> town_pointers;
    town_pointers.reserve(towns.size());

    // Push back Town* pointers to the vector
    std::transform(towns.begin(), towns.end(),
                   std::back_inserter(town_pointers),
                   [](const auto& town){return town.second;});

    // Sort using lambda comparison function
    std::sort(town_pointers.begin(), town_pointers.end(),
              [](const auto& a, const auto& b)
              {return a->distance_from_origin < b->distance_from_origin;});

    // Create a return vector containing ids and reserve memory for it
    std::vector<TownID> town_ids;
    town_ids.reserve(towns.size());

    // Push back the sorted ids to the return vector
    std::transform(town_pointers.begin(), town_pointers.end(),
                   std::back_inserter(town_ids),
                   [](const auto& town_pointer){return town_pointer->id;});

    return town_ids;
}

TownID Datastructures::min_distance()
{
    if (towns.empty())
    {
        return NO_TOWNID;
    }

    // Find min element and return it
    return std::min_element(towns.begin(), towns.end(),
                            [](const auto& a, const auto& b)
                            {return a.second->distance_from_origin < b.second->distance_from_origin;})->second->id;
}

TownID Datastructures::max_distance()
{
    if (towns.empty())
    {
        return NO_TOWNID;
    }

    // Find max element and return it
    return std::max_element(towns.begin(), towns.end(),
                            [](const auto& a, const auto& b)
                            {return a.second->distance_from_origin < b.second->distance_from_origin;})->second->id;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    const auto vassal = towns.find(vassalid);
    const auto master = towns.find(masterid);

    // Checks that the given ids are found and
    // that the towns don't already have a master - vassal relationship
    if (vassal == towns.end() || master == towns.end() ||
        vassal->second->master == master->second)
    {
        return false;
    }

    // add the relationship
    vassal->second->master = master->second;
    master->second->vassals.push_back(vassal->second);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return {NO_TOWNID};
    }

    // Create a vector and reserve space for it
    std::vector<TownID> vassals;
    vassals.reserve(town_pair->second->vassals.size());

    // Push vassal ids tot the vector
    std::transform(town_pair->second->vassals.begin(), town_pair->second->vassals.end(),
                   std::back_inserter(vassals), [](const auto& vassal){return vassal->id;});

    return vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxers;

    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return {NO_TOWNID};
    }

    // Add the town itself in to the vector
    taxers.push_back(id);

    // Iterate through masters and add them to the vector
    auto town_iter = town_pair->second;
    while (town_iter->master != nullptr)
    {
        town_iter = town_iter->master;
        taxers.push_back(town_iter->id);
    }
    return taxers;
}

bool Datastructures::remove_town(TownID id)
{
    auto town = towns.find(id);
    if (town == towns.end())
    {
        return false;
    }

    auto master = town->second->master;
    // Remove town as a vassal
    if (master != nullptr)
    {
        int i = 0;
        for (const auto& vassal : master->vassals)
        {
            if (vassal->id == id)
            {
                master->vassals.erase(master->vassals.begin() + i);
                break;
            }
            ++i;
        }
    }
    // Remove town as a master
    if (!town->second->vassals.empty())
    {
        for (const auto& vassal : town->second->vassals)
        {
            // add the new relationship
            vassal->master = master;
            master->vassals.push_back(vassal);
        }
    }

    // delete the allocated memory and clear the data structure
    delete town->second;
    towns.erase(id);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    struct distance_id
    {
        TownID id;
        int distance;
    };

    // Create a vector for sorting and reserve memory for it
    std::vector<distance_id*> town_structs;
    town_structs.reserve(towns.size());

    // Push back newly created struct pointers to the vector
    for (const auto& town : towns)
    {
        distance_id* temp_struct = new distance_id;
        temp_struct->distance = distance(town.second->coordinates, coord);
        temp_struct->id = town.first;

        town_structs.push_back(temp_struct);
    }

    // Sort using lambda comparison
    std::sort(town_structs.begin(), town_structs.end(),
              [](const auto& a, const auto& b){return a->distance < b->distance;});

    // Create a return vector and reserve memory for it
    std::vector<TownID> town_ids;
    town_ids.reserve(towns.size());

    // Push back sorted ids to the vector
    std::transform(town_structs.begin(), town_structs.end(), std::back_inserter(town_ids),
                   [](const auto& a){return a->id;});

    // delete structs
    for (const auto& town_struct : town_structs)
    {
        delete town_struct;
    }

    return town_ids;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    auto town = towns.find(id);
    if (town == towns.end())
    {
        return {NO_TOWNID};
    }
    return recursive_vassal_path(id, town->second, {});
}

int Datastructures::total_net_tax(TownID id)
{
    auto town = towns.find(id);
    if (town == towns.end())
    {
        return NO_VALUE;
    }

    // If the town has a master it loses 10% tax income
    if (town->second->master != nullptr)
    {
        int tax_income = recursive_tax_income(town->second);
        return tax_income - static_cast<int>(0.1 * tax_income);
    }

    return static_cast<int>(recursive_tax_income(town->second));
}

// Helper functions are here:

int Datastructures::distance(Coord coord_1, Coord coord_2)
{
    int x1 = coord_1.x;
    int y1 = coord_1.y;

    int x2 = coord_2.x;
    int y2 = coord_2.y;

    // Calculate distance
    int towns_distance = static_cast<int>(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    return towns_distance;
}

std::vector<TownID> Datastructures::recursive_vassal_path(TownID id, Town* town, std::vector<TownID> current_longest_path)
{
    // If town has no vassals we have reached the leaf node
    if (town->vassals.empty())
    {
        std::vector<TownID> temp_vassal_path;

        // Iterate nodes upwards until it finds the id that the function was called for
        auto temp_town = town;
        while ( true )
        {
            // Adds ids into the vassal path
            // temp_vassal_path.insert(temp_vassal_path.begin(), temp_town->id);
            temp_vassal_path.push_back(temp_town->id);
            if (temp_town->id == id)
            {
                break;
            }
            temp_town = temp_town->master;
        }
        std::reverse(temp_vassal_path.begin(), temp_vassal_path.end());
        return temp_vassal_path;
    }
    // Iterate through all nodes vassals
    for (const auto& vassal : town->vassals)
    {
        // Store the newly found vassal path temporarily
        auto temp_vassal_path = recursive_vassal_path(id, vassal,
                                                      current_longest_path);

        // Only the longer vassal path will stay in memory
        if (temp_vassal_path.size() > current_longest_path.size())
        {
            current_longest_path = temp_vassal_path;
        }
    }
    return current_longest_path;
}

int Datastructures::recursive_tax_income(Town* town)
{
    double current_income = 0;

    // If town doesn't have vassals it doesn't get any additional tax income
    if (town->vassals.empty())
    {
        return town->tax;
    }

    // Add the 10% extra income for each vassal the master has
    current_income += town->tax;
    for (const auto& vassal : town->vassals)
    {
        current_income += static_cast<int>(0.1 * recursive_tax_income(vassal));
    }
    return current_income;
}
