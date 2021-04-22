#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h>
using namespace std;


/* ALGORITHM

Input : A file accessible through the File Manager API containing the sorted integers, and num,
the integer to search
Output: An output file accessible through the File Manager API in which result is to be stored.
Store (Page number, offset) containing the integer if present, else (-1, -1) in the output file
 curr = first Page Number
 while curr ≤ lastP ageN umber do
 Read page curr into the buffer
	if num is present in curr then
		output (curr, offset at which num is present)
		return
	end
	else
		increment curr by 1
	end
end
output(-1, -1)


*/


int main(int argc,char* argv[]) {
	char* input_file = argv[1];
	char* output_file = argv[3];

	FileManager fm;

	FileHandler fh_input = fm.OpenFile(input_file);
	int num = 1;	// later add a while loop on query_search.txt, and also handle execptions (optional)
	FileHandler fh_output = fm.CreateFile(output_file);

	PageHandler cur = fh_input.FirstPage();
	char* data;
	int value,count;
	// also need to enter (-1,-1) and INT_MIN if output_count<PAGE_CONTENT_SIZE
	PageHandler cur_output = fh_output.NewPage();
	char* data_output;	
	data_output = cur_output.GetData();
	int output_count=0;
	string query_line;
	string token;
	ifstream query_file(argv[2]);
	while(getline(query_file,query_line)){
		vector<string> queries;
		stringstream query(query_line);
		while(getline(query,token,' ')){
			queries.push_back(token.c_str());
		}
		num = atoi(queries[1].c_str());
		if(queries[0].compare("SEARCH")!=0){
			continue;
		}
		while(true){
			data = cur.GetData();
			count=0;
			while(count<PAGE_CONTENT_SIZE){
				memcpy(&value,&data[count],sizeof(int));
				if(value==INT_MIN){	// empty entry reached
					break;
				}
				if(value==num){
					if(output_count==PAGE_CONTENT_SIZE){
						fh_output.FlushPages();
						cur_output = fh_output.NewPage();
						output_count=0;
						data_output = cur_output.GetData();
					}
					// page number
					value = cur.GetPageNum();
					memcpy(&data_output[output_count],&value,sizeof(int));
					output_count+=4;
					if(output_count==PAGE_CONTENT_SIZE){
						fh_output.FlushPages();
						cur_output = fh_output.NewPage();
						output_count=0;
						data_output = cur_output.GetData();
					}
					// offset
					value = count/4;
					memcpy(&data_output[output_count],&value,sizeof(int));
					output_count+=4;
				}
				count+=4;
			} 
			if(cur==fh_input.LastPage()){
				// all pages have been processed
				break;
			}
			else{
				cur=fh_input.NextPage(cur.GetPageNum());
				count=0;
			}
		}
		// Insert -1,-1
		if(output_count==PAGE_CONTENT_SIZE){
			fh_output.FlushPages();
			cur_output = fh_output.NewPage();
			output_count=0;
			data_output = cur_output.GetData();
		}
		// -1
		value = -1;
		memcpy(&data_output[output_count],&value,sizeof(int));
		output_count+=4;
		if(output_count==PAGE_CONTENT_SIZE){
			fh_output.FlushPages();
			cur_output = fh_output.NewPage();
			output_count=0;
			data_output = cur_output.GetData();
		}
		memcpy(&data_output[output_count],&value,sizeof(int));
		output_count+=4;

		// re-initialized cur for next query
		cur = fh_input.FirstPage();
			
	}

	// Now fill empty entries by INT_MIN
	value=INT_MIN;
	while(output_count<PAGE_CONTENT_SIZE){
		memcpy(&data_output[output_count],&value,sizeof(int));
		output_count+=4;
	}
	fh_output.FlushPages();

	// Now close both files
	fm.CloseFile(fh_input);
	fm.CloseFile(fh_output);
}