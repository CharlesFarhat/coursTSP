package CF12018.univers;

import java.util.Collection;
import java.util.HashMap;

public class Univers {

    public Univers() {
        this.carte = new HashMap<>();
    }


    public Corps get(String find) {
        return carte.get(find); // return null si il trouve rien
    }

    void put(Corps corps) throws CorpsExistDeja {
        String key = corps.toString();
        if ( carte.get(key) != null ) {
            carte.put(key, corps);
        } else {
            throw new CorpsExistDeja();
        }
    }

    public void printCarte() {
        Collection<Corps> planettes;
        planettes = carte.values();
        System.out.println(planettes.toString());
    }

    public void apply(SkyWalker luke) {
        for (Corps cur : carte.values()) {
            luke.compute(cur);
        }
    }

    HashMap<String, Corps> carte;
}

class CorpsExistDeja extends Exception { }