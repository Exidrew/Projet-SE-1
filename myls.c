#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <linux/kdev_t.h>
#include <pwd.h>
#include <grp.h>

#include "headers/error.h"
#include "headers/myls.h"


#define true 1
#define false 0

char* permissions(char *file){
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    if(stat(file, &st) == 0){
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
        return modeval;     
    }
    else{
        return strerror(errno);
    }   
}


int main(int argc, char* argv[]) {
    struct dirent *dir;
    struct stat st;
    int totalSize = 0;
    // opendir() renvoie un pointeur de type DIR. 
    DIR *d = opendir("."); 
    printf("ARGC : %d\n", argc);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_name[0] != '.'){
                printf("%s\t", permissions(dir->d_name));
                if(stat(dir->d_name, &st) == 0){
                    struct passwd *pw = getpwuid(st.st_uid);
                    struct group *gr = getgrgid(st.st_gid);
                    printf("%d\t", dir->d_type);
                    if(pw != 0 && gr != 0){
                        printf("%s\t",pw->pw_name);
                        printf("%s\t",gr->gr_name);
                    }
                    totalSize += st.st_size;
                    printf("%ld\t", st.st_size);
                    char month[5];
                    strftime(month, 5, "%m", localtime(&st.st_mtime));
                    char time[50];
                    strftime(time, 50, "%d %H:%M", localtime(&st.st_mtime));
                    printf("%s\t",months[atoi(month) - 1]);
                    printf("%s\t", time);
                }
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    printf("total %d", totalSize / 1000);
    return 0;
}
