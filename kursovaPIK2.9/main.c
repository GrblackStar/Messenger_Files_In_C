#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct pointer_data {
    int DD;
    int MM1;
    int year;
    int HH;
    int MM2;
    int SS;
    char client[15];
    char protocol[15];
    char name[20];
    char sentence[200];

	struct pointer_data *next;
	struct pointer_data *previous;
} data;


//reading input
char *readStringFromFile(int maxLength, FILE *fp) {
	//динамично локиране. когато го заделя така, няма да загубя "input", когато изляза от функцията
	char *input = malloc(sizeof(char)*(maxLength+1));
	fgets(input, maxLength, fp);

	// Remove the newline after fgets
	unsigned inputLength = strlen(input);
	if (input[inputLength-1] == '\n') {
        input[inputLength-1] = '\0';
        inputLength--;
	}

	return input;
}


data *MakeNew() {
    char *read = NULL;
	data *result = malloc(sizeof(data));

    if (!result) {
        printf("\nNo memory allocated!");
        return result;
    } else {
        printf("Enter DD: ");
        scanf("%d", &result->DD);
        fflush(stdin);

        printf("Enter MM: ");
        scanf("%d", &result->MM1);
        fflush(stdin);

        printf("Enter YYYY: ");
        scanf("%d", &result->year);
        fflush(stdin);

        printf("Enter HH: ");
        scanf("%d", &result->HH);
        fflush(stdin);

        printf("Enter MM: ");
        scanf("%d", &result->MM2);
        fflush(stdin);

        printf("Enter SS: ");
        scanf("%d", &result->SS);
        // Flush when done reading with scanf
        fflush(stdin);

        //Read the strings
        printf("Enter client: ");
        read = readStringFromFile(15, stdin);
        strcpy(result->client, read);

        printf("Enter protocol: ");
        read = readStringFromFile(15, stdin);
        strcpy(result->protocol, read);

        printf("Enter user: ");
        read = readStringFromFile(20, stdin);
        strcpy(result->name, read);

        printf("Enter message: ");
        read = readStringFromFile(200, stdin);
        strcpy(result->sentence, read);

        result->next = NULL;
        result->previous = NULL;

        return result;
    }
}


data *AddToStart(data *startPtr) {
	data *new_item = MakeNew();
	if(startPtr != NULL) {
		startPtr->previous = new_item;
		new_item->next = startPtr;
	}

	return new_item;
}


data *AddToEnd(data *startPtr) {
	data *returnPtr = startPtr;
	data *new_item = NULL;

	if(startPtr == NULL) {
		new_item = AddToStart(startPtr);
		returnPtr = new_item;
	} else {
		data *index = startPtr;
		while (index->next != NULL) {
			index = index->next;
		}
		new_item = MakeNew();
		index->next = new_item;
		new_item->next = NULL;
		new_item->previous = index;
	}
	return returnPtr;
}


data *Delete(data *startPtr) {

	printf("Enter User To Delete:");
    char *input = readStringFromFile(15, stdin);

	data *ref = startPtr;
	data *userToDelete = NULL;

	while(ref != NULL) {

		if(strncmp(input, ref->name, strlen(ref->name)) == 0) {
			userToDelete = ref;
			break;
		}

		ref = ref->next;
	}

	if(startPtr != NULL && userToDelete == startPtr) {
		if(userToDelete->next != NULL) {
			userToDelete->next->previous = NULL;
			startPtr = userToDelete->next;
		} else {
			startPtr = NULL;
		}
	} else {
		if(userToDelete != NULL) {
			if(userToDelete->previous != NULL) {
				userToDelete->previous->next = userToDelete->next;
			}

			if(userToDelete->next != NULL) {
				userToDelete->next->previous = userToDelete->previous;
			}
		}
	}

	if(userToDelete != NULL) {
		userToDelete = NULL;
		free(userToDelete);
	}

	return startPtr;

}


//second function - transferring chat history to a binary file
void WriteListToFile(data *root) {
	FILE *pFile;
	pFile = fopen("history.bin", "wb");

	if(pFile != NULL) {
		data *curr_item = root;

		data *holdNext = NULL;
		data *holdPrevious = NULL;

		while(curr_item != NULL) {
			holdNext = curr_item->next;
			holdPrevious = curr_item->previous;

			curr_item->next = NULL;
			curr_item->previous = NULL;

			fseek(pFile, 0, SEEK_END);
			fwrite(curr_item, sizeof(data), 1, pFile);

			curr_item->next = holdNext;
			curr_item->previous = holdPrevious;

			holdNext = NULL;
			holdPrevious = NULL;

			curr_item = curr_item->next;
		}
		fclose(pFile);
		pFile = NULL;
	} else {
		printf("FILE OPEN ERROR\n");
	}

}


//reading from a binary file
data *ReadNextFromFile(data *root, FILE *pFile) {
	size_t returnValue;
	if(root == NULL) {
		root = malloc(sizeof(data));
		root->next = NULL;
		root->previous = NULL;

		returnValue = fread(root, sizeof(data), 1, pFile);
		if (returnValue != 1) {
            perror("File read error");
            free(root);
            return NULL;
		}
	} else {
		data *index = root;
		data *new_item = malloc(sizeof(data));
		new_item->next = NULL;
		new_item->previous = NULL;

		while(index->next != NULL) {
			index = index->next;
		}

		returnValue = fread(new_item, sizeof(data), 1, pFile);
		if (returnValue != 1) {
            perror("File read error");
            free(root);
            return NULL;
		}

		index->next = new_item;
		new_item->previous = index;
	}
	return root;
}


void countingLines(data *startPtr) {
    int count = 0;

	printf("Enter User:");
    char *input = readStringFromFile(15, stdin);

	data *ref = startPtr;

	while(ref != NULL) {
		if(strcmp(input, ref->name) == 0) {
			count++;
        }
		ref = ref->next;
	}

		printf("%d\n\n", count);
}


void countingCharacters(data *startPtr) {
    int count = 0;

	printf("Enter User:");
    char *input = readStringFromFile(15, stdin);

	data *ref = startPtr;

	while(ref != NULL) {

		if(strcmp(input, ref->name) == 0) {
			//strcpy (refCom, ref->sentence);
			count += strlen(ref->sentence);
        }
		ref = ref->next;
	}

		printf("%d\n\n", count);
}


//counts how many times a string occurs in another string
int countSubstrings(const char *str, const char *substr)
{
    int count = 0;
    size_t substrLength = strlen(substr);
    const char *p = str;
    while (NULL != (p = strstr(p, substr)))
    {
        count++;
        p += substrLength;
    }
    return count;
}


void moodDeterminator (data *startPtr)
{
    int countHappiness = 0;
    int countSadness = 0;

	printf("Enter User:");
    char *input = readStringFromFile(15, stdin);

	for (data *ref = startPtr; ref != NULL; ref = ref->next) {
		if(strcmp(input, ref->name) == 0) {
            countHappiness += countSubstrings(ref->sentence, ":)");
            countHappiness += countSubstrings(ref->sentence, ":D");
            countHappiness += countSubstrings(ref->sentence, "=]");

            countSadness += countSubstrings(ref->sentence, ":(");
            countSadness += countSubstrings(ref->sentence, ":'(");
            countSadness += countSubstrings(ref->sentence, "=(");
        }
	}

    float mood;
    if (countSadness != 0) {
        mood = (float)countHappiness / countSadness;
    } else {
        mood = countHappiness;
    }

	if (mood > 1.5) {
        printf("HAPPY\n\n");
	}
    else if (mood < 0.5) {
        printf ("Sad\n\n");
    }
    else if (mood >= 0.5 && mood <= 1.5) {
        printf ("Neutral\n\n");
    }
}


// Writes the data from the list to a txt file
void writeToTxtFile(data *head) {
    FILE *fp = fopen("chat.txt", "w");

    if (fp == NULL) {
        perror("File write open error");
        return;
    }

    printf("Writing to txt file...\n\n");

    for (data *p = head; p != NULL; p = p->next) {
        int res = fprintf(fp, "[%d.%d.%d %d:%d:%d] [%s .%s ] %s : %s\n", p->DD, p->MM1, p->year, p->HH, p->MM2, p->SS, p->client, p->protocol, p->name, p->sentence);
        if (res < 0) {
            perror("File write error");
            return;
        }
    }

    fclose(fp);
}

// Prints the data from the list in a readable format
void printList(data *head) {
    for (data *p = head; p != NULL; p=p->next) {
        printf("[%d.%d.%d %d:%d:%d] [%s.%s] %s: %s\n", p->DD, p->MM1, p->year, p->HH, p->MM2, p->SS, p->client, p->protocol, p->name, p->sentence);
    }
    printf("\n");
}

// Reads data from a txt file and populates a list
data *readFromTxtFile(data *head) {
    FILE *fp = fopen("chat.txt", "r");

    if (fp == NULL) {
        perror("File read open error");
        return NULL;
    }

    data *current = NULL;
    while (!feof(fp)) {
        data *newItem = malloc(sizeof(data));
        newItem->next = NULL;
        newItem->previous = NULL;

        // without & => 0xC00000FD unknown software exception
        // fscanf with %s reads characters until the next whitespace
        int res = fscanf(fp, "[%d.%d.%d %d:%d:%d] [%s .%s ] %s : ", &newItem->DD, &newItem->MM1, &newItem->year, &newItem->HH, &newItem->MM2, &newItem->SS, newItem->client, newItem->protocol, newItem->name, newItem->sentence);

        if ((res >= 0 && res < 9) || ferror(fp)) {
            perror("File read error");
            free(newItem);
            return head;
        } else if (res < 0 || feof(fp)) {
            // EOF reached
            free(newItem);
            return head;
        }

        strcpy(newItem->sentence, readStringFromFile(200, fp));

        if (head == NULL) {
            head = newItem;
        } else {
            current->next = newItem;
            // new current
        }
        current = newItem;
    }

    fclose(fp);

    return head;
}


int Readmenu(char *menu[]){
	int choice;
	printf("%s", menu[0]);

	for (int b = 1; (menu[b]); b++){
		printf("\n%d. %s", b, menu[b]);
	}

	printf("\nEnter your choice: ");
	scanf("%d", &choice);
	fflush(stdin);
	return choice;
}


int main() {
	int choiceFromMenu;
	data *root = NULL;

	root = readFromTxtFile(root);

    char *menu[] = {
		"MENU",
		"ADD message",
		"transfer chat history to file",
		"DELETE messages from user",
		"determine mood of user",
		"show how many messages a user has",
		"show how many symbols a user has",
		"write data to a text file",
		"print list",
		"Exit",
		NULL,
	};


	do{
		choiceFromMenu = Readmenu(menu);
		switch (choiceFromMenu)
		{
        case 1:
            root = AddToEnd(root);
            break;


        case 2:
            WriteListToFile(root);
            break;


        case 3:
            root = Delete(root);
            break;

        case 4:
            moodDeterminator (root);
            break;

        case 5:
            countingLines(root);
            break;

        case 6:
            countingCharacters(root);
            break;

        case 7:
            writeToTxtFile(root);
            break;

        case 8:
            printList(root);
            break;

		}
		} while (choiceFromMenu != 9);



	return 0;

}
