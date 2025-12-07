// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <algorithm>
#include <queue>
#include <functional>

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
    // Write any initialization you need here

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here

}

bool Datastructures::add_beacon(BeaconID id, const Name& name, Coord xy, Color color)
{
    // Check if the beacon with this ID exists
    if (beacons_.find(id) != beacons_.end()) {
        return false;
    }

    // Create new beacon
    Beacon beacon;
    beacon.id = id;
    beacon.name = name;
    beacon.color = color;
    beacon.coord = xy;
    beacon.target = NO_BEACON;

    // Add to map
    beacons_[id] = beacon;
    return true;
}

int Datastructures::beacon_count()
{
    return beacons_.size();
}

void Datastructures::clear_beacons()
{
    beacons_.clear();
}

std::vector<BeaconID> Datastructures::all_beacons()
{
    std::vector<BeaconID> ids;
    ids.reserve(beacons_.size());

    for (const auto& pair : beacons_) {
        ids.push_back(pair.first);
    }

    return ids;
}

Name Datastructures::get_name(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it == beacons_.end()) {
        return NO_NAME;
    }

    return it->second.name;
}

Coord Datastructures::get_coordinates(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it == beacons_.end()) {
        return NO_COORD;
    }

    return it->second.coord;
}

Color Datastructures::get_color(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it == beacons_.end()) {
        return NO_COLOR;
    }

    return it->second.color;
}

std::vector<BeaconID> Datastructures::beacons_alphabetically()
{
    std::vector<BeaconID> alphabetically_ids;
    alphabetically_ids.reserve(beacons_.size());

    // Store all beacon IDs
    for (const auto& pair : beacons_) {
        alphabetically_ids.push_back(pair.first);
    }

    // Sort IDs by comparing their beacon names
    std::sort(alphabetically_ids.begin(), alphabetically_ids.end(),
              [this](const BeaconID& id1, const BeaconID& id2) {
                    return beacons_.at(id1).name < beacons_.at(id2).name;
    });

    return alphabetically_ids;
}

std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
    std::vector<BeaconID> brightness_ids;

    // Store all beacon IDs
    for (const auto& pair : beacons_) {
        brightness_ids.push_back(pair.first);
    }

    // Sort IDs by comparing their brightness
    std::sort(brightness_ids.begin(), brightness_ids.end(),
              [this](const BeaconID& id1, const BeaconID& id2) {
        int brightness1 = calculate_brightness(beacons_.at(id1).color);
        int brightness2 = calculate_brightness(beacons_.at(id2).color);
        return brightness1 < brightness2;
    });

    return brightness_ids;
}

BeaconID Datastructures::min_brightness()
{
    if (beacons_.size() == 0) {
        return NO_BEACON;
    }

    auto min_it = beacons_.begin();
    int min = calculate_brightness(min_it->second.color);

    for(auto it = beacons_.begin(); it != beacons_.end(); ++it) {
        int brightness = calculate_brightness(it->second.color);
        if (min > brightness) {
            min = brightness;
            min_it = it;
        }
    }

    return min_it->first;
}

BeaconID Datastructures::max_brightness()
{
    if (beacons_.size() == 0) {
        return NO_BEACON;
    }

    auto max_it = beacons_.begin();
    int max = calculate_brightness(max_it->second.color);

    for(auto it = beacons_.begin(); it != beacons_.end(); ++it) {
        int brightness = calculate_brightness(it->second.color);
        if (max < brightness) {
            max = brightness;
            max_it = it;
        }
    }

    return max_it->first;
}

std::vector<BeaconID> Datastructures::find_beacons(Name const& name)
{
    std::vector<BeaconID> ids;

    // Find all beacons with the given name
    for (const auto& pair : beacons_) {
        if(pair.second.name == name) {
            ids.push_back(pair.first);
        }
    }

    // Sort alphabetically
    std::sort(ids.begin(), ids.end());

    return ids;
}

bool Datastructures::change_beacon_name(BeaconID id, const Name& newname)
{
    if (beacons_.find(id) == beacons_.end()) {
        return false;
    }

    beacons_[id].name = newname;
    return true;
}

bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
    // Check if the beacons exist
    if (beacons_.find(sourceid) == beacons_.end() or beacons_.find(targetid) == beacons_.end()) {
        return false;
    }

    // Check if the source beacon already has a target
    if (beacons_[sourceid].target != NO_BEACON) {
        return false;
    }

    beacons_[sourceid].target = targetid;
    beacons_[targetid].sources.push_back(sourceid);

    return true;
}

std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{
    auto sources = beacons_[id].sources;

    // Beacon not found
    if (beacons_.find(id) == beacons_.end()) {
        return {NO_BEACON};
    }

    // Sort alphabetically
    std::sort(sources.begin(), sources.end());
    return sources;
}

std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
    // Beacon not found
    if (beacons_.find(id) == beacons_.end()) {
        return {NO_BEACON};
    }

    // Store the beacon
    std::vector<BeaconID> targets;
    auto target = beacons_[id].target;
    targets.push_back(id);

    // Recursively store the beacon's target
    if (beacons_[id].target != NO_BEACON) {
        auto rest = path_outbeam(target);
        targets.insert(targets.end(), rest.begin(), rest.end());
    }

    return targets;
}
std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID id)
{
    // Beacon not found
    if (beacons_.find(id) == beacons_.end()) {
        return {NO_BEACON};
    }

    std::vector<BeaconID> longest_chain;

    // Return a vector with just id if the sources is empty
    if (beacons_[id].sources.empty()) {
        return {id};
    }
    // Recursively find the longest path
    else {
        for (const auto& source : beacons_[id].sources) {
            auto longest = path_inbeam_longest(source);
            if (longest.size() > longest_chain.size()) {
                longest_chain = longest;
            }
        }
    }

    // Store the id last
    longest_chain.push_back(id);
    return longest_chain;
}

Color Datastructures::total_color(BeaconID id)
{
    // Beacon not found
    if (beacons_.find(id) == beacons_.end()) {
        return NO_COLOR;
    }

    int sum_r = beacons_[id].color.r;
    int sum_g = beacons_[id].color.g;
    int sum_b = beacons_[id].color.b;
    int count = 1;

    if (beacons_[id].sources.empty()) {
        return beacons_[id].color;
    }
    // Recursively calculate sum of all r, g, and b
    else {
        for (const auto& source : beacons_[id].sources) {
            auto sum = total_color(source);

            sum_r += sum.r;
            sum_g += sum.g;
            sum_b += sum.b;
            count++;
        }
    }

    // Return average
    return Color{sum_r/count, sum_g/count, sum_b/count};
}

bool Datastructures::add_fibre(Coord xpoint1, Coord xpoint2, Cost cost)
{
    // Check if both points are the same
    if (xpoint1 == xpoint2) {
        return false;
    }
    // Check if there is already a fiber between the given points
    for (const auto& pair : fibres_[xpoint1]) {
        if (pair.first == xpoint2) {
            return false;
        }
    }

    // Add fiber to data structure
    fibres_[xpoint1].push_back({xpoint2, cost});
    fibres_[xpoint2].push_back({xpoint1, cost});

    return true;
}

std::vector<Coord> Datastructures::all_xpoints()
{
    std::vector<Coord> result;

    // Store all points with at least one fiber
    for (const auto& fiber : fibres_) {
        result.push_back(fiber.first);
    }

    // Sort the list
    std::sort(result.begin(), result.end());

    return result;
}

std::vector<std::pair<Coord, Cost> > Datastructures::get_fibres_from(Coord xpoint)
{
    auto it = fibres_.find(xpoint);
    // Check if xpoint exist
    if (it == fibres_.end()) {
        return {};
    }

    std::vector<std::pair<Coord, Cost>> coord_cost = it->second;

    std::sort(coord_cost.begin(), coord_cost.end());

    return coord_cost;
}

std::vector<std::pair<Coord, Coord> > Datastructures::all_fibres()
{
    std::vector<std::pair<Coord, Coord>> fibres;

    for (const auto& fibre : fibres_) {
        Coord start_point = fibre.first;

        for (const auto& pair : fibre.second) {
            Coord end_point = pair.first;

            // Check if the first point is smaller than the end point
            if (start_point < end_point) {
                fibres.push_back({start_point, end_point});
            }
        }
    }

    std::sort(fibres.begin(), fibres.end());

    return fibres;
}

bool Datastructures::remove_fibre(Coord xpoint1, Coord xpoint2)
{
    if (fibres_.find(xpoint1) == fibres_.end() or fibres_.find(xpoint2) == fibres_.end()) {
        return false;
    }

    auto& neighbors = fibres_[xpoint1];

    auto it = std::find_if(neighbors.begin(), neighbors.end(),
        [&] (const std::pair<Coord, Cost>& coord_cost) {
        return coord_cost.first == xpoint2;
    });

    if (it == neighbors.end()) {
        return false;
    }

    neighbors.erase(it);

    auto& neighbors2 = fibres_[xpoint2];

    auto it2 = std::find_if(neighbors2.begin(), neighbors2.end(),
        [&] (const std::pair<Coord, Cost>& coord_cost) {
        return coord_cost.first == xpoint1;
    });

    neighbors2.erase(it2);

    return true;
}

void Datastructures::clear_fibres()
{
    fibres_.clear();
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_any(Coord fromxpoint, Coord toxpoint)
{
    // Start and End are the same
    if (fromxpoint == toxpoint) {
        return {{fromxpoint, 0}};
    }

    // If one or both don't exist, return empty path
    if (fibres_.find(fromxpoint) == fibres_.end() || fibres_.find(toxpoint) == fibres_.end()) {
        return {};
    }

    // Data Structures for BFS
    std::queue<Coord> q;
    q.push(fromxpoint);

    // Track where we came from to reconstruct the path later
    // Key: Current Node, Value: Parent Node
    std::unordered_map<Coord, Coord, CoordHash> came_from;

    // Track cumulative cost to reach each node
    std::unordered_map<Coord, Cost, CoordHash> cost_so_far;

    // Initialize start point
    came_from[fromxpoint] = fromxpoint;
    cost_so_far[fromxpoint] = 0;

    bool found = false;

    // Run BFS
    while (!q.empty()) {
        Coord current = q.front();
        q.pop();

        // Stop if we reached the target
        if (current == toxpoint) {
            found = true;
            break;
        }

        // Check all neighbors
        for (const auto& edge : fibres_[current]) {
            Coord neighbor = edge.first;
            Cost edge_cost = edge.second;

            // If we have not visited neighbor
            if (came_from.find(neighbor) == came_from.end()) {

                // Record the path and cost
                came_from[neighbor] = current;
                cost_so_far[neighbor] = cost_so_far[current] + edge_cost;

                // Add to queue to explore next
                q.push(neighbor);
            }
        }
    }

    // No route found
    if (!found) {
        return {};
    }

    std::vector<std::pair<Coord, Cost>> path;
    Coord curr = toxpoint;

    // Walk backward from End
    while (curr != fromxpoint) {
        path.push_back({curr, cost_so_far[curr]});
        curr = came_from[curr];
    }

    // Add the starting point
    path.push_back({fromxpoint, 0});

    std::reverse(path.begin(), path.end());

    return path;
}

// Since I implement BFS in previous function, this one is almost the same
std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord fromxpoint, Coord toxpoint)
{
    if (fromxpoint == toxpoint) {
        return {{fromxpoint, 0}};
    }
    if (fibres_.find(fromxpoint) == fibres_.end() || fibres_.find(toxpoint) == fibres_.end()) {
        return {};
    }

    std::queue<Coord> q;
    q.push(fromxpoint);

    std::unordered_map<Coord, Coord, CoordHash> came_from;
    std::unordered_map<Coord, Cost, CoordHash> cost_so_far;

    came_from[fromxpoint] = fromxpoint;
    cost_so_far[fromxpoint] = 0;

    bool found = false;

    while (!q.empty()) {
        Coord current = q.front();
        q.pop();

        if (current == toxpoint) {
            found = true;
            break;
        }

        for (const auto& edge : fibres_[current]) {
            Coord neighbor = edge.first;
            Cost edge_cost = edge.second;

            if (came_from.find(neighbor) == came_from.end()) {
                came_from[neighbor] = current;
                cost_so_far[neighbor] = cost_so_far[current] + edge_cost;
                q.push(neighbor);
            }
        }
    }

    if (!found) return {};

    std::vector<std::pair<Coord, Cost>> path;
    Coord curr = toxpoint;

    while (curr != fromxpoint) {
        path.push_back({curr, cost_so_far[curr]});
        curr = came_from[curr];
    }
    path.push_back({fromxpoint, 0});
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord fromxpoint, Coord toxpoint)
{
    // Some basic Checks
    if (fromxpoint == toxpoint) return {{fromxpoint, 0}};
    if (fibres_.find(fromxpoint) == fibres_.end() || fibres_.find(toxpoint) == fibres_.end()) return {};

    // Ordered so the smallest cost is always on top
    std::priority_queue<std::pair<Cost, Coord>, std::vector<std::pair<Cost, Coord>>, std::greater<std::pair<Cost, Coord>>> pq;

    // Stores the cheapest cost found so far to get to a node
    std::unordered_map<Coord, Cost, CoordHash> dist;

    // Reconstruct the path
    std::unordered_map<Coord, Coord, CoordHash> came_from;

    // Initialize Start
    dist[fromxpoint] = 0;
    pq.push({0, fromxpoint}); // Cost 0 to start

    // Run Dijkstra
    while (!pq.empty()) {
        // Get the node with the lowest cost
        Cost current_cost = pq.top().first;
        Coord current = pq.top().second;
        pq.pop();

        // If we found a shorter way to 'current' before processing this item, skip it.
        if (dist.count(current) && current_cost > dist[current]) {
            continue;
        }

        // Found the target
        if (current == toxpoint) {
            break;
        }

        // Check Neighbors
        for (const auto& edge : fibres_[current]) {
            Coord neighbor = edge.first;
            Cost edge_weight = edge.second;

            Cost new_cost = current_cost + edge_weight;

            // If we found a faster way to the neighbor, update it
            if (dist.find(neighbor) == dist.end() || new_cost < dist[neighbor]) {
                dist[neighbor] = new_cost;
                came_from[neighbor] = current;
                pq.push({new_cost, neighbor});
            }
        }
    }

    // Target never reached
    if (came_from.find(toxpoint) == came_from.end()) {
        return {};
    }

    std::vector<std::pair<Coord, Cost>> path;
    Coord curr = toxpoint;

    while (curr != fromxpoint) {
        path.push_back({curr, dist[curr]});
        curr = came_from[curr];
    }
    path.push_back({fromxpoint, 0});
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<Coord> Datastructures::route_fibre_cycle(Coord startxpoint)
{
    // Some basic checks
    if (fibres_.find(startxpoint) == fibres_.end()) {
        return {};
    }

    std::unordered_map<Coord, int, CoordHash> state;

    // Store the path as we explore
    std::vector<Coord> path;

    // Recursive DFS Function
    std::function<bool(Coord, Coord)> dfs =
        [&](Coord current, Coord parent) -> bool {

        state[current] = 1; // Mark as Visiting (Grey)
        path.push_back(current);

        // Get neighbors and sort them
        auto neighbors = fibres_[current];
        std::sort(neighbors.begin(), neighbors.end());

        for (const auto& edge : neighbors) {
            Coord next = edge.first;

            // Don't simply go back to where we just came from
            if (next == parent) {
                continue;
            }

            // Found a back-edge to an active node
            if (state[next] == 1) {
                path.push_back(next); // Add the closing node to complete the loop visualization
                return true;
            }

            // Unvisited node, continue DFS
            if (state[next] == 0) {
                if (dfs(next, current)) {
                    return true;
                }
            }
        }

        // If we reach here, this path didn't lead to a cycle
        state[current] = 2; // Mark as Visited
        path.pop_back();    // Remove from current path
        return false;
    };

    // Start the Search
    if (dfs(startxpoint, NO_COORD)) {
        return path;
    }

    return {}; // No cycle found
}

// TEST SUBMISSION
