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
        
        // Returns true if User is free
        bool isFree();
        
        // Returns the number of tasks assigned to this user
        int getTasksAssined();
        
        // Increases task count when task is assinged to this user
        void increaseTaskCount();
        
        // Decrease task count from the user
        void decreaseTaskCount();
        
        // Adds workload (time to complete) to this user
        void addWorkLoad();
        
        // Decreases workload from the user
        void removeWorkLoad();
        
        int getTaskCount();
        double getWorkLoad();
        
    private:
        std::string mId;
        std::string mName;
        int mTasks;
        double mWorkLoad;
};

// Resource for tasks
class Resource
{
    public:
        Resource(std::string pId);
        std::string getId();
        
        // Sets mIsAssigned to true;
        void occupyResource();
        
        // Sets mIsAssigned to false;
        void freeResource();
        
        // Returns true if Resource is free
        bool isFree();
        
    private:
        std::string mId;
        bool mIsAssigned;
};

// Working Site as Resource
class Site : public Resource
{
    public:
        Site(std::string pId);
};

// Cost as Resource
class Cost : public Resource
{
    public:
        Cost(std::string pId);

};

// Material as Resource
class Material : public Resource
{
    public:
        Material(std::string pId);
};

enum ResourceType
{
    SITE =0,
    COST,
    MATERIAL
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
        
        // Removes linked task
        void removeLinkedTask(Task *pLinkedTask);
        
        // Retruns all the linked tasks with type BLOCKEDBY
        std::vector<Task *> getBlockingTasks();
        
        // Assigns an user to this task, adds task's "time to complete" to User
        // and increases task count for User, returns true if successful
        // returns false if no more tasks can be assigned to user
        bool assignUser(User *pUser);
        
        // Unassign User from task, removes task's "time to complete" from User
        // and decreases task count from User
        void unAssignUser();
        
        // Assigns a Resource to this task by calling Resource::occupyResource,
        //returns true if successful, returns false if resoure is not available
        bool assignResource(Resource *pResource);
        
        // Unassign Resource from task
        void unAssignResource();
        
        // Changes the progress percentage of this task
        void setProgress(double pProgess);
        double getProgress();
        
        // Returns true if getStatus() returns DONE
        bool isCompleted();
        
        // Returns true if ratio between "time from now till pEndDate" and
        // "remaining progress" is same. canCompleteBy() of all the blocking
        // tasks are called recursively
        bool canCompleteBy(Clock pEndDate);

    private:
        std::string mName;
        Clock mStartTime;
        Clock mEndTime;
        Status mStatus;
        double mProgress;
        std::vector<Task *>mLinkedTasks;
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
        
        // Returns Users that are not assinged to any task
        std::vector<User*> getFreeUsers();
        
        // Removes user from mUsers and deletes it
        void deleteUser(std::string pId);
        
    private:
        std::vector<User*> mUsers;
};

// Creates and manages Resources
class ResourceManager
{
    public:
        ResourceManager();
        
        // Creates a Resource of ResourceType and adds to mResources
        Resource *createResource(std::string pId, ResourceType pType);
        
        // Finds Resource by its name
        Resource *getResource(std::string pName);
        
        // Returns Resources that are not assgined to any task
        std::vector<Resource*> getFreeResources();
        
        // Removes Resource from mResources and deletes its
        void deleteResource(std::string pResource);
        
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
        Task *CreateTask(std::string pName, double pTimeToComplete);
        
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
        // retruns true if successful otherwise false
        bool assignUserToTask(Task *pTask, std::string pUser);
        
        // Gets the Resource from ResourceManager and assigns Resource to a Task,
        // calls Task::assignResource, retruns true if successful otherwise false
        bool assignResourceToTask(Task *pTask, std::string pResource);
        
        // Calculates and returns progress percentage of this Project
        // Progress of Project is calculated from progress of all the tasks
        double getProgress();
        
    private:
        std::string mName;
        Clock mStartTime;
        Clock mEndTime;
        Status mStatus;
        double mProgress;
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
