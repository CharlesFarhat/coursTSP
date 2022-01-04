package gps;

public class Location {

    public Location(Double latitude, Double longitude) {
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public String toString() {
        return this.latitude + ", " + this.longitude;
    }



    private Double latitude;
    private Double longitude;
}
