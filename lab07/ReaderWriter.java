public class ReaderWriter extends Thread {
    private static final int ITERATIONS = 10;
    private static final int DELAY = 100;

    private int id;
    private ReaderWriter_Mon monitor;

    public ReaderWriter(int id, ReaderWriter_Mon monitor) {
        this.id = id;
        this.monitor = monitor;
    }

    private static boolean isEven(int n) {
        return (n % 2) == 0;
    }

    private void write() {
        this.monitor.writerIn(this.id);
        int newValue = this.monitor.getValue() * 2;
        this.monitor.setValue(newValue);
        System.out.println("[LeitorEscritor " + this.id + "] mudou valor para " + newValue);
        this.monitor.writerOut(this.id);
    }

    private void read() {
        this.monitor.readerIn(this.id);
        int value = this.monitor.getValue();
        boolean isValueEven = isEven(value);
        System.out.println("[LeitorEscritor " + this.id + "] " + value + " é par = " + isValueEven);
        this.monitor.readerOut(this.id);
    }

    @Override
    public void run() {
        try {
            for (int i = 0; i < ITERATIONS; i++) {
                read();
                write();
                sleep(DELAY);
            }
        }
        catch (InterruptedException e) {
            System.err.println(e);
            return;
        }
    }
}