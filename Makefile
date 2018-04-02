all:
	g++ main.cpp -lpthread -o s.exe
clean:
	rm -f *~ s.exe
