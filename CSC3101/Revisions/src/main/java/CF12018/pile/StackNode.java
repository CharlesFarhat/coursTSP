package CF12018.pile;

public class StackNode<E> {
    StackNode<E> next;
    E            element;
    StackNode(StackNode<E> next, E element) { this.next = next; this.element = element; }
}