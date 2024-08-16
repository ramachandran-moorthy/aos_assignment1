#include<iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;
#define buffer_size 1000000
char buffer1[buffer_size];
char buffer2[buffer_size];

bool compareBuffers(char b1[], char b2[], int n)
{
    for(int i=0;i<n;i++)
    {
        if(b1[i]!=b2[n-i-1])
            return false;
    }
    return true;
}

bool checkReverse(int fd1, int fd2, long long file_size)
{
    int temp1=5, temp2=5;
    long long bytesRead = 0;
    int bytesToRead = 0;
    while(bytesRead<=file_size)
    {
        bytesToRead = (int)min((long long)buffer_size, file_size-bytesRead);
        lseek(fd2, file_size-bytesRead-bytesToRead, SEEK_SET);
        temp1 = read(fd1, buffer1, bytesToRead);
        temp2 = read(fd2, buffer2, bytesToRead);

        if(temp1==0 || temp2==0)
            return true;
        if(!compareBuffers(buffer1, buffer2, bytesToRead))
            return false;
        bytesRead += bytesToRead;
    }
    return false;
}

int main(int argc, char * argv[])
{
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDONLY);
    if(fd2==-1 || fd1==-1)
    {
        perror("Invalid input");
        return 0;
    }
    struct stat st1, st2, dir;
    bool flag = true;
    string str;
    fstat(fd1, &st1);
    fstat(fd2, &st2);
    int dirstat = stat(argv[3], &dir);
    if((dir.st_mode & S_IFMT)!= S_IFDIR)
    {
        str = "Invalid input: Not a directory\n";
        write(1, &str[0], str.length());
        return 0;
    }
    if((st1.st_mode & S_IFMT)!= S_IFREG)
    {
        str = "Invalid input: Incorrect new file path\n";
        write(1, &str[0], str.length());
        return 0;
    }
    if((st2.st_mode & S_IFMT)!= S_IFREG)
    {
        str = "Invalid input: Incorrect old file path\n";
        write(1, &str[0], str.length());
        return 0;
    }
    str = "Directory is created: ";
    str+=(!dirstat)? "Yes\n" : "No\n";
    write(1, &str[0], str.length());

    if(st1.st_size!=st2.st_size)
        flag = false;
    
    if(flag==false)
        str = "Whether file contents are reversed in new file: No\n";
    else
    {
        str = "Whether file contents are reversed in new file: ";
        str+= checkReverse(fd1, fd2, st1.st_size)?"Yes\n":"No\n";
        write(1, &str[0], str.length());
    }

    str = "Both file sizes are same: ";
    str += flag?"Yes\n\n":"No\n\n";
    write(1, &str[0], str.length());

    //New file permissions
    int x = ((st2.st_mode) & (S_IRUSR));
    str = "User has read permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IWUSR));
    str = "User has write permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IXUSR));
    str = "User has execute permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IRGRP));
    str = "Group has read permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IWGRP));
    str = "Group has write permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IXGRP));
    str = "Group has execute permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IROTH));
    str = "Other has read permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IWOTH));
    str = "Other has write permission on new file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st2.st_mode) & (S_IXOTH));
    str = "Other has execute permission on new file: ";
    str += x?"Yes\n\n":"No\n\n";
    write(1, &str[0], str.length());

    //Old file permissions
    x = ((st1.st_mode) & (S_IRUSR));
    str = "User has read permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IWUSR));
    str = "User has write permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IXUSR));
    str = "User has execute permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IRGRP));
    str = "Group has read permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IWGRP));
    str = "Group has write permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IXGRP));
    str = "Group has execute permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IROTH));
    str = "Other has read permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IWOTH));
    str = "Other has write permission on old file: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((st1.st_mode) & (S_IXOTH));
    str = "Other has execute permission on old file: ";
    str += x?"Yes\n\n":"No\n\n";
    write(1, &str[0], str.length());

    //Directory permissions
    x = ((dir.st_mode) & (S_IRUSR));
    str = "User has read permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IWUSR));
    str = "User has write permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IXUSR));
    str = "User has execute permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IRGRP));
    str = "Group has read permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IWGRP));
    str = "Group has write permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IXGRP));
    str = "Group has execute permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IROTH));
    str = "Other has read permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IWOTH));
    str = "Other has write permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    x = ((dir.st_mode) & (S_IXOTH));
    str = "Other has execute permission on directory: ";
    str += x?"Yes\n":"No\n";
    write(1, &str[0], str.length());

    return 0;
}