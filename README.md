# Buffer Manager

Implemented following functions on top of Buffer Manager:
+ Linear Search
+ Binary Search
+ Deletion
+ Join 1: The relations may not be sorted.
+ Join 2: One of the relations is sorted.

## Build and Run Instructions
### Linear Search
```shell
make linearsearch
./linearsearch <input_filename> <query_filename> <output_filename>
```
### Binary Search
```shell
make binarysearch
./binarysearch <sorted_input_filename> <query_filename>.txt <output_filename>
```

### Deletion
```shell
make deletion
./deletion <sorted_input_filename> <query_filename>.txt
```

### Join 1
```shell
make join1
./join1 <unsorted_input_file1name> <unsorted_input_file2name> <output_filename>
```

### Join 2
```shell
make join2
./join2 <unsorted_input_file1name> <sorted_input_file2name> <output_filename>
```
