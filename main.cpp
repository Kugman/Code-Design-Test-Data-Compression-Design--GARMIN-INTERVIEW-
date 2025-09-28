
#include <iostream>
#define MASK 0x80

using namespace std;

void printBuffer(uint8_t* data_ptr, int data_size) {
    
    cout << "{";
    for (int i = 0; i < data_size - 1; i++)
        cout << hex << data_ptr[i] + 0 << ", ";

    cout << hex << data_ptr[data_size - 1] + 0;
    cout << "}" << endl;
}

uint8_t repeats(uint8_t* data_ptr, int data_size, int start_index) {
    int repeat = 1;
    while(start_index < data_size - 1 && data_ptr[start_index] == data_ptr[start_index + 1]) {
        repeat++;
        start_index++;
    }
    return repeat;
}

/*
  Compress a buffer of data using a run-length encoding algorithm.
 
  This function compresses the buffer by identifying repeatition of the same value.
  If a sequence of repeated values is found, the value with a MASK (to indicate repetition) 
  stores in the compressed buffer, followed by the number of repetitions.
  For sequences where the repeat count exceeds 127, it splits the count to multiple bytes, 
  with each byte representing up to 127 repetitions (using the same MASK to sign continuously).
 
  If there is no repetition, the value is copied as-is to the compressed buffer.
  The function modifies the original buffer with the compressed data values without changing its size.
 
  @param data_ptr pointer to a data buffer.
  @param data_size the size of the buffer.
  @return the new size of the compressed buffer.
 */
int bufferCompression(uint8_t* data_ptr, int data_size) {
    
    int new_buffer_index = 0, old_buffer_index = 0;

    while (old_buffer_index < data_size) {
        // Call repeats function to find how many bytes in-a-row are the same (starting from the current index)
        uint8_t repeat = repeats(data_ptr, data_size, old_buffer_index);
        
        if (repeat > 1) {
            // store the value with 1 prefix mask to indicate a repeatition
            data_ptr[new_buffer_index++] = data_ptr[old_buffer_index] | MASK;
            old_buffer_index += repeat;
            
            // if the repetition count exceeds 127 (since we are limited to 7-bit encoding)-
            // it splits to multiple bytes with 0xFF to indicate continuation.
            while (repeat > 127) {
                data_ptr[new_buffer_index++] = numeric_limits<uint8_t>::max(); //0xFF
                repeat -= 127;
            }
            // store the remaining count of repeated values
            data_ptr[new_buffer_index++] = repeat;
        }
        // if no repetition - copy the value as is
        else data_ptr[new_buffer_index++] = data_ptr[old_buffer_index++];
    }
    
    return new_buffer_index;
}

/*
  Decompress a compressed buffer of data using a run-length encoding algorithm.
 
  This function takes a compressed buffer of data and decompresses it back to its original form.
  The compression uses a special MASK to identify repeated sequences of the same value.
  If the first bit of a byte is set (indicating the MASK), it represents a sequence where
  the same number is repeated. The number of repetitions is encoded in subsequent bytes.
 
  The function reconstructs the original uncompressed data by repeating values accordingly
  and stores the result back in the original buffer.
 
  @param data_ptr a pointer to a compressed data buffer.
  @param data_size the size of the compressed buffer.
  @return the size of the decompressed buffer.
 */
int bufferDecompression(uint8_t* data_ptr, int data_size) {
    
    int old_index = 0, new_index = 0;
    
    //alocate memory - restore the original size
    uint8_t* new_buffer = new uint8_t[sizeof(data_ptr) / sizeof(data_ptr[0])];
    
    while (old_index < data_size) {
        
        // check if the first bit is set - indicates a repeatition
        if (MASK == (data_ptr[old_index] & MASK)) {
            
            // extract the number by removing the prefix bit
            uint8_t tmp = data_ptr[old_index++] ^ MASK;
            int repeat = 0;
            
            // if the repeat count exceeds 127- add 127
            while (MASK == (data_ptr[old_index] & MASK)) {
                repeat += 127;
                old_index++;
            }
            repeat += data_ptr[old_index++];
            
            // copy the number to the new buffer by repeat
            for (int i = repeat; i > 0; i--)
                new_buffer[new_index++] = tmp;
        }

        // if the current data point with no repeatition, copy it as-is.
        else new_buffer[new_index++] = data_ptr[old_index++];
    }

    copy(new_buffer, new_buffer + new_index, data_ptr);

    return new_index;
}

int main() {

    //Data before the call
    uint8_t data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
                           0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };
    int data_size = 24;

    /*
    //more than 255 bumbers in a row - example
    int data_size = 256;
    uint8_t data_ptr[256];
    for (int i = 0; i < data_size; i++) data_ptr[i] = 0x74;
    */

    
    printBuffer(data_ptr, data_size);

    //compression function
    data_size = bufferCompression(data_ptr, data_size);
    printBuffer(data_ptr, data_size);

    //showing that decompression works well
    data_size = bufferDecompression(data_ptr, data_size);
    printBuffer(data_ptr, data_size);

    return 0;
}
