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
        std::string getId();
        std::string getName();
        
    private:
        std::string mId;
        std::string mName;
};

// Resource for tasks
class Resource
{
    public:
        Resource(std::string pId);
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
enum Status
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
            Status pStatus = Status::NEW,
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
        void setStatus(Status pStatus);
        // returns status of a task
        Status getStatus();
        
        // Links another task to the current Task
        // The link can be a blocking link or relates to link
        void linkTask(Task *pTask, LinkType pType);
        
        // Assigns an user to this task
        void assignUser(User *pUser);
        
        // Unassign User from task
        void unAssignUser();
        
        // Assigns a Resource to this task
        void assignResource(Resource *pResource);
        
        // Unassign Resource from task
        void unAssignResource();
        
    private:
        std::string mName;
        double mTimeToComple;
        Clock mStartTime;
        Clock mEndTime;
        Status mStatus;
        Task *mLinkedTask;
        User *mUser;
        Resource *mResource;
};

// Creates and manages Users
class UserManager
{
    public:
        UserManager();
        
        // Creates an User and adds to mUsers
        User *createUser(std::string pId, std::string pName);

        // Finds user by Id and returns
        User *getUser(std::string pId);
        
    private:
        std::vector<User*> mUsers;
};

// Creates and manages Resources
class ResourceManager
{
    public:
        ResourceManager();
        
        // Creates a Resource and adds to mResources
        Resource *createResource(std::string pId);
        
        // Finds Resource by its name
        Resource *getResource(std::string pName);
        
    private:
        std::vector<Resource*> mResources;
};

// A Project is a set of Tasks. A project's completion time depends on its tasks'
// completion time.
class Project
{
    public:
        Project(
            std::string pName,
            Clock pStartTime,
            Clock pEndTime,
            ResourceManager *pResourceManager,
            UserManager *pUserManager,
            Status pStatus = Status::NEW);
        
        // Checks if the project can be completed by given pEndDate
        // If all the tasks can be completed within the specified date adhering
        // to their completion time, then project can be completed.
        bool canCompleteBy(Clock pEndDate);
        
        // Setters for start and end date
        void setStartDate(Clock pStartDate);
        void setEndDate(Clock pEndDate);
        
        // Getters for start and end date
        Clock getStartDate();
        Clock getEndDate();
        
        // Sets the status of a Project
        void setStatus(Status pStatus);
        
        // returns status of a Project
        Status getStatus();
        
        // Creates a task with specified name and time to complete the task
        // The created task is added to list of tasks (mTasks)
        void CreateTask(std::string pName, double pTimeToComplete);
        
        // Finds a task by its name
        Task *getTask(std::string pName);
        
        // methods for changing Task status, calls Task::setStatus(Status pStatus);
        void startTask(Task *pTask);
        void undoStartTask(Task *pTask);
        void holdOnTask(Task *pTask);
        void completeTask(Task *pTask);
        
        // Adds one task as bloking task or "related to" task for another
        void linkTasks(Task *pTask, Task *pDependentTask, LinkType pType);
        
        // checks to avoid cyclic dependency
        bool isCyclicDependencyExists(Task *pTask, Task *pDependentTask);
        
        // Gets the user from UserManager and assigns User to a Task, calls Task::assignUser
        void assignUserToTask(Task *pTask, std::string pUser);
        
        // Gets the Resource from ResourceManager and assigns Resource to a Task,
        // calls Task::assignResource
        void assignResourceToTask(Task *pTask, std::string pResource);
        
    private:
        std::string mName;
        Clock mStartTime;
        Clock mEndTime;
        Status mStatus;
        std::vector<Task*> mTasks;
        ResourceManager *mResourceManager;
        UserManager *mUserManager;
};

// Creates and manages Project
class ProjectManager
{
    public:
        // Initializes mResourceManager, mUserManager
        ProjectManager();
        
        // Creates a Project by passing mResourceManager and mUserManager
        // and adds to mProjects
        Project createProject(
            std::string pName,
            Clock pStartTime,
            Clock pEndTime);
        
        // Finds Project by its name
        Project getProject(std::string pName);
        
        // methods for changing Project status calls Project::setStatus()
        void startProject(Project *pProject);
        void undoStartProject(Project *pProject);
        void completeProject(Project *pProject);
        
    private:
        std::vector<Project> mProjects;
        ResourceManager *mResourceManager;
        UserManager *mUserManager;
};
