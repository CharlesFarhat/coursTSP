package hashmap;

public class ListMap<K, V> {

    public ListMap(K key, V value) {
        this.key = key;
        this.value = value;
        this.prev = this;
        this.next = this;
    }

    public String toString() {
        return "(" + key.toString() + ", " + value.toString() + ")";
    }

    public void append(K key, V value) {
        ListMap<K, V> insert = new ListMap<>(key, value);
        insert.next = this.next;
        insert.prev = this;
        this.next.prev = insert;
        this.next = insert;
    }

    public void display() {
        ListMap<K, V> cur = this.next;
        while (cur.value != null) {
            System.out.println(cur.toString());
            cur = cur.next;
        }
    }

    private ListMap<K, V> next;
    private ListMap<K, V> prev;
    private K key;
    private V value;

}
