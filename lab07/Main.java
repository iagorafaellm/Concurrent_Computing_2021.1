/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Módulo 2 - Laboratório 7 */
/* Código: projetar e implementar uma aplicação concorrente em Java usando o padrão básico leitores/escritores. */

import static java.lang.Math.sqrt;

public class Monitor_Main {
    public static void main(String[] args) {
        int vectorSize = 10;
        ReaderWriter_Mon monitor = new ReaderWriter_Mon();

        Reader[] readers = new Reader[vectorSize];
        Writer[] writers = new Writer[vectorSize];
        ReaderWriter[] readersWriters = new ReaderWriter[vectorSize];


        for (int i = 0; i < vectorSize; i++) {
            readers[i] = new Reader(i + 1, monitor);
            readers[i].start();
        }

        for (int i = 0; i < vectorSize; i++) {
            writers[i] = new Writer(i + 1, monitor);
            writers[i].start();
        }

        for (int i = 0; i < vectorSize; i++) {
            readersWriters[i] = new ReaderWriter(i + 1, monitor);
            readersWriters[i].start();
        }
    }
}
