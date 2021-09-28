/* Disciplina: Computação Concorrente - 2021.1 */
/* Prof.: Silvana Rossetto */
/* Estudante: Iago Rafael Lucas Martins */
/* Segunda lista de exercícios - Exercício 04, letra b) */

class Exercise04LetterB {
    private int reader; //quantidade de leitores lendo
    private int writer;  //quantidade de escritores escrevendo
    private int writersWaiting; //quantidade de escritores na fila (bloqueados)

    Exercise04LetterB () {
        this.reader = 0;
        this.writer = 0;
        this.writersWaiting = 0;
    }

    //entrada para leitores
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.writer > 0 || this.writersWaiting > 0) {
                //aqui, enquanto tivermos um escritor na fila, bloquearemos
                System.out.println ("le.leitorBloqueado("+id+")");
                wait();
            }
            this.reader++;  //rmais de um leitor lendo
            System.out.println ("le.leitorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    //saída para leitores
    public synchronized void SaiLeitor (int id) {
        this.reader--; //sai um leitor
        if (this.reader == 0)
            this.notify(); //se existir escritor bloqueado, desbloqueia um escritor
        System.out.println ("le.leitorSaindo("+id+")");
    }

    //entrada para escritores
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.reader > 0) || (this.writer > 0)) {
                //aqui, enquanto tivermos um escritor ou leitor sendo executado, bloquearemos
                System.out.println ("le.escritorBloqueado("+id+")");
                this.writersWaiting +=1;
                wait();
            }
            this.escr++; //escritor escrevendo
            System.out.println ("le.escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }

    //saída para escritores
    public synchronized void SaiEscritor (int id) {
        this.writer--; //escritor sai
        this.writersWaiting--;
        notifyAll(); //se existir escritor ou leitor bloqueado, desbloqueia escritores e leitores
        System.out.println ("le.escritorSaindo("+id+")");
    }
}