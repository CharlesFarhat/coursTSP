package shortestpath;

public class Location {
    String name;
    Double latitude;
    Double longitude;
    Location[] neighbors;
    Double distance;
    Location from;

    Location(String name, Double longitude, Double latitude) {
        longitude = (Math.PI * longitude) / 180;
        latitude = (Math.PI * latitude) / 180;
        this.name = name;
        this.longitude = longitude;
        this.latitude = latitude;
        this.distance = Double.POSITIVE_INFINITY;
    }

    public void display() {
        System.out.println("The city is : " + this.name + " and his location : " + this.longitude.toString() + " " + this.latitude.toString() + "\n And distance is " + this.distance);
    }

    double distanceTo(Location to) {
        double R = 6378.0;
        // calcul la distance entre deux villes
        return R * (Math.PI / 2 - Math.asin(Math.sin(to.latitude) * Math.sin(this.latitude) + Math.cos(to.longitude - this.longitude) * Math.cos(to.latitude) * Math.cos(this.latitude)));
    }

    void setNeighbors(Location... neighbors) {
        this.neighbors = neighbors;
    }

    double getDistance() {
        return distance;
    }

    // Mise en place de l'algo

    void processNode(Location cur, LocationSet locSet) {
        for (Location neighbor : cur.neighbors) {
            double distance = cur.distance + cur.distanceTo(neighbor);
            if (neighbor.distance == Double.POSITIVE_INFINITY) {
                neighbor.distance = distance;
                neighbor.from = cur;
                locSet.add(neighbor);
            }
            if (neighbor.distance > distance) {
                neighbor.distance = distance;
                neighbor.from = cur;
            }
        }
    }

    void findPathTo(Location to) {
        this.distance = (double) 0;
        LocationSet locationSet = new LocationSet();
        this.processNode(this, locationSet);
        Location cur = locationSet.removeMin();
        while (cur != to) {
            processNode(cur, locationSet);
            cur = locationSet.removeMin();
        }

        Location out = cur;
        while (out != this) {
            System.out.print("hop \n");
            out.display();
            out = out.from;
        }
        out.display();
    }
}
