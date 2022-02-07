// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <list>
#include <stack>
#include <queue>
#include <map>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)


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
    // Clear all roads
    clear_roads();

    // Deletes all the stored towns
    for (const auto& town_pair : towns)
    {
        delete town_pair.second;
    }
    towns.clear();
    towns.rehash(1);
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
    auto town_pair = towns.find(id);
    if (town_pair == towns.end())
    {
        return false;
    }

    auto master = town_pair->second->master;
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
    if (!town_pair->second->vassals.empty())
    {
        for (const auto& vassal : town_pair->second->vassals)
        {
            // add the new relationship
            vassal->master = master;
            master->vassals.push_back(vassal);
        }
    }

    // Delete roads
    while (!town_pair->second->roads_to.empty())
    {
        Road* road_to_delete = town_pair->second->roads_to.back();

        // Erase from the data structure
        auto it = roads.find({road_to_delete->distance, road_to_delete});
        roads.erase(it);

        // Delete pointer
        delete road_to_delete;

        // Pop from the vector
        town_pair->second->roads_to.pop_back();
    }

    // delete the allocated memory and clear the data structure
    delete town_pair->second;
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
    for (const auto& town_pair : towns)
    {
        distance_id* temp_struct = new distance_id;
        temp_struct->distance = distance(town_pair.second->coordinates, coord);
        temp_struct->id = town_pair.first;

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


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    if (roads.empty())
    {
        return;
    }

    // Deletes the Road struct
    for (const auto& road_pair : roads)
    {
        delete road_pair.second;
    }

    // Clears all pointers to the struct
    roads.clear();

    for (const auto& town_pair : towns)
    {
        town_pair.second->roads_to.clear();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> roads_vector = {};

    // return empty vector if there are no towns or roads
    if (towns.empty() || roads.empty())
    {
        return roads_vector;
    }

    // add all the roads to the return vector
    for (const auto& id_struct : roads)
    {
        TownID id_1 = id_struct.second->town_1->id;
        TownID id_2 = id_struct.second->town_2->id;

        if (id_1 < id_2)
        {
            roads_vector.push_back({id_1, id_2});
        }
        else
        {
            roads_vector.push_back({id_2, id_1});
        }
    }
    return roads_vector;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    auto town_pair_1 = towns.find(town1);
    auto town_pair_2 = towns.find(town2);

    // If either of towns doesn't exist or they are the same return false
    if (town_pair_1 == towns.end() || town_pair_2 == towns.end() || town1 == town2)
    {
        return false;
    }

    // A lambda function to check if the road exists
    auto road_exists = [&town1, &town2](Road* a)
    {
        if (a->town_1->id == town1 && a->town_2->id == town2)
        {
            return true;
        }
        else if (a->town_1->id == town2 && a->town_2->id == town1)
        {
            return true;
        }
        return false;
        };

    // If road exists, don't add it
    if (std::find_if(town_pair_1->second->roads_to.begin(), town_pair_1->second->roads_to.end(), road_exists) != town_pair_1->second->roads_to.end())
    {
        return false;
    }

    int road_distance = distance(town_pair_1->second->coordinates, town_pair_2->second->coordinates);

    // Create a new road struct
    Road* new_road (new Road{town_pair_1->second, town_pair_2->second, road_distance});

    // Insert to the towns and data structure
    town_pair_1->second->roads_to.push_back(new_road);
    town_pair_2->second->roads_to.push_back(new_road);

    roads.insert({new_road->distance, new_road});

    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    auto town_pair = towns.find(id);

    // If town not found
    if (town_pair == towns.end())
    {
        return {NO_TOWNID};
    }

    // Create a return vector, iterate through roads and push back them to the vector
    std::vector<TownID> roads_vector = {};

    if (town_pair->second->roads_to.empty())
    {
        return roads_vector;
    }

    for (const auto& road_struct : town_pair->second->roads_to)
    {
        if (road_struct->town_1->id == id)
        {
            roads_vector.push_back(road_struct->town_2->id);
        }
        else
        {
            roads_vector.push_back(road_struct->town_1->id);
        }
    }
    return roads_vector;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    // Least towns route is the fastest way to find any route
    return least_towns_route(fromid, toid);
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    auto town_pair_1 = towns.find(town1);
    auto town_pair_2 = towns.find(town2);

    // If either of towns don't exist or they are the same, do nothing
    if (town_pair_1 == towns.end() || town_pair_2 == towns.end() || town1 == town2)
    {
        return false;
    }

    // A lambda function to compare towns
    auto road_exists = [&town1, &town2](Road* a)
    {
        if (a->town_1->id == town1 && a->town_2->id == town2)
        {
            return true;
        }
        else if (a->town_1->id == town2 && a->town_2->id == town1)
        {
            return true;
        }
        return false;
        };

    // Check if road exists by checking the other town
    auto it_1 = std::find_if(town_pair_1->second->roads_to.begin(), town_pair_1->second->roads_to.end(), road_exists);
    if (it_1 == town_pair_1->second->roads_to.end())
    {
        return false;
    }

    // Find iterator for the other town
    auto it_2 = std::find_if(town_pair_2->second->roads_to.begin(), town_pair_2->second->roads_to.end(), road_exists);

    // Find iterator for roads set
    Road* road = *it_1;
    auto it_3 = roads.find({road->distance, road});

    // Remove road from town structs
    town_pair_1->second->roads_to.erase(it_1);
    town_pair_2->second->roads_to.erase(it_2);
    roads.erase(it_3);

    delete road;

    return true;
}

// BFS
std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    auto town_pair_from = towns.find(fromid);
    auto town_pair_to = towns.find(toid);

    if (town_pair_from == towns.end() || town_pair_to == towns.end())
    {
        return {NO_TOWNID};
    }

    // Return colour to white and clear route vector
    reset_nodes();

    // Least towns route found
    std::vector<TownID> least_towns = {};

    // Queue
    std::list<Town*> queue_list = {town_pair_from->second};

    while (!queue_list.empty())
    {
        // Take the top Town
        const auto current_town = queue_list.front();
        queue_list.pop_front();

        // The first route that a BFS algorithm finds is always the least town route
        // Save the found route to a vector and break the loop to return it
        if (current_town->id == toid)
        {
            current_town->route_from_root.push_back(toid);
            least_towns = current_town->route_from_root;
            break;
        }

        // Iterate through the neighbours
        for (const auto& road_struct : current_town->roads_to)
        {
            // Check which of the two towns in the struct is the correct one
            Town* town_to;
            if (road_struct->town_1->id == current_town->id)
            {
                town_to = road_struct->town_2;
            }
            else
            {
                town_to = road_struct->town_1;
            }

            if (town_to->current_color == white)
            {
                town_to->current_color = gray;

                // Push back current town to the route vector
                town_to->route_from_root = current_town->route_from_root;
                town_to->route_from_root.push_back(current_town->id);

                queue_list.push_back(town_to);
            }
        }
        current_town->current_color = black;
    }
    return least_towns;
}

// DFS
std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    auto town_pair_start = towns.find(startid);

    if (town_pair_start == towns.end())
    {
        return {NO_TOWNID};
    }

    // Return color to white and clear route vector
    reset_nodes();

    // Vector for storing the cycle
    std::vector<TownID> cycle_route = {};

    // Stack for storing the Towns
    std::stack<Town*> stack;
    stack.push(town_pair_start->second);

    while (!stack.empty())
    {
        // Take the top Town
        const auto current_town = stack.top();
        stack.pop();


        if (current_town->current_color == white)
        {
            // Add TownID to the cycle route
            cycle_route.push_back(current_town->id);

            // Mark the town as visited and push it back to stack
            current_town->current_color = gray;
            stack.push(current_town);

            // Check the neighbouring Towns
            for (const auto& road_struct : current_town->roads_to)
            {
                // Determine which of the two Towns in the struct is the correct one
                Town* town_to;
                if (road_struct->town_1->id == current_town->id)
                {
                    town_to = road_struct->town_2;
                }
                else
                {
                    town_to = road_struct->town_1;
                }

                // If neighbouring Town is unvisited, push it to the stack
                if (town_to->current_color == white)
                {
                    stack.push(town_to);
                }
                // If the neighbouring town is visited but it's not the one we came from then we have a cycle!
                else if (town_to->current_color == gray && cycle_route.end()[-2] != town_to->id)
                {
                    cycle_route.push_back(town_to->id);

                    return cycle_route;
                }
            }
        }
        // If the top Town isn't white, it means we went to wrong direction
        else
        {
            cycle_route.pop_back();
            current_town->current_color = black;
        }
    }
    return {};
}


// A*
std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    auto town_pair_from = towns.find(fromid);
    auto town_pair_to = towns.find(toid);

    if (town_pair_from == towns.end() || town_pair_to == towns.end())
    {
        return {NO_TOWNID};
    }

    // Return color to white and clear route vector
    reset_nodes();

    Town* beginning = town_pair_from->second;

    // Mark the first node gray and distance from beginning 0.
    beginning->current_color = gray;
    beginning->distance_from_beginning = 0;

    // Priority queue that sorts itself by the distance estimate
    std::priority_queue<std::pair<int, Town*>> queue;

    // Push the first town into the queue
    queue.push({0 - beginning->distance_estimate, beginning});

    while (!queue.empty())
    {
        // Take the first value from the list
        const auto current_town = queue.top();
        queue.pop();

        // If current town is the end, return vector
        if (current_town.second->id == toid)
        {
            current_town.second->route_from_root.push_back(toid);

            return current_town.second->route_from_root;
        }

        // Check the neighbours
        for (const auto& road_struct : current_town.second->roads_to)
        {
            // Determine which of the two Towns in the struct is the correct one
            Town* town_to;
            if (road_struct->town_1->id == current_town.second->id)
            {
                town_to = road_struct->town_2;
            }
            else
            {
                town_to = road_struct->town_1;
            }

            // Determine new distance estimate
            relax_A(current_town.second, town_to, road_struct->distance, town_pair_to->second);

            // If town is unvisited, push it to the queue
            if (town_to->current_color == white)
            {
                town_to->current_color = gray;

                // Push town to the queue
                queue.push({0 - town_to->distance_estimate, town_to});
            }
        }
        current_town.second->current_color = black;
    }
    return {};
}

// Kruskal
Distance Datastructures::trim_road_network()
{
    if (roads.empty())
    {
        return 0;
    }

    // Return color to white and clear route vector
    reset_nodes();

    // A vector of sets to determine road connections between towns
    std::vector<std::unordered_set<Town*>> tree;

    std::vector<Road*> all_roads;

    // Clear the data structure while pushing all the towns to a vector
    while (roads.begin() != roads.end())
    {
        all_roads.push_back(roads.begin()->second);
        roads.erase(roads.begin());
    }
    for (const auto& town_pair : towns)
    {
        town_pair.second->roads_to.clear();
    }

    int all_roads_distance = 0;

    int towns_count = towns.size();
    int roads_added = 0;

    // A set indicator is used to determine a set that each town belongs to
    int set_indicator = 0;

    for (const auto& road : all_roads)
    {
        Town* town_1 = road->town_1;
        Town* town_2 = road->town_2;
        int distance = road->distance;

        // The maximum amount of roads in a minimum spanning tree is towns count - 1
        if (roads_added == towns_count - 1)
        {
            delete road;
            continue;
        }

        // If both of the towns are univisted, add them to the tree and data structure
        if (town_1->set_indicator == -1 && town_2->set_indicator == -1)
        {
            // Push a new set into the tree
            tree.push_back({town_1, town_2});

            // Mark towns as visited
            town_1->set_indicator = set_indicator;
            town_2->set_indicator = set_indicator;
            set_indicator += 1;

            // Add the road back to the data structure
            roads.insert({road->distance, road});
            town_1->roads_to.push_back(road);
            town_2->roads_to.push_back(road);

            // Add up distance and added roads
            all_roads_distance += distance;
            roads_added += 1;
            continue;
        }

        // If either of the two are univisted, insert road to an existing set
        else if (town_1->set_indicator == -1)
        {
            // Insert town 1 to the same set as town 2
            tree.at(town_2->set_indicator).insert(town_1);
            town_1->set_indicator = town_2->set_indicator;

            roads.insert({road->distance, road});
            town_1->roads_to.push_back(road);
            town_2->roads_to.push_back(road);

            all_roads_distance += distance;
            roads_added += 1;
            continue;
        }
        // Same as before but for the other town
        else if (town_2->set_indicator == -1)
        {
            tree.at(town_1->set_indicator).insert(town_2);
            town_2->set_indicator = town_1->set_indicator;

            roads.insert({road->distance, road});
            town_1->roads_to.push_back(road);
            town_2->roads_to.push_back(road);

            all_roads_distance += distance;
            roads_added += 1;
            continue;
        }

        // If sets are not the same, merge them and add road
        if (town_1->set_indicator != town_2->set_indicator)
        {
            // Get pointeers for each set for faster access
            std::unordered_set<Town*>* tree_pointer_1 = &tree.at(town_1->set_indicator);
            std::unordered_set<Town*>* tree_pointer_2 = &tree.at(town_2->set_indicator);

            // Determine which set has lesser items so that iterating through them is faster
            // Insert Towns to a bigger set and change the set indicator
            if (tree_pointer_1->size() < tree_pointer_2->size())
            {
                for (const auto& town : *tree_pointer_1)
                {
                    tree_pointer_2->insert(town);
                    town->set_indicator = town_2->set_indicator;
                }
            }
            else
            {
                for (const auto& town : *tree_pointer_2)
                {
                    tree_pointer_1->insert(town);
                    town->set_indicator = town_1->set_indicator;
                }
            }

            roads.insert({road->distance, road});
            town_1->roads_to.push_back(road);
            town_2->roads_to.push_back(road);

            all_roads_distance += distance;
            roads_added += 1;
        }
        // If towns were already connected in the tree, the road can be deleted
        else
        {
            delete road;
        }
    }
    return all_roads_distance;
}


// Helper functions

void Datastructures::relax_A(Town* current_town, Town* neighbour, int distance_between_towns, Town* end)
{
    // If road distance from the beginning to the neighbouring town is bigger than
    // road distance from the beginning to the current town
    if (neighbour->distance_from_beginning > current_town->distance_from_beginning
        + distance_between_towns)
    {
        // Evaluate the road distance from the beginning
        neighbour->distance_from_beginning = current_town->distance_from_beginning
                                             + distance_between_towns;

        // New distance estimate
        neighbour->distance_estimate = neighbour->distance_from_beginning +
                                       distance(neighbour->coordinates, end->coordinates);

        // Store the route from root
        neighbour->route_from_root = current_town->route_from_root;
        neighbour->route_from_root.push_back(current_town->id);
    }
}

void Datastructures::reset_nodes()
{
    // Loop through towns and reset the values
    for (const auto& town_pair : towns)
    {
        town_pair.second->current_color = white;

        town_pair.second->distance_from_beginning = std::numeric_limits<int>::max();
        town_pair.second->distance_estimate = std::numeric_limits<int>::max();

        town_pair.second->route_from_root.clear();

        town_pair.second->set_indicator = -1;
    }
}
