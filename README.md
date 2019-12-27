# Garbage collector
Its a Library build in and for C-language programs to track the objects allocated by an application and report the Leaked objects.

In order to use it, you have to first register structures used by your application with it.

This application is built on Linux machine, so it would be advised to use Linux.

---
**PHASE 1 COMPLETE**:

* [x] Defined structure database and registered applications structures with the garbage collector library.

To test Phase1, type the following commands in the terminal.
```bash
gcc -g -c gc.c gc.o
gcc -g -c app.c app.o
gcc -g -o exe gc.o app.o
./exe
```
---
**Phase 2 COMPLETE**

* [x] Defined Object database. Now, application can register objects for the structures using modified calloc function (xcalloc).

To test Phase2, type the following commands in the terminal.
```bash
gcc -g -c gc.c gc.o
gcc -g -c app.c app.o
gcc -g -o exe gc.o app.o
./exe
```
---
**Phase 3 ONGOING**

* [x] Define is_Root flag.
* [x] Add function to mark objects as root.
* [x] Modify functions to add this functionality.
* [x] Add is_visited flag for each object.
* [x] Add function to Initialize all objects as unvisited.
* [x] Add function to find next root object.
* [x] Add function to explore all the object reachable from current root object.
* [x] Add function to explore the reachable objects from a root node.
* [ ] Add function to Report leaked objects.