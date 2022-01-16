package CF12018.univers;

public class CalculateurMasse implements SkyWalker {

    @Override
    public void compute(Corps corps) {
        massUnivers += corps.mass;
    }

    private int massUnivers = 0;
}
