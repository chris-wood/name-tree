all:
	g++ -o main src/name_tree.cpp src/main.cpp src/hasher.cpp src/cJSON.c src/generator.cpp -lcryptopp

clean: 
	rm main