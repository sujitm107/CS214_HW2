#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef struct {
    char* fileName;
    struct stat statBuf;
} FileData;

int* decimalToBinary(int decNum){
    int* bin = malloc(3 * sizeof(int));
    int i = 0;
        while(decNum > 0){
            bin[i] = decNum % 2;
            decNum /= 2;
            i++;
        }
    return bin;
}

int decimalToOctal(int decNum){
    int octNum = 0, tmp = 1;

    while(decNum > 0){
        octNum = octNum + (decNum % 8) * tmp;
        decNum /= 8;
        tmp *= 10;
    }
    return octNum;
}

int* trimOctal(int octal){
    int* trimmedOctal = malloc(3 * sizeof(int));

    for(int i = 2; i >= 0; i--){
        trimmedOctal[i] = octal % 10;
        octal /= 10;
    }
    return trimmedOctal;
}

void printPermissionsByGroup(int num){
    int* binaryNum = decimalToBinary(num);
    char perms[] = {'x', 'w', 'r'};
    for(int i = 2; i>= 0; i--){
        if(binaryNum[i]){
            printf("%c", perms[i]);
        } else {
            printf("-");
        }
    }
    free(binaryNum);
}

void printPermissions(mode_t mode){
    //get decimal format -> octal format -> last 3 digits of octal
    int modeInDecimal = mode;
    int octal = decimalToOctal(modeInDecimal);
    int* modeInOctal = trimOctal(octal); //gets last three digits

    //print file type
    char type = (S_ISDIR(mode)) ? 'd' : '-';
    printf("%c", type);

    //print permission digit by digit
    for(int i = 0; i < 3; i++){
        printPermissionsByGroup(modeInOctal[i]);
    }
    printf(" ");
    free(modeInOctal);
}

void printUserName(int uid){
    struct passwd *pw = getpwuid(uid);
    printf("%s ", pw->pw_name);
}

void printGroupName(int gid){
    struct group *gr = getgrgid(gid);
    printf("%s ", gr->gr_name);
}

void printFileSize(int fs){
    printf("%d ", fs);
}

void printModTime(time_t t){
    char buffer[80];
    strftime(buffer, 80, "%h %d %R", localtime(&t));
    printf("%s ", buffer);
}

void printFileData(FileData *files, int size, int lsL){
    for(int i = 0; i < size; i++){
        if(lsL){
            printPermissions(files[i].statBuf.st_mode);
            printUserName(files[i].statBuf.st_uid);
            printGroupName(files[i].statBuf.st_gid);
            printFileSize(files[i].statBuf.st_size);
            printModTime(files[i].statBuf.st_mtime);
        }
        printf("%s\n", files[i].fileName);   
    }

}

int compareFunc(const void *p1, const void *p2){
    FileData *f1 = (FileData *) p1;
    FileData *f2 = (FileData *) p2;

    return strcasecmp(f1->fileName, f2->fileName);
}

int main(int argc, char **argv){
    int lsL = 0;
    if(argc == 2 && strcmp(argv[1], "-l") == 0){
        lsL = 1;
    }

    //make array to store words
    int capacity = 2;
    FileData *files = malloc(capacity * sizeof(FileData));
    int size = 0;

    //read names into array
    DIR *directory = opendir(".");
    struct dirent *dirEntry;
    while((dirEntry = readdir(directory)) != NULL){
        if(dirEntry->d_name[0]=='.'){
            continue;
        }
        if(size == capacity){
            capacity *= 2;
            files = realloc(files, capacity * sizeof(FileData));
        }
        files[size].fileName = dirEntry->d_name;
        stat(files[size].fileName, &files[size].statBuf);
        size++;
    }

    //sort array by fileName
    qsort(files, size - 1, sizeof(FileData), compareFunc);

    //print appropriate data
    printFileData(files, size, lsL);

    //free data
    free(files);
    free(directory);
}