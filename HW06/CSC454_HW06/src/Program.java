import java.util.LinkedHashSet;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CyclicBarrier;

public class Program {
	public static ConcurrentHashMap<DeltaWorker, Integer> map;
	public static int totalRound = 10;
    public static ConcurrentLinkedQueue<Integer> queue;
    
    
	public static void main() {
		int nThread = 10;
		CyclicBarrier b = new CyclicBarrier(nThread);
		for (int i = 0; i < 10; i++) {
			DeltaWorker tmp = new DeltaWorker(b, i*10, (i+1)*10);
			map.put(tmp, 0);
		}
		for (DeltaWorker w : map.keySet()) {
			w.run();
		}
		for (DeltaWorker w : map.keySet()) {
			System.out.println("this is number " + map.get(w));
		}
		;
	}
}


/* HERE, the real thread class for delta-stepping algorithm.  */
class DeltaWorker implements Runnable {
	private LinkedHashSet<Integer> set;
	private CyclicBarrier barrier;
	private int from;
	private int to;
	public DeltaWorker(CyclicBarrier b, int f, int t) {
		from = f;
		to = t;
		for (int i = from; i < to; i++) {
			set.add(i);
		}
		barrier = b;
	}
	@Override
	public void run() {
		int cnt = 0;
		while (cnt < Program.totalRound) {
			for (int i: set) {
				Program.queue.add(i);
			}
			try {
				barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}
			int total = 0;
			while (!Program.queue.isEmpty()) {
				int num = Program.queue.peek() - cnt;
				if (num < to && from <= num) {
					total += Program.queue.poll();
				}
			}
			Program.map.put(this, Program.map.get(this)+total);
			try {
				barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}
		}	
	}
}