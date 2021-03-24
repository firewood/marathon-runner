.PHONY: default server build-tools

run: a.out
	./run.rb 0001

default: a.out
	./a.out < in.txt > out.txt
	cd visualizer && cargo run --release --bin vis ../in.txt ../out.txt

a.out: main.cpp
	g++ -g -std=c++14 $< -o $@

server:
	yarn run serve

