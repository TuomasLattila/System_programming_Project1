# System_programming_Project1
C-program that reads text row by row and reverses them. Can be used with or without input and output files.


With 0 command-line arguments, the program asks user to write multiple lines of text into the stdin. With ctrl + D, user can end writing mode. After this, the program reverses the written lines and writes them to stdout.  

With 1 command-line argument, the program expects a text file within the same folder. The program will read the given file, then reverse the lines inside the file, and writes them to the stdout.

With 2 command-line arguments, the program expects two different text files within the same folder. The program will read the lines from the first file and then reverses the lines and writes them to the second file. The files must be different, otherwise error will occur.

