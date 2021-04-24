#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h>
using namespace std;


int main(int agrc,char* argv[]){
	char* input_file1 = argv[1];
	FileManager fm;
	// Enter 10 1's, 2's, 3's,...

	int start=0,end=100;
	int value;

	FileHandler fh_output = fm.CreateFile(input_file1);
	PageHandler cur_output = fh_output.NewPage();
	int count=0;
	char* data = cur_output.GetData();
	for(int i=0;i<100;i++){
		if(i%10==0){
			continue;
		}
		for(int j=0;j<10;j++){
			if(count==PAGE_CONTENT_SIZE){
				fh_output.UnpinPage(cur_output.GetPageNum());
				cur_output = fh_output.NewPage();
				data = cur_output.GetData();
				count=0;
			}
			value = (rand()%(end-start+1))+start;
			memcpy(&data[count],&value,sizeof(int));
			count+=4;
		}
	}
	fm.CloseFile(fh_output);
}