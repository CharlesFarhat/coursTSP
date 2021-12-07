package bank;

import account.Account;

public class Node {

    public Node() {
        this.etiquette = null;
        this.left = null;
        this.right = null;
    }

    // Plus petit = gauche, plus grand = droite
    public Account lookupOrCreate(String name) {

        if (this.etiquette == null) {
            this.etiquette = new Account("name", 9999);
            return this.etiquette;
        }
        else if (this.etiquette.compareTo(name) > 0) {
            return this.left.lookupOrCreate(name);
        }
        else if (this.etiquette.compareTo(name) < 0) {
            return this.right.lookupOrCreate(name);
        }
        else if (this.etiquette.compareTo(name) == 0) {
            return this.etiquette;
        }

        return null;
    }

    Account etiquette;
    Node left;
    Node right;
}
