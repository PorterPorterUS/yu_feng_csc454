using System;
using System.Collections.Generic;
/*
 -- Here is the C# implementation of combination;
    To compile this code, simply using: /u/cs254/bin/csc combinations.cs
    To run the code, simply run: /u/cs254/bin/mono combinations.exe k n;

*/
namespace HW01
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            // make a list to store the combination in string;
            // print all the combination into concole;
            foreach (string line in CombinationThree(Int32.Parse(args[0]), 1, Int32.Parse(args[1])))
            {
                Console.WriteLine(line);
            }

        }
        /*
            This combination function is a recursive function to recursively call 
            itself, and then examine the returned list, several "if" statement is
            listed below:

            if:     the return list is empty, return a empty list, or if the 
                    combination number is larger the range of (start, end);
            elseif: the combination number is "1", just return all the number into
                    string as a list;
            elseif: the combination just equals to the range between the "start" 
                    and "end", then just simply return all the number in a single
                    string; 
            else:   add the additional number (if fits the critera) on each list;

        */
        public static List<string> CombinationThree(int num, int start, int end) {
            /* check the first "if" statement. */
            if (num > (end - start + 1)) {
                return new List<string>();
            }
            /* check the second statement, and 
            return a list a single number string. */ 
            else if (num == 1) {
                /* make a new list. */
                List<string> ret_list = new List<string>();
                for (int i = start; i <= end; i++) {
                    ret_list.Add(i.ToString());
                }
                return ret_list;
            } 
            /* if the combination number equals to (end - start),
            then return a single string of numbers as a list. */
            else if (num == (end - start + 1)) {
                return MakeNumsString(start, end);
            } 
            /* make a for loop to call every sub combination function, and then,
                if the sub-function return a actually list, add the frist element
                into each string, such as:
                    list = combination(1, 2, 3);  <-- return {"2", "3"};
                    for loop add each string "1 ";
                    so it will return {"1 2", "1 3"};
                if the string.length == 0, then it won't add additional number. 
            */
            else {
                List<string> ret_list = new List<string>();
                for (int i = start + 1; i <= end; i++) {
                    /* recursively call the sub-combination function. */
                    List<string> tmp_list = CombinationThree(num - 1, i, end);
                    foreach (string line in tmp_list) {
                        /* check and add each string with additional number in 
                            the beginning. */
                        if (line.Length > 0) {
                            ret_list.Add((i - 1).ToString() + " "+ line);
                        }
                    }
                }
        return ret_list;

            }
        }
        /* this is a simple function to make a list of integer to a list of 
            single string with space as a seperator.
        */
        private static List<string> MakeNumsString(int start, int end)
        {
            string line = start.ToString();
            for (int num = start; num <= end; num++) {
                line += (" " + num.ToString());
            }
            List<string> ret_list = new List<string>();
            ret_list.Add(line);
            return ret_list;
        }
    }
}