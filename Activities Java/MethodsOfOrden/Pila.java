public class Pila {
    static final int MAX_SIZE = 100;
    static int[] stack = new int[MAX_SIZE];
    static int top = -1;

    static void push(int item) {
        if (top == MAX_SIZE - 1) {
            System.out.println("Stack Overflow");
            return;
        }
        stack[++top] = item;
    }

    static int pop() {
        if (top == -1) {
            System.out.println("Stack Underflow");
            return -1;
        }
        return stack[top--];
    }

    static int peek() {
        if (top == -1) {
            System.out.println("Pila vacía");
            return -1;
        }
        return stack[top];
    }

    static boolean isEmpty() {
        return top == -1;
    }

    static boolean isFull() {
        return top == MAX_SIZE - 1;
    }

    public static void main(String[] args) {
        push(10);
        push(20);
        push(30);

        System.out.println("Elemento Superior: " + peek());
        System.out.println("Extrae elemento: " + pop());
        System.out.println("Elemento Superior: " + peek());
    }
}
