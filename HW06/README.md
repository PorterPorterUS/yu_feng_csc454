NetID: yfeng28
Name:  Yu Feng
# This README is used for HW06 in CSC454.
# In this folder, it contains several files:
    -- program code: 
        SSSP.java and cooradinate.java
    -- result.pdf:
        this contains some parallel computed results from 
        node2x18a.csug.rochester.edu
        more details about the result and analysis are in
        this file.
    -- tester.sh:
        the little bash script to run java in batch.
    -- raw.txt:
        some raw data from node2x18a.csug.rochester.edu

In this homework, I used the realtively the same model as the HW suggested.


Some of extra features I added into this program:
    1. other than letting each thread has an array of buckets, I 
    only maintain a global bucket list, and all the threads can
    access to it, and check if the vertexs in bucket list needs to
    be handled by itself.
    2. The whole program, it maintains a list of queues, each queue
    corresponds to one thread, whenever, there is a edge relaxation 
    "request" to that thread, we push the request to that queue.
    Afterwards, threads will check their own queues and do the edge 
    relaxation.
    3. I also made the "-a 3" command works for multi-threads. 



The layout of my program is shown below:

The global variables: (all the threads can see.)
1.
        -------------------------------------
        |    |   ....  |     |     |        |
        |    |  Buckets| ... |     |        |   Global buckets:
        |    |   ....  |     |     |        |
        -------------------------------------
        (this series of buckets is used to store all the vertexs 
             that will used and relaxed by the resposible trheads.)
2.
        ----------------------------
        |HashMap<vertex, threadNum>|   
        ----------------------------
        (this will be used to map each vertex that we need to relax
         to the corresponding threads ID and put the vertex in to 
         the right queue, using threadID.)
3.
        ------------------
        |ArrayList<Queue>|   
        ------------------
        (each queue is used to store the "request<vertex, edge>")
        After using the hashmap to push the request into the right 
        queue, the "right" thread will handle and clear the queue
        request.

And eah thread will have these several attributes:
1.
    -------
    | set |   <---- the set is used to lookup, so which vertex will be 
    -------         handled by this thread can be lookup in this set.
2. 
    cyclebarrier <- <reference>: this will be used to make all the threads 
                    work at the same step.
3. 
    threadID  <---- so we can use this to index to the right queue.


The workflow is similar to the procedure as the HW described . but one thing 
I made a little different is to set up each thread has it own queue, so they
won't interference each other. 


NOTE: the result analysis and the conclusion are included in the result.pdf


