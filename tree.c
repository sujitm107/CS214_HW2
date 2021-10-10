#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


typedef struct Node{
	char *name;
	char *path;
	struct Node* sibling;
	struct Node* children;
}Node;

Node* getChildren(char* cur_path);
int isDirectory(const char *path);
void insert(Node **head, Node* new);
void printTree(Node *head, int level);
void freeTree(Node *head);

int main(int argc, char **argv){
	
	Node head;
	head.name = ".";
	head.sibling = NULL;
	head.children = getChildren("."); 
	printf(".\n");
	printTree(head.children, 0);
	freeTree(head.children);

}
int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

void insert(Node **head, Node* new){
	
	char* name = new->name;
	
	
	if (*head == NULL){
		*head = new;
		new->sibling = NULL;
	}else {
	
		Node *ptr = *head;
		
		Node *prev = NULL;
		while(ptr!=NULL && strcmp(name, ptr->name)>0){
			prev = ptr;
			ptr = ptr->sibling;
		}

		if (prev == NULL){
			new->sibling = *head;
			*head = new;
		}else{
			prev->sibling = new;
			new->sibling = ptr;
		}
	}

	if (isDirectory(new->path)){
		new->children = getChildren(new->path);
	}

}
Node* getChildren(char* cur_path){

	DIR *d;
	struct dirent *dir;
	errno = 0;
	d = opendir(cur_path);
	Node *head = NULL;
	if (d) {
    	while ((dir = readdir(d)) != NULL) {
      		if(dir->d_name[0]=='.'){
				  continue; //skip hidden files
			}else{
				Node *ins = (Node*)malloc(sizeof(Node));
				if (ins == NULL){
					perror("Memory error!");
					exit(EXIT_FAILURE);
				}
				ins->name = (char*)malloc(sizeof(char)*(strlen(dir->d_name)+1));
				strcpy(ins->name, dir->d_name);
				// printf("%s\n",ins->name);
				int path_len = strlen(cur_path)+strlen(ins->name)+2; //+2 for file seperator('/') and terminator ('\0')
				ins->path = (char*) malloc(sizeof(char)*path_len);
				if (ins->path == NULL){
					perror("Memory error!");
					exit(EXIT_FAILURE);
				}
				strcpy(ins->path, cur_path); //copy current path to path
				strcat(ins->path, "/"); //add delimiter
				strcat(ins->path, ins->name); //add file name
				// printf("%s",ins->path);
				ins->sibling = NULL;
				ins->children = NULL;
				insert(&head, ins);
			}
		}
	}
	closedir(d);
	return head;
}

void printTree(Node* head, int level){
	while(head!=NULL){
		printf("%*s- ", level*2, "");
		printf("%s\n", head->name);
		if(head->children!=NULL){
			printTree(head->children, level+1);
		}
		head = head->sibling;
	}
}

void freeTree(Node *head){
	if (head->children!=NULL){
		freeTree(head->children);
	}
	if(head->sibling!=NULL){
		freeTree(head->sibling);
	}
	// printf("freeing %s", head->name);
	free(head->path);
	free(head->name);
	free(head);
}

