package CF12018.univers;

public class Main {

    public static void main(String[] args) {
        Univers univers = new Univers();


        CalculateurMasse calculateurMasse = new CalculateurMasse();
        univers.apply(calculateurMasse);
    }
}
