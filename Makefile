sampleobjects = buffer_manager.o file_manager.o sample_run.o
linearsearchObjects = buffer_manager.o file_manager.o linearsearch.o
view_fileobjects = buffer_manager.o file_manager.o view_file.o

sample_run : $(sampleobjects) linearsearch view_file
	     g++ -std=c++11 -o sample_run $(sampleobjects)

sample_run.o : sample_run.cpp
	g++ -std=c++11 -c sample_run.cpp

linearsearch : $(linearsearchObjects)
		g++ -std=c++11 -o linearsearch $(linearsearchObjects)
		cp linearsearch ../Run/TC_search/Run_linear/linearsearch

linearsearch.o : linearsearch.cpp
		 g++ -std=c++11 -c linearsearch.cpp

view_file : $(view_fileobjects)
		g++ -std=c++11 -o view_file $(view_fileobjects)

view_file.o : view_file.cpp
		g++ -std=c++11 -c view_file.cpp	

buffer_manager.o : buffer_manager.cpp
	g++ -std=c++11 -c buffer_manager.cpp

file_manager.o : file_manager.cpp
	g++ -std=c++11 -c file_manager.cpp

clean :
	rm -f *.o
	rm -f sample_run linearsearch view_file
