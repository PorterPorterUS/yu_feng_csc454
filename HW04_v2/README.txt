In this HW, I used Ruby as a language to write the prgram.
The main idea about this program is first extract some potential useful
information from objdump and dwarfdump
From the objdump, I extract all the assembly code, with addresses, 
function names, and subroutine function names, etc. Some of them might 
be not useful in making a single index file, but it can be useful in 
making multiple files if we need to.
Next, I run the dwarfdump and extract the coresponding lines numbers
with the assembly address, so we can use this assembly addresses to 
include the source code that index with it and the following codes that 
are not index with any assembly address. So, we can associate them to 
this assembly code too.
Finally, we can start to assemble our HTML code list.
So, I maintain two lists, one is for source code, another is for assembly 
code. And I will interleave these two lists to make a table in HTML. The 
name of the final content page is called "main.html", and the index file 
name is "index.html"
