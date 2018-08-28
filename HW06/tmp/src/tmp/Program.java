package tmp;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CyclicBarrier;

public class Program {
	public static ConcurrentHashMap<DeltaWorker, Integer> map;
	public static int totalRound = 10;
	public static ConcurrentLinkedQueue<Integer> queue;
	public static Object lock = new Object();
	public static ArrayList<Thread> threads = new ArrayList<>();

	public static void main(String[] args) {
		int nThread = 10;
		map = new ConcurrentHashMap<>();
		queue = new ConcurrentLinkedQueue<>();
		CyclicBarrier b = new CyclicBarrier(nThread);
		for (int i = 0; i < 10; i++) {
			synchronized (lock) {
				int seed = i;
				System.out.println("seed: " + seed);
				map.put(new DeltaWorker(b, seed * 10, (seed + 1) * 10), 0);
			}

		}
		for (DeltaWorker w : map.keySet()) {
			System.out.println("Thread: " + w.toString());
			Thread t = new Thread(w);
			threads.add(t);
			t.start();
		}
		for (Thread t : threads) {
			try {
				System.out.println("Joined: " + t.toString());
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		for (DeltaWorker w : map.keySet()) {
			System.out.println("this is number " + map.get(w));
		}
	}
}

/* HERE, the real thread class for delta-stepping algorithm. */
class DeltaWorker implements Runnable {
	private LinkedHashSet<Integer> set;
	private CyclicBarrier barrier;
	private int from;
	private int to;

	public DeltaWorker(CyclicBarrier b, int f, int t) {
		from = f;
		to = t;
		set = new LinkedHashSet<>();
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
				System.out.println("Add: " + i);
				Program.queue.add(i+cnt);
			}
			try {
				barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}
			int total = 0;
			
			while (true) {
				int num = 0;
				synchronized(Program.lock) {
					if (!Program.queue.isEmpty()) {
						num = Program.queue.peek() - cnt;
					} else {	
						break;
					}	
				if (num < to && from <= num) {
					System.out.println("Get: " + num);
					total += Program.queue.poll();
				}
				}
			}
			
			Program.map.put(this, Program.map.get(this)+total);
			try {
				System.out.println("I am waiting.");
				barrier.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}
			cnt += 1;
			
			// Resetting the newBarrier
//			System.out.println("Barrier num cnt: " + barrier.getParties() + " : " + barrier.getNumberWaiting());
	        
		}	
	}
}
