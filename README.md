# ProjectManagementApp
1. UserManager class creates, deletes, returns User object
2. User class stores user information, number of tasks assinged and their workload.
3. ResourceManager class creates, deletes, returns Resource Object of ResourceType.
4. ResourceType can be Site, Cost, Material.
5. Resource class stores resouce information such as id, assignment status
6. Site, Cost, Material classes inherit from Resource class.
7. ProjectManager class holds objects of UserManager, ResourceManager, and it creates, retrieves multiple Project objects.
8. ProjectManager class retrieves User & Resource objects from UserManager & ResourceManager respectively.
9. Project created by ProjectManager has its start and end date, progression status.
10. Project can create multiple Task objects, change progression status of Task objects, and assigns User & Resource to Task objects
11. Task class holds, progrssion status, start, end date, linked Task
12. A Task object can be linked to another Task as blocking or "relates to" task
13. The progression status of Project & Task can be new, todo, inprogress, onhold, done.
