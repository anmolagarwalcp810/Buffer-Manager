#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h>
using namespace std;


int main(int argc,char* argv[]) {
	FileManager fm;
	char* input_file = argv[1];
	vector<pair<int,int>> v;
	int vector_count=0;
	FileHandler fh = fm.OpenFile(input_file);
	PageHandler ph = fh.FirstPage ();
	char *data;
	int value;
	int count;
	bool flag;
	while(true){
		data = ph.GetData ();
		count=0;
		while(count<PAGE_CONTENT_SIZE){
			memcpy(&value,&data[count],sizeof(int));
			flag=true;
			for(int i=0;i<vector_count;i++){
				if(v[i].first==value){
					v[i].second++;
					flag=false;
					break;
				}
			}
			if(flag){
				vector_count++;
				v.push_back({value,1});
			}
			count+=4;
		}
		fh.UnpinPage(ph.GetPageNum());
		if(ph==fh.LastPage()){
			break;
		}
		else{
			ph=fh.NextPage(ph.GetPageNum());
			count=0;
		}
	}
	fm.CloseFile(fh);
	//print stats
	sort(v.begin(),v.end());
	for(int i=0;i<vector_count;i++){
		cout<<(v[i].first)<<": "<<(v[i].second)<<endl;
	}
}