package main;

import account.Account;
import bank.Bank;

public class Main {
    public static void main(String[] args) {
        Bank bank = new Bank();
        bank.lookupOrCreate("Tyrion");
    }
}