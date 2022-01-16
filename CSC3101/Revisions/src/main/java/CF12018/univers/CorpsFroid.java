package CF12018.univers;

public class CorpsFroid extends Corps {
    Boolean estHabitable;

    public CorpsFroid(String name, int mass, Boolean estHabitable) {
        super(name, mass);
        this.estHabitable = estHabitable;
    }

    @Override
    public String famille() {
        return "Corps Froid";
    }
}
