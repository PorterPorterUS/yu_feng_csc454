using System;
using System.Collections.Generic;

namespace HW01
{
    class MainClass
    {
        static private Dictionary<int, bool> IntDictionary =
            new Dictionary<int, bool>();
        public static void Main(string[] args)
        {
            foreach (string line in PrimePartition(20, 1))
			{
                Console.WriteLine(line);
			}

        }

        public static List<string> PrimePartition(int n, int i) {
            if (n <= i) {
                return new List<string>();
            } else {
				List<string> newlist = new List<string>();
				if (IsPrime(n))
				{
					newlist.Add(n.ToString());
				}
				foreach (int num in Prime(NextPrime(i), n))
				{
					List<string> list = PrimePartition(n - num, num);
					if (list.Count > 0)
					{
						foreach (string line in list)
						{
							newlist.Add(num.ToString() + " + " + line);
						}
					}
				}
				return newlist;

            }
        }

        public static List<int> Prime(int a, int b) 
        {
            List<int> list = new List<int>();
            for (int num = a; num <= b; num++) {
                if (IsPrime(num)) 
                {
                    list.Add(num);
                }
            }
            return list;
        }

        private static bool IsPrime(int num)
        {
            if (IntDictionary.ContainsKey(num)) {
                return IntDictionary[num];
            } else {
                if (num < 2) {
                    IntDictionary[num] = false;
                } else if (num == 2) {
                    IntDictionary[num] = true;
                } else if (num % 2 == 0) {
                    IntDictionary[num] = false;
                } else {
                    for (int i = 3; i * i <= num; i += 2) {
                        if (num % i == 0) {
                            IntDictionary[num] = false;
                            return false;
                        }
                    }
                    IntDictionary[num] = true;
                }
                return IntDictionary[num];
            }
        }

        private static int NextPrime(int num) {
            if (num > 0 && num < Int32.MaxValue) {
                num++;
                while (!IsPrime(num)) {
                    num++;
                }
                return num;
            } else {
                Console.WriteLine("Illegal Input value!");
                return -1;
            }

        }
    }
}
