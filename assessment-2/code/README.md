# Parallel Design Patterns
## Squirrel infection simulation

Use `make` to build. Requires MPI compilers. 

Run the simlulation with the command

```
mpirun -n 200 /bin/main $total_squirrels $live_squirrels, $infected_squirrels, $number_of_months
```
For example

```
mpirun -n 200 ./bin/main 200 24 4 24
```

If you're on a system with PBS, you can instead just `qsub run.pbs`
