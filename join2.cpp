#include <iostream>
#include "file_manager.h"
#include "errors.h"
#include <cstring>
#include <bits/stdc++.h> /// TODO: Remove this line as it consumes a lot of memory!
using namespace std;

void write(int& output_count,int count, PageHandler& cur,PageHandler& cur_output, FileHandler& fh_output,char* data_output){
    if(output_count==PAGE_CONTENT_SIZE){
        /// Need to create a new page
        fh_output.UnpinPage(cur_output.GetPageNum()); // We no longer need this page
        fh_output.FlushPages();
        cur_output = fh_output.NewPage(); // Create a new page
        output_count=0;
        data_output = cur_output.GetData();
    }
    // Now we definitely have space on the output page!
    // page number
    int value = cur.GetPageNum();
    memcpy(&data_output[output_count],&value,sizeof(int)); // Write on the output page the current page number
    output_count+=4;
    if(output_count==PAGE_CONTENT_SIZE){
        /// Again check if we need to create a new page
        fh_output.UnpinPage(cur_output.GetPageNum());
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



pair<int,int> binary_search(FileHandler &fh_input,int num) {
    PageHandler first = fh_input.FirstPage();
    int firstPageNum = first.GetPageNum();
    fh_input.UnpinPage(firstPageNum);
    PageHandler last = fh_input.LastPage();
    int lastPageNum = last.GetPageNum();
    fh_input.UnpinPage(lastPageNum);


    int total_first = firstPageNum;
    int total_last = lastPageNum;

    char* data;
    int value,count;
    /// Binary Search
    PageHandler cur;
    bool done = false;
    firstPageNum = total_first;
    lastPageNum = total_last;

    pair<int,int> answer = {-1,-1};

    while(lastPageNum >= firstPageNum){
        int mid = (lastPageNum + firstPageNum)/2; //TODO: Change to avoid overflow
        fh_input.UnpinPage(cur.GetPageNum());
        cur = fh_input.PageAt(mid);
        data = cur.GetData(); // Get the data for the current page
        count = 0;
        while(count<PAGE_CONTENT_SIZE){ // Read all the integers on the page
            memcpy(&value,&data[count],sizeof(int));
            if(value==INT_MIN){	// empty entry reached
                /// We are on the last Page, and no more entries are present!
                done = true;
                break;
            }
            if(value==num){
                // Check if we need to load previous pages:
                // If the first val is num and this is not the first page
                memcpy(&value,&data[0],sizeof(int));    // first entry of current page
                if(value == num && cur.GetPageNum() != total_first){
                    while(true){
                        // Load the just previous page and write its content
                        fh_input.UnpinPage(cur.GetPageNum()); /// Or use Mid
                        cur = fh_input.PrevPage(cur.GetPageNum());
                        data = cur.GetData();
                        // Go through all values of this page and write to output
                        count = PAGE_CONTENT_SIZE;
                        while(count > 0){
                            count -= 4;
                            memcpy(&value,&data[count],sizeof(int));
                            if(value!=num){
                                count+=4;
                                break;
                            }
                        }
                        // If this page is the first page, or the first entry is not num, we are done!
                        memcpy(&value,&data[0],sizeof(int));
                        if(cur.GetPageNum() == total_first || value != num) break;
                    }
                }
                if(count==PAGE_CONTENT_SIZE){
                	answer={cur.GetPageNum()+1,0};
                }
                else{
                	answer={cur.GetPageNum(),count};
                }
                // We are done. Proceed to (-1,-1) Part!
                done = true;
                break;
            }
            count+=4;
        }
        if(done) break;
        int first_val, last_val;
        memcpy(&first_val,&data[0],sizeof(int));
        memcpy(&last_val,&data[PAGE_CONTENT_SIZE - 4],sizeof(int));
        if(first_val > num){
            lastPageNum = mid - 1;
        }
        else if(last_val < num){
            firstPageNum = mid+1;
        } 
        else {
            // last_val > num > first_val
            // No entry in here exit!
            break;
        }
        fh_input.UnpinPage(cur.GetPageNum()); // Unpin it
        count = 0;
    }
    fh_input.UnpinPage(cur.GetPageNum());
    return answer;
}


/* ALGORITHM
Assume you have n memory blocks available then you will use 1 block for output, 1 for R2 and n − 2
blocks for R1.
Input : Two files accessible through the File Manager API containing integers from which integers
are to be read.
Output: An output file accessible through the File Manager API in which result is to be stored.
for each page p in R1 do
	for each integer num in p do
		(q, of f set) = Use Algorithm 1 or 2 to locate the page q where num is present if q == −1
		then
			continue
		end
		else
			while q ≤ last page number of file and integer at (q, of f set) == num do
				add num in the outfile file
				if of f set == last offset of page q then
					of f set = 0
					q = q +1
				end
				else
					of f set = of f set + 1
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
	cur1 = fh1.FirstPage(),cur2 = fh2.FirstPage();
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
	int num;

	pair<int,int> answer;

	int page_at = 0;

	bool done=false;

	page_at = 0;

	// loop for R1
	while(true){
		vector<PageHandler> R1;
		// fill the vector with n-2 pages
		// go through the vector with normal algo
		// by replacing for each page in R2_vector, instead of R2
		// fm.PrintBuffer();
		// printf("212\n");
		cur1 = fh1.PageAt(page_at);
		vector_count=0;
		while(true){
			R1.push_back(cur1);
			if(cur1.GetPageNum()==total_last1){
				done=true;
				break;
			}
			vector_count++;
			if(vector_count==n-2){
				break;
			}
			cur1 = fh1.NextPage(cur1.GetPageNum());
		}

		page_at = cur1.GetPageNum()+1;		// next page to look at

		// cout<<"PAGES FROM R1"<<" "<<"page: "<<cur1.GetPageNum()<<endl;
		// for(auto i:R1){
		// 	cout<<i.GetPageNum()<<" "<<flush;
		// }
		// cout<<endl;
		for(auto i:R1){
			data1 = i.GetData();
			// printf("237\n");
			for(count1=0;count1<PAGE_CONTENT_SIZE;count1+=4){
				// printf("240\n");
				memcpy(&num,&data1[count1],sizeof(int));
				// fm.PrintBuffer();
				fh2.UnpinPage(cur2.GetPageNum());
				answer = binary_search(fh2,num);
				// cout<<"225 "<<"PAGE : "<<i.GetPageNum()<<", OFFSET "<<count1/4<<" answer: "<<answer.first<<" "<<answer.second/4<<endl;

				// printf("242\n");
				if(answer.first==-1){
					continue;
				}
				else{
					// printf("247\n");
					// need to check that R2's pages have been unpinned.
					fh2.UnpinPage(cur2.GetPageNum());
					cur2 = fh2.PageAt(answer.first);
					data2 = cur2.GetData();
					count2 = answer.second;
					// fm.PrintBuffer();
					while(true){
						while(count2<PAGE_CONTENT_SIZE){
							memcpy(&value,&data2[count2],sizeof(int));
							if(value!=num)break;
							if(output_count==PAGE_CONTENT_SIZE){
								fh_output.UnpinPage(cur_output.GetPageNum());
								fh_output.FlushPages();
								cur_output = fh_output.NewPage();
								output_count=0;
								data_output = cur_output.GetData();
							}
							memcpy(&data_output[output_count],&value,sizeof(int));
							output_count+=4;
							count2+=4;
						}
						fh2.UnpinPage(cur2.GetPageNum());
						// printf("270\n");
						if(cur2.GetPageNum()==total_last2 || value!=num){
							break;
						}
						else{
							cur2=fh2.NextPage(cur2.GetPageNum());
							data2=cur2.GetData();
							count2=0;
						}
					}
					// printf("279\n");
				}
			}
		}
		for(auto i:R1){
			fh1.UnpinPage(i.GetPageNum());
		}
		if(done){
			// page_at=0;
			break;
		}
	}

	// Insert -1,-1, here we need to insert only once
	// if(output_count==PAGE_CONTENT_SIZE){
	// 	fh_output.UnpinPage(cur_output.GetPageNum());
	// 	fh_output.FlushPages();
	// 	cur_output = fh_output.NewPage();
	// 	output_count=0;
	// 	data_output = cur_output.GetData();
	// }
	// // -1
	// value = -1;
	// memcpy(&data_output[output_count],&value,sizeof(int));
	// output_count+=4;
	// if(output_count==PAGE_CONTENT_SIZE){
	// 	fh_output.UnpinPage(cur_output.GetPageNum());
	// 	fh_output.FlushPages();
	// 	cur_output = fh_output.NewPage();
	// 	output_count=0;
	// 	data_output = cur_output.GetData();
	// }
	// memcpy(&data_output[output_count],&value,sizeof(int));
	// output_count+=4;

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