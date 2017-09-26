#include "FileMonitor.h"

bool FileMonitor::Init()
{
    fd = inotify_init();
    if (fd < 0)
    {
        std::cout << "Error failed to initialize inotify" << std::endl;
        return false;
    }
    
    wd = inotify_add_watch(fd, filePath.c_str(), IN_MODIFY);
    if (!SetNonBlocking())
    {
        std::cout << "Error failed to set fd to non-blocking" << std::endl;
        return false;
    }
    std::cout << "Monitoring " << filePath << " for modifications." << std::endl;
    return true;
}

/** Be super lazy and just assume any successful read call means the file was modified. **/
bool FileMonitor::ShouldReload()
{
    int i = 0;
    int length = 0;
    memset(buffer, 0, BUF_LEN);
    length = read(fd, buffer, BUF_LEN); 
    if (length < 0)
    {
        return false;
    }
    return true;
}   

bool FileMonitor::SetNonBlocking()
{
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    return (ret != -1) ? true : false;
}

FileMonitor::~FileMonitor()
{
    inotify_rm_watch(fd, wd);
    close(fd);
}