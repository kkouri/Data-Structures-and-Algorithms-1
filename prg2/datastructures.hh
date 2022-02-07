// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <memory>
#include <unordered_set>
#include <math.h>
#include <set>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();


    // Many functions would be (omega) om(1) if the id is invalid.
    // Estimates are only counted for cases where id is valid.


    // Estimate of performance: th(1)
    // Short rationale for estimate: .size() is a constant
    unsigned int town_count();

    // Estimate of performance: om(n), O(n+e), n = node, e = edge
    // Short rationale for estimate: for loop is O(n)
    //                               .clear() is O(n)
    // Removing roads takes O(n+e).
    void clear_all();

    // Estimate of performance: O(log(n)), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .end() is a constant
    //                               distance() function is constant
    //                               inserting to a unordered_map is O(log(n))
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .end() is a constant
    Name get_town_name(TownID id);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .end() is a constant
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .end() is a constant
    int get_town_tax(TownID id);

    // Estimate of performance: th(n), where n is number of towns
    // Short rationale for estimate: .reserve() is O(n)
    //                               transform is O(n)
    //                               .push_back is constant when enough memory
    //                               has been allocated
    // Never quicker than om(n)
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n), where n is number of towns
    // Short rationale for estimate: for loop is O(n)
    //                               .push_back is constant but reallocation
    //                               might happen
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .end() is a constant
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(nlog(n)), where n is number of towns
    // Short rationale for estimate: .reserve() is O(n)
    //                               transform is O(n)
    //                               sort() is O(nlog(n))
    //                               .reserve() is O(n)
    //                               transform is O(n)
    // The biggest counts O(nlog(n))
    std::vector<TownID> towns_alphabetically();


    // Estimate of performance: O(nlog(n)), where n is number of towns
    // Short rationale for estimate: .reserve() is O(n)
    //                               transform is O(n)
    //                               sort() is O(nlog(n))
    //                               .reserve() is O(n)
    //                               transform is O(n)
    // The biggest counts O(nlog(n))
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: th(n), where n is number of towns
    // Short rationale for estimate: .empty() is constant
    //                               min_element is O(n)
    // Never quicker than om(n)
    TownID min_distance();

    // Estimate of performance: th(n), where n is number of towns
    // Short rationale for estimate: .empty() is constant
    //                               max_element is O(n)
    // Never quicker than om(n)
    TownID max_distance();

    // Estimate of performance: th(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               .push_back() is amortized constant
    // Never quicker than om(n)
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               for loop is O(n)
    //                               .push_back() is amortized constant
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    //                               transform is O(n)
    //                               .push_back() is amortized constant
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: om(n), O(e^2) n = nodes, e = edges.
    // Short rationale for estimate: If the town doesn't exist, does nothing.
    // Otherwise removing roads takes time. Looping through roads from the town with
    // a .find() algorithm inside has worst case scenario of O(e^2).
    bool remove_town(TownID id);

    // Estimate of performance: th(nlog(n)), where n is number of towns
    // Short rationale for estimate: .reserve() is O(n)
    //                               for loop is om(n)
    //                               .push_back() is amortized constant
    //                               sort() is O(nlog(n))
    //                               transform is O(n)
    //                               for loop is O(n)
    // The biggest counts. Never quicker than O(nlog(n))
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    // estimate for recursive_vassal_path():
    //                               while loop is O(n)
    //                               .push_back is amortized constant
    //                               reverse() is O(n)
    //                               for loop is O(n)
    // Function goes through every town only once. Town might not have vassals
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: om(1), O(n), where n is number of towns
    // Short rationale for estimate: .find() is O(n), but constant on average
    // estimate for recursive_tax_income():
    //                               for loop is O(n)
    // Function goes through every town once. Town might not have vassals
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: om(1), O(n+e), n = node, e = edge
    // Short rationale for estimate: If there is no roads the function does nothing.
    // Otherwise there is a .clear() operation inside a for loop.
    // .clear() is linear in size of a container and the for loop goes through each town.
    void clear_roads();

    // Estimate of performance: om(1), O(e), e = edge
    // Short rationale for estimate: If there is no towns or roads, the function does nothing.
    // Otherwise it loops through roads once.
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: om(1), O(e), e = edge
    // Short rationale for estimate: If either of the towns doesn't exist or the towns are the same,
    // the function does nothing.
    // Otherwise the find_if algorithm goes through roads. find_if complexity O(e).
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: om(1), O(e), e = edge
    // Short rationale for estimate: If the town doesn't exist or has no roads, does nothing.
    // Otherwise the function will loop all the roads connecting to the town.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: om(1), O(n+e), n = node, e = edge
    // Short rationale for estimate: Same as the least_towns_route
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: om(1), O(e), e = edge
    // Short rationale for estimate: If either of the towns doesn't exist or they are the same town,
    // the function does nothing.
    // Otherwise the find_if algorithm will go through roads connected to each town.
    // find_if complexity O(e).
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: om(1), O(n+e), n = node, e = edge
    // Short rationale for estimate: If either of the towns doesn't exist, does nothing.
    // Otherwise a BFS algorithm has a complexity of O(n+e).
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: om(1), O(n+e), n = node, e = edge
    // Short rationale for estimate: If either of the towns doesn't exist, does nothing.
    // Otherwise a DFS algorithm has a complexity of O(n+e).
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:om(1), O(n+e), n = node, e = edge
    // Short rationale for estimate: If town doesn't exist, does nothing.
    // Otherwise a A* algorithm has a complexity of O(n+e).
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance: om(1), O(e*log(n)), n = node, e = edge
    // Short rationale for estimate: If roads don't exist, return 0.
    // Otherwise a Kruskal algorithm has a complexity of O(e*log(n)).
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here

    // Color for Town nodes
    enum color {white, gray, black};

    // Initializer
    struct Town;

    // Implements a road between two towns with distance information
    struct Road
    {
        Town* town_1;
        Town* town_2;
        int distance;
    };

    // Implements a single town with all necessary info
    struct Town
    {
        const TownID id;
        Name name;
        Coord coordinates;
        int tax;
        int distance_from_origin;

        std::vector<Town*> vassals = {};
        Town* master = nullptr;

        // Color of the node and vector for neighbouring nodes
        color current_color = white;
        std::vector<Road*> roads_to = {};

        // Parameters for A* :
        int distance_from_beginning = std::numeric_limits<int>::max();
        // Distance estimate = distance from beginning + distance to end
        int distance_estimate = std::numeric_limits<int>::max();

        // A vector to store the route to the Town
        std::vector<TownID> route_from_root = {};

        // A set indicator for Kruskal
        int set_indicator = -1;
    };

    // A map containing all the towns
    std::unordered_map<TownID, Town*> towns = {};

    // A set containing all the roads
    std::set<std::pair<int, Road*>> roads = {};

    // Helper functions prg1
    int distance(Coord coord_1, Coord coord_2);
    std::vector<TownID> recursive_vassal_path(TownID id, Town* town, std::vector<TownID> current_longest_path);
    int recursive_tax_income(Town* town);

    // Helper functions prg2
    void relax_A(Town* current_town, Town* neighbour, int distance_between_towns, Town* end);
    void reset_nodes();
};

#endif // DATASTRUCTURES_HH
