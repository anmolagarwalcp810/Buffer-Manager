#include <iostream>
#include "file_manager.h"
#include "errors.h"
#include <cstring>
#include <bits/stdc++.h> /// TODO: Remove this line as it consumes a lot of memory!
using namespace std;

/// Algorithm:
// Find the first occurrence of num in (p, off_p)
// Find the last occurrence of num in (q, off_q)
// increment(q,off_q)
// while (! (q, off_q) is the last valid element){
//      Copy(q, off_q, p, off_p) // copy the element at (q, off_q to p,off_p)
// }
// Delete all pages after p!
// If off_p is not 0:
// Make all entries (p, off_p) -> (p, last) as INT_MIN
// else delete page q

pair<int, int> binSearch(int num, FileHandler &handler, int first, int last) {
    /// Returns the first occurrence of num in handler!
    // The first and last are the page numbers of the  pages
    return make_pair(-1,-1);
}

pair<int,int> linScan(int num, FileHandler &fh_input, int total_first, int total_last, pair<int,int> start){
    // Linearly Scan for the last occurrence of num
    return make_pair(-1,-1);
}

pair<int,int> increment(pair<int, int> inp){
    // The input is p, off_p. -> Increment to p, off_p+4, or p+1, 0 (if no space left)
}

int copy(pair<int,int> start, pair<int, int> end){
    // Copy from end to start!
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

        // Bin Search for the (p, off_p)

        pair<int,int> start = binSearch(num, fh_input, total_first, total_last);

        // Linearly Scan for (q, off_q)
        pair<int, int> end;
        end = linScan(num, fh_input, total_first, total_last, start);

        // Increment
        end = increment(end);

        while (end.first <= total_last){
            // Copy(q, off_q, p, off_p)
            // copy the element at (q, off_q to p,off_p)
            int res = copy(start, end);
            if(res == 1){
                // end now has INT_MIN
                break;
            }
            increment(end);
            increment(start);
        }

        // Delete all pages after p

        // Make all values from off_p  to last as 0

    }

    // Now close the file
    fm.CloseFile(fh_input);

}
