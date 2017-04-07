make

echo 'Starting tests'

echo 's5e50.10'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s5e50.in 10 >! s5e50i10.out
  echo 'diff'
  diff s5e50i10.out ../testcases/s5e50.10.out
done

echo 's20e400.500'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s20e400.in 500 >! s20e400i500.out
  echo 'diff'
  diff s20e400i500.out ../testcases/s20e400.500.out
done

echo 's50e5k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s50e5k.in 1000 >! s50e5ki1000.out
  echo 'diff'
  diff s50e5ki1000.out ../testcases/s50e5k.1000.out
done

echo 's150e10k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s150e10k.in 1000 >! s150e10ki1000.out
  echo 'diff'
  diff s150e10ki1000.out ../testcases/s150e10k.1000.out
done

echo 's200e50k.1000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s200e50k.in 1000 >! s200e50ki1000.out
  echo 'diff'
  diff s200e50ki1000.out ../testcases/s200e50k.1000.out
done

echo 's500e300k.2000'
for i in {1..8}
do
  echo 'OMP_NUM_THREADS=' + $i
  OMP_NUM_THREADS=$i
  /usr/bin/time -v ./project ../testcases/s500e300k.in 2000 >! s500e300ki2000.out
  echo 'diff'
  diff s500e300ki2000.out ../testcases/s500e300k.2000.out
done
