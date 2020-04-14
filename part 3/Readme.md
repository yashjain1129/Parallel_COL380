### Usage

- Before compilation (for windows subsystem for linux)
```
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```

- Compile
```
make -f Makefile.pagerank
```

- Run
```
mpirun -np 8 wordfreq file1 file2
```
