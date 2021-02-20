//-----------------------------------------------------------------------------
// This program reads in a text file of names and grades. It then sorts them
// alphabetically, removes any error checks, displays a histogram of the
// grades and calculates and displays the average of the grades.

// Assumptions:
//  -- Data from text file being read, is formatted correctly.
//  -- The names are not longer than MAXLENGTH.
//  -- Grades in the text file are integers.
//  -- Any names beyond the MAXSIZE arent added to the list
//  -- That by de-selecting "use tabs as indentation", 
//     setting my columns per indentation level and columns per tab
//     character in document both to 3, then using then auto format in my
//     IDE, the tabs and spaces are what you are looking for.
//  -- That whatever IDE you are viewing this code on, will not reformat
//     any of the comments in my function to be in line with the code directly 
//     below it.

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
using namespace std;

int const MAXSIZE = 100;  // maximum number of records in total
int const MAXLENGTH = 31; // maximum string length
int const MAXGRADE = 100; // highest possible grade
int const LOWGRADE = 0;   // lowest possible grade
int const GROUP = 10;     // group amount
int const HISTOGRAMSIZE = (MAXGRADE - LOWGRADE) / GROUP + 1; // grouped by 
                                                             // GROUP

struct StudentType {      // information of one student
   int grade;             // the grade of the student
   char last[MAXLENGTH];  // last name (MAXLENGTH-1 at most)
   char first[MAXLENGTH]; // first name (MAXLENGTH-1 at most)
};

// prototypes
bool sortInput(ifstream& infile, StudentType students[], int& size);
void displayList(StudentType students[], int size);
void setHistogram(StudentType students[], int histogram[], int size);
void displayHistogram(int histogram[], int size);
int findAverage(StudentType students[], int size);
int moveNameUp(StudentType students[], int cell);
//-----------------------------------------------------------------------------
int main(){
   StudentType students[MAXSIZE];        // list of MAXSIZE number of students
   int size = 0;                         // total number of students
   int histogram[HISTOGRAMSIZE] = { 0 }; // grades grouped by GROUP
   int average = 0;                      // average exam score, truncated

   // creates file object and opens the data file
   ifstream infile("data1.txt");
   if(!infile) {
      cout << "File could not be opened." << endl;
      return 1;
   }

   // read and sort input by last then first name
   bool successfulRead = sortInput(infile, students, size);

   // display list, histogram, and class average
   if(successfulRead) {
      displayList(students, size);
      setHistogram(students, histogram, size);
      displayHistogram(histogram, size);
      average = findAverage(students, size);
      cout << "Average grade: " << average << endl << endl;
   }
   return 0;
}

// ----------------------------------------------------------------------------
// findAverage
// this loops through the student array once and adds each students grade to
// the variable total. It then divides the total by the size of the array and
// returns the result

int findAverage(StudentType students[], int size){
   int total = 0;
   for(int i = 0; i < size; i++) {
      total += students[i].grade;
   }
   return total / size;
}

// ----------------------------------------------------------------------------
// displayHistogram
// this loops through the histogram array once to display the grade ranges
// and the number of students in those ranges

void displayHistogram(int histogram[], int size){
   int x = 0;
   int y = 9;

   cout << "Histogram of grades:" << endl;

   for(int i = 0; i < HISTOGRAMSIZE; i++) {
      if(i == HISTOGRAMSIZE - 1) {
         cout << setw(3) << x << "--> " << setw(3) << x << ":";
      } else {
         cout << setw(3) << x << "--> " << setw(3) << y << ":";
      }
      cout << " ";
      for(int j = 0; j < histogram[i]; j++) {
         cout << '*';
      }
      cout << endl;
      x += 10;
      y += 10;
   }
   cout << endl;
}

// ----------------------------------------------------------------------------
// setHistogram
// This loops through the student array once and looks at each cells grade. 
// It then divides the grade by 10, to get the grades first digit. It then 
// uses that digit to increment the correct cell of another array (histogram) 
// by one to keep track of how many times each grade appears.

void setHistogram(StudentType students[], int histogram[], int size){
   for(int i = 0; i < size; i++) {
      int reducedScore = students[i].grade/10;      
      histogram[reducedScore] += 1;
   }
}

// ----------------------------------------------------------------------------
// displayList
// prints the number of students (size variable) of the list of students in
// the array by grade, last, then first name

void displayList(StudentType students[], int size){
   cout << "List of names sorted: " << endl;
   for(int i = 0; i < size; i++) {
      cout << setw(3) << students[i].grade << "  " << students[i].last 
                                    << " " << students[i].first << endl;
   }
   cout << endl;
}

// ----------------------------------------------------------------------------
// sortInput
// This function first reads the data of a file (first parameter) and sorts 
// it alphabetically into an array of StudentType (second parameter). If the
// name "error" is read, it skips that name and grade and moves on to the
// next name. The third parameter, size, is the number of items in the
// students[] array which has be under MAXSIZE.

bool sortInput(ifstream& infile, StudentType students[], int& size){
   char tempLast[MAXLENGTH];
   char tempFirst[MAXLENGTH];
   int gradeNumber;

   infile >> students[size].last >> students[size].first >> 
                                       students[size].grade;
   size++;
//loop scans the file until the end or until students[] 
//reaches it MAXSIZE
   while(size < MAXSIZE && !infile.eof()) { 
      int cell = size - 1;

      infile >> tempLast >> tempFirst >> gradeNumber;

      if(gradeNumber >= 0 && gradeNumber <= 100) {
// 2nd while loop moves the 
//higher last name up
         while(strncmp(students[cell].last, tempLast, MAXLENGTH) > 0 &&
                                                           cell >= 0) { 
            cell = moveNameUp(students, cell);
         }
         
         if(strncmp(students[cell].last, tempLast, MAXLENGTH) == 0) {
//if the last names are the same, 
//this loop moves the higher first name up
            while(strncmp(students[cell].last, tempLast, MAXLENGTH) == 0 &&
                 strncmp(students[cell].first, tempFirst, MAXLENGTH) > 0 &&
                                                               cell >= 0) {
               cell = moveNameUp(students, cell);              
            }
         }
//this puts the temp student 
//into the correct position
         strncpy(students[cell + 1].last, tempLast, MAXLENGTH);
         strncpy(students[cell + 1].first, tempFirst, MAXLENGTH);
         students[cell + 1].grade = gradeNumber;

         size++;
      }
   }
   return true;
}
// ----------------------------------------------------------------------------
// moveNameUp
// This function moves a current student higher in the array if it has been 
// determined by the sortInput function that the name is alphabetically higher
// than the temporary student. It also returns the cell position for use in
// sortInput.

int moveNameUp(StudentType students[], int cell){
   strncpy(students[cell + 1].last, students[cell].last, MAXLENGTH);
   strncpy(students[cell + 1].first, students[cell].first, 
                                                MAXLENGTH);
   students[cell + 1].grade = students[cell].grade;
   cell--;
   
   return cell;
}
   
   
