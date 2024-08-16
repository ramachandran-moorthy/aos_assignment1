#include<iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include<time.h>
#include<string.h>
using namespace std;
#define buffer_size 2
char buffer[buffer_size];

void writeToTerminal(string s)
{
    char * ptr = &s[0];
    write(1, ptr, s.size());
}

void reverse(char buffer[], int n)
{
    char temp;
    for(int i=0;i<n/2;i++)
    {
        temp = buffer[i];
        buffer[i] = buffer[n-i-1];
        buffer[n-i-1] = temp;
    }
}

bool checkInput(int argc, char *argv[])
{
    long long a, b;
    if(argc!=3 && argc!=5)
    {
        writeToTerminal("Invalid input : Please check number of arguments\n");
        return false;
    }
    if(argc==3)
    {
        try
        {
            int arg2 = atoi(argv[2]);
            if(arg2!=0)
            {
                writeToTerminal("Invalid input : Please check value of argument 3\n");
                return false;
            }
        }
        catch(exception& e)
        {
            writeToTerminal("Invalid input : Please check value of argument 3\n");
            return false;
        }
    }
    if(argc==5)
    {
        try
        {
            int arg2 = atoi(argv[2]);
            if(arg2!=1)
            {
                writeToTerminal("Invalid input : Please check value of arguments\n");
                return false;
            }
        }
        catch(exception& e)
        {
            writeToTerminal("Invalid input : Please check value of argument 3\n");
            return false;
        }
        try
        {
            a = atoll(argv[3]);
            b = atoll(argv[4]);
            if(a>b)
            {
                writeToTerminal("Invalid input : Start value is greater than End value\n");
                return false;
            }
            if(a<0 || b<0)
            {
                writeToTerminal("Invalid input : Start and end indices cannot be negative\n");
                return false;
            }
        }
        catch(exception& e)
        {
            writeToTerminal("Invalid input : Please check value of arguments\n");
            return false;
        }
    }
    return true;
}

void readWrite(int fd_read, int fd_write, long long startIndex, long long endIndex, 
                long long completed, long long file_size, bool flag)
{
    int progress = 0, temp1=5, temp2=5;
    long long bytesRead = 0;
    while(completed<=file_size)
    {
        //cout << completed << "\n";
        progress = (completed*100/file_size);
        writeToTerminal("\rProgress: ( " + to_string(progress) + "% )");
        temp1 = read(fd_read, buffer, (int)min((long long)buffer_size, endIndex-(startIndex+bytesRead)));

        completed +=temp1;
        bytesRead+=temp1;

        if(temp1==0)
            break;
        if(temp1==-1)
        {
            completed+=1;
            bytesRead+=1;
            writeToTerminal("Invalid input : Please check value of arguments\n");
            break;
        }
        if(flag==true)
        {
            reverse(buffer, temp1);
            lseek(fd_write, endIndex-bytesRead, SEEK_SET);
        }
        temp2 = write(fd_write, buffer, temp1);
    }
    lseek(fd_write, endIndex, SEEK_SET);
}

int main(int argc, char *argv[])
{
    time_t start_time = time(NULL);
    int mode;
    if(!checkInput(argc, argv))
        return 0;
    
    string valTest = argv[2];
    try
    {
        mode = atoi(argv[2]);
        if((to_string(mode)).length()!=valTest.length())
        {
            writeToTerminal("Invalid input: Incorrect mode\n");
            return 0;
        }
    }
    catch(exception& e)
    {
        perror("Invalid input");
        return 0;
    }
    struct stat st;
    int fd_read = open(argv[1], O_RDONLY);
    if(fd_read==-1)
    {
        perror("Invalid input");
        return 0;
    }
    int folderstatus = mkdir("Assignment1", S_IRWXU);
    string ipfile = argv[1];
    size_t slashpos = (ipfile).find_last_of('/');
    if(slashpos!=string::npos)
        ipfile = ipfile.substr(slashpos+1);
    string opfilename = "Assignment1/" + to_string(mode) + "_" + ipfile;
    char *ptr1 = &opfilename[0];
    int fd_write = creat(ptr1, O_WRONLY);

    fstat(fd_read, &st);
    if((st.st_mode & S_IFMT)!= S_IFREG)
    {
        writeToTerminal("Invalid input: Incorrect file path\n");
        return 0;
    }
    long long file_size = (long long) st.st_size;
    long long completed = 0;
    fchmod(fd_write, S_IRUSR | S_IWUSR);

    if(mode==0)
    {
        readWrite(fd_read, fd_write, 0, file_size, 0, file_size, true);
    }
    if(mode==1)
    {
        int startIndex, endIndex;
        bool flag = true;
        try
        {
            startIndex = atoi(argv[3]);
            endIndex = atoi(argv[4]);
            valTest = argv[3];
            if((to_string(startIndex)).length()!=valTest.length())
            {
                writeToTerminal("Invalid input: Invalid start index\n");
                return 0;
            }
            valTest = argv[4];
            if((to_string(startIndex)).length()!=valTest.length())
            {
                writeToTerminal("Invalid input: Invalid end index\n");
                return 0;
            }
            if(startIndex>file_size)
            {
                writeToTerminal("Invalid input : Invalid start index\n");
                return 0;
            }
            if(endIndex>file_size)
            {
                writeToTerminal("Invalid input : Invalid end index\n");
                return 0;
            }
        }
        catch(exception& e)
        {
            writeToTerminal("Invalid input : Issue with start and end indices\n");
            return 0;
        }
        readWrite(fd_read, fd_write, 0, startIndex, 0, file_size, true);
        completed=startIndex;
        readWrite(fd_read, fd_write, startIndex, endIndex+1, completed, file_size, false);
        completed = endIndex+1;
        readWrite(fd_read, fd_write, endIndex+1, file_size, completed, file_size, true);
        completed = file_size;
        
    }
    close(fd_read);
    close(fd_write);
    time_t time_taken = time(NULL) - start_time;
    writeToTerminal("\nTime taken = " + to_string(time_taken) + " sec\n");
    return 0;
}