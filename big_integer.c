/*
 *  CSE202: Big Integer Manipulation Program
 *  Full name: Nicholas Carnevale
 *  Full Lehigh Email Address: nic225@lehigh.edu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//big_integer structure
struct two_quadwords
{
  unsigned long lsq;
  long msq;
};

typedef struct two_quadwords big_integer;

//union structure
union value
{
  big_integer binary;
  char hex[16];
};

//read big_integer string and store in union
int read_big_integer(union value *v, char *input) 
{
  // ensure that the input length is 32 characters
  if (strlen(input) != 32) 
  {
    fprintf(stderr, "Invalid input length: %s\n", input);
    return 0;
  }

  // initialize big_integer from the input
  for (int i = 0; i < 16; i++) 
  {
    char hex[3];
    hex[0] = input[i * 2];
    hex[1] = input[i * 2 + 1];
    hex[2] = '\0';

    char *endptr; // check for errors with pointer
    long value = strtol(hex, &endptr, 16); // convert to hex

    // any errors?
    if (*endptr != '\0') {
      printf("Invalid hex value: %s\n\n", input);
      return 0;
    }
    v->hex[15 - i] = value; // little endian
  }
  return 1; 
}

//not used
char hexDigit(char c);

//print big_integer
void write_big_integer(union value b)
{
  for (int i = 15; i > 0; i -= 2) // reading little endian form
  {
    printf("%02x", (unsigned char)b.hex[i]);
    printf("%02x ", (unsigned char)b.hex[i - 1]);
  }
}

// & both parts of big_integer
void and_big_integers(big_integer b1, big_integer b2, big_integer *b1_and_b2)
{
  b1_and_b2->lsq = b1.lsq & b2.lsq;
  b1_and_b2->msq = b1.msq & b2.msq;
}

// | both parts of big_integer
void or_big_integers(big_integer b1, big_integer b2, big_integer *b1_or_b2)
{
  b1_or_b2->lsq = b1.lsq | b2.lsq;
  b1_or_b2->msq = b1.msq | b2.msq;
}

// ^ both parts of big_integer
void xor_big_integers(big_integer b1, big_integer b2, big_integer *b1_xor_b2)
{
  b1_xor_b2->lsq = b1.lsq ^ b2.lsq;
  b1_xor_b2->msq = b1.msq ^ b2.msq;
}

// ~ both parts of big_integer
void not_big_integer(big_integer *b)
{
  b->lsq = ~b->lsq;
  b->msq = ~b->msq;
}

// << shift left
void shift_big_integer_left(big_integer *b, unsigned k){
  if (k < 64)
  {
    b->lsq = b->lsq << k;
    b->msq = b->msq << k;
    if (k != 0)
    {
      b->msq += b->lsq >> (64 - k);
    }
  }
  else // need different behavior if over 64
  {
    b->msq = b->msq << (k - 64);
    b->lsq = 0;
  }
}

// >> shift right
void shift_big_integer_right(big_integer *b, unsigned k){
  if (k < 64)
  {
    b->lsq = b->lsq >> k;
    b->msq = b->msq >> k;
    if (k != 0)
    {
      b->lsq += ((b->msq & ((1UL << k) - 1)) << (64 - k));
    }
  }
  else // need different behavior if over 64
  {
    b->lsq = b->msq >> (k - 64);
    b->msq = b->msq >> 63;
  }
}

// sum two big_integers
int add_big_integers(big_integer b1, big_integer b2, big_integer *sum)
{
  sum->lsq = b1.lsq + b2.lsq;
  sum->msq = b1.msq + b2.msq;

  //check for overflow
  if (b1.lsq > sum->lsq){sum->msq += 1;}

  //return 1 if overflow
  if ((b1.msq > 0 && b2.msq > 0 && sum->msq < 0) || (b1.msq < 0 && b2.msq < 0 && sum->msq > 0)){return 1;}

  return 0;
}

//not used
void print_usage(const char *message, const char* value);


// MAIN METHOD
int main(int argc, char* argv[])
{
  //VAR
  char* function = argv[1]; 
  union value value1;
  union value value2;
  union value result;
  
  //PARSING

  //NOT
  if(strcmp(function, "not") == 0) 
  { 
    if(argc == 3)
    { 
      //read integer
      if(read_big_integer(&value1, argv[2]) == 1)
      {
        //perform printing and evaluation
        printf("~ ");
        write_big_integer(value1);
        not_big_integer(&value1.binary);
        printf(" = ");
        write_big_integer(value1);
        printf("\n\n");
      }
      else {exit(0);}
    } 
    else {printf("Invalid number of arguments for 'not'\n"); exit(0);}
  }
  
  //SHIFT LEFT
  else if(strcmp(function, "sl") == 0) 
  {
    if (argc == 4) 
    {
      //store num shifts
      unsigned k = strtoul(argv[2], NULL, 10);
      //read integer
      if(read_big_integer(&value1, argv[3]) == 1)
      {
        //perform printing and evaluation
        write_big_integer(value1);
        printf(" << %d = ", k);
        shift_big_integer_left(&value1.binary, k);
        write_big_integer(value1);
        printf("\n\n");
      }
      else {exit(0);}
    } 
    else {printf("Invalid number of arguments for 'sl'\n"); exit(0);}
  } 
  
  //SHIFT RIGHT
  else if (strcmp(function, "sr") == 0) 
  {
    if (argc == 4) 
    {
      //store num shifts
      unsigned k = strtoul(argv[2], NULL, 10);
      //read integer
      if(read_big_integer(&value1, argv[3]) == 1)
      {
        //perform printing and evaluation
        write_big_integer(value1);
        printf(" >> %d = ", k);
        shift_big_integer_right(&value1.binary, k);
        write_big_integer(value1);
        printf("\n\n");
      }
      else {exit(0);}
    } 
    else {printf("Invalid number of arguments for 'sr'\n"); exit(0);}
  } 
  
  //AND
  else if (strcmp(function, "and") == 0) 
  {
    if (argc == 4)
    { 
      //read integers
      if(read_big_integer(&value1, argv[2]) == 1 && read_big_integer(&value2, argv[3]) == 1)
      {
        //perform printing and evaluation
        write_big_integer(value1);
        printf(" &\n");
        write_big_integer(value2);
        printf(" =\n");
        and_big_integers(value1.binary, value2.binary, &result.binary);
        write_big_integer(result);
        printf("\n\n");
      }
      else {exit(0);}
    }
    else {printf("Invalid number of arguments for 'and'\n"); exit(0);}
  } 
  
  //OR
  else if (strcmp(function, "or") == 0) 
  {
    if (argc == 4) 
    {
      //read integers
      if(read_big_integer(&value1, argv[2]) == 1 && read_big_integer(&value2, argv[3]) == 1)
      {
        //perform printing and evaluation
        write_big_integer(value1);
        printf(" |\n");
        write_big_integer(value2);
        printf(" =\n");
        or_big_integers(value1.binary, value2.binary, &result.binary);
        write_big_integer(result);
        printf("\n\n");
      }
      else {exit(0);} 
    } 
    else {printf("Invalid number of arguments for 'or'\n"); exit(0);}
  } 
  
  //XOR
  else if (strcmp(function, "xor") == 0) 
  {
    if (argc == 4) 
    {
      //read integers
      if(read_big_integer(&value1, argv[2]) == 1 && read_big_integer(&value2, argv[3]) == 1)
      {
        //perform printing and evaluation
        write_big_integer(value1);
        printf(" ^\n");
        write_big_integer(value2);
        printf(" =\n");
        xor_big_integers(value1.binary, value2.binary, &result.binary);
        write_big_integer(result);
        printf("\n\n");
      }
      else {exit(0);}
    } 
    else {printf("Invalid number of arguments for 'xor'\n"); exit(0);}
  } 
  
  //ADD
  else if (strcmp(function, "add") == 0) 
  {
    if (argc == 4) 
    {
      //read integers
      if(read_big_integer(&value1, argv[2]) == 1 && read_big_integer(&value2, argv[3]) == 1 )
      {
        //perform printing and evaluation        
        write_big_integer(value1);
        printf(" +\n");
        write_big_integer(value2);
        printf(" =\n");
        int overflow = add_big_integers(value1.binary, value2.binary, &result.binary);
        write_big_integer(result);
        if(overflow == 1){printf(" - Overflow");}
        printf("\n\n");
      }
      else {exit(0);}
    } 
    else {printf("Invalid number of arguments for 'xor'\n"); exit(0);}
  }

  //INVALID
  else {printf("Invalid operation: %s\n", function); exit(0);}

  return 0;
}
