#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

typedef struck {
	int id;
	char name[MAX_NAME_LENGTH];
	int age;
} User;


void insert_user(FILE *file, User user) {
	fseek(file, 0, SEEK_END);
	fwrite(&user, sizeof(User), 1, file);

}

void select_all_users(FILE *file) {
    User user;
    fseek(file, 0, SEEK_SET);  // pointer to the start of the filea

    while (fread(&user, sizeof(User), 1, file)) {
        printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
    }
}

void update_user(FILE *file, int id, const char *new_name, int new_age) {
	User user;
	fseek(file, 0, SEEK_SET);

	while(fread(&user, sizeof(User), 1, file)) {
		if (user.id ==id) {
			strcpy(user.name, new_name);
			user.age = new_age;

			fseek(file, -sizeof(User), SEEK_CUR);
			fwrite(&user, sizeof(User), 1, file);
			break;
		}
	}
}

void delete_user(FILE *file, int id) {
	FILE *temp_file = fopen("temp.dat", "wb");
	User user;

	fseek(file, 0, SEEK_SET);

	while (fread(&user, sizeof(User), 1, file)) {
		if (user.id != id) {
			fwrite(&user, sizeof(User), 1, temp_file);
		}
	}

	fclose(file);
	fclose(temp_file);
	remove("database.dat");
	rename("temp.dat", "database.dat");
}

int main() { // test operation functions of the database
    FILE *file = fopen("database.dat", "rb+");

    if (!file) {
        file = fopen("database.dat", "wb+");  // create new database
    }

    User user1 = {1, "Alice", 30};
    User user2 = {2, "Bob", 25};
    
    insert_user(file, user1);
    insert_user(file, user2);

    printf("All users:\n");
    select_all_users(file);

    update_user(file, 1, "Alice Cooper", 31);

    printf("\nAfter update:\n");
    select_all_users(file);

    delete_user(file, 2);

    printf("\nAfter delete:\n");
    select_all_users(file);

    fclose(file);
    return 0;
}

