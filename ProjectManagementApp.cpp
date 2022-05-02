/******************************************************************************
                      Project Management Application
*******************************************************************************/

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

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
    ONHOLD,
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
            TaskStatus pStatus = TaskStatus::NEW,
            Task *pLinkedTask = nullptr,
            User *pUser = nullptr,
            Resource *pResource = nullptr);
            
        // Setters for assigning start and end date
        void setStartDate(Clock pStartDate);
        void setEndDate(Clock pEndDate);
        
        // Getters for start and end date
        Clock getStartDate();
        Clock getEndDate();
        
        // Sets the status of a task
        void setStatus(TaskStatus pStatus);
        // returns status of a task
        TaskStatus getStatus();
        
        // Links another task to the current Task
        // The link can be a blocking link or relates to link
        void linkTask(Task *pTask, LinkType pType);
        
        // Assigns an user to this task
        void assignUser(User *pUser);
        
        // Assigns a Resource to this task
        void assignResource(Resource *pResource);
        
    private:
        std::string mName;
        double mTimeToComple;
        Clock mStartTime;
        Clock mEndTime;
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
        Project(std::string pName, Clock pStartTime, Clock pEndTime);
        
        // Checks if the project can be completed by given pEndDate
        // If all the tasks can be completed within the specified date adhering
        // to their completion time, then project can be completed.
        bool canCompleteBy(Clock pEndDate);
        
        // Setters for start and end date
        void setStartDate(Clock pStartDate);
        void setEndDate(Clock pEndDate);\
        
        // Getters for start and end date
        Clock getStartDate();
        Clock getEndDate();
        
        // Creates a task with specified name and time to complete the task
        // The created task is added to list of tasks (mTasks)
        void CreateTask(std::string pName, double pTimeToComplete);
        
        // Finds a task by its name
        Task *getTask(std::string pName);

        
    private:
        std::string mName;
        Clock mStartTime;
        Clock mEndTime;
        std::vector<Task> mTasks;
};
