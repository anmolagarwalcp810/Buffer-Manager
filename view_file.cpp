#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h>
using namespace std;


int main(int argc,char* argv[]) {
	FileManager fm;
	char* input_file = argv[1];
	FileHandler fh = fm.OpenFile(input_file);
	PageHandler ph = fh.FirstPage ();
	char *data;
	int value;
	int count;
	while(true){
		printf("PAGE: %d\n",ph.GetPageNum());
		data = ph.GetData ();

		count=0;
		while(count<PAGE_CONTENT_SIZE){
			memcpy(&value,&data[count],sizeof(int));
			cout<<value<<endl;
			count+=4;
		}

		if(ph==fh.LastPage()){
			break;
		}
		else{
			ph=fh.NextPage(ph.GetPageNum());
			count=0;
		}
	}
	fm.CloseFile(fh);
}