To begin, I'll go into detail of the four memory allocation algorithms that are being implemented:

# (1) First Fit:
        Given a number of partitions in memory and a number of processes for which each are assigned 
        a certain amount of memory to be allocated for, First Fit places each process in the first partition of 
        memory for which the process will "fit" into. For each process, the total allotted memory will be iterated 
        through and assign each process an appropriate space.
# (2) Best Fit:
        Given a number of partitions in memory and a number of processes for which each are assigned 
        a certain amount of memory to be allocated for, Best Fit places each process in the smallest possible 
        partition of memory. In other words, Best Fit designates the partition in memory for which the difference 
        between the processes memory allocation and the partition's allocation is the smallest (so long as the
        partition is big enough to store it). For each process, the total allotted memory will be iterated through 
        and assign each process an appropriate space.
# (3) Worst Fit:
        Given a number of partitions in memory and a number of processes for which each are assigned 
        a certain amount of memory to be allocated for, Worst Fit places each process in the partition that 
        posseses the greatest difference between the processes memory allocation and the partition's allocation.
        For each process, the total allotted memory will be iterated through and assign each process an 
        appropriate space.
# (4) Next Fit:
	Similar to First Fit, but instead of beginning the search for partitioned memory at the beginning of 
	memory, the search begins at the last point of allocated memory.

I approached this project from the standpoint of managing an array, for reasons being that I was more familiar
and comfortable with managing this sort of data structure. Over time, I began to realize, that given the various
methods of array "management" being employed throughout the program, this made it much more difficult to account
for the numerous edge/corner cases as well as ensuring the correct "shifting" and searching was being performed.
However, I did manage to reuse variables as extensively as possible to alleviate any sacrifice in computation 
time. This project actually taught me a lot in terms of the reuseability of variables in a program as well as
the importance of following the intended method. As far as the structure of the code here is concerned, I 
basically wrapped all of the main computational areas in a while loop for reading the provided .txt file, then
broke the loop into segments specified for each expected command as they required very differing approaches.
Originally, I felt it was impossible to contain the "searching" aspect of the program into one area as each
algorithm is unique in it's searching functionality. But as time went on, I was able to recognize enough
similarities between each algorithm to confine them into a single area. I wish that I could say that this was
a complete success, but there were a few instances in which my program was unable to account for and I believe
that shows. Thank you.
