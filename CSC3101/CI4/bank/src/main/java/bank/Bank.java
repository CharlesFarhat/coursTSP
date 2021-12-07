package bank;

import account.Account;

public class Bank {

    public Bank() {
        this.root = new Node();
    }

    public Account lookupOrCreate(String name) {
        return this.root.lookupOrCreate(name);
    }

    Node root;
}
