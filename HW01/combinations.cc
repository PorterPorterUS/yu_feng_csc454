#include <iostream>
#include <list>
#include <string>
#include <unistd.h>
#include <ctype.h>

using namespace std;

/*
 -- Here is the C++ implementation of combination;
    To compile this code, simply using: g++ -o combinations combinations.cc
    To run the code, simply run: ./combinations -n (num) -m (max);

*/

list<string> combination(int num, int end);
list<string> combination_three(int num, int start, int end);
list<string> make_a_nums_string(int start, int end);


int main(int argc, char *argv[]) {
    // set the variable to store num and maximum value for the combination.
    int num, max;
    int opt; 
    int count = 0;

    // frist check if correct argments are used in the program;
    while ((opt = getopt(argc, argv, "k:n:"))!= -1) {
        switch(opt) {
            case 'k': {
                num = atoi(optarg);
                count++;
                break;
            } 
            case 'n': {
                max = atoi(optarg);
                count++;
                break;
            }
            default: {
                cout << "Usage:: ./combinations -k (NUM) -n (MAX);" << endl;
                break;
            }
        }
    }
    if (count != 2) {
        cout << "ERROR::incorrect input parameters." << endl;
        cout << "\tPlease use this command: " << endl;
        cout << "\t./combinations -k (NUM) -n (MAX)." << endl;

    }

    // make a list to store the combination in string;
    list<string> list_of_combination = combination(num, max);

    // print all the combination into concole;
    for (list<string>::iterator itr = list_of_combination.begin();
         itr != list_of_combination.end(); itr++) {
        cout << *itr <<endl;
    }
}

/*
    This function is just a simple wrapper to call the recursive function 
    "combination_three".
    num: the number for combination;
    end: the maximum number that allows in the combination;
*/
list<string> combination(int num, int end) {
    return combination_three(num, 1, end);
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
list<string> combination_three(int num, int start, int end) {
    /* check the first "if" statement. */
    if (num > (end - start + 1)) {
        list<string> ret_list = {};
        return ret_list;
    }
    /* check the second statement, and 
        return a list a single number string. */ 
    else if (num == 1) {
        /* make a new list. */
        list<string> ret_list;
        for (int i = start; i <= end; i++) {
            ret_list.push_back(to_string(i));
        }
        return ret_list;
    }
    /* if the combination number equals to (end - start),
        then return a single string of numbers as a list. */
    else if (num == (end - start + 1)) {
        return make_a_nums_string(start, end);
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
        list<string> ret_list = {};
        for (int i = start + 1; i <= end; i++) {
            /* recursively call the sub-combination function. */
            list<string> tmp_list = combination_three(num - 1, i, end);
            for (list<string>::iterator itr = tmp_list.begin();
                 itr != tmp_list.end(); itr++) {
                string tmp_line = *itr;
                /* check and add each string with additional number in 
                    the beginning. */
                if (tmp_line.length() > 0) {
                    ret_list.push_back(to_string(i - 1) + " "+ tmp_line);
                }
            }
        }
        return ret_list;
    }
}
/* this is a simple function to make a list of integer to a list of 
    single string with space as a seperator.
*/
list<string> make_a_nums_string(int start, int end) {
    string line = to_string(start);
    while (start != end) {
        start++;
        line += (" " + to_string(start));
    }
    list<string> ret_list = {};
    ret_list.push_back(line);
    return ret_list;
}



