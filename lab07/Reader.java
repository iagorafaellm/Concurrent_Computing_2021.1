import static java.lang.Math.sqrt;
public class Reader extends Thread {

    private static final int ITERATIONS = 10;
    private static final int DELAY = 100;

    private int id;
    private ReaderWriter_Mon monitor;

    public Reader(int id, ReaderWriter_Mon monitor) {
        this.id = id;
        this.monitor = monitor;
    }

    private static boolean isPrime(int n) {
        if (n <= 1)
            return false;


        for (int i = 2; i <= sqrt(n); i++)
            if (n % i == 0)
                return false;

        return true;
    }

    private void read() {
        this.monitor.readerIn(this.id);
        int value = this.monitor.getValue();
        boolean valueIsPrime = isPrime(value);
        System.out.println("[Leitor " + this.id + "] " + value + " é primo = " + valueIsPrime);
        this.monitor.readerOut(this.id);
    }

    @Override
    public void run() {
        try {
            for (int i = 0; i < ITERATIONS; i++) {
                read();
                sleep(DELAY);
            }
        }
        catch (InterruptedException e) {
            System.err.println(e);
            return;
        }
    }
}