package shortestpath;

public class Location {
    String name;
    Double latitude;
    Double longitude;
    Location[] neighbors;
    Double distance;

    Location() {
        this.name = "Evry";
        this.latitude = 48.629828;
        this.longitude = 2.4417819999999892;
    }

    Location(String name, Double longitude, Double latitude) {
        longitude = (Math.PI * longitude) / 180;
        latitude = (Math.PI * latitude) / 180;
        this.name = name;
        this.longitude = longitude;
        this.latitude = latitude;
        this.distance = Math.random() * 100;
    }

    public void display() {
        System.out.println("The city is : " + this.name + " and his location : " + this.longitude.toString() + " " + this.latitude.toString());
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
}
