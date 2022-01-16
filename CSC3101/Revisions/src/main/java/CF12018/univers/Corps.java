package CF12018.univers;

public abstract class Corps {
    private String name;
    public int mass;

    public Corps(String name, int mass) {
        this.mass = mass;
        this.name = name;
    }

    public String toString() {
        return this.name;
    }

    public abstract String famille();

}
