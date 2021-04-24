sampleobjects = buffer_manager.o file_manager.o sample_run.o
linearsearchObjects = buffer_manager.o file_manager.o linearsearch.o
binarysearchObjects = buffer_manager.o file_manager.o binarysearch.o
deletionObjects = buffer_manager.o file_manager.o deletion.o
join1objects = buffer_manager.o file_manager.o join1.o
join2objects = buffer_manager.o file_manager.o join2.o
view_fileobjects = buffer_manager.o file_manager.o view_file.o
count_each_elementObjects = buffer_manager.o file_manager.o count_each_element.o
create_filesObjects = buffer_manager.o file_manager.o create_files.o

sample_run : $(sampleobjects) linearsearch view_file join1 join2 count_each_element binarysearch deletion create_files
	     g++ -std=c++11 -o sample_run $(sampleobjects)
		#remove this line in final submission
		cp linearsearch ../Run/TC_search/Run_linear/linearsearch	     
		cp binarysearch ../Run/TC_search/Run_binary/binarysearch
		cp deletion ../Run/TC_delete/deletion
		cp TestCases/TC_delete/sorted_input ../Run/TC_delete/sorted_input
		cp join1 ../Run/TC_join1/Run/join1
		cp join2 ../Run/TC_join2/Run/join2
		cp create_files ../Run/NEW/create_files


sample_run.o : sample_run.cpp
	g++ -std=c++11 -c sample_run.cpp

linearsearch : $(linearsearchObjects)
		g++ -std=c++11 -o linearsearch $(linearsearchObjects)


linearsearch.o : linearsearch.cpp
		 g++ -std=c++11 -c linearsearch.cpp

binarysearch : $(binarysearchObjects)
		g++ -std=c++11 -o binarysearch $(binarysearchObjects)

binarysearch.o : binarysearch.cpp
		 g++ -std=c++11 -c binarysearch.cpp

deletion : $(deletionObjects)
		g++ -std=c++11 -o deletion $(deletionObjects)

deletion.o : deletion.cpp
		 g++ -std=c++11 -c deletion.cpp		


join1 : $(join1objects)
		g++ -std=c++11 -o join1 $(join1objects)

join1.o : join1.cpp
		g++ -std=c++11 -c join1.cpp

join2 : $(join2objects)
		g++ -std=c++11 -o join2 $(join2objects)

join2.o : join2.cpp
		g++ -std=c++11 -c join2.cpp

view_file : $(view_fileobjects)
		g++ -std=c++11 -o view_file $(view_fileobjects)

view_file.o : view_file.cpp
		g++ -std=c++11 -c view_file.cpp	

count_each_element : $(count_each_elementObjects)
		g++ -std=c++11 -o count_each_element $(count_each_elementObjects)

count_each_element.o : count_each_element.cpp
		g++ -std=c++11 -c count_each_element.cpp	

create_files : $(create_filesObjects)
		g++ -std=c++11 -o create_files $(create_filesObjects)		

create_files.o : create_files.cpp
		g++ -std=c++11 -c create_files.cpp			


buffer_manager.o : buffer_manager.cpp
	g++ -std=c++11 -c buffer_manager.cpp

file_manager.o : file_manager.cpp
	g++ -std=c++11 -c file_manager.cpp

clean :
	rm -f *.o
	rm -f sample_run linearsearch view_file join1 join2 count_each_element binarysearch deletion create_files
