
#include <iostream>
#include <bitset>
#include <math.h>
#include <climits>
#include <cstdint>

class Bitpacker{
   char buffer[1024] = {0};
   int bit_offset;
   int byte_index;
   int read_bit_offset;
   int read_byte_index;

public:
  Bitpacker(){
    bit_offset = 8;
    byte_index = 0;
    read_bit_offset = 8;
    read_byte_index = 0;
  }

  void pack(bool pack_value);
  void pack(int pack_value, int min_value, int max_value);
  int unpack(int& value_read, int min_value, int max_value);
  bool unpack(bool& value_read);
  
};

void Bitpacker::pack(bool pack_value){

  bit_offset -= 1;
  if(bit_offset < 0){
    bit_offset = 7;
    byte_index++;
  }
  buffer[byte_index] |= pack_value << bit_offset;
  std::bitset<8> set(buffer[byte_index]);

}

void Bitpacker::pack(int pack_value, int min_value, int max_value){
  int max_value_length = 0;
  int v = 1;
  for(int i = 0; i < (sizeof(max_value) * 8) - 1; i++){
      if(v > max_value)
        break;
      v <<= 1;
      max_value_length++;
  }
  if(max_value_length > 8){
    bit_offset = max_value_length - bit_offset;
    if(bit_offset > 8){
      bit_offset = max_value_length - 8;
      byte_index++;
    }
    buffer[byte_index] |= pack_value >> bit_offset;
    std::bitset<8> set(buffer[byte_index]);
    byte_index++;
    bit_offset = 8 - bit_offset;

  }
  else{
    bit_offset -= max_value_length;
    if(bit_offset == -(max_value_length)){
      bit_offset = 8 - max_value_length;
      byte_index++;
    }
    if(bit_offset < 0){
      bit_offset = -(bit_offset);
  	  buffer[byte_index] |= pack_value >> bit_offset;
    	std::bitset<8> set1(buffer[byte_index]);
    	byte_index++;
    	bit_offset = (8 - bit_offset);
    }
  }
  buffer[byte_index] |= pack_value << bit_offset;
  std::bitset<8> set(buffer[byte_index]);
}


bool Bitpacker::unpack(bool& value_read){
  bool out;
  read_bit_offset -= 1;
  if(read_bit_offset < 0){
    read_bit_offset = 7;
    read_byte_index++;
  }
  out = (buffer[read_byte_index] >> read_bit_offset) & 1;
  if(out == 1)
    value_read = true;
  else
    value_read = false;
  return value_read;
}

int Bitpacker::unpack(int& value_read, int min_value, int max_value){
  int out = 0, value1, value2;
  int u_max_value_length = 0;
  int v = 1, and_value;
  for(int i = 0; i < (sizeof(max_value) * 8) - 1; ++i){
      if(v > max_value)
        break;
      v <<= 1;
      u_max_value_length++;
  }
  if(u_max_value_length > 8){
    and_value = pow(2, u_max_value_length) - 1;
    read_bit_offset = u_max_value_length - read_bit_offset;
    if(read_bit_offset > 8){
      read_bit_offset = u_max_value_length - 8;
      read_byte_index++;
    }
    out = (buffer[read_byte_index] << read_bit_offset) & and_value;
    read_byte_index++;
    read_bit_offset = 8 - read_bit_offset;
    std::cout << out << std::endl;
    and_value = pow(2, (8 - read_bit_offset)) - 1;
    out += buffer[read_byte_index] >> read_bit_offset & and_value;
  }
  else{
    and_value = pow(2, u_max_value_length) - 1;
    read_bit_offset -= u_max_value_length;

    if(read_bit_offset == -(u_max_value_length)){
      read_bit_offset = 8 - u_max_value_length;
      read_byte_index++;
    }

    if(read_bit_offset < 0){
      read_bit_offset = -(read_bit_offset);
      out = (buffer[read_byte_index] << read_bit_offset) & and_value;
      read_byte_index++;
      read_bit_offset = 8 - read_bit_offset;
      and_value = pow(2, (8 - read_bit_offset)) - 1;
      out += buffer[read_byte_index] >> read_bit_offset & and_value;
    }
    else
      out = (buffer[read_byte_index] >> read_bit_offset) & and_value;
    }

  value_read = out;
  return value_read;

}


int main(){
  Bitpacker obj;
  int min = 0, max = 3000; // Change values between 0 and 65535
  obj.pack(200, min, max);
  obj.pack(509, min, max);
  obj.pack(7, min, max);
  obj.pack(2, min, max);
  obj.pack(3, min, max);

  int value_read;

  for(int i = 0; i < 5; i++){
    obj.unpack(value_read, min, max);
    std::cout << "Value read: " << value_read << std::endl;
  }

  for(int i = 0; i < 3; i++){
    obj.pack(true);
    obj.pack(false);
    obj.pack(true);
  }

  bool bool_value_read;
  for(int j = 0; j < 9; j++){
    obj.unpack(bool_value_read);
    std::cout << std::boolalpha << bool_value_read << std::endl;
  }


}
