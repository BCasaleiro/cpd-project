make

echo 'Starting tests'

echo 's5e50.10'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s5e50.in 10&>results.out
  echo 'diff'
  diff result.out ../testcases/s5e50.10.out
done

echo 's20e400.500'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s20e400.in 500&>results.out
  echo 'diff'
  diff result.out ../testcases/s20e400.500.out
done

echo 's50e5k.1000'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s50e5k.in 300&>results.out
  echo 'diff'
  diff result.out ../testcases/s50e5k.300.out
done

echo 's150e10k.1000'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s150e10k.in 1000&>results.out
  echo 'diff'
  diff result.out ../testcases/s150e10k.1000.out
done

echo 's200e50k.1000'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s200e50k.in 1000&>results.out
  ./life3d-omp s200e50k.in 1000
  echo 'diff'
  diff result.out ../testcases/s200e50k.1000.out
done

echo 's500e300k.2000'
for i in {1, 2, 4, 8}
do
  echo 'np=' + $i
  mpirun -np $i ./life3d ../testcases/s500e300k.in 2000&>results.out
  echo 'diff'
  diff result.out ../testcases/s500e300k.2000.out
done
