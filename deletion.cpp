#include <iostream>
#include "file_manager.h"
#include "errors.h"
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

// Algorithm:
// Find the first occurrence of num in (p, off_p)
// Find the last occurrence of num in (q, off_q)
// while (! (q, off_q) is the last valid element){
//      Copy(q, off_q, p, off_p) // copy the element at (q, off_q to p,off_p)
// }
// Delete all pages after p!
// If off_p is not 0:
// Make all entries (p, off_p) -> (p, last) as INT_MIN
// else delete page q

pair<int, int> binSearch(int num, FileHandler &fh_input, int first, int last, PageHandler &cur) {
    /// Returns the first occurrence of num in handler!
    // The first and last are the page numbers of the  pages

    int lastPageNum = last;
    int firstPageNum = first;

    while(lastPageNum >= firstPageNum){
        int mid = (lastPageNum + firstPageNum)/2; //TODO: Change to avoid overflow
        cur = fh_input.PageAt(mid);
        char* data = cur.GetData(); // Get the data for the current page
        int count = 0;
        while(count<PAGE_CONTENT_SIZE){ // Read all the integers on the page
            int value;
            memcpy(&value,&data[count],sizeof(int));
            if(value==INT_MIN){	// empty entry reached
                fh_input.UnpinPage(cur.GetPageNum());
                return make_pair(-1,-1);
            }
            if(value==num){
                // Found a value on this page!
                // Fetch the starting position:
                if(count != 0 || cur.GetPageNum() == first){
                    // This is the starting page and offset, we are done!
                    fh_input.UnpinPage(cur.GetPageNum());
                    return make_pair(mid,count/4);
                }
                // Otherwise Fetch the previous pages!
                while(cur.GetPageNum() != first) {
                    fh_input.UnpinPage(cur.GetPageNum());
                    cur = fh_input.PrevPage(cur.GetPageNum());
                    data = cur.GetData();
                    // Fetch the last value of cur
                    memcpy(&value, &data[PAGE_CONTENT_SIZE - 4], sizeof(int));
                    if (value != num) {
                        // This page doesn't have num
                        fh_input.UnpinPage(cur.GetPageNum());
                        return make_pair(cur.GetPageNum() + 1, 0);
                    }
                    // Fetch the first value of cur
                    memcpy(&value, &data[0], sizeof(int));
                    if (value != num) {
                        count = 0;
                        while(count < PAGE_CONTENT_SIZE){
                            memcpy(&value, &data[count], sizeof(int));
                            if(value == num){
                                fh_input.UnpinPage(cur.GetPageNum());
                                return make_pair(cur.GetPageNum(),count/4);
                            }
                            count += 4;
                        }
                    }
                }
                fh_input.UnpinPage(cur.GetPageNum());
                return make_pair(cur.GetPageNum(), 0);
            }
            count+=4;
        }
        // This page does not contain the value!

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
        }
        else {
            // last_val > num > first_val
            // No entry in here exit!
            fh_input.UnpinPage(cur.GetPageNum());
            return make_pair(-1,-1);
        }
        fh_input.UnpinPage(cur.GetPageNum()); // Unpin it
    }
    return make_pair(-1,-1);
}

pair<int,int> increment(pair<int, int> &inp, FileHandler &fh_input, PageHandler& cur, char* &data, int total_last){
    // The input is p, off_p. -> Increment to p, off_p+4, or p+1, 0 (if no space left)
    // Update Cur and data if required
    // TODO: @Anmol
    if(inp.second*4==PAGE_CONTENT_SIZE-4){
        if(inp.first==total_last){
            return {-1,-1};
        }
        else{
            // update cur and data
            fh_input.UnpinPage(cur.GetPageNum());
            cur = fh_input.NextPage(cur.GetPageNum());
            data = cur.GetData();
            // update inp
            inp.first = cur.GetPageNum();
            inp.second = 0;
        }
    }
    else{
        inp.second+=1;
    }
    return inp;

}




pair<int,int> linScan(int num, FileHandler &fh_input, int total_first, int total_last, pair<int,int> start, PageHandler &cur){
    // Linearly Scan for the last occurrence of num
    cur = fh_input.PageAt(start.first);
    int value;
    char* data = cur.GetData();
    memcpy(&value,&data[4*start.second],sizeof(int));
    while(value == num){
        start = increment(start,fh_input, cur, data,total_last);
        if(start.first == -1 && start.second == -1) return start;
        memcpy(&value,&data[4*start.second],sizeof(int));
    }
    fh_input.UnpinPage(cur.GetPageNum());
    return start;
}


int copy(pair<int,int> &start, pair<int, int> &end,FileHandler &fh_input){
    // Copy from end to start!
    // if(end.second==PAGE_CONTENT_SIZE){
    //     return 0;
    // }
    int value;
    PageHandler cur = fh_input.PageAt(end.first);
    char* data = cur.GetData();
    memcpy(&value,&data[end.second*4],sizeof(int));
    if(value==INT_MIN){
        return 1;
    }
    fh_input.UnpinPage(cur.GetPageNum());
    cur = fh_input.PageAt(start.first);
    data = cur.GetData();
    memcpy(&data[start.second*4],&value,sizeof(int));
    // Value copied! Now unpin page
    fh_input.MarkDirty(cur.GetPageNum());
    fh_input.UnpinPage(cur.GetPageNum());
    return 0;

}



int main(int argc, char* argv[]) {
    char* input_file = argv[1];

    FileManager fm;

    FileHandler fh_input = fm.OpenFile(input_file);
    int num = 1;	// later add a while loop on query_search.txt, and also handle exceptions (optional)


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
    string query_line;
    string token;
    ifstream query_file(argv[2]); // Open the query file!

    printf("198\n");

    while(getline(query_file,query_line)){
        vector<string> queries;
        stringstream query(query_line);
        while(getline(query,token,' ')){
            queries.push_back(token.c_str());
        }
        num = atoi(queries[1].c_str());
        if(queries[0].compare("DELETE")!=0){
            continue;
        }

        PageHandler cur;
        cur = fh_input.LastPage();
        total_last = cur.GetPageNum();
        fh_input.UnpinPage(total_last);
        printf("212 %d\n",num );
        // Bin Search for the (p, off_p)
        pair<int,int> start = binSearch(num, fh_input, total_first, total_last, cur);

        printf("216 %d %d\n", start.first,start.second);


        if(start.first == -1 && start.second == -1){
            // No such value:
            continue;
        }

        // Linearly Scan for (q, off_q)
        pair<int, int> end;
        end = linScan(num, fh_input, total_first, total_last, start, cur);


        printf("229 %d %d\n", end.first,end.second);        

        while (end.first != -1 && end.first <= total_last){
            // Copy(q, off_q, p, off_p)
            // copy the element at (q, off_q to p,off_p)
            int res = copy(start, end,fh_input);
            if(res == 1){
                // end now has INT_MIN
                break;
            }
            cur = fh_input.PageAt(start.first);
            data = cur.GetData();
            start = increment(start,fh_input, cur, data,total_last);
            fh_input.UnpinPage(cur.GetPageNum());
            cur = fh_input.PageAt(end.first);
            data = cur.GetData();
            end = increment(end,fh_input, cur, data,total_last);
            fh_input.UnpinPage(cur.GetPageNum());
            if(end.first==-1 && end.second==-1){
                break;
            }
            printf("250\n");
            printf("start: %d %d\n",start.first,start.second );
            printf("end: %d %d\n",end.first,end.second );
        }
        
        // Delete all pages after p

        printf("257\n");

        // INT_MIN
        cur = fh_input.PageAt(start.first);
        data = cur.GetData();
        value = INT_MIN;
        while(start.second*4<PAGE_CONTENT_SIZE){
            memcpy(&data[start.second*4],&value,sizeof(int));
            start.second++;
        }

        printf("268\n");

        fh_input.MarkDirty(cur.GetPageNum());
        fh_input.UnpinPage(cur.GetPageNum());
        int temp_num;
        if(cur.GetPageNum()!=total_last){
            cur = fh_input.NextPage(cur.GetPageNum());
            while(true){
                temp_num = cur.GetPageNum();
                printf("temp_num %d\n",temp_num);
                fh_input.DisposePage(cur.GetPageNum());
                fh_input.FlushPage(cur.GetPageNum());
                if(temp_num==total_last)break;
                cur = fh_input.NextPage(temp_num);
            };
        }
    }

    // Now close the file
    fm.CloseFile(fh_input);
}
