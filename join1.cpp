#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h>
using namespace std;


/* ALGORITHM

Input : Two files accessible through the File Manager API containing integers from which integers
are to be read.
Output: An output file accessible through the File Manager API in which result is to be stored.
for each page p in R1 do
	for each page q in R2 do
		for each integer num1 in p do
			for each integer num2 in q do
				if num1 == num2 then
					add num1 in the output file
				end
			end
		end
	end
end
*/


int main(int argc,char* argv[]) {
	char* input_file1 = argv[1];
	char* input_file2 = argv[2];
	char* output_file = argv[3];

	FileManager fm;

	FileHandler fh1 = fm.OpenFile(input_file1);
	FileHandler fh2 = fm.OpenFile(input_file2);
	FileHandler fh_output = fm.CreateFile(output_file);

	PageHandler cur1,cur2;
	cur1 = fh1.LastPage(),cur2 = fh2.LastPage();
	int total_last1 = cur1.GetPageNum(),total_last2 = cur2.GetPageNum();
	fh1.UnpinPage(total_last1);
	fh2.UnpinPage(total_last2);

	// Define cur1,cur2, data1, data2, count1,count2
	cur1 = fh1.FirstPage();
	// cur2 = fh2.FirstPage();
	char* data1,*data2;
	int value,value1,value2,count1,count2;

	// cur_output, data_output, output_count
	PageHandler cur_output = fh_output.NewPage();
	char* data_output;	
	data_output = cur_output.GetData();
	int output_count=0;

	int n = BUFFER_SIZE;

	// also need vector of n-2 pages for R2
	// n-2>=1, n>=3

	int vector_count=0;

	int page_at = 0;	// page_at > total_last2 implies that all pages have been processed

	// loop for R1
	while(true){
		// loop for R2
		fh2.FlushPages();
		// fm.PrintBuffer();
		cur2 = fh2.FirstPage();
		while(true){
			// fm.PrintBuffer();
			vector<PageHandler> R2;
			// fill the vector with n-2 pages
			// go through the vector with normal algo
			// by replacing for each page in R2_vector, instead of R2
			cur2 = fh2.PageAt(page_at);

			vector_count=0;
			while(true){
				R2.push_back(cur2);
				if(cur2.GetPageNum()==total_last2){
					break;
				}
				vector_count++;
				if(vector_count==n-2){
					break;
				}
				cur2 = fh2.NextPage(cur2.GetPageNum());
			}
			
			page_at = cur2.GetPageNum()+1;		// next page to look at

			// printf("PAGE_AT,total_last2 : %d, %d\n",page_at,total_last2 );

			// cout<<"PAGES FROM R2"<<" "<<"R1 page: "<<cur1.GetPageNum()<<endl;
			// for(auto i:R2){
			// 	cout<<i.GetPageNum()<<" "<<flush;
			// }
			// cout<<endl;


			data1 = cur1.GetData();
			count1=0;
			while(count1<PAGE_CONTENT_SIZE){
				memcpy(&value1,&data1[count1],sizeof(int));
				if(value1==INT_MIN){	// empty entry reached
					break;
				}
				for(auto i:R2){
					data2 = i.GetData();
					count2=0;
					while(count2<PAGE_CONTENT_SIZE){
						memcpy(&value2,&data2[count2],sizeof(int));
						if(value1==value2){
							if(output_count==PAGE_CONTENT_SIZE){
								fh_output.UnpinPage(cur_output.GetPageNum());
								fh_output.FlushPages();
								cur_output = fh_output.NewPage();
								output_count=0;
								data_output = cur_output.GetData();
							}
							value = value1;
							memcpy(&data_output[output_count],&value,sizeof(int));
							output_count+=4;
						}
						count2+=4;
					}
					// fh2.UnpinPage(i.GetPageNum());		
				}
				count1+=4;
			}
			for(auto i:R2){
				fh2.UnpinPage(i.GetPageNum());
			}
			if(page_at > total_last2){
				page_at=0;
				break;
			}
		}
		fh1.UnpinPage(cur1.GetPageNum());
		if(cur1.GetPageNum()==total_last1){
			// all pages have been processed
			break;
		}
		else{
			cur1=fh1.NextPage(cur1.GetPageNum());
			data1=cur1.GetData();
		}	
	}

	// Insert -1,-1, here we need to insert only once
	if(output_count==PAGE_CONTENT_SIZE){
		fh_output.UnpinPage(cur_output.GetPageNum());
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
		fh_output.UnpinPage(cur_output.GetPageNum());
		fh_output.FlushPages();
		cur_output = fh_output.NewPage();
		output_count=0;
		data_output = cur_output.GetData();
	}
	memcpy(&data_output[output_count],&value,sizeof(int));
	output_count+=4;

	// Now fill empty entries by INT_MIN
	value=INT_MIN;
	while(output_count<PAGE_CONTENT_SIZE){
		fh_output.UnpinPage(cur_output.GetPageNum());
		memcpy(&data_output[output_count],&value,sizeof(int));
		output_count+=4;
	}
	fh_output.FlushPages();

	// Now close both files
	fm.CloseFile(fh1);
	fm.CloseFile(fh2);
	fm.CloseFile(fh_output);
}