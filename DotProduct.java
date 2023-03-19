import java.math.BigDecimal;
import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;

public class DotProduct {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        System.out.print("生成的长度：");
        int n = in.nextInt();
        System.out.println();
        long startTime = System.currentTimeMillis();
        double[] vector1 = generateVector(n);
        double[] vector2 = generateVector(n);
        advancedCalculate(vector1,vector2,8);
        long finishTime = System.currentTimeMillis();
        System.out.println("花费时间：" + (finishTime - startTime) + "ms");
    }

    public static double[] generateVector(int n){
        double[] nums = new double[n];
        Arrays.parallelSetAll(nums, i -> ThreadLocalRandom.current().nextDouble(-10000, 10000));
        return nums;
    }

    public static void basicCalculate(double[] vector1, double[] vector2){
        BigDecimal sum = new BigDecimal("0");
        for(int i = 0; i < vector1.length; i++){
            sum = sum.add(BigDecimal.valueOf(vector1[i] * vector2[i]));
        }
        System.out.println("结果是：" + sum);
    }

    public static void advancedCalculate(double[] vector1, double[] vector2, int numThreads) {
        BigDecimal[] partialSums = new BigDecimal[numThreads];
        for (int i = 0; i < numThreads; i++) {
            partialSums[i] = BigDecimal.ZERO;
        }

        int n = vector1.length;
        int chunkSize = (n + numThreads - 1) / numThreads; // 计算每个线程处理的元素个数
        Thread[] threads = new Thread[numThreads];
        for (int i = 0; i < numThreads; i++) {
            final int tid = i;
            threads[i] = new Thread(() -> {
                int start = tid * chunkSize;
                int end = Math.min(n, start + chunkSize); //防止爆掉
                for (int j = start; j < end; j++) {
                    partialSums[tid] = partialSums[tid].add(BigDecimal.valueOf(vector1[j] * vector2[j])); //每一个线程处理一部分
                }
            });
            threads[i].start();
        }

        // 等待所有线程执行完毕
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                // ignored
            }
        }

        // 将所有部分和累加起来
        BigDecimal sum = BigDecimal.ZERO;
        for (int i = 0; i < numThreads; i++) {
            sum = sum.add(partialSums[i]);
        }

        System.out.println("结果是：" + sum);
    }
}
