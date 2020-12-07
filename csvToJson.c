/*
 * CSV to JSON concerter for Chronograph purposes
 * A part of SEC_COM project for European Space Agency
 * Copyright (c) 2020 Newind S.A.
 */

/* 
 * File:   main.c
 * Author: Łukasz Pieczonka
 *
 * Created on 25 maja 2020, 11:15
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#define copy_buf_size 1024

// Converts text to UNIX timestamp
time_t Timestamp2Time(char *TimestampStr)
{
struct tm tm;

    strptime(TimestampStr, "%Y-%m-%dT%H:%M:%S", &tm);
    return mktime(&tm);
}

// Creates an empty file
int CreateFile(const char *FileName)
{
int fd;

    fd = open(FileName, O_CREAT | O_WRONLY, 0666);
    if (fd != -1)
    {
        close(fd);
        return 0;
    }
    else return 1;
}

int skipLine(int fd)
{
    int R=0;
    char c= 'c';

    while(c != 10)
    {
        R = read(fd, &c, 1);
        if(R == 0 )
        return R;
    }
    return R;
}
// Reads the nearest token from file
int ReadToken(int fd, char *Token, char Delimiter)
{
int i = 0;
ssize_t nread;
char c;

    do
    {
        nread = read(fd, &c, 1);

        if ((nread > 0) && (c != 13) && (c != 10) && (c != Delimiter)) Token[i++] = c;
        else if (nread > 0)
        {
            if (c == 10) continue;
            nread = 0;
        }
    } while (nread > 0);
    Token[i] = 0;
   // printf("Token : %s  \n", Token);
    return (i == 0);
}

// Reads and decodes complete header from CSV file
int skipHeader(int fd)
{
int R = 0;
    char c;
    R = skipLine(fd);
    if (R != 0) R = skipLine(fd);
    if (R != 0) R = skipLine(fd);
    //if (R != 0) R = skipLine(fd);
    return R;
}
int readType(int fd)
{
    int type = 0;
    ssize_t nread;
    char c;
    do
    {
        nread = read(fd, &c, 1);

        if ((nread > 0) && (c == ','))
        {
            type++;
        }
    } while (c!=10);
    return type;
}
// Writes JSON data based on CSV file
int JsonWriteFromCSV(int csv_fd, int json_fd, char* FieldMesurmend,  int type)
{
int R = 0;
char FieldTag[256];
char FieldName[256];
char FieldTime[256];
char FieldValue[256];
char Empty[256];
char TimeName[] = "time";
time_t Time;
    
    while (R == 0)
    {
        R = ReadToken(csv_fd, Empty, '-');
        if (R == 0) R = ReadToken(csv_fd, Empty, ',');
        if (R == 0) R = ReadToken(csv_fd, Empty, ',');

        if (R == 0) R = ReadToken(csv_fd, FieldTime, ',');
        if (R == 0) R = ReadToken(csv_fd, FieldValue, ',');
        if (R == 0) R = ReadToken(csv_fd, FieldName, ',');
        if (R == 0) R = ReadToken(csv_fd, Empty, 10);
        
        if (R == 0)
        {
            Time = Timestamp2Time(FieldTime);
            dprintf(json_fd, "\"%s\" : ", FieldMesurmend);
            dprintf(json_fd, "{");
            dprintf(json_fd, "\"%s\" : %ld,", TimeName, (long int)Time*1000);
            dprintf(json_fd, " \"%s\" : %s", FieldName, FieldValue);
            dprintf(json_fd, " } \n");
        }

    }
}

// The main application process
int main(int argc, char** argv)
{
char *ParamInFile, *ParamOutFile, *ParamJSONPrefix, *ParamJSONSuffix, *ParamTimeZone;
int R = 0;
int fd_in, fd_out;
char *FieldMesurmend;
    
    // Initialization
    if ((argc < 3) || (argc > 4))
    {
        printf("CSV to JSON converter for Chronograph\n\r");
        printf("  for CEP GS Project @ European Space Agency\n\r");
        printf("  developed by: Łukasz Pieczonka\n\r\n\r");
        printf("Usage\n\r");
        printf("chronocsv2json <CSV File> <JSON Output File> <JSON Prefix File> <JSON Suffix File> [Time Zone]\n\r");
        return 1;
    }

    ParamInFile     = argv[1];
    ParamOutFile    = argv[2];
    FieldMesurmend  = argv[3];
    if (argc == 4) ParamTimeZone = argv[4]; else ParamTimeZone = NULL;

    // The main application process
    if (argc == 4)
    {
        setenv("TZ", ParamTimeZone, 1);
        tzset();
    }
   
    if (access(ParamOutFile, F_OK) != -1) unlink(ParamOutFile);
        
    if (!CreateFile(ParamOutFile))
    {
        fd_out = open(ParamOutFile, O_WRONLY|O_APPEND);
        if (fd_out != -1)
        {
            fd_in = open(ParamInFile, O_RDONLY);
            if (fd_in != -1)
            {
                R = R || skipHeader(fd_in);
                int type = readType(fd_in);
                JsonWriteFromCSV(fd_in, fd_out, FieldMesurmend, type);               
            }
            else R = 1;
            
            close(fd_out);
        }
        else R = 1;
    }
    else R = 1;
    
    return R;
}

