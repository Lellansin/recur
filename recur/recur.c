#include <stdio.h>
#include <windows.h>

void help();

int checkSuffix(char *name, char *suffix)
{
    char *p = suffix;
    char *ext = strrchr(name, '.');
    int len, end;

    if (strcmp(suffix, "*") == 0)
    {
        return 1;
    }

    while (*p)
    {
        end = *(p + 1) == '\0';
        if (*p == ',' || end)
        {
            len = p - suffix;

            if (end)
                len++;

            if (strncmp(ext + 1, suffix, len) == 0)
                return 1;

            suffix = p + 1;
        }
        p++;
    }

    return 0;
}

int isDirectory(WIN32_FIND_DATA file)
{
    return file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

char *getFilepath(char *dest, char *dir, char *name)
{
    strcpy(dest, dir);
    strcat(dest, "\\");
    strcat(dest, name);
    return dest;
}

void recur(char *src, char *command, char *suffix)
{
    char *name;
    char path[MAX_PATH];
    char dir[MAX_PATH];
    char buf[MAX_PATH];
    WIN32_FIND_DATA file;
    HANDLE hDir;

    getFilepath(dir, src, "*.*");
    hDir = FindFirstFile(dir, &file);

    if (hDir == INVALID_HANDLE_VALUE)
        return;

    while (FindNextFile(hDir, &file))
    {
        name = file.cFileName;
        if (isDirectory(file))
        {
            if (name[0] != '.')
            {
                getFilepath(path, src, name);
                recur(path, command, suffix);
            }
        }
        else if (checkSuffix(name, suffix))
        {
            getFilepath(path, src, name);
            wsprintf(buf, command, path);
            system(buf);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        help();
        return 0;
    }

    recur(argv[1], argv[2], argv[3]);

    printf("\nRecur over\n");
    return 0;
}

void help()
{
    printf("Usage:  recur <dir> <cmd> <suffix>\n" \
           "        recur E:\\test \"echo %%s\" c,cpp \n"
           "   dir  Directorys that you deal with.\n" \
           "        It can be like E:\\test E:\\test2 \n" \
           "   cmd  command you want run for example:\n" \
           "        1) type %%s \n"
           "        2) echo %%s \n"
           "        3) your.bat %%s \n"
           " suffix filter suffix, for example:\n"
           "        c,cpp \n"
          );
}