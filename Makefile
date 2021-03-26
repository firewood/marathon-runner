.PHONY: default server build-tools run-windows run

ifeq ($(OS),Windows_NT)
run-windows: main.exe
	ruby run.rb 0001
endif

run: a.out
	./run.rb 0001

default: a.out
	./a.out < in.txt > out.txt
	cd visualizer && cargo run --release --bin vis ../in.txt ../out.txt

a.out: main.cpp
	g++ -DREPORT -g -std=c++14 $< -o $@

main.exe: main.cpp
	cl /O2 /Ot /Oi /GS /Gd /GL /Gy /std:c++17 /D "REPORT" $<

server:
	yarn run serve

