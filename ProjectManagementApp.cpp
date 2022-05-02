/******************************************************************************
                      Project Management Application
*******************************************************************************/

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

// User details
class User
{
    public:
        User(std::string pId, std::string pName);
    private:
        std::string mId;
        std::string mName;
};

// Resource for tasks
class Resource
{
    public:
        Resource();
        std::string getId();
    private:
        std::string mId;
};

// Type of relation/dependency between tasks
enum LinkType
{
    BLOCKEDBY = 0,
    RELATESTO
};

class Task;
class TaskLink
{
    public:
        TaskLink(Task pTask, LinkType pLinkType);
    private:
        Task *mTask;
        LinkType mLinkType;
};

// Progression Status of Task
enum TaskStatus
{
    NEW = 0,
    TODO,
    INPROGRESS,
    DONE
};

// A task has target completion time set, link to another task,
// an User and resource can be assigned to the task
class Task
{
    public:
        Task(
            std::string pName,
            double pTimeToComplete,
            TaskStatus pStatus,
            Task *pLinkedTask,
            User *pUser,
            Resource &pResource);
    
    private:
        std::string mName;
        std::chrono::time_point<std::chrono::system_clock> mStartTime;
        std::chrono::time_point<std::chrono::system_clock> mEndTime;
        TaskStatus mStatus;
        Task *mLinkedTask;
        User *mUser;
        Resource *mResource;
};

// A Project is a set of Tasks. A project's completion time depends on its tasks'
// completion time.
class Project
{
    public:
        bool canCompleteBy(std::chrono::time_point<std::chrono::system_clock> pEndDate);
    
    private:
        std::string mName;
        std::vector<Task> mTasks;
        std::chrono::time_point<std::chrono::system_clock> mStartTime;
        std::chrono::time_point<std::chrono::system_clock> mEndTime;
};
