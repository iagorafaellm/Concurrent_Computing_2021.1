public class ReaderWriter_Mon {
    private int value;
    private int readers;
    private int writers;

    public ReaderWriter_Mon() {
        this.value = 0;
        this.readers = 0;
        this.writers = 0;
    }

    public synchronized int getValue() {
        return this.value;
    }
    public synchronized void setValue(int value) {
        this.value = value;
    }

    public synchronized void readerIn(int id) {
        try {
            while (this.writers > 0) {
                System.out.println("le.leitorBloqueado(" + id + ")");
                wait();
            }
            this.readers++;
            System.out.println("le.leitorLendo(" + id + ")");
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public synchronized void readerOut(int id) {
        this.readers--;
        if (this.readers == 0)
            this.notify();
        System.out.println ("le.leitorSaindo(" + id + ")");
    }

    public synchronized void writerIn(int id) {
        try {
            while ((this.readers > 0) || (this.writers > 0)) {
                System.out.println("le.escritorBloqueado(" + id + ")");
                wait();
            }
            this.writers++;
            System.out.println("le.escritorEscrevendo(" + id + ")");
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public synchronized void writerOut(int id) {
        this.writers--;
        notifyAll();
        System.out.println ("le.escritorSaindo("+id+")");
    }
}