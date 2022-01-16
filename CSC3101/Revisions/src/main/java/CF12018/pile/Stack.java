package CF12018.pile;

public class Stack<E> {
    StackNode<E> head;
    Stack() { head = null; }


    public void push(E element) {
        StackNode node = new StackNode<>(head, element);
        this.head = node;
    }

    public E pop() {
        if (this.head != null) {
            E res = this.head.element;
            this.head = this.head.next;
            return res;
        }
        return null;
    }
}