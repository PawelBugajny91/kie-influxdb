/*
 * JSON to CSV concerter for Chronograph purposes
 * A part of SEC_COM project for European Space Agency
 * Copyright (c) 2020 Newind S.A.

 * File:   main.c
 * Author: Łukasz Pieczonka
 *
 * Created on 22 June 2020, 09:00
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Reads the line from file
int ReadLine(int fd, char *Token)
{
int i = 0;
ssize_t nread;
char c;

    do
    {
        nread = read(fd, &c, 1);
        if ((nread > 0) && (c != 13) && (c != 10)) Token[i++] = c;
        else if (nread > 0)
        {
            if (c == 10) break;
            nread = 0;
        }
    } while (nread > 0);
    Token[i] = 0;
    
    return (i == 0);
}

// Reads the nearest token from line
int ReadToken(char *inLine, char *TokenName, int Length)
{
int InPos = 0;
int OutPos = 0;
int Scanning = 0;

    while (InPos < Length)
    {
        if (Scanning)
        {
            if ((inLine[InPos] != '"') && (inLine[InPos] != ',')) TokenName[OutPos++] = inLine[InPos];
            else
            {
                TokenName[OutPos] = 0;
                return InPos;
            }
        }
        else
        {
            if (inLine[InPos] == '\"') Scanning = 1;
            else if ((inLine[InPos] != ' ') && (inLine[InPos] != 9))
            {
                Scanning = 1;
                TokenName[OutPos++] = inLine[InPos];
            }
        }
        InPos++;
    }
}

// Reads a parameter pair from Json file
int JSONReadParamValue(char *inLine, char *ParamName, char *ParamValue)
{
int l = strlen(inLine);
int p;

    p = ReadToken(inLine, ParamName, l);
    if (p++ < 0) return 1;

    while (p < l)
    {
        if (inLine[p] == ':')
        {
            p++;
            break;
        }
        else if ((inLine[p] == ' ') || (inLine[p] == 9)) p++;
        else return 2;
    }
    
    if (p >= l) return 3;
    
    p = ReadToken(inLine + p, ParamValue, l - p);
    if (p < 0) return 4;
    
    return 0;    
}

// Writes complete CSV file, based on given Json
void CSVWriteFromJson(int InFile, int OutFile, char* objectName)
{
char vLine[256];
char vParamName[256];
char vParamValue[256];
char vID[256];
char* ptr = NULL;
int vDuration;
long int vTime = 0 ;
char vStatus[256];
int R = 0;
int ParamScanned = -1;
char form[256]="com.myspace.cep_1.";
strcat(form, objectName);
    while (!R)
    {
        R = R || ReadLine(InFile, vLine);
        if (!R)
        {
            if ((ParamScanned <= 0) && (strstr(vLine, form) > 0))
            {
                ParamScanned = 3;
                continue;
            }
            else if (ParamScanned > 0)
            {
                R = JSONReadParamValue(vLine, vParamName, vParamValue);
                if (!R)
                {
                    if (strcmp(vParamName, "duration") == 0)
                    {
                        vDuration = atoi(vParamValue);
                        ParamScanned--;
                    }
                    else if (strcmp(vParamName, "time") == 0) 
                    {
                        vTime = atol(vParamValue);
                        ParamScanned--;
                    }
                    else if (strcmp(vParamName, "status") == 0) 
                    {
                        strcpy(vStatus, vParamValue);
                        ParamScanned--;
                    }
                }
                if (ParamScanned == 0)
                {
                    dprintf(OutFile, "%s,status=%s,type=null duration=0 %ld\n",objectName, vStatus, vTime - 1);
                    dprintf(OutFile, "%s,status=%s,type=start duration=%d %ld\n",objectName, vStatus, vDuration, vTime);
                    dprintf(OutFile, "%s,status=%s,type=end duration=0 %ld\n",objectName, vStatus, vTime + vDuration);
                    dprintf(OutFile, "%s,status=%s,type=null duration=0 %ld\n",objectName, vStatus, vTime + vDuration + 1);
                    dprintf(OutFile, "%s,status=%s,type=null value=0i %ld\n",objectName, vStatus, vTime - 1);
                    dprintf(OutFile, "%s,status=%s,type=start value=1i %ld\n",objectName, vStatus, vTime);
                    dprintf(OutFile, "%s,status=%s,type=end value=1i  %ld\n",objectName, vStatus, vTime + vDuration);
                    dprintf(OutFile, "%s,status=%s,type=null value=0i %ld\n",objectName, vStatus, vTime + vDuration + 1);
                }
            }
        }
    }
}

// The main application process
int main(int argc, char** argv)
{
char *ParamInFile, *ParamOutFile;
int R = 0;
int fd_in, fd_out;
char *objectName;
    
    // Initialization
    if ((argc < 3) || (argc > 4))
    {
        printf("CSV to JSON converter for Chronograph\n\r");
        printf("  for CEP GS Project @ European Space Agency\n\r");
        printf("  developed by: Łukasz Pieczonka\n\r\n\r");
        printf("Usage\n\r");
        printf("chronocsv2json <JSON Input File> <CSV Output File>\n\r");
        return 1;
    }

    ParamInFile     = argv[1];
    ParamOutFile    = argv[2];
    objectName      = malloc(strlen(argv[3])+1);
    objectName = argv[3];
    // Main action
    if (access(ParamOutFile, F_OK) != -1) unlink(ParamOutFile);
        
    fd_out = open(ParamOutFile, O_WRONLY | O_CREAT, 0666);
    if (fd_out != -1)
    {
        fd_in = open(ParamInFile, O_RDONLY);
        if (fd_in != -1)
        {
            CSVWriteFromJson(fd_in, fd_out, objectName);
        }
        else R = 2;

        close(fd_out);
    }
    else R = 3;
    
    return R;
}
