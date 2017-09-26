#ifndef FILE_MONITOR_H
#define FILE_MONITOR_H

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

class FileMonitor
{
public:
    FileMonitor(std::string& filePath) : filePath(filePath), fd(0), wd(0) {}
    
    FileMonitor(const FileMonitor &other) : filePath(other.filePath), fd(other.fd), wd(other.wd) {}

    bool Init();

    bool ShouldReload();

    ~FileMonitor();

private:
    bool SetNonBlocking();

private:
    std::string& filePath;

    int fd;
    int wd;
    char buffer[BUF_LEN];

};

#endif // FILE_MONITOR_H