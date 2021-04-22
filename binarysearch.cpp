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


int main(int argc,char* argv[]) {
    char* input_file = argv[1];
    char* output_file = argv[3];

    FileManager fm;

    FileHandler fh_input = fm.OpenFile(input_file);
    int num = 1;	// later add a while loop on query_search.txt, and also handle exceptions (optional)
    FileHandler fh_output = fm.CreateFile(output_file);

    PageHandler first = fh_input.FirstPage();
    PageHandler last = fh_input.LastPage();

    int firstPageNum = first.GetPageNum();
    int lastPageNum = last.GetPageNum();

    int total_first = firstPageNum;
    int total_last = lastPageNum;

    // Now unpin these pages
    fh_input.UnpinPage(firstPageNum);
    fh_input.UnpinPage(lastPageNum);

    char* data;
    int value,count;
    // also need to enter (-1,-1) and INT_MIN if output_count<PAGE_CONTENT_SIZE
    PageHandler cur_output = fh_output.NewPage();
    char* data_output;
    data_output = cur_output.GetData();
    int output_count=0;
    string query_line;
    string token;
    ifstream query_file(argv[2]); // Open the query file!
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
        /// Binary Search

        printf("82 Starting Query Execution: \n");
        PageHandler cur;
        bool done = false;
        firstPageNum = total_first;
        lastPageNum = total_last;

        printf("88 First Page Number: %d\n", firstPageNum);
        printf("89 Last Page Number: %d\n", lastPageNum);

        while(lastPageNum >= firstPageNum){
            int mid = (lastPageNum + firstPageNum)/2; //TODO: Change to avoid overflow
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
                    printf("104 The Page %d contains the values\n", mid);
                    // The page contains num
                    // Write the values on this page to output:
                    while(count < PAGE_CONTENT_SIZE && value == num){
                        write(output_count,count,cur,cur_output,fh_output, data_output);
                        count += 4;
                        memcpy(&value,&data[count],sizeof(int));
                    }

                    // Check if we need to load previous pages:
                    // If the first val is num and this is not the first page
                    memcpy(&value,&data[0],sizeof(int));
                    if(value == num && cur.GetPageNum() != total_first){
                        while(true){
                            // Load the just previous page and write its content
                            fh_input.UnpinPage(cur.GetPageNum()); /// Or use Mid
                            cur = fh_input.PrevPage(cur.GetPageNum());
                            data = cur.GetData();
                            // Go through all values of this page and write to output
                            count = 0;
                            while(count < PAGE_CONTENT_SIZE){
                                memcpy(&value,&data[count],sizeof(int));
                                if(value == num) {
                                    write(output_count, count, cur, cur_output, fh_output, data_output);
                                }
                                count += 4;
                            }
                            // If this page is the first page, or the first entry is not num, we are done!
                            memcpy(&value,&data[0],sizeof(int));
                            if(cur.GetPageNum() == total_first || value != num) break;
                        }
                    }
                    // Load the page back!
                    fh_input.UnpinPage(cur.GetPageNum());
                    cur = fh_input.PageAt(mid);
                    // Now let's check for next pages!

                    // TODO: Check if PAGE_CONTENT_SIZE - 4 gives the last value!
                    memcpy(&value,&data[PAGE_CONTENT_SIZE - 4],sizeof(int));
                    if(value == num && cur.GetPageNum() != total_last){
                        while(true){
                            // Load the just next page and write its content
                            fh_input.UnpinPage(cur.GetPageNum()); /// Or use Mid
                            cur = fh_input.NextPage(cur.GetPageNum());
                            data = cur.GetData();
                            // Go through all values of this page and write to output
                            count = 0;
                            while(count < PAGE_CONTENT_SIZE){
                                memcpy(&value,&data[count],sizeof(int));
                                if(value == num) {
                                    write(output_count, count, cur, cur_output, fh_output, data_output);
                                }
                                count += 4;
                            }
                            // If this page is the last page, or the last entry is not num, we are done!
                            memcpy(&value,&data[PAGE_CONTENT_SIZE - 4],sizeof(int));
                            if(cur.GetPageNum() == total_last || value != num) break;
                        }
                    }
                    // We are done. Proceed to (-1,-1) Part!
                    done = true;
                    break;
                }
                count+=4;
            }
            if(done) break;
            /// This page does not contain the value!

            // If the first value of this page is greater than num,
            // we will move the lastPage to mid-1
            int first_val, last_val;
            memcpy(&first_val,&data[0],sizeof(int));
            memcpy(&last_val,&data[PAGE_CONTENT_SIZE - 4],sizeof(int));
            if(first_val > num){
                lastPageNum = mid - 1;
            }
            else if(last_val < num){
                firstPageNum = mid+1;
            } else {
                // last_val > num > first_val
                // No entry in here exit!
                break;
            }
            fh_input.UnpinPage(cur.GetPageNum()); /// Unpin it
            count = 0;
        }
        // Insert -1,-1
        if(output_count==PAGE_CONTENT_SIZE){
            /// Again create a new page if required
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

        // re-initialized cur for next query
//        cur = fh_input.FirstPage();
    }

    // Now fill empty entries by INT_MIN
    value=INT_MIN;
    while(output_count<PAGE_CONTENT_SIZE){
        memcpy(&data_output[output_count],&value,sizeof(int));
        output_count+=4;
    }
    fh_output.UnpinPage(cur_output.GetPageNum());
    fh_output.FlushPages();

    // Now close both files
    fm.CloseFile(fh_input);
    fm.CloseFile(fh_output);
}