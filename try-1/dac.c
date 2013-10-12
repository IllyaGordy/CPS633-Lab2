/* CPS633 - Lab #2
 * Question #2 - DAC
 *
 * Group members:
 * 		Daniel Moscuzza
 * 		Omar Mamiche
 * 		James Goe
 * 		Illya Gordiyenko
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define MAXINPUTLENGTH  90 //max length for the input stream
#define USERS  3 //define the amount of users
#define OBJECTS  7 //define the amount of objects

// Function prototypes
void changeUser(char *user); // separate the password in to three and hash each section
void clearInputStream(char *in); //make sure all the processe that are going in to stadard io are good


/*
 * Most of the stuff are done in the main class for now
 * ToDo: split in to more reuasable classes
 *
 *
 */
int main(int argc, char *argv[]) { //start of app
    int i,j;
    
    char temp_user[11] = ""; // user setter
    char temp_object[25] = ""; // object setter
    int len; //set the length
    
    // let's make our own array of strings for the users
    char *users[] = {
        "instructor", "TA",
        "student"
    };
    int num_users = 3;
    
    // let's make our own array of strings for the objects
    char *objects[] = {
        "Course Management Form",
        "Announcement Page",
        "Lab Information",
        "Lab Group Reports",
        "Discussion Page",
        "Lab Marks",
        "Master Course Mark",
    };
    int num_objects = 7;
    
    //set up a matrix of user privilages, set it for read/write at first
    char *privileges_matrix[USERS][OBJECTS] = {
        {"rw", "rw", "rw", "rw", "rw", "rw", "rw"},
        {"rw", "rw", "rw", "rw", "rw", "rw", "rw"},
        {"rw", "rw", "rw", "rw", "rw", "rw", "rw"},
    };
    
    for(i = 0; i < USERS; i++){
        for(j = 0; j < OBJECTS; j++){
            printf("privileges_matrix[%s][%s] is %s\n", users[i], objects[j], privileges_matrix[i][j]);
        }
    }
    
    
    printf("\n");
    

    
    printf("Which user would you like to change: "); //print username
    //fgets(temp_user, USERNAMEMAXLEN, stdin);
    
    if (fgets(temp_user, MAXINPUTLENGTH, stdin) != NULL) { //wait for input for username to a max lenght
        clearInputStream(temp_user);	// Clear stdin buffer
		
		len = strlen(temp_user); //set the length of the username
		
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0'; //set the new line char to null
	}
    
    
    //Go thought the matrix is the user is matched then change the privalges to "w" for now
    for (i = 0; i < USERS; i++) {
        printf("temp_user: %s; user[%d]: %s \n", temp_user, i, users[i]);
        if(strcmp(temp_user, users[i]) == 0){
            for (j=0; j < OBJECTS; j++) {
                privileges_matrix[i][j] = "w";
            }
        }
    }
    
    
    printf("\n");

    
    printf("There are these objects in the system:\n"); //print username
    for (i = 0; i<OBJECTS; i++) {
        printf("%s \n",objects[i]);
    }
    printf("Enter the one you want to change: "); //print username
    
    if (fgets(temp_object, MAXINPUTLENGTH, stdin) != NULL) { //wait for input for username to a max lenght
        clearInputStream(temp_object);	// Clear stdin buffer
		
		len = strlen(temp_object); //set the length of the username
		
		if(temp_object[len - 1] == '\n') temp_object[len - 1] = '\0'; //set the new line char to null
	}
    
    printf("Object is: %s", temp_object); //print username
    
    
    //Go thought the matrix is the object is matched then change the privileges to "r" for now
    for (i = 0; i < OBJECTS; i++) {
        printf("temp_object: %s; object[%d]: %s \n", temp_object, i, objects[i]);
        if(strcmp(temp_object, objects[i]) == 0){
            printf("compared");
            for (j=0; j < USERS; j++) {
                privileges_matrix[j][i] = "r";
            }
        }
    }
    
    printf("\n");
    
    for(i = 0; i < USERS; i++){
        for(j = 0; j < OBJECTS; j++){
            printf("privileges_matrix[%s][%s] is %s\n", users[i], objects[j], privileges_matrix[i][j]);
        }
    }
    
    printf("\n");
    
    return 0;
}

/* I just took this from Dan's previous lab
 *
 * This function ensures that the input buffer (stdin) does not contain
 * any inproperly processed/non-terminating lines, which may in turn
 * affect values supplied for passwords.
 */
void clearInputStream(char *in) {
	int c, len; //initiate c and len int
	
	len = strlen(in); //set the length to input
	
	/* If the input buffer is not correctly terminated by a new-line
	 * character, clear the buffer by "disposing" of excess until
	 * a new-line character is found.
	 */
	if (in[len - 1] != '\n')
		while ((c = getchar()) != '\n' && c != EOF); //while there is a char not 'next line' and not end of the file
}


/* All this does for now is print the user...
 * Change the a specific user privalages
 *
 *
 */
void changeUser(char *user) {
    
    printf("User is: %s", user); //print username
    
    //for (i=0; i<USER_ROWS; i++) {
    //    if(matrix[i][i])
    //}
    


}