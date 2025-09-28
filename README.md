# Code-Design-Test-Data-Compression-Design--GARMIN-INTERVIEW-Code Design Test: Data Compression Design
Design an algorithm that will compress a given data buffer of bytes. 
Please describe your design and submit an implementation in a language of your choice.
The algorithm will live within a function.  
This function will be called with two arguments; a pointer to the data buffer (data_ptr) and the number of bytes to compress (data_size).  
After the function executes the data in the buffer will be modified and the size of the modified buffer will be returned.

Assumptions:
1.	The data_ptr will point to an array of bytes.  Each byte will contain a number from 0 to 127 (0x00 to 0x7F).
2.	It is common for the data in the buffer to have the same value repeated in the series.
3.	The compressed data will need to be decompressable.  Please ensure that your algorithm allows for a decompression algorithm to return the buffer to it’s previous form.


Example data and function call:

```cpp

// Data before the call
// data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 
//                0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
//                0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };
// data_size = 24;

new_size = byte_compress( data_ptr, data_size );

