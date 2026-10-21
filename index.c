#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define TEXT 100
 
struct Location {
    int locationID;
    char locationName[TEXT];
    char category[TEXT];
    char building[TEXT];
    char floor[TEXT];
    char navigation[TEXT];
    struct Location *next;
};
 
struct Student {
    int studentID;
    char studentName[TEXT];
    char service[TEXT];
    struct Student *next;
};
 
struct History {
    int locationID;
    char locationName[TEXT];
    struct History *next;
};
 
struct Location *head = NULL;
struct Student *front = NULL, *rear = NULL;
struct History *top = NULL;
 
/* Read a complete line safely */
void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}
 
/* Read an integer and reject invalid input */
int readInt(const char *prompt, int *value) {
    char line[100], extra;
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, " %d %c", value, &extra) != 1) {
        printf("Invalid numeric input. Please try again.\n");
        return 0;
    }
    return 1;
}
 
void addCollegeLocation(void) {
    struct Location *node = malloc(sizeof(struct Location));
    int id;
    if (!node) { printf("Memory allocation failed.\n"); return; }
 
    if (!readInt("Enter Location ID: ", &id)) { free(node); return; }
    node->locationID = id;
    readLine("Enter Location Name: ", node->locationName, sizeof(node->locationName));
    readLine("Enter Category: ", node->category, sizeof(node->category));
    readLine("Enter Building/Block: ", node->building, sizeof(node->building));
    readLine("Enter Floor: ", node->floor, sizeof(node->floor));
    readLine("Enter Navigation Information: ", node->navigation, sizeof(node->navigation));
 
    node->next = head;
    head = node;
    printf("College location added successfully!\n");
}
 
void enqueueStudent(void) {
    struct Student *node = malloc(sizeof(struct Student));
    if (!node) { printf("Memory allocation failed.\n"); return; }
    if (!readInt("Enter Student ID: ", &node->studentID)) { free(node); return; }
    readLine("Enter Student Name: ", node->studentName, sizeof(node->studentName));
    readLine("Enter Service/Department Required: ", node->service, sizeof(node->service));
    node->next = NULL;
    if (rear == NULL) front = rear = node;
    else { rear->next = node; rear = node; }
    printf("Student added to the Queue successfully!\n");
}
 
void displayLocations(void) {
    struct Location *p = head;
    if (!p) { printf("No college locations available.\n"); return; }
    printf("\n--- COLLEGE LOCATIONS ---\n");
    while (p) {
        printf("ID: %d | Name: %s | Category: %s | Building: %s | Floor: %s | Route: %s\n",
               p->locationID, p->locationName, p->category, p->building, p->floor, p->navigation);
        p = p->next;
    }
}
 
void displayQueue(void) {
    struct Student *p = front;
    if (!p) { printf("No students are waiting in the Queue.\n"); return; }
    printf("\n--- STUDENT SERVICE QUEUE (Front to Rear) ---\n");
    while (p) {
        printf("ID: %d | Name: %s | Service: %s\n", p->studentID, p->studentName, p->service);
        p = p->next;
    }
}
 
struct Location *findLocation(int id) {
    struct Location *p = head;
    while (p) {
        if (p->locationID == id) return p;
        p = p->next;
    }
    return NULL;
}
 
void pushHistory(const struct Location *loc) {
    struct History *node = malloc(sizeof(struct History));
    if (!node) { printf("Could not save navigation history (memory allocation failed).\n"); return; }
    node->locationID = loc->locationID;
    snprintf(node->locationName, sizeof(node->locationName), "%s", loc->locationName);
    node->next = top;
    top = node;
}
 
void searchLocation(void) {
    int id;
    if (!head) { printf("No college locations available to search.\n"); return; }
    if (!readInt("Enter Location ID to search: ", &id)) return;
    struct Location *loc = findLocation(id);
    if (!loc) { printf("Location not found.\n"); return; }
    printf("\nLocation Found!\nID: %d\nName: %s\nCategory: %s\nBuilding: %s\nFloor: %s\nNavigation: %s\n",
           loc->locationID, loc->locationName, loc->category, loc->building, loc->floor, loc->navigation);
    pushHistory(loc);
}
 
void dequeueStudent(void) {
    if (!front) { printf("No students are waiting in the Queue.\n"); return; }
    struct Student *temp = front;
    printf("Serving Student: ID: %d | Name: %s | Service: %s\n",
           temp->studentID, temp->studentName, temp->service);
    front = front->next;
    if (!front) rear = NULL;
    free(temp);
    printf("Student service completed. Next student, if any, is now at the front.\n");
}
 
/* Stage 3 feature: stack of recently searched locations */
void displayNavigationHistory(void) {
    struct History *p = top;
    if (!p) { printf("Navigation history is empty.\n"); return; }
    printf("\n--- RECENTLY SEARCHED LOCATIONS (Top to Bottom) ---\n");
    while (p) {
        printf("Location ID: %d | Name: %s\n", p->locationID, p->locationName);
        p = p->next;
    }
}
 
/* Optional stack operation: remove the latest history entry */
void popNavigationHistory(void) {
    if (!top) { printf("Navigation history is empty. Nothing to remove.\n"); return; }
    struct History *temp = top;
    printf("Removed latest history entry: %d - %s\n", temp->locationID, temp->locationName);
    top = top->next;
    free(temp);
}
 
void freeAll(void) {
    while (head) { struct Location *t = head; head = head->next; free(t); }
    while (front) { struct Student *t = front; front = front->next; free(t); }
    while (top) { struct History *t = top; top = top->next; free(t); }
}
 
int main(void) {
    int choice;
    do {
        printf("\n========================================\n");
        printf(" SMART COLLEGE MANAGEMENT & NAVIGATION\n");
        printf("========================================\n");
        printf("1. Add College Location\n");
        printf("2. Add Student to Service Queue\n");
        printf("3. Display College Locations\n");
        printf("4. Display Student Queue\n");
        printf("5. Search College Location\n");
        printf("6. Serve Next Student\n");
        printf("7. Display Navigation History (Stack)\n");
        printf("8. Remove Latest History Entry (Pop)\n");
        printf("9. Exit\n");
        if (!readInt("Enter your choice: ", &choice)) continue;
        switch (choice) {
            case 1: addCollegeLocation(); break;
            case 2: enqueueStudent(); break;
            case 3: displayLocations(); break;
            case 4: displayQueue(); break;
            case 5: searchLocation(); break;
            case 6: dequeueStudent(); break;
            case 7: displayNavigationHistory(); break;
            case 8: popNavigationHistory(); break;
            case 9: printf("Exiting program...\n"); break;
            default: printf("Invalid choice. Please select 1 to 9.\n");
        }
    } while (choice != 9);
    freeAll();
    return 0;
}
