// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <source_location>
#include <unordered_map>

// Type for beacon IDs
using BeaconID = std::string;
using Name = std::string;

// Return value for cases where required beacon was not found
BeaconID const NO_BEACON= "--NO_BEACON--";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "-- NO_NAME --";

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

// Type for color (RGB)
struct Color
{
    int r = NO_VALUE;
    int g = NO_VALUE;
    int b = NO_VALUE;
};

// Equality and non-equality comparisons for Colors
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1==c2); }

// Return value for cases where color was not found
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost (used only in the second assignment)
using Cost = int;

// Return value for cases where cost is unknown
Cost const NO_COST = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    explicit NotImplemented(std::string const msg = "",
                            const std::source_location location = std::source_location::current())
        : msg_{}
    {
        std::string funcname = location.function_name();
        if (auto namestart = funcname.find_last_of(':'); namestart != std::string::npos)
        { funcname.erase(0, namestart+1); }
        if (auto nameend = funcname.find_first_of('('); nameend != std::string::npos)
        { funcname.erase(nameend, std::string::npos); }
        msg_ = (!msg.empty() ? msg+" in " : "")+funcname+"()";
    }
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

    // A operations

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map insertion is constant time on average.
    bool add_beacon(BeaconID id, Name const& name, Coord xy, Color color);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map size method is constant time.
    int beacon_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Clearing the map requires destructing all N elements.
    void clear_beacons();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Iterating through the map to copy all IDs is linear.
    std::vector<BeaconID> all_beacons();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map lookup is constant time on average.
    Name get_name(BeaconID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map lookup is constant time on average.
    Coord get_coordinates(BeaconID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map lookup is constant time on average.
    Color get_color(BeaconID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: Collecting all beacons takes O(N) and sorting them takes O(N log N).
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: Collecting all beacons takes O(N) and sorting them by brightness takes O(N log N).
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance: O(N)
    // Short rationale for estimate: We must iterate through all beacons to find the minimum.
    BeaconID min_brightness();

    // Estimate of performance: O(N)
    // Short rationale for estimate: We must iterate through all beacons to find the maximum.
    BeaconID max_brightness();

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: Finding matches takes O(N) in worst case (iterating all), sorting the result takes O(N log N).
    std::vector<BeaconID> find_beacons(Name const& name);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map lookup is constant time on average.
    bool change_beacon_name(BeaconID id, Name const& newname);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: Unordered map lookup and vector push_back are constant time on average.
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance: O(K log K)
    // Short rationale for estimate: Copying K sources and sorting them.
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: In the worst case (a single long chain), we visit all N beacons recursively.
    std::vector<BeaconID> path_outbeam(BeaconID id);

    // B operations

    // Estimate of performance: O(N)
    // Short rationale for estimate: Recursively traverses the light beam graph; worst case visits all beacons.
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Recursively traverses incoming beams to calculate sums; worst case visits all beacons.
    Color total_color(BeaconID id);

    // Estimate of performance: O(K) (where K is degree of node)
    // Short rationale for estimate: We check neighbors to prevent duplicates, which is linear in terms of edge count at that node.
    bool add_fibre(Coord xpoint1, Coord xpoint2, Cost cost);

    // Estimate of performance: O(P log P) (where P is number of xpoints)
    // Short rationale for estimate: We collect all points and sort them.
    std::vector<Coord> all_xpoints();

    // Estimate of performance: O(K log K) (where K is number of fibres)
    // Short rationale for estimate: Copying neighbors and sorting them.
    std::vector<std::pair<Coord, Cost>> get_fibres_from(Coord xpoint);

    // Estimate of performance: O(E log E) (where E is total number of fibres)
    // Short rationale for estimate: We iterate all edges and sort them.
    std::vector<std::pair<Coord, Coord>> all_fibres();

    // Estimate of performance: O(K) (where K is degree of node)
    // Short rationale for estimate: Finding the specific fiber in the adjacency vector is linear in the number of neighbors.
    bool remove_fibre(Coord xpoint1, Coord xpoint2);

    // Estimate of performance: O(P)
    // Short rationale for estimate: Clearing the map requires destructing all elements.
    void clear_fibres();

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(V + E)
    // Short rationale for estimate: Breadth-First Search (BFS) visits every node and edge at most once.
    std::vector<std::pair<Coord, Cost>> route_any(Coord fromxpoint, Coord toxpoint);

    // C operations

    // Estimate of performance: O(V + E)
    // Short rationale for estimate: Breadth-First Search (BFS) is used to find the unweighted shortest path.
    std::vector<std::pair<Coord, Cost>> route_least_xpoints(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance: O(E log V)
    // Short rationale for estimate: Dijkstra's algorithm using a binary heap (priority queue).
    std::vector<std::pair<Coord, Cost>> route_fastest(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance: O(V + E)
    // Short rationale for estimate: Depth-First Search (DFS) visits nodes and edges to detect cycles.
    std::vector<Coord> route_fibre_cycle(Coord startxpoint);

private:
    // Explain below your rationale for choosing the data structures you use in this class.
    // - std::unordered_map<BeaconID, Beacon>: Provides O(1) average time complexity for beacon lookups by ID, essential for frequent access operations.
    // - struct Beacon: Aggregates all beacon data. 'target' and 'sources' (vector) allow efficient traversal of the light beam graph.
    // - std::unordered_map<Coord, std::vector...>: Implements an Adjacency List for the fiber network. This is optimal for sparse graphs and enables efficient BFS/DFS/Dijkstra implementations.
    // - CoordHash: Required to use Coord as a key in unordered_map.

    // Add stuff needed for your class implementation below
    struct Beacon {
        BeaconID id;
        Name name;
        Coord coord;
        Color color;

        BeaconID target = NO_BEACON; // Outgoing light beam
        std::vector<BeaconID> sources; // Incoming light beams
    };

    std::unordered_map<BeaconID, Beacon> beacons_;

    // Helper function to calculate brightness
    int calculate_brightness(const Color& color) const {
        return 3*color.r + 6*color.g + color.b;
    }

    std::unordered_map<Coord, std::vector<std::pair<Coord, Cost>>, CoordHash> fibres_;
};

#endif // DATASTRUCTURES_HH
