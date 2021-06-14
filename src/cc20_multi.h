#pragma once
/*
cc20_multi.h

pdm/Personal Data Management system is a encrypted and multiplatform data searching, building, archiving tool.

author:     Yi Yang
            5/2021
*/


#include <thread>
//#include <stdio.h>
#include <chrono>
// Added 


#ifdef __WXMSW__
#include <windows.h>
#include <tchar.h>

#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
#include <sys/mman.h>
#endif
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <mutex>


using namespace std;


class Cc20 {


public:

    //void start_seq();
    void rd_file_encr(const std::string file_name, string oufile_name);
    //void stream(uint8_t* plain, unsigned int len);
    void set_vals(uint8_t* nonce, uint8_t* key);
    void one_block(int thrd, uint32_t count);
    void endicha(uint8_t* a, uint32_t* b);

    // Make sure this number is same as THREAD_COUNT
    //           *
    uint8_t nex[41][65];


private:

    uint8_t* nonce;

    uint32_t count;

    // Make sure this number is same as THREAD_COUNT
    //          *
    uint32_t cy[41][17];

    uint8_t* key;

    // Binary constant for chacha20 state, modified 
    unsigned long b1 = 0B01100001011100010111100011100101;
    unsigned long b2 = 0B10110111001011000110011101101110;
    unsigned long b3 = 0B01111001111000101010110100110010;
    unsigned long b4 = 0B01101011001001000110010101110100;
};


void enc_writing(string oufile_name);
void multi_enc_pthrd(int thrd);
void set_thread_arg(int thrd, long int np, long int tracker, long int n, long int tn, uint8_t* line, uint32_t count, Cc20* ptr);
int new_way(string usr_input);
int encrypt_cc20(string usr_input);
