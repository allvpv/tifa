# :sparkles: `tifa` :sparkles:
### :sparkles: `tifa` :sparkles: exposes the musl libc with zero-cost c++ interfaces

templates and v-tables often come in handy. however, the standard c++ library,
with an emphasis on platform-independence, without the direct access to system
calls, with invisible dynamic allocations going on behind the scenes, is not
very well suited for system programming or embedded development

`tifa` is nothing more than extensively documented header-only wrapper over musl
libc with a lot of handy interfaces for c++ programmers. with the help of `tifa`
you can avoid linking `libstdc++` or `libc++` altogether, while still enjoying
convinient c++ features

`tifa` encourages direct memory-management. it does not compromise on security,
though. with the help of template parameters, `void*` pointers are not needed.
optional bound-checking and other safety checks are also implemented where
possible and enabled by default in debug mode

:construction: the description above is an idea :construction:
right now, almost nothing is implemented. anyone is more than welcome to
contribute

- [ ] memory mappings
- [ ] input/output
- [ ] pipes and fifos
- [ ] processes and signals
- [ ] posix shared memory
- [ ] system v shared memory
- [ ] filesystem access, `inotify`
- [ ] users, groups, access control
- [ ] pthreads
- [ ] posix semaphores
- [ ] sockets and networking



