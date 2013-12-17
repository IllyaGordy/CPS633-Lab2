/* CPS633 - Lab #2
 * Question #2 - DAC
 *
 * Group members:
 * 		Daniel Moscuzza
 * 		Omar Mamiche
 * 		James Goe
 * 		Illya Gordiyenko
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//New Code
#define MAX 5

#define MAX_LEN  		90
#define USERS           3
#define OBJECTS         7

#define NONE            0
#define READ            1
#define WRITE           2
#define READ_WRITE      3

void clearInputStream(char *in);

void cleanup(int** array, int x);
 
// File struct
typedef struct {
	char name[50];
	char label[4];
} SYSFILE;

// Permissiion struct
typedef struct {
	int permission;
	char label[3];
} SYSPERM;

// User struct
typedef struct {
	char name[12];
	char group[12];
	int group_num;
} SYSUSER;

// Function prototypes
void printPrivileges(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void fillPrivileges(int** array, int num_users, int num_files, int num_perm, SYSPERM *tempPerm, char *newPerm);
void changeUserPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeFilePriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeUserObjectPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void printCapabilityTicket(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void printAccessControlList(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);

int main(int argc, char *argv[]) {
    int i;
    char cmd[MAX_LEN];

    // Populate a file struct with default files
    SYSFILE files[20] = {
		{"Course Management Form", "CMF"},
		{"Announcement Page", "AP"},
		{"Lab Information","LI"},
		{"Lab Group Reports","LGR"},
		{"Discussion Page","DP"},
		{"Lab Marks","LM"},
		{"Master Course Mark","MCM"}
	};
    int num_files = 7;

	// Populate a permission struct with default permissions
    SYSPERM permissions[4] = {
        {0, "-"},
        {1, "r"},
        {2, "w"},
        {3, "rw"}
    };
    int num_permissions = 4;
    
    // Populate a user struct with default users
    SYSUSER users[20] = {
        {"Instructor", "Instructors", 1},
        {"TA", "TAs", 2},
        {"Student1", "Students", 3},
        {"Student2", "Students", 3}
    };
    int num_users = 4;
    //To add more users in the future, we will have to increment this value with each new user
    //As well we have to make sure that 'users' do not have null valued users in the middle some where
    //a senario could be that a user is deleted and the count is one down, but it is nulled and gets
    //printed that way
    
    
    //Initialize the matrix an allocate the space for it
    int** privileges_matrix = (int**)malloc(num_users * sizeof(int*));
    
    
    if(!privileges_matrix){
        printf("Not enough memory.\n");
        exit(1);
    }
    
    /*
     Set all pointers to NULL.
     This will make it possible to call free() if
     out of memory during data allocation.
     */
    memset(privileges_matrix, 0, num_users * sizeof(int*));
    
    for(i=0; i<num_users; i++){
        privileges_matrix[i] = (int*)malloc(num_files * sizeof(int));
        if(!privileges_matrix[i]){
            printf("Not enough memory.\n");
            cleanup(privileges_matrix, num_users);
            exit(1);
        }
    }
    
    fillPrivileges(privileges_matrix, num_users, num_files, num_permissions, permissions, "-");
    printPrivileges(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
    
    // Menu
	printf("\nPlease enter a command or \"?cmds\" for a list of commands: ");
	while (fgets(cmd, MAX_LEN, stdin) != NULL) {
		cmd[strlen(cmd)-1] = '\0';
		
		if (strcmp(cmd, "cup") == 0) {    
			changeUserPriv(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "pam") == 0) {
			printPrivileges(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "cfp") == 0) {
			changeFilePriv(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "cuop") == 0) {
			changeUserObjectPriv(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "pacl") == 0) {
			printAccessControlList(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "pct") == 0) {
			printCapabilityTicket(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "?cmds") == 0) {
			printf("Available commands:\n"
				"\tpam  -- Print access matrix\n"
				"\tcup  -- Change user privileges\n"
				"\tcfp  -- Change file privileges\n"
				"\tcuop -- Change user object privileges\n"
				"\tpacl -- Print access control list\n"
				"\tpct  -- Print compatability ticket\n"
				"\texit -- Terminate application\n"
			);
		} else if (strcmp(cmd, "exit") == 0) {
			break;
		}
		
		printf("\nPlease enter a command or \"?cmds\" for a list of commands: ");
	}
    
    return 0;
}

/* Print the Access Control Matrix
 * Get the array with all the needed variables, num_user, num_files, tempUser, tempFile, tempPerm
 */
void printPrivileges(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm){
    int i, j, k;
    //int num_perm = 4;
    
    printf("             ");
    for (i=0; i<num_files; i++) {
        printf("%*s", 5, tempFile[i].label);
    }
    printf("\n");
    for (i=0; i < num_users; i++){
        printf("%12s ", tempUser[i].name);
        for(j=0; j < num_files; j++){
            for(k=0; k < num_perm; k++){
                if(array[i][j] == tempPerm[k].permission){
                    printf("%*s", 5, tempPerm[k].label);
                }
            }
        }
        printf("\n");
    }
}

/* Fill in the Matrix with some value inputed directly in the code
 * Get the array with all the needed variables, num_user, num_files, num_perm, tempPerm, newPerm
 */
void fillPrivileges(int** array, int num_users, int num_files, int num_perm, SYSPERM *tempPerm, char *newPerm){
    int i, j;

    int def_AM[6][7] = {
	//	CMF		AP		LI		LGR		DP		LM		MCM	
		{3,		3,		3,		3,		0,		3,		3}, // Instructor
		{1,		3,		3,		3,		0,		3,		0}, // TAs
		{1,		3,		1,		3,		0,		0,		0}, // S1
		{1,		3,		1,		3,		0,		0,		0}, // S2
		{1,		3,		1,		3,		0,		0,		0}, // S3
		{1,		3,		1,		3,		0,		0,		0}	// S4
	};

    for(i=0; i<num_users; i++)
       for(j=0; j<num_files; j++)
			array[i][j]=def_AM[i][j];
}


/* Change the a specific user privalages
 * Get the array, ask for the user to be changed and find it in the matrix. 
 * Change to the one inputted
 */
void changeUserPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_user[15] = "";
    char temp_priv[10] = "";
    int len,i,j,k;
    
    
    // Promt for the User
    printf("Which user would you like to change(user change only): ");
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
		len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    //Promt for the Privilage to change
    printf("Which privelages would you like to set(NO, R, W, RW): ");
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}
    
    
    //Go thought the matrix is the user is matched then change the privalges
    for (i = 0; i < num_users; i++)
        if(strcmp(temp_user, tempUser[i].name) == 0)
           for (j=0; j < num_files; j++)
               for(k=0; k < num_perm; k++)
                   if(strcmp(temp_priv,tempPerm[k].label) == 0)
                       array[i][j] = tempPerm[k].permission;
}
        
           

/* Change the a specific file privalages
 * Same as the user change but in this one you change all the privilages for an object
 */
void changeFilePriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_file[25] = "";
    char temp_priv[10] = "";
    int len,i,j,k;
    
    // Promt for the User
    printf("Which file would you like to change(file change only): ");
    if (fgets(temp_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_file);
		len = strlen(temp_file);
		if(temp_file[len - 1] == '\n') temp_file[len - 1] = '\0';
	}
    
    //Promt for the Privilage to change
    printf("Which privelages would you like to set(NO, R, W, RW): ");
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}

    
    
    //Go thought the matrix if the object is matched then change the privalges
    for (i = 0; i < num_files; i++)
        if(strcmp(temp_file, tempFile[i].label) == 0)
            for (j=0; j < num_users; j++)
                for(k=0; k < num_perm; k++)
                    if(strcmp(temp_priv,tempPerm[k].label) == 0)
                        array[j][i] = tempPerm[k].permission;
}


/* Change the a specific user and object privalages
 * Ask for the user, object and the permission and apply all of those. Same as in the other two
 */
void changeUserObjectPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_user[15] = "";
    char temp_priv[15] = "";
    char temp_file[25] = "";
    int len,i,j,k;
    
    // Select User
    printf("Which user would you like to change(user and file change): ");
    //Clearing of Input Buffer
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
        len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    //Select Object
    printf("Which file would you like to change: ");
    //Clearing of Input Buffer
    if (fgets(temp_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_file);
		len = strlen(temp_file);
		if(temp_file[len - 1] == '\n') temp_file[len - 1] = '\0';
	}
    
    //Select Privilege
    printf("Which privileges would you like to set(NO, R, W, RW): ");
    //Clearing of Input Buffer
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}
    
    
    printf("user:%s, file:%s, privilege:%s\n", temp_user, temp_file, temp_priv);
    //Go thought the matrix if the object and the user is matched then change the privilege
    for (i = 0; i < num_files; i++){
        if(strcmp(temp_file, tempFile[i].label) == 0){
            for (j=0; j < num_users; j++){
                if(strcmp(temp_user, tempUser[j].name) == 0){
                    for(k=0; k < num_perm; k++){
                        if(strcmp(temp_priv,tempPerm[k].label) == 0){
                            array[j][i] = tempPerm[k].permission;
                        }
                    }
                }
            }
        }
    }
}

/* This function prints out an access control list for a particular file.
 * Every user in the access matrix is listed with their corresponding
 * access privileges to the file.
 */
void printAccessControlList(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm){
    int i, j, k, len;
    char temp_file[MAX_LEN] = "";
    int sizeOfArray = num_users * num_files;
    
    // Select User
    printf("Which file would you like to view: ");
    //Clearing of Input Buffer
    if (fgets(temp_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_file);
        len = strlen(temp_file);
		if(temp_file[len - 1] == '\n') temp_file[len - 1] = '\0';
	}
    
    /* Look for the file label the user is searching for and when found
     * print all users in the access matrix along with the corresponding
     * list of privileges in the file's field.
     */
    for(i=0; i<sizeOfArray; i++){
        if(strcmp(temp_file, tempFile[i].label) == 0){
            printf("Access Control List for: %s\n\n", tempFile[i].name);
            
            for (j=0; j < num_users; j++){
                printf("%*s:", 25,tempUser[j].name);
                
                for(k=0; k < num_perm; k++){
				if(array[j][i] == tempPerm[k].permission){
                        printf(" %s ", tempPerm[k].label);
                    }
                }
                printf("\n");
            }
        }
    }
}

/* Print the Capability Ticket for a specific user
 * Prompt for input for the user and then show capabilities for each file
 */
void printCapabilityTicket(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm){
    int i, j, k, len;
    char temp_user[15] = "";
    int sizeOfArray = num_users * num_files;
    
    // Select User
    printf("Which user would you like to view: ");
    //Clearing of Input Buffer
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
        len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    /* Look for the name of the user is searching for and when found
     * print all users in the access matrix along with the corresponding
     * list of privileges in the file's field.
     */
    for(i=0; i<sizeOfArray; i++){
        if(strcmp(temp_user, tempUser[i].name) == 0){
            printf("Capability Ticket for: %s\n\n", tempUser[i].name);
            
            for (j=0; j < num_files; j++){
                printf("%*s:", 25,tempFile[j].name);
                
                for(k=0; k < num_perm; k++){
				if(array[i][j] == tempPerm[k].permission){
                        printf(" %s ", tempPerm[k].label);
                    }
                }
                printf("\n");
            }
        }
    }
}


void cleanup(int** array, int x){
    int i;
    
    for(i=0; i<x; i++){
        free(array[i]);
        free(array); 
    }
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
