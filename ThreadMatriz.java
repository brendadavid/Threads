public class ThreadMatriz extends Thread {
    private int posicaoInicial;
    private int posicaoFinal;

    ThreadMatriz (int posicaoInicial, int posicaoFinal){
        this.posicaoInicial = posicaoInicial;
        this.posicaoFinal = posicaoFinal;
    }

    @Override
    public void run(){
        // REALIZA A MULTIPLICACAO
        int k = 1;
        for (int i = posicaoInicial; i < posicaoFinal; i++) {
            for (int j = 0; j < MultiThread.mres[0].length; j++) {
                MultiThread.mres[i][j] = 0;
                for (k = 0; k < MultiThread.m2.length; k++) {
                    MultiThread.mres[i][j] += MultiThread.m1[i][k] * MultiThread.m2[k][j];
                }
            }
        }
    }
}
