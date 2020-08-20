/*
| Files and Directories
|
| Name: lab3.c
|
| Written by: Denzel Awuah - March 2020
|
| Purpose: Read and print out files in a directory based on command line arguments
|          and file modification dates.  
|
| Usage:    ./lab3result #directory_of_items_list_file
|        e.g. ./a.out /home/COIS/3380/lab2/item_list_1.txt
|
|          ./lab3result #type_of_operation #number_days_SINCEorBEFORE #directory_path  
|	e.g.  ./lab3result -s 90 /home/COIS/3380/lab3
|
| Description of parameters:
| (If argc = 2) 
|	argv[1] - the directory to output 
|
| (if argc = 4)
|	argv[1] - either "-s" for SINCE, or "-b" for BEFORE 
|	argv[2] - the number that comes after the the output type
|	argv[3] - the directory to output
|
| Subroutines/libraries required:
| See include statements.
|
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


//Methods to perform the required operation
void printFileProperties(char s[]);
void printFileProperties2(char first[], char second[], char third[]);
void printFileProperties3(char first[], char second[], char third[]);
void printFileProperties4(char first[], char second[], char third[], char fourth[], char fifth[]);


int main (int argc, char *argv[])
{

	
	// If there is only 1 parameter 
	if(argc == 2)
	{
	  printFileProperties(argv[1]);
	}
	else if(argc == 4 && strcmp("-s", argv[1]) == 0) //if there is 4 parameters and -s is defined
	{
	  printFileProperties2(argv[1], argv[2], argv[3]); //call the function for -s 
	}
	else if(argc == 4 && strcmp("-b", argv[1]) == 0) //if there is 4 parameters and -b is defined 
	{
	  printFileProperties3(argv[1], argv[2], argv[3]); //call the function for -b
	}
	 else if(argc == 6 && strcmp("-b", argv[1]) == 0 && strcmp("-s", argv[3]) == 0) //if there is 6 parameters and -b and -s are defined
        {
          printFileProperties4(argv[1], argv[2], argv[3], argv[4], argv[5]); //call the function for -b and -s
        }
	

 
return (0);

} //end main
    




//printFileProperties
//if there is only one argument from the command line and it is a directory
//List information on all files in the directory
int *uid;
void printFileProperties(char s[])
{

DIR *dp; //define a directory 
struct dirent *dir; //struct to read a directory 

struct stat filestat;  
uid = &filestat.st_uid;
if( (dp = opendir(s)) == NULL) // if you cant find the directory to read
{
  fprintf(stderr, "Cannot open dir\n"); // error message 
  exit(1); 
}


/*read entries */
char str[100]; //to concatenate a the file name to the directory
printf("inode       Type     UID      GID        Size                          Name     Modification Date\n");


while( (dir = readdir(dp)) != NULL) // go through each file in the database
{
strcpy(str, s);  //copy the directory path to the string s
strcat(str,"/"); //Concatenate "/" to the path
strcat(str,dir->d_name); //Concatenate the file name to the directory
char ftype[15]; //determine the file type
stat(str,&filestat);  //store file information into filestat struct

if(S_ISREG(filestat.st_mode)){  //check if the file type is REG
strcpy(ftype, "REG");  //set ftype to REG
}
else if(S_ISDIR(filestat.st_mode)){  //check if the file type is DIR
strcpy(ftype, "DIR"); //set ftype to DIR
}
else if(S_ISCHR(filestat.st_mode)){  //check if the file type is CHR
strcpy(ftype, "CHR"); //set ftype to CHR
}
else if(S_ISBLK(filestat.st_mode)){  //check if the file type is BLK
strcpy(ftype, "BLK"); //set ftype to BLK
}
else if(S_ISFIFO(filestat.st_mode)){ //check if the file type is FIFO
strcpy(ftype, "FIFO"); //set ftype to FIFO
}
else if(S_ISLNK(filestat.st_mode)){ //check if the file type is LNK
strcpy(ftype, "LNK"); //set ftype to LNK
}
else if(S_ISSOCK(filestat.st_mode)){ //check if the file type is SOCK
strcpy(ftype, "SOCK"); //set ftype to SOCK
}


//Print out the file information
printf("%-10d  %3s %8ld %9ld  %8ld %30s %28s"
, dir->d_ino, ftype, (int)filestat.st_uid, (int)filestat.st_gid, 
(int)filestat.st_size, dir->d_name, ctime(&filestat.st_mtime));

}
closedir(dp);

}  // end print function







//printFileProperties2
//Performs operation for "-s" argument (SINCE)
void printFileProperties2(char first[], char second[], char third[])
{

DIR *dp; //directory file pointer
struct dirent *dir; //directory structure

struct stat filestat; //struct to read files
uid = &filestat.st_uid; 
if( (dp = opendir(third)) == NULL) //if you cant read the directory
{
  fprintf(stderr, "Cannot open dir\n");  //print error message
  exit(1);
}



double days = atof(second);  //gets the number of days argument from the command line
days = days * 86400; //gets the number of days in seconds
time_t t;  //to get the current date and time
time(&t);  //gets date and time 

/*read entries */
char str[100]; //for getting the file in the directort
printf("inode       Type     UID      GID        Size                          Name     Modification Date\n");

while( (dir = readdir(dp)) != NULL) //go through every file in the directory
{
strcpy(str, third);  //copy directory to string
strcat(str,"/");     //concatenate "/" to directory 
strcat(str,dir->d_name); //concatenate file name to directory
char ftype[15]; //stores file type
stat(str,&filestat); //stores file information into structure

//If the difference between the two times is greater the number of days SINCE current time
if(difftime(t, filestat.st_mtime) >= days) 
{
  // do not list this file in the output (too old)
}
else
{
if(S_ISREG(filestat.st_mode)){ //check if the file type is REG

strcpy(ftype, "REG"); //set ftype to REG
}
else if(S_ISDIR(filestat.st_mode)){ //check if the file type is SOCK
strcpy(ftype, "DIR"); //set ftype to DIR
}
else if(S_ISCHR(filestat.st_mode)){ //check if the file type is CHR
strcpy(ftype, "CHR"); //set ftype to CHR
} 
else if(S_ISBLK(filestat.st_mode)){ //check if the file type is BLK
strcpy(ftype, "BLK"); //set ftype to BLK
}
else if(S_ISFIFO(filestat.st_mode)){ //check if the file type is FIFO
strcpy(ftype, "FIFO"); //set ftype to FIFO
}
else if(S_ISLNK(filestat.st_mode)){ //check if the file type is LNK
strcpy(ftype, "LNK"); //set ftype to LNK
}
else if(S_ISSOCK(filestat.st_mode)){ //check if the file type is SOCK
strcpy(ftype, "SOCK"); //set ftype to SOCK
}

//Print file information
printf("%-10d  %3s %8ld %9ld  %8ld %30s %28s"
, dir->d_ino, ftype, (int)filestat.st_uid, (int)filestat.st_gid,
(int)filestat.st_size, dir->d_name, ctime(&filestat.st_mtime));


} // end else


}// end while

closedir(dp);



}// end method





//printFileProperties3
//Performs operation for "-b" argument (BEFORE)
void printFileProperties3(char first[], char second[], char third[])
{

DIR *dp; //Directory file pointer
struct dirent *dir; //directory structure

struct stat filestat; //file structure 
uid = &filestat.st_uid;
if( (dp = opendir(third)) == NULL) //if cannot open directory
{
  fprintf(stderr, "Cannot open dir\n"); //error message
  exit(1);
}



double days = atof(second); //gets number of days argument from th command line
days = days * 86400;  //gets the number of dayys in seconds 
time_t t; // to get the current date and time
time(&t); //gets date and time

/*read entries */
char str[100]; //for getting the file in the directory
printf("inode       Type     UID      GID        Size                          Name     Modification Date\n");

while( (dir = readdir(dp)) != NULL) //go through every file in the directory 
{
strcpy(str, third);  //copy directory path to string 
strcat(str,"/"); //concatenate "/" to directory
strcat(str,dir->d_name);  //concatenate filename to directory
char ftype[15];  //stores file type
stat(str,&filestat); //stores file information into structure

//If the difference between the two times is greater or equal to then the number of days BEFORE in seconds
if(difftime(t, filestat.st_mtime) >= days) 
{
  
if(S_ISREG(filestat.st_mode)){ //check if the file type is REG

strcpy(ftype, "REG"); //set ftype to REG
}
else if(S_ISDIR(filestat.st_mode)){ //check if the file type is SOCK
strcpy(ftype, "DIR"); //set ftype to DIR
}
else if(S_ISCHR(filestat.st_mode)){ //check if the file type is CHR
strcpy(ftype, "CHR"); //set ftype to CHR
}
else if(S_ISBLK(filestat.st_mode)){ //check if the file type is BLK
strcpy(ftype, "BLK"); //set ftype to BLK
}
else if(S_ISFIFO(filestat.st_mode)){ //check if the file type is FIFO
strcpy(ftype, "FIFO"); //set ftype to FIFO
}
else if(S_ISLNK(filestat.st_mode)){ //check if the file type is LNK
strcpy(ftype, "LNK"); //set ftype to LNK
}
else if(S_ISSOCK(filestat.st_mode)){ //check if the file type is SOCK
strcpy(ftype, "SOCK"); //set ftype to SOCK
}

//Print file information
printf("%-10d  %3s %8ld %9ld  %8ld %30s %28s"
, dir->d_ino, ftype, (int)filestat.st_uid, (int)filestat.st_gid,
(int)filestat.st_size, dir->d_name, ctime(&filestat.st_mtime));


}
else
{
// do not list this file
} 


}// end while

closedir(dp);

}//end method












//printFileProperties4 
//Performs operation for "-s" argument (SINCE) 
void printFileProperties4(char first[], char second[], char third[], char fourth[], char fifth[]) 
{ 
DIR *dp; //directory file pointer 
struct dirent *dir; //directory structure 
struct stat filestat; //struct to read files 
uid = &filestat.st_uid; 

if( (dp = opendir(fifth)) == NULL) //if you cant read the directory 
{
  fprintf(stderr, "Cannot open dir\n"); //print error message
  exit(1);
}

double days = atof(fourth); //gets the number of days argument from the command line 
days = days * 86400; //gets the number of days in seconds 
time_t t; //to get the current date and time 
time(&t); //gets date and time 

double days2 = atof(second); //gets the number of days argument from the command line
days2 = days2 * 86400; //gets the number of days in seconds
time_t t2; //to get the current date and time
time(&t2); //gets date and time


/*read entries 
*/ char str[100]; //for getting the file in the directory 
printf("inode       Type     UID      GID        Size                          Name     Modification Date\n");



while( (dir = readdir(dp)) != NULL) //go through every file in the directory 
{ 
strcpy(str, fifth); //copy directory to string 
strcat(str,"/"); //concatenate "/" to directory 
strcat(str,dir->d_name); //concatenate file name to directory 
char ftype[15];  //stores file type 
stat(str,&filestat); //stores file information into structure 

//If the difference between the two times is greater the number of days SINCE current time 
if(difftime(t, filestat.st_mtime) >= days && difftime(t, filestat.st_mtime) <= days2) 
{
 // do not list this file in the output (too old)
}
else { 

if(S_ISREG(filestat.st_mode)){ //check if the file type is REG 
strcpy(ftype, "REG"); //set ftype to REG
}
else if(S_ISDIR(filestat.st_mode)){ //check if the file type is SOCK 
strcpy(ftype, "DIR"); //set ftype to DIR
}
else if(S_ISCHR(filestat.st_mode)){ //check if the file type is CHR 
strcpy(ftype, "CHR"); //set ftype to CHR
} 
else if(S_ISBLK(filestat.st_mode)){ //check if the file type is BLK 
strcpy(ftype, "BLK"); //set ftype to BLK
}
else if(S_ISFIFO(filestat.st_mode)){ //check if the file type is FIFO 
strcpy(ftype, "FIFO"); //set ftype to FIFO
}
else if(S_ISLNK(filestat.st_mode)){ //check if the file type is LNK 
strcpy(ftype, "LNK"); //set ftype to LNK
}
else if(S_ISSOCK(filestat.st_mode)){ //check if the file type is SOCK 
strcpy(ftype, "SOCK"); //set ftype to SOCK
}


//Print file information
 printf("%-10d %3s %8ld %9ld %8ld %30s %28s" , dir->d_ino, ftype, (int)filestat.st_uid, (int)filestat.st_gid, (int)filestat.st_size, 
dir->d_name, ctime(&filestat.st_mtime));

} // end else 

}// end while
closedir(dp);

}// end method
