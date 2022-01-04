import gps.CityId;
import gps.Location;
import hashmap.ListMap;

public class Main {

    public static void main(String[] args) {
        CityId toulouse = new CityId("Toulouse");
        Location locTls = new Location(43.604652, 1.4442090000000007);
        CityId paris = new CityId("Paris");
        Location locParis = new Location(43.60282, 1.448525580000007);

        ListMap<CityId, Location> map = new ListMap<>(null, null);
        map.append(toulouse, locTls);
        map.append(paris, locParis);

        map.display();

        System.out.println();
    }
}