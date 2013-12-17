/* CPS633 - Lab #2
 * Question #2 - RBAC
 *
 * Group members:
 * 		Daniel Moscuzza
 * 		Omar Mamiche
 * 		James Goe
 * 		Illya Gordiyenko
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 5

#define MAX_LEN  90
#define USERS           3
#define OBJECTS         7

#define NONE            0
#define READ            1
#define WRITE           2
#define READ_WRITE      3

void clearInputStream(char *in);

void cleanup(int** array, int x);

typedef struct {
	char name[50];
	char label[4];
} SYSFILE;

typedef struct {
	int permission;
	char label[3];
} SYSPERM;

typedef struct {
	char name[12];
	char group[12];
	int group_num[10];
} SYSUSER;

// Function prototypes
void printPrivileges(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void fillPrivileges(int** array, int num_users, int num_files, int num_perm, SYSPERM *tempPerm, char *newPerm);
void changeUserPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeFilePriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeUserObjectPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void printCapabilityTicket(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeStudentLabPerm(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeStudentLabFilePerm(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);
void changeUserRole(int num_users, SYSUSER *tempUser);
int arraySize(int* array);
void checkChangesForTA(int** array, char* temp_user_group, int num_users, SYSUSER *tempUser, int temp_group_num, int num_files, char* temp_file_name, SYSFILE *tempFile, int num_perm, SYSPERM *tempPerm, int temp_user_perm);
void checkChangesForInstructor(int** array, char* temp_user_group, int num_users, SYSUSER *tempUser, int temp_group_num, int num_files, char* temp_file_name, SYSFILE *tempFile, int num_perm, SYSPERM *tempPerm, int temp_user_perm);
void printAccessControlList(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm);

int main(int argc, char *argv[]) {
    int i;
    char cmd[MAX_LEN];
    
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
    
    SYSPERM permissions[4] = {
        {0, "-"},
        {1, "r"},
        {2, "w"},
        {3, "rw"}
    };
    int num_permissions = 4;
    
    SYSUSER users[20] = {
        {"Instructor", "Instructors"},
        {"Student1", "TAs", {1,2}},
        {"Student2", "TAs", {3,4}},
        {"Student3", "Students", 1},
        {"Student4", "Students", 1},
        {"Student5", "Students", 2},
        {"Student6", "Students", 2},
        {"Student7", "Students", 3},
        {"Student8", "Students", 3},
        {"Student9", "Students", 4},
        {"Student10", "Students", 4},
        {"Student11", "Students"},
        {"Student12", "Students"}
    };
    int num_users = 13;
    //Increment this value with each new user
    
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
    
    
    //Here are all the working functions
    //Fill with dummy data, note more users then last time
    fillPrivileges(privileges_matrix, num_users, num_files, num_permissions, permissions, "-");

    // Print the matrix
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
		} else if (strcmp(cmd, "cslp") == 0) {
			changeStudentLabPerm(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "cslfp") == 0) {
			changeStudentLabFilePerm(privileges_matrix, num_users, num_files, num_permissions, users, files, permissions);
		} else if (strcmp(cmd, "cur") == 0) {
			changeUserRole(num_users, users);
		} else if (strcmp(cmd, "?cmds") == 0) {
			printf("Available commands:\n"
				"\tpam   -- Print access matrix\n"
				"\tcup   -- Change user privileges\n"
				"\tcfp   -- Change file privileges\n"
				"\tcuop  -- Change user object privileges\n"
				"\tcslp  -- Change student lab privileges\n"
				"\tcslfp -- Change student lab file privileges\n"
				"\tcur   -- Change user's role\n"
				"\tpacl  -- Print access control list\n"
				"\tpct   -- Print compatability ticket\n"
				"\texit  -- Terminate application\n"
			);
		} else if (strcmp(cmd, "exit") == 0) {
			break;
		}
		
		printf("\nPlease enter a command or \"?cmds\" for a list of commands: ");
	}

    return 0;
}

/*
 * Print the Access Control Matrix
 * Get the array with all the needed variables, num_user, num_files, tempUser, tempFile, tempPerm
 *
 * This makes it easier to view the matrix with the addition of the User Role and Lab number
 *
 */
void printPrivileges(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm){
    int i, j, k, x = 0;
    
    //print the file names
    printf("        User      User Role Lab#");
    for (i=0; i<num_files; i++) {
        printf("%*s", 5, tempFile[i].label);
    }
    printf("\n");
    for (i=0; i < num_users; i++){
        if(strncmp(tempUser[i].group, "", 100) == 0 && tempUser[i].group_num[0] == 0){         //print for users with no group or lab number
            printf("%12s%20s", tempUser[i].name, "");
        }else if(tempUser[i].group_num[0] == 0){                                                //print for users with just a group name
            printf("%12s  %12s     ", tempUser[i].name, tempUser[i].group);
        }else{                                                                                  //print for the group name and lab number
            for (k=0; k < (sizeof(tempUser[i].group_num) / sizeof(int)); k++) {
                if (!tempUser[i].group_num[k] == 0) {
                    x = x + 1;
                }
            }
            if(x > 1){
                printf("%12s  %12s: %d", tempUser[i].name, tempUser[i].group, tempUser[i].group_num[0]);
                for (k=1; k < x; k++) {
                    if (!tempUser[i].group_num[k] == 0) {
                        printf(",%d", tempUser[i].group_num[k]);
                    }
                }
            }else
                printf("%12s  %12s: %d  ", tempUser[i].name, tempUser[i].group, tempUser[i].group_num[0]);
            x = 0;
        }
        //Print the actual permissions
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



/*
 * Fill in the Matrix with some value inputed directly in the code
 * Get the array with all the needed variables, num_user, num_files, num_perm, tempPerm, newPerm
 */
void fillPrivileges(int** array, int num_users, int num_files, int num_perm, SYSPERM *tempPerm, char *newPerm){
    int i, j;

    int def_AM[15][7] = {
	//	CMF		AP		LI		LGR		DP		LM		MCM	
		{3,		3,		3,		3,		0,		3,		3}, // Instructor
		{1,		3,		3,		3,		0,		3,		0}, // TAs
		{1,		3,		1,		3,		0,		0,		0}, // S1
		{1,		3,		1,		3,		0,		0,		0}, // S2
		{1,		3,		1,		3,		0,		0,		0}, // S3
		{1,		3,		1,		3,		0,		0,		0},	// S4
		{1,		3,		1,		3,		0,		0,		0},	// S5
		{1,		3,		1,		3,		0,		0,		0},	// S6
		{1,		3,		1,		3,		0,		0,		0},	// S7
		{1,		3,		1,		3,		0,		0,		0},	// S8
		{1,		3,		1,		3,		0,		0,		0},	// S9
		{1,		3,		1,		3,		0,		0,		0},	// S10
		{1,		3,		1,		3,		0,		0,		0},	// S11
		{1,		3,		1,		3,		0,		0,		0},	// S*
		{1,		3,		1,		3,		0,		0,		0}	// S*
	};

    for(i=0; i<num_users; i++)
       for(j=0; j<num_files; j++)
			array[i][j]=def_AM[i][j];
}

/*
 * Change the user privilages
 * Does all the change but does not work with the group array for the TAs 
 * This could be done in further iteration
 *
 */
void changeUserRole(int num_users, SYSUSER *tempUser){
    char temp_user[15] = "";
    char new_group[15] = "";

    char term;

    int i, len, new_lab;
    
    // Promt for the User
    printf("Which user would you like to change: ");
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
		len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    for(i=0;i<num_users; i++){
        if(strcmp(temp_user, tempUser[i].name) == 0){
            printf("User: %s is part of group:%s and lab number:%d\n", tempUser[i].name, tempUser[i].group, tempUser[i].group_num[0]);
            
            // Promt for Lab change
            printf("Which lab would you like to assign to this user(integer): ");
            if(scanf("%d%c", &new_lab, &term) != 2 || term != '\n')
                printf("failure\n");
            
            // Promt for the group change
            printf("Which group would you like to assign the user to: ");
            if (fgets(new_group, MAX_LEN, stdin) != NULL) {
                clearInputStream(new_group);
                len = strlen(new_group);
                if(new_group[len - 1] == '\n') new_group[len - 1] = '\0';
            }
            
            strcpy(tempUser[i].group, new_group);
            tempUser[i].group_num[0] = new_lab;
        }
    }
    
}



/* THIS is take directly from Part 1
 *
 * Change the a specific user privalages
 * Get the array, ask for the user to be changed and find it in the matrix.
 * Change to the one inputted
 *
 */
void changeUserPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_user[15] = "";
    char temp_priv[10] = "";
    int len,i,j,k;
    
    
    // Promt for the User
    printf("Which user permission would you like to change(user change only): ");
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
                    if(strcmp(temp_priv,tempPerm[k].label) == 0){
                        array[i][j] = tempPerm[k].permission;
                    }
}



/* This is take directly from Part 1
 *
 * Change the a specific file privalages
 * Same as the user change but in this one you change all the privilages for an object
 *
 */
void changeFilePriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char input_file[25] = "";
    char temp_priv[10] = "";
    int len,i,j,k;
    
    // Promt for the User
    printf("Which file would you like to change(file change only): ");
    if (fgets(input_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(input_file);
		len = strlen(input_file);
		if(input_file[len - 1] == '\n') input_file[len - 1] = '\0';
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
        if(strcmp(input_file, tempFile[i].label) == 0)
            for (j=0; j < num_users; j++)
                for(k=0; k < num_perm; k++)
                    if(strcmp(temp_priv,tempPerm[k].label) == 0){
                        array[j][i] = tempPerm[k].permission;
                    }
}


/*
 *
 * Change the a specific user and object privalages
 * Ask for the user, object and the permission and apply all of those. Same as in the other two
 *
 */
void changeUserObjectPriv(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_user[15] = "";
    char temp_priv[15] = "";
    char input_file[25] = "";
    int len,i,j,k;
    
    // Select User
    printf("Which user permission would you like to change(user and file change): ");
    //Clearing of Input Buffer
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
        len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    //Select Object
    printf("Which file would you like to change: ");
    //Clearing of Input Buffer
    if (fgets(input_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(input_file);
		len = strlen(input_file);
		if(input_file[len - 1] == '\n') input_file[len - 1] = '\0';
	}
    
    //Select Privilege
    printf("Which privelages would you like to set(NO, R, W, RW): ");
    //Clearing of Input Buffer
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}
    
    
    //printf("used:%s, file:%s, privelage:%s\n", temp_user, input_file, temp_priv);
    //Go thought the matrix if the object and the user is matched then change the privilege
    for (i = 0; i < num_files; i++){
        if(strcmp(input_file, tempFile[i].label) == 0){
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

/*
 *
 * Change the a specific labs permissions for all the students in it
 * Ask for the user, object and the permission and apply all of those. Same as in the other two
 *
 */
void changeStudentLabPerm(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_priv[15] = "";
    char input_group[25] = "";
    int input_lab;
    char term;
    int len,i,j,k,x, temp_user_perm;
    
    // Select Lab Number
    printf("Which lab would you like to modify(integer): ");
    if(scanf("%d%c", &input_lab, &term) != 2 || term != '\n')
        printf("failure\n");
    
    
    //Select Object
    printf("Which group would you like to change: ");
    //Clearing of Input Buffer
    if (fgets(input_group, MAX_LEN, stdin) != NULL) {
        clearInputStream(input_group);
		len = strlen(input_group);
		if(input_group[len - 1] == '\n') input_group[len - 1] = '\0';
	}
    
    //Select Privilege
    printf("Which privelages would you like to set(NO, R, W, RW): ");
    //Clearing of Input Buffer
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}
    
    
    //printf("Lab#:%d, group:%s, privelage:%s\n", input_lab, input_group, temp_priv);
    
    //Go thought the matrix matching the Lab number, the file, and the privilage
    // Then change the permissions and after that call the "checkChangesForInstructor" to make sure that any other lower hierarchical users are changed as well
    for(i=0; i<num_users; i++){
        for(x=0; x< arraySize(tempUser[i].group_num); x++){
            if((strcmp(input_group, tempUser[i].group) == 0) && input_lab == tempUser[i].group_num[x])
                for (j=0; j < num_files; j++)
                    for(k=0; k < num_perm; k++)
                        if(strcmp(temp_priv,tempPerm[k].label) == 0){
                            array[i][j] = tempPerm[k].permission;
                            temp_user_perm = tempPerm[k].permission;
                            
                            checkChangesForInstructor(array, tempUser[i].group, num_users, tempUser, input_lab, num_files, tempFile[j].label, tempFile, num_perm, tempPerm, temp_user_perm);
                        }
            
        }
        
    }

    
}



/*
 * Works for the hierchies of the system
 * Change the a specific labs permissions for all the students in it
 * Ask for the user, object and the permission and apply all of those. Same as in the other two
 *
 */
void changeStudentLabFilePerm(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    char temp_priv[15] = "";
    char input_group[25] = "";
    char input_file[25] = "";
    char temp_user_group[25] = "";
    char temp_file_name[25];
    int input_lab = 0;
    char term;
    int len,i,j,k,x, temp_group_num=0;
    int temp_user_perm = 0;
    
    
    // Select Lab
    printf("Which lab would you like to modify(integer)(0 for instructors): ");
    
    if(scanf("%d%c", &input_lab, &term) != 2 || term != '\n')
        printf("failure\n");
    
    
     //Select Group
    printf("Which user role would you like to change: ");
    //Clearing of Input Buffer
    if (fgets(input_group, MAX_LEN, stdin) != NULL) {
        clearInputStream(input_group);
		len = strlen(input_group);
		if(input_group[len - 1] == '\n') input_group[len - 1] = '\0';
	}
    
    
     
    //Select File
    printf("Which file would you like to change: ");
    //Clearing of Input Buffer
    if (fgets(input_file, MAX_LEN, stdin) != NULL) {
        clearInputStream(input_file);
		len = strlen(input_file);
		if(input_file[len - 1] == '\n') input_file[len - 1] = '\0';
	}
    
    //Select Privilege
    printf("Which privelages would you like to set(NO, R, W, RW): ");
    //Clearing of Input Buffer
    if (fgets(temp_priv, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_priv);
		len = strlen(temp_priv);
		if(temp_priv[len - 1] == '\n') temp_priv[len - 1] = '\0';
	}
    
    
    printf("Lab#:%d, group:%s, privelage:%s\n", input_lab, input_group, temp_priv);
    
    
    //Go thought the matrix matching the Lab number, the file, privilage and name of the group
    // Then change the permissions and after that call the "checkChangesForInstructor" to make sure that any other lower hierarchical users are changed as well
    // in this one the checkChangesForInstructor is called after the loop, using the set temporary_files
    for (i = 0; i < num_users; i++){
        for(x=0; x<= arraySize(tempUser[i].group_num)+1; x++){
            if((strcmp(input_group, tempUser[i].group) == 0) && input_lab == tempUser[i].group_num[x]){
                for (j=0; j < num_files; j++){
                    if (strcmp(input_file, tempFile[j].label) == 0){
                        for(k=0; k < num_perm; k++){
                            if(strcmp(temp_priv,tempPerm[k].label) == 0){
                                array[i][j] = tempPerm[k].permission;
                                strcpy(temp_user_group, tempUser[i].group);
                                strcpy(temp_file_name, tempFile[j].label);
                                temp_user_perm = tempPerm[k].permission;
                                temp_group_num = tempUser[i].group_num[x];
                                
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    checkChangesForInstructor(array, temp_user_group, num_users, tempUser, temp_group_num, num_files, temp_file_name, tempFile, num_perm, tempPerm, temp_user_perm);    
}

/*
 * A helper function that changes the privilages for users who are lower in the hierckiee and have a higher permission
 * example: TA - RW, Student RW => change TA to R will result in Student also becoming R; but not the Instructor
 *
 */
void checkChangesForInstructor(int** array, char* temp_user_group, int num_users, SYSUSER *tempUser, int temp_group_num, int num_files, char* temp_file_name, SYSFILE *tempFile, int num_perm, SYSPERM *tempPerm, int temp_user_perm){

    int i,x,j,k;
    
    if(strcmp(temp_user_group, "Instructors") == 0){
        for (i = 0; i < num_users; i++){
            if(strcmp("TAs", tempUser[i].group) == 0){
                for (j=0; j < num_files; j++){
                    if (strcmp(temp_file_name, tempFile[j].label) == 0){
                        for(k=0; k < num_perm; k++){
                            if(tempPerm[k].permission > temp_user_perm){
                                array[i][j] = temp_user_perm;
                                for(x=0; x<= arraySize(tempUser[i].group_num)+1; x++){
                                    checkChangesForTA(array, tempUser[i].group, num_users, tempUser, tempUser[i].group_num[x], num_files, tempFile[j].label, tempFile, num_perm, tempPerm, temp_user_perm);

                                }
                            }
                        }
                    }
                }
            }
        }
    }else
        checkChangesForTA(array, temp_user_group, num_users, tempUser, temp_group_num, num_files, temp_file_name, tempFile, num_perm, tempPerm, temp_user_perm);

}

/*
 * A helper function that changes the privilages for users who are lower in the hierckiee and have a higher permission
 * example: TA - RW, Student RW => change TA to R will result in Student also becoming R; but not the Instructor
 *
 */
void checkChangesForTA(int** array, char* temp_user_group, int num_users, SYSUSER *tempUser, int temp_group_num, int num_files, char* temp_file_name, SYSFILE *tempFile, int num_perm, SYSPERM *tempPerm, int temp_user_perm){
    int i,x,j,k;
    
    if(strcmp(temp_user_group, "TAs") == 0){
        for (i = 0; i < num_users; i++){
            if(strcmp("Students", tempUser[i].group) == 0){
                for(x=0; x<= arraySize(tempUser[i].group_num); x++){
                    if(temp_group_num == tempUser[i].group_num[x]){
                        for (j=0; j < num_files; j++){
                            if (strcmp(temp_file_name, tempFile[j].label) == 0){
                                for(k=0; k < num_perm; k++){
                                    if(tempPerm[k].permission > temp_user_perm){
                                        array[i][j] = temp_user_perm;
                                    }
                                }
                            }
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

/*
 * This is Take from the Last Lab
 *
 * Print the Capability Ticket for a specific user
 * Prompt for input for the user and then show capabilities for each file
 */
void printCapabilityTicket(int** array, int num_users, int num_files, int num_perm, SYSUSER *tempUser, SYSFILE *tempFile, SYSPERM *tempPerm) {
    int i, j, len, k;
    char temp_user[25] = "";
    
    // Select User
    printf("Which user would you like to view: ");
    //Clearing of Input Buffer
    if (fgets(temp_user, MAX_LEN, stdin) != NULL) {
        clearInputStream(temp_user);
        len = strlen(temp_user);
		if(temp_user[len - 1] == '\n') temp_user[len - 1] = '\0';
	}
    
    for(i=0; i<num_users; i++){
        if(strcmp(temp_user, tempUser[i].name) == 0){
            printf("Capability Ticket for: %s\n\n", tempUser[i].name);
            
            for (j=0; j < num_files; j++){
                printf("%*s:", 25,tempFile[j].name);
                
                for(k=0; k < num_perm; k++){
                    if(array[i][j] == '\0'){
                        printf("   ");
                        break;
                    }else if(array[i][j] == tempPerm[k].permission){
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

/* Count the size of an Array
 *
 */
int arraySize(int* array){
    int i, x=0;
    for (i=0; i < (sizeof(array) / sizeof(int)); i++) {
        if (!array[i] == 0) {
            x = x + 1;
        }
    }
    return x;
    
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
