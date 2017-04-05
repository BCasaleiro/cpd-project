all: project.c octree.c
	gcc -o project project.c octree.c
	##gprof project gmon.out > prof_output

debug: project.c octree.c
	gcc -pg -o project project.c octree.c

mem-check: project.c octree.c
	gcc -O0 -g -o project project.c octree.c
