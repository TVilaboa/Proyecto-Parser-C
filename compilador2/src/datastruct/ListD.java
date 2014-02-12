package datastruct;

public class ListD {
    private int length;
    private Node actual;
    private Node first;

    public ListD() {
        Node header = new Node();
        Node sentinel = new Node();
        first = header;
        actual = header;
        first.next = sentinel;
        length = 0;
    }

    public void goFirst() {
        actual = first.next;
    }

    public void goLast() {
        while (actual.next.next != null)
            goAhead();
    }

    public Object getActual() {
        return actual.x;
    }

    public void goBack() {
        Node p = first;
        while (p.next != actual) {
            p = p.next;
        }
        actual = p;
    }

    public void goAhead() {
        actual = actual.next;
    }

    public void addAfter(Object x) {
        Node p = new Node();
        p.x = x;
        p.next = actual.next;
        actual.next = p;
        actual = p;
        length++;
    }

    public void addBefore(Object x) {
        goBack();
        addAfter(x);
        length++;
    }

    public void remove() {
        actual.x = actual.next.x;
        actual.next = actual.next.next;
        length--;
    }

    public int getLength() {
        return length;
    }

    public void removeAll() {
        length = 0;
    }

    public boolean isEmpty() {
        return length == 0;
    }

    static class Node {
        Object x;
        Node next;

        public Node() {
            x = null;
            next = null;
        }

    }
}
