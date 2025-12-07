// Datastructures.cc

#include "datastructures.hh"

#include <random>

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

std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

Color Datastructures::total_color(BeaconID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

bool Datastructures::add_fibre(Coord /*xpoint1*/, Coord /*xpoint2*/, Cost /*cost*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<Coord> Datastructures::all_xpoints()
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<std::pair<Coord, Cost> > Datastructures::get_fibres_from(Coord /*xpoint*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<std::pair<Coord, Coord> > Datastructures::all_fibres()
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

bool Datastructures::remove_fibre(Coord /*xpoint1*/, Coord /*xpoint2*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

void Datastructures::clear_fibres()
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<std::pair<Coord, Cost> > Datastructures::route_any(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}

std::vector<Coord> Datastructures::route_fibre_cycle(Coord /*startxpoint*/)
{
    // Replace the line below with your implementation
    throw NotImplemented();
}
