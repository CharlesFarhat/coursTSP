package account;


public class Account {

    public Account(String name, double balance) {
        this.balance = balance;
        this.name = name;
    }

    public void display() {
        System.out.println(this.name + " : " + this.balance + " €");
    }

    public int compareTo(String name) {
        return this.name.compareTo(name);
    }

    public String name;
    public Double balance;
}
