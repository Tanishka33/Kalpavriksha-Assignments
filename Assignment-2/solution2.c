#include<stdio.h> 
#include<stdlib.h>
#include<string.h> // for functions like strcspn(used to trim newline)

#define FILENAME "users.txt"
#define NAME_LEN 100

//Structure defination
typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
} User;

void ensureFileExist();
void readUsers();
void createUser();
void updateUser();
void deleteUser();

int main(){
    ensureFileExist();

    int choice;
    while(1){ // while will keep showing menu until user exits 
        printf("______ User Management _______\n");
        printf("1. Add user\n");
        printf("2. Display all users\n");
        printf("3. Update user\n");
        printf("4. Delete user\n");
        printf("5. Exit\n");
        printf("Enter choice: \n");

        if(scanf("%d", &choice) !=1 ){
            printf("Invalid input. Try again. \n");
            while(getchar()!= '\n'); // clear wront input 
            continue;
        }
        while (getchar() != '\n'); // to clear leftover newline also prevents fgets() issues later

        switch ((choice)){
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: 
                printf("Exiting...\n");
                exit(0);
                break;
            default: printf("Invalid choice. Try again. \n");
        }  
    }
}

// code of all functions

void ensureFileExist(){
    FILE *fp = fopen(FILENAME, "r"); // try opening in r mode
    if(fp == NULL){
        fp = fopen(FILENAME,"w"); // if r mode fails use w mode which creates empty file
        if(fp == NULL){
            perror("failed to create file");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    } else{
        fclose(fp);
    }
}

void readUsers(){
    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL){
        perror("Failed to open file for reading");
        return;
    }

    int id, age;
    char name[NAME_LEN];
    int found = 0;
    while (fscanf(fp,"%d|%99[^|]|%d \n", &id, name, &age) == 3) // reads line in format like id|name|age , and %99[^|] means " reads upto 99 characters stopping at |".
    {
        printf("%d\t %-20s\t %d\n", id, name, age); // if record found we print it
        found = 1;
    }
    if(!found){
        printf("No user found.\n");
    }
    fclose(fp);
}

void createUser(){
    User u;
    printf("Enter ID of User: ");
    if(scanf("%d", &u.id) !=1){ // asking for valid input 
        printf("Invalid user id.\n");
        while (getchar()!='\n');
        return;
    }
    while (getchar()!='\n'); // consume newline
    
    printf("Enter name: ");
    if(fgets(u.name, NAME_LEN, stdin)==NULL) //reading full name with fgets
    {
        printf("Failed to read name.\n");
        return;
    }
    u.name[strcspn(u.name, "\n")] = '\0'; // it trims newline

    printf("Enter age: ");   
    if(scanf("%d", &u.age) != 1){
        printf("Invalid age.\n");
        while (getchar()!='\n');
        return;
    }
    if(u.age<=0 || u.age>120){
        printf("Invalid age. Must be valid. \n");
        return;
    } 
    
    FILE *fp = fopen(FILENAME, "r");
    if(fp != NULL){
        int id;
        char name[NAME_LEN];
        int age;
        while (fscanf(fp, "%d|%99[^|]|%d \n", &id, name, &age) == 3){
            if(id == u.id){
                printf("Error: ID %d already exists. Aborting add. \n");
                fclose(fp);
                return;
            }
        }
        fclose(fp);  
    }
    fp = fopen(FILENAME, "a");
    if(fp == NULL){
        perror("Failed to open file for appending");
        return;
    }
    fprintf(fp, "%d|%s|%d \n", u.id, u.name, u.age);
    fclose(fp);

    printf("User added successfully. \n");
}

void updateUser() {
    int targetId;
    int found = 0;
    User u;

    printf("Enter the ID of the user to update: ");
    if(scanf("%d", &targetId) != 1){
        printf("Invalid input.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n'); // clear leftover newline

    FILE *fp = fopen(FILENAME, "r");
    if(fp == NULL){
        perror("Failed to open file for reading");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if(temp == NULL){
        perror("Failed to create temp file.");
        fclose(fp);
        return;
    }

    while(fscanf(fp, "%d|%99[^|]|%d \n", &u.id, u.name, &u.age) == 3){
        if(u.id == targetId){
            found = 1;
            printf("Enter new name: ");
            if(fgets(u.name, NAME_LEN, stdin) == NULL){
                strcpy(u.name, "Unknown");
            }
            u.name[strcspn(u.name, "\n")] = '\0';

            printf("Enter new age: ");
            if(scanf("%d", &u.age) != 1 || u.age <= 0 || u.age > 120){
                printf("Invalid age. Skipping age update.\n");
                while(getchar() != '\n');
            }
            while(getchar() != '\n'); // consume newline
        }

        // Write record (updated or unchanged) to temp
        fprintf(temp, "%d|%s|%d \n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    if(found){
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("User updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", targetId);
    }
}

void deleteUser(){
    int targetId;
    int found=0;
    User u;
    printf("Enter the ID of user to delete: ");
    if(scanf("%d", &targetId)!=1){
        printf("Invalid input. \n");
        while (getchar()!='\n');
        return;
    }
    while (getchar()!='\n');

    FILE*fp = fopen(FILENAME,"r");
    if(fp == NULL){
        perror("Failed to open file for reading.");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if(temp==NULL){
        perror("Failed to create temp file.");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%99[^|]|%d \n", &u.id, &u.name, &u.age)==3){
        if(u.id==targetId){
            found=1;
            continue;
        }
        fprintf(temp, "%d|%s|%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    if(found){
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("User with ID %d deleted succesfully. \n", targetId);
    } else
    {
        remove("temp.txt");
        printf("User with ID %d not found. \n", targetId);
    }
}


