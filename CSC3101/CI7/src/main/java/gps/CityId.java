package gps;

public class CityId {

    public CityId(String name) {
        this.name = name;
    }

    public String toString() {
        return this.name;
    }

    public boolean equals(Object o){
        if (o instanceof CityId) {
            return ((CityId) o).name.equals(this.name);
        }
        return false;
    }

    public int hashCode() {
        return this.name.hashCode();
    }

    private String name;
}
