# Garbage collector
Its a Library build in and for C-language programs to track the objects allocated by an application and report the Leaked objects.

In order to use it, you have to first register structures used by your application with it.

This application is built on Linux machine, so it would be advised to use Linux.

---
**PHASE 1 COMPLETE**:

* [x] Defined structure database and registered applications structures with the garbage collector library.

To test Phase1, type the following command in the terminal.
```bash
gcc -g -c gc.c gc.o
gcc -g -c app.c app.o
gcc -g -o exe gc.o app.o
./exe
```