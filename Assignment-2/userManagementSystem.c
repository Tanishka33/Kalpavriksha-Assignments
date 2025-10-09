#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "users.txt"
#define NAME_LENGTH 100
#define MAX_USERS 100

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
} User;

enum CrudOperation {
    ADD_USER = 1,
    DISPLAY_USERS,
    UPDATE_USER,
    DELETE_USER,
    EXIT_APP
};

void ensureFileExist();
FILE* openFile(const char *fileName, const char *mode);
User readUserData();
int validateUserData(User user);
void createUser();
void readUsers();
void updateUser();
void deleteUser();

int main() {
    ensureFileExist();

    bool running = true;
    int choice;

    while (running) {
        printf("______ User Management _______\n");
        printf("1. Add user\n");
        printf("2. Display all users\n");
        printf("3. Upadate user\n");
        printf("4. Delete user\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        enum CrudOperation operation = (enum CrudOperation) choice;

        switch (operation) {
            case ADD_USER:
                createUser();
                break;
            case DISPLAY_USERS:
                readUsers();
                break;
            case UPDATE_USER:
                updateUser();
                break;
            case DELETE_USER:
                deleteUser();
                break;
            case EXIT_APP:
                printf("Exiting...\n");
                running = false;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

void ensureFileExist() {
    FILE *filePointer = fopen(FILE_NAME, "r");
    if (filePointer == NULL) {
        filePointer = fopen(FILE_NAME, "w");
        if (filePointer == NULL) {
            perror("Failed to create file");
            exit(EXIT_FAILURE);
        }
    }
    fclose(filePointer);
}

FILE* openFile(const char *fileName, const char *mode) {
    FILE *filePointer = fopen(fileName, mode);
    if (filePointer == NULL) {
        perror("File open failed");
    }
    return filePointer;
}

User readUserData() {
    User user;
    printf("Enter ID of User: ");
    if (scanf("%d", &user.id) != 1) {
        user.id = -1;
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    printf("Enter name: ");
    if (fgets(user.name, NAME_LENGTH, stdin) == NULL) {
        strcpy(user.name, "Unknown");
    }
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter age: ");
    if (scanf("%d", &user.age) != 1) {
        user.age = -1;
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return user;
}

int validateUserData(User user) {
    int isValid = 1;
    if (user.id <= 0) {
        printf("Invalid ID.\n");
        isValid = 0;
    }
    if (strlen(user.name) == 0) {
        printf("Name cannot be empty.\n");
        isValid = 0;
    }
    if (user.age <= 0 || user.age > 120) {
        printf("Invalid age.\n");
        isValid = 0;
    }
    return isValid;
}

void createUser() {
    User newUser = readUserData();
    if (!validateUserData(newUser)) return;

    FILE *filePointer = openFile(FILE_NAME, "r");
    if (filePointer != NULL) {
        int id, age;
        char name[NAME_LENGTH];
        while (fscanf(filePointer, "%d|%99[^|]|%d\n", &id, name, &age) == 3) {
            if (id == newUser.id) {
                printf("Error: ID %d already exists.\n", id);
                fclose(filePointer);
                return;
            }
        }
        fclose(filePointer);
    }

    filePointer = openFile(FILE_NAME, "a");
    if (filePointer == NULL) return;

    fprintf(filePointer, "%d|%s|%d\n", newUser.id, newUser.name, newUser.age);
    fclose(filePointer);
    printf("User added successfully.\n");
}

void readUsers() {
    FILE *filePointer = openFile(FILE_NAME, "r");
    if (filePointer == NULL) return;

    int id, age, found = 0;
    char name[NAME_LENGTH];
    while (fscanf(filePointer, "%d|%99[^|]|%d\n", &id, name, &age) == 3) {
        printf("%d\t%-20s\t%d\n", id, name, age);
        found = 1;
    }
    if (!found) {
        printf("No user found.\n");
    }
    fclose(filePointer);
}

void updateUser() {
    int targetId, found = 0;
    User users[MAX_USERS];
    int userCount = 0;

    FILE *filePointer = openFile(FILE_NAME, "r");
    if (filePointer == NULL) return;

    while (fscanf(filePointer, "%d|%99[^|]|%d\n", &users[userCount].id, users[userCount].name, &users[userCount].age) == 3) {
        userCount++;
    }
    fclose(filePointer);

    printf("Enter the ID of the user to update: ");
    if (scanf("%d", &targetId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == targetId) {
            found = 1;
            User updatedUser = readUserData();
            if (validateUserData(updatedUser)) {
                users[i] = updatedUser;
            }
            break;
        }
    }

    if (!found) {
        printf("User with ID %d not found.\n", targetId);
        return;
    }

    filePointer = openFile(FILE_NAME, "w");
    if (filePointer == NULL) return;

    for (int i = 0; i < userCount; i++) {
        fprintf(filePointer, "%d|%s|%d\n", users[i].id, users[i].name, users[i].age);
    }
    fclose(filePointer);

    printf("User updated successfully.\n");
}

void deleteUser() {
    int targetId, found = 0;
    User users[MAX_USERS];
    int userCount = 0;

    FILE *filePointer = openFile(FILE_NAME, "r");
    if (filePointer == NULL) return;

    while (fscanf(filePointer, "%d|%99[^|]|%d\n", &users[userCount].id, users[userCount].name, &users[userCount].age) == 3) {
        userCount++;
    }
    fclose(filePointer);

    printf("Enter the ID of user to delete: ");
    if (scanf("%d", &targetId) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == targetId) {
            found = 1;
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            break;
        }
    }

    if (!found) {
        printf("User with ID %d not found.\n", targetId);
        return;
    }

    filePointer = openFile(FILE_NAME, "w");
    if (filePointer == NULL) return;

    for (int i = 0; i < userCount; i++) {
        fprintf(filePointer, "%d|%s|%d\n", users[i].id, users[i].name, users[i].age);
    }
    fclose(filePointer);

    printf("User with ID %d deleted successfully.\n", targetId);
}
