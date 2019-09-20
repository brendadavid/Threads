import java.util.ArrayList;
import java.util.List;

public class MultiThread {
    // DIMENSOES DAS MATRIZES
    public static final int SIZE = 2000;

    // ESTRUTURAS DE DADOS COMPARTILHADA
    public static int[][] m1;
    public static int[][] m2;
    public static int[][] mres;

    private static void criarMatrizes(int tamanho) {
        // INICIALIZA OS ARRAYS A SEREM MULTIPLICADOS
        m1 = new int[tamanho][tamanho];
        m2 = new int[tamanho][tamanho];
        mres = new int[tamanho][tamanho];
        if (m1[0].length != m2.length || mres.length != m1.length || mres[0].length != m2[0].length) {
            System.err.println("Impossivel multiplicar matrizes: parametros invalidos.");
            System.exit(1);
        }
        int k = 1;
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                if (k % 2 == 0) {
                    m1[i][j] = -k;
                } else {
                    m1[i][j] = k;
                }
            }
            k++;
        }
        k = 1;
        for (int j = 0; j < tamanho; j++) {
            for (int i = 0; i < tamanho; i++) {
                if (k % 2 == 0) {
                    m2[i][j] = -k;
                } else {
                    m2[i][j] = k;
                }
            }
            k++;
        }
    }

    private static void verificaResultado(int tamanho){
        // VERIFICA SE O RESULTADO DA MULTIPLICACAO ESTA CORRETO
        int k;
        for (int i = 0; i < tamanho; i++) {
            k = tamanho * (i + 1);
            for (int j = 0; j < tamanho; j++) {
                int k_col = k * (j + 1);
                if (i % 2 == 0) {
                    if (j % 2 == 0) {
                        if (mres[i][j] != k_col) {
                            System.exit(1);
                        }
                    } else {
                        if (mres[i][j] != -k_col) {
                            System.exit(1);
                        }
                    }
                } else {
                    if (j % 2 == 0) {
                        if (mres[i][j] != -k_col) {
                            System.exit(1);
                        }
                    } else {
                        if (mres[i][j] != k_col) {
                            System.exit(1);
                        }
                    }
                }
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        final int[] NUMERO_THREADS = {2, 4, 8};
        List<ThreadMatriz> threads;
        int tamanhoPorThread;
        double tamanhoThreadDouble;

        for(int core : NUMERO_THREADS){
            for(int vezes = 100; vezes <= SIZE; vezes += 100){
                threads = new ArrayList<ThreadMatriz>(core);
                int posFinalAnterior = 0;
                criarMatrizes(vezes);
                tamanhoThreadDouble = vezes/(double)core;

                // PREPARA PARA MEDIR TEMPO
                long inicio = System.nanoTime();

                for(int i = 0; i < core; i++){
                    if (i % 2 == 0 && tamanhoThreadDouble % 1 != 0) {
                        tamanhoPorThread = (int)tamanhoThreadDouble + 1;
                    } else {
                        tamanhoPorThread = (int)tamanhoThreadDouble;
                    }
                    threads.add(new ThreadMatriz(posFinalAnterior, (tamanhoPorThread + posFinalAnterior)));
                    posFinalAnterior = (tamanhoPorThread + posFinalAnterior);
                    threads.get(i).start();
                }

                for(int i = 0; i < core; i++){
                    threads.get(i).join();
                }

                verificaResultado(vezes);

                // OBTEM O TEMPO
                long fim = System.nanoTime();
                double total = (fim - inicio) / 1000000000.0;

                // MOSTRA O TEMPO DE EXECUCAO
                System.out.printf("%d Threads - %dX%d: %f\n", core, vezes, vezes, total);
            }
        }


//        ThreadMatriz t1 = new ThreadMatriz(0, 250);
//        ThreadMatriz t2 = new ThreadMatriz(250, 500);
//        ThreadMatriz t3 = new ThreadMatriz(500, 750);
//        ThreadMatriz t4 = new ThreadMatriz(750, 1000);
//        ThreadMatriz t5 = new ThreadMatriz(1000, 1250);
//        ThreadMatriz t6 = new ThreadMatriz(1250, 1500);
//        ThreadMatriz t7 = new ThreadMatriz(1500, 1750);
//        ThreadMatriz t8 = new ThreadMatriz(1750, 2000);
//        t1.start();
//        t2.start();
//        t3.start();
//        t4.start();
//        t5.start();
//        t6.start();
//        t7.start();
//        t8.start();
//        t1.join();
//        t2.join();
//        t3.join();
//        t4.join();
//        t5.join();
//        t6.join();
//        t7.join();
//        t8.join();
//
//        verificaResultado();
//
//        // OBTEM O TEMPO
//        long fim = System.nanoTime();
//        double total = (fim - inicio) / 1000000000.0;
//
//        // MOSTRA O TEMPO DE EXECUCAO
//        System.out.printf(SIZE + ": %f\n", total);
    }
}

