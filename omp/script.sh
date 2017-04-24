make

echo 'Starting tests'

echo 's5e50.10'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s5e50.in 10
  echo 'diff'
  diff result.out s5e50.10.out
done

echo 's20e400.500'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s20e400.in 500
  echo 'diff'
  diff result.out s20e400.500.out
done

echo 's50e5k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s50e5k.in 300
  echo 'diff'
  diff result.out s50e5k.300.out
done

echo 's150e10k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s150e10k.in 1000
  echo 'diff'
  diff result.out s150e10k.1000.out
done

echo 's200e50k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s200e50k.in 1000
  echo 'diff'
  diff result.out s200e50k.1000.out
done

echo 's500e300k.2000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  export OMP_NUM_THREADS=$i
  ./life3d-omp s500e300k.in 2000
  echo 'diff'
  diff result.out s500e300k.2000.out
done
