sampleobjects = buffer_manager.o file_manager.o sample_run.o
linearsearchObjects = buffer_manager.o file_manager.o linearsearch.o
binarysearchObjects = buffer_manager.o file_manager.o binarysearch.o
join1objects = buffer_manager.o file_manager.o join1.o
view_fileobjects = buffer_manager.o file_manager.o view_file.o
count_each_elementObjects = buffer_manager.o file_manager.o count_each_element.o

sample_run : $(sampleobjects) linearsearch view_file join1 count_each_element binarysearch
	     g++ -std=c++11 -o sample_run $(sampleobjects)

sample_run.o : sample_run.cpp
	g++ -std=c++11 -c sample_run.cpp

linearsearch : $(linearsearchObjects)
		g++ -std=c++11 -o linearsearch $(linearsearchObjects)
		#remove this line in final submission
# 		cp linearsearch ../Run/TC_search/Run_linear/linearsearch

linearsearch.o : linearsearch.cpp
		 g++ -std=c++11 -c linearsearch.cpp

binarysearch : $(binarysearchObjects)
		g++ -std=c++11 -o binarysearch $(binarysearchObjects)
		#remove this line in final submission
# 		cp binarysearch ../Run/TC_search/Run_binary/binarysearch

binarysearch.o : binarysearch.cpp
		 g++ -std=c++11 -c binarysearch.cpp


join1 : $(join1objects)
		g++ -std=c++11 -o join1 $(join1objects)
		#remove this line in final submission
# 		cp join1 ../Run/TC_join1/Run/join1

join1.o : join1.cpp
		g++ -std=c++11 -c join1.cpp

view_file : $(view_fileobjects)
		g++ -std=c++11 -o view_file $(view_fileobjects)

view_file.o : view_file.cpp
		g++ -std=c++11 -c view_file.cpp	

count_each_element : $(count_each_elementObjects)
		g++ -std=c++11 -o count_each_element $(count_each_elementObjects)

count_each_element.o : count_each_element.cpp
		g++ -std=c++11 -c count_each_element.cpp	

buffer_manager.o : buffer_manager.cpp
	g++ -std=c++11 -c buffer_manager.cpp

file_manager.o : file_manager.cpp
	g++ -std=c++11 -c file_manager.cpp

clean :
	rm -f *.o
	rm -f sample_run linearsearch view_file join1 count_each_element binarysearch
