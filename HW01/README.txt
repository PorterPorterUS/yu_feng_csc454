NAME: Yu feng
NetID: yfeng28
A01 CSC454

#####################################################
                File Directory 
#####################################################
In the structure of this directory is shown below:

----README.txt
----ada  
    |---combinations  
    |---combinations.adb  
    |---combinations.ali  
    |---combinations.o
----CC_code  
    |---combinations  
    |---combinations.cc
----c_sharp  
    |---combinations.cs  
    |---combinations.exe
----oCaml  
    |---combinations  
    |---combinations.cmi  
    |---combinations.cmo  
    |---combinations.ml
----prolog  
    |---combinations.pl
----ruby  
    |---combinations.rb
----scheme
    |---combinations.rkt

#####################################################
            General Description
#####################################################

The general idea about the conbination problem for amost 
all seven different program is almost the same.

I use the recursive call combinationThree(k, 1, n) to 
recursively call the sub-problem.
The base case is combinationThree(1, a, b), which is return 
a list of "single-number-string". 
Then, the upper caller will check if the returned list is empty,
if not, add additional num to the front (or at tail), based on the
easiness of different program language.

A little improvement is that I will check in the sub-call 
combinationThree(k, a, b) if k == (b - a), then I will return a long 
string, this will reduce a little running time.

The bigO for most of 1/2*(n^k),(almost close to factorial),
but if we add a hash table,(due to the time limination, I didn't use
dynamic programming) and the key is (a*10 + b) and set 
the hash value to be the list of resulted strings, in any 
combinationThree(k, a, b) call, if look up this table is not null, then,
we can directly return this value, Then, it will save a lot of time.

Only exception is scheme: due to the fact that I am not family with 
this language, thus, I wrote a very brutal-force program to generate 
almost all the combination of the number in acending order (without 
considering k value). Then, creating another list and add the length-
of-k array to the return list. This program is not an efficient program,
but my hands are tight.

#####################################################
            Feeling on those languages
#####################################################

Personally, I feel ada and scheme are most difficult languages to 
program among those seven. Ada has a very restrict copy string and 
claim variables policy, and scheme is quite difficult to "thinking 
revercely" during programming. 

ruby, c# and c++ are the languages I familar with, and they are easy 
to program, both c# and ruby has very friendly iterator, and ruby also 
has "yield" if you want to build a small sub-function.

I think iterator is a very helpful tool and also it is good for 
information-hide.

OCaml is a little difficult, because I have to build some small functions
in order to circumvent the fact that it don't allow "variables". So, I built 
several some gadgets to make the program work.

#####################################################
                    Note
#####################################################

More compilation information for each language, please see each indivual 
code.



