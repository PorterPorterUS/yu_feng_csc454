Name: Yu Feng
NetID: yfeng28

In this HW04, I used Ruby as a language to write the program. And generate
the one index.html, so that it has links to: 
    -- main
    -- file1.h
    -- file2.c
    -- ....

So, each is a hyperlink to the specific HTML with source code.

Some of issue with the multiple file version:

If there is a inline code, for example, the assembly is related to the 
main function, but the callee function is in "blah.h", you can't expect 
this program to use the actual information from the objdump and dwarfdump 
to get the idea that 
"okay, here is the inline func, but where is this inline function end from 
the assembly code of main function, so that we can split this part of code
into another HTML file page." 

So this is impossible to do with very simplified index program, because there
is no any signature like "retq" to get those information!

Another thing about my program is that I utilize the information from 
"dwarfdump" to match exactly the source line with assembly line.
for example:
 "0x00400594  [  10, 0] NS"
 which means that the source line in file LINE 10 is exactly align with 
 assembly code address "400594"

Another feature of my program is to index each table line with row number
for example: 
    "[100, X]" 100 means row 100, X means this assembly code is not index by
               dwarfdump, but it will follow the previous assembly code.
    "[103,13]" 13 means this assembly code is exactly match with the source 
               code LINE 13;

When the program jump, it will let the address of the link appear at the top
of the page.

########################################################################
                The procedure of my program.
########################################################################

The main idea about this program is first extract some potential useful
information from objdump and dwarfdump.

From the objdump, I extract all the assembly code with addresses, 
function names, and subroutine function names, etc. Some of them might 
be not useful in making a single index file, but it can be useful in 
making multiple files if we need to. Like in my case.

Next, I run the dwarfdump and extract the coresponding lines numbers
with the assembly address, so we can use this assembly addresses to 
include the source code that index with it and the following codes that 
are not index with any assembly address. So, we can associate them to 
this assembly code too.

Finally, we can start to assemble our HTML code from these two list.
So, I maintain two lists, one is for source code, another is for assembly 
code. And I will interleave these two lists to make a table in HTML. The 
name of the final content pages are in the directory called "sub", and 
the index file name is "index.html"


