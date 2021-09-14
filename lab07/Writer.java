public class Writer extends Thread {
    private static final int ITERATIONS = 10;
    private static final int DELAY = 100;

    private int id;
    private ReaderWriter_Mon monitor;

    public Writer(int id, ReaderWriter_Mon monitor) {
        this.id = id;
        this.monitor = monitor;
    }

    private void write() {
        this.monitor.writerIn(this.id);
        this.monitor.setValue(this.id);
        System.out.println("[Escritor " + this.id + "] mudou valor para " + this.id);
        this.monitor.writerOut(this.id);
    }

    @Override
    public void run() {
        try {
            for (int i = 0; i < ITERATIONS; i++) {
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