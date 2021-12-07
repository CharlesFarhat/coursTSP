package shortestpath;

public class LocationSet {
    Location[] locations;
    int nbLocations;

    LocationSet() {
        this.nbLocations = 0;
        this.locations = new Location[1000];
    }

    void add(Location location) {
        this.locations[nbLocations] = location;
        nbLocations++;
    }

    Location removeMin() {
        if (nbLocations == 0) {
            return null;
        }
        double minDistance = locations[0].distance;
        int leastDistanceIndice = 0;
        for (int i=1; i < nbLocations; i++) {
            if (locations[i] != null) {
                if ((locations[i].distance < minDistance)) {
                    minDistance = locations[i].distance;
                    leastDistanceIndice = i;
                }
            }
        }
        // Remove it from the list
        Location out = locations[leastDistanceIndice];
        locations[leastDistanceIndice] = locations[nbLocations-1]; // exchange with the last city to keep the list without any nulls
        nbLocations--;
        return out;
    }
}
