#pragma once
#pragma warning(disable : 4996)
#include "cc20_multi.h"
#include "cc20_dev.hpp"
#include "cMain.h"

#include "pdm-service.h"
#include <iostream>
#include <climits>
#include <ostream>
#include <random>
#include <wchar.h>

#ifdef DEEP_DEBUG
#include <iomanip>
#include <iostream>
#endif

#ifdef MACOS
#include <unistd.h>
#include <termios.h>
#endif

#ifdef LINUX
#include <unistd.h>
#include <termios.h>
#endif

#ifdef WINDOWS
#include <locale.h>
#include <windows.h>
#define _MBCS
//#include <io.h>
//#include <fcntl.h>

#endif

using namespace std;
unsigned int i = 0;


//void stream(uint8_t* key, uint8_t* nonce, uint32_t count, uint8_t* plain, unsigned int len);

#define U32T8_S(p, v)    \
  {                            \
    (p)[0] = (v >> 0) & 0xff;  \
    (p)[1] = (v >> 8) & 0xff;  \
    (p)[2] = (v >> 16) & 0xff; \
    (p)[3] = (v >> 24) & 0xff; \
  }

#define U8T32_S(p)                              \
  (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) | \
   ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))

// INT should only be unsigned, no checks here.
template <typename NT>
void roln(NT& val, unsigned int n) {
    val = (val << n) | (val >> (8 - n));
}

template <>
void roln<uint32_t>(uint32_t& val, unsigned int n) {
    val = (val << n) | (val >> (8 - n));
}

//void endicha(uint8_t* a, uint32_t* b) {
//    for (i = 0; i < 16; i++) {
//        U32T8_S(a + 4 * i, b[i]);
//
//    }
//}

void expan(uint32_t* ot, unsigned int off, const uint8_t* in, unsigned int n) {
    for (i = 0; i < n; i++) {
        ot[off + i] = U8T32_S(in + 4 * i);
    }
}

// Operate a quarter-round chacha state on total of 16 bytes or 4 32-bit numbers at a time.
void quarteround(uint32_t* state, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    state[a] += state[b]; state[d] ^= state[a]; roln(state[d], 16);
    state[c] += state[d]; state[b] ^= state[c]; roln(state[b], 12);
    state[a] += state[b]; state[d] ^= state[a]; roln(state[d], 8);
    state[c] += state[d]; state[b] ^= state[c]; roln(state[b], 7);
}

void tworounds(uint32_t* state) {
    quarteround(state, 0, 4, 8, 12);
    quarteround(state, 1, 5, 9, 13);
    quarteround(state, 2, 6, 10, 14);
    quarteround(state, 3, 7, 11, 15);
    quarteround(state, 0, 5, 10, 15);
    quarteround(state, 1, 6, 11, 12);
    quarteround(state, 2, 7, 8, 13);
    quarteround(state, 3, 4, 9, 14);
}
#ifdef PRINTING
// Print a hex unsigned number
template <typename NT>
void p_hex(NT i) {
    cout << " 0x"; cout << setfill('0') << setw(8) << hex << right << i << flush;

}
template <>
void p_hex<uint8_t>(uint8_t i) {
    cout << dec << i << flush;
}

// Print a chacha state
template <typename NT>
void p_state(NT* state) {
    for (i = 0; i < 16; i++) {
        p_hex(state[i]);
        if ((i + 1) % 4 == 0)cout << "\n";
    }
    cout << endl;
}
template <>
void p_state<uint8_t>(uint8_t* state) {
    int n = 16;
    for (unsigned int i = 0; i < 64; i++) {
        // if((i+1)%n==0)cout<<setw(1)<<right<<"\t";
        p_hex(state[i]);
        if ((i + 1) % n == 0)cout << "\n";
    }
    cout << endl;
}
#endif

template<typename NT>
void state_cpy(NT* a, NT* b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) a[i] = b[i];
}

/*
    Require: key has 8, nonce has 3, and count has 1 elements.
*/
// void one_block (uint32_t*state, uint8_t* nex){
//     uint32_t folow[17];
//     memcpy(folow,state,sizeof(uint32_t)*16);
//     for (unsigned int i=0; i<10;i++) tworounds(folow);      // 20 rounds
//     set_conc(state,folow,16);
//     endicha(nex,folow);
// }



//                     8              3                1
//                     32             12               4
//                     key            nonce            count

// void chasetup(uint8_t * key, uint8_t * nonce, uint32_t count,uint8_t* nex, uint32_t*cy){
//     cy[12] = count;
//     one_block(cy,nex);
// }



void init_byte(Bytes& a, int n) {
    for (int i = 0; i < n; i++) a.push_back((uint8_t)0);
}
void init_byte_rand(Bytes& a, int n) {
    for (int i = 0; i < n; i++) {
        random_device rd;   // non-deterministic generator
        mt19937 gen(rd());
        a.push_back((uint8_t)gen());
    }
}

/*
cc20_multi.cpp

pdm/Personal Data Management system is a encrypted and multiplatform data searching, building, archiving tool.
This is the encryption core module for pdm.

author:     Yi Yang
            5/2021
*/

//cc20_multi.cpp
//#include "cc20_multi.h"



const int BLOCK_SIZE = 576000;                  /* Invariant: BLOCK_SIZE % 64 == 0
                                                   115200, 256000, 576000, ...
                                                   Block size*/

const int THREAD_COUNT = 40;                    // Make sure to change the header file's too.

const int PER_THREAD_BACK_LOG = 0;              // This is not enabled.

uint32_t folow[THREAD_COUNT][17];               // A copy of a state.

                                                // Statically allocates, and uses BLOCK_SIZE*THREAD_COUNT of memory.
char thread_track[THREAD_COUNT][BLOCK_SIZE] = { {0} };

long int writing_track[THREAD_COUNT];           // Tells the writer thread how much to read; should only be different on the last block.

long int arg_track[THREAD_COUNT][6];            /* Passes arguments into threads.
                                                   arg_track[THREAD_COUNT][0] ---> Thread number
                                                   arg_track[THREAD_COUNT][1] ---> NOT USED
                                                   arg_track[THREAD_COUNT][2] ---> NOT USED
                                                   arg_track[THREAD_COUNT][3] ---> Remaining plain size
                                                   arg_track[THREAD_COUNT][4] ---> NOT USED*/

uint8_t* arg_line[THREAD_COUNT];               // Addresses of memory mapped plain text from disk.

uint32_t arg_count[THREAD_COUNT];               // Count of each chacha 20 block

Cc20* arg_ptr[THREAD_COUNT];                    // Parent pointers for each thread.

thread threads[THREAD_COUNT];                   // Threads


int final_line_written = 0;                     // Whether or not the fianl line is written

#define FILE_MAP_START 0
long int  BUFFSIZE = THREAD_COUNT * BLOCK_SIZE;
// mutex mtx;



/*
    XOR's two objects begaining at s1's off for n.
    And beginging at s2's 0 for n.

*/

template<typename NU>
void set_xor(NU* s1, NU* s2, std::ofstream s3, unsigned int n, unsigned int off) {
    for (unsigned int i = 0; i < n; i++) {
        s3 << s1[i + off] ^ s2[i];
    }
}


/*
    Given nonce is already set, one_block takes the thrd number and the block count and
    modifies nex[thrd] for the next block of chacha20.

    This doesn't track whether or not the count is increamented; thus, to ensure security
    please increament the count before passing it into one_block

*/

void Cc20::one_block(int thrd, uint32_t count) {
    cy[thrd][12] = count;
    memcpy(folow[thrd], cy[thrd], sizeof(uint32_t) * 16);
    for (unsigned int i = 0; i < 10; i++) tworounds(folow[thrd]);      // 20 rounds
    set_conc(cy[thrd], folow[thrd], 16);
    endicha(this->nex[thrd], folow[thrd]);
}



/*
    Creates one thread for writing and THREAD_COUNT threads for calculating the
    cypher text. It also handles disk mapping for reading, and opens oufile for
    writing. After, it will dispatch threads when there are vacancy in threads[].
    Returns when all plain is read, and all threads are joined.

*/

void  Cc20::rd_file_encr(const std::string file_name, string oufile_name) {
    std::vector<uint8_t> content;
    unsigned long int n = 0;



    struct stat sb;
    long int fd;
    uint8_t* data;
    uint8_t* line;

    //
    #ifdef __WXMSW__

    TCHAR* lpcTheFile = new TCHAR[file_name.size()+1]; // the file to be manipulated
    lpcTheFile[file_name.size()] = 0;
    std::copy(file_name.begin(),file_name.end(),lpcTheFile);


    HANDLE hMapFile;      // handle for the file's memory-mapped region
    HANDLE hFile;         // the file handle
    BOOL bFlag;           // a result holder
    DWORD dBytesWritten;  // number of bytes written
    DWORD dwFileSize;     // temporary storage for file sizes
    DWORD dwFileMapSize;  // size of the file mapping
    DWORD dwMapViewSize;  // the size of the view
    DWORD dwFileMapStart; // where to start the file map view
    DWORD dwSysGran;      // system allocation granularity
    SYSTEM_INFO SysInfo;  // system information; used to get granularity
    LPVOID lpMapAddress;  // pointer to the base address of the
                          // memory-mapped region

    int iViewDelta;       // the offset into the view where the data
                      //shows up

    hFile = CreateFile(lpcTheFile,
        GENERIC_READ ,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("hFile is NULL\n"));
        _tprintf(TEXT("Target file is %s\n"),
            lpcTheFile);
        return ;
    }

    GetSystemInfo(&SysInfo);
    dwSysGran = SysInfo.dwAllocationGranularity;

    if (BUFFSIZE > GetFileSize(hFile, NULL))
        BUFFSIZE = GetFileSize(hFile, NULL);

    wxMessageBox(wxString::Format(wxT("%i"), dwSysGran), wxString::Format(wxT("%i"), dwSysGran),wxOK);
    // Set the starting point of the file, will need to update this everytime we np%THREADCOUNT==0
    dwFileMapStart = (0 / dwSysGran) * dwSysGran;
    wxMessageBox(wxString::Format(wxT("%i"), dwSysGran), wxT("dwFileMapStart"), wxOK);
    _tprintf(TEXT("The file map view starts at %ld bytes into the file.\n"),
        dwFileMapStart);

    // Calculate the size of the file mapping view.
    dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
    wxMessageBox(wxString::Format(wxT("%i"), dwMapViewSize), wxT("dwMapViewSize"), wxOK);
    _tprintf(TEXT("The file map view is %ld bytes large.\n"),
        dwMapViewSize);

    // How large will the file mapping object be?
    dwFileMapSize = FILE_MAP_START + BUFFSIZE;
    wxMessageBox(wxString::Format(wxT("%i"), dwFileMapSize), wxT("dwFileMapSize"), wxOK);
    _tprintf(TEXT("The file mapping object is %ld bytes large.\n"),
        dwFileMapSize);

    // The data of interest isn't at the beginning of the
    // view, so determine how far into the view to set the pointer.
    iViewDelta = FILE_MAP_START - dwFileMapStart;
    wxMessageBox(wxString::Format(wxT("%i"), iViewDelta), wxT("delta"), wxOK);
    _tprintf(TEXT("The data is %d bytes into the view.\n"),
        iViewDelta);
    wxMessageBox(wxString::Format(wxT("%i"), GetFileSize(hFile, NULL)), wxT("File size"), wxOK);

    hMapFile = CreateFileMapping(hFile,          // current file handle
        NULL,           // default security
        PAGE_READONLY, // read/write permission
        0,              // size of mapping object, high
        dwFileMapSize,  // size of mapping object, low
        NULL);          // name of mapping object

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("hMapFile is NULL: last error: %d\n"), GetLastError());
        return;
    }

    lpMapAddress = MapViewOfFile(hMapFile,            // handle to
                                                  // mapping object
        FILE_MAP_READ, // read/write
        0,                   // high-order 32
                             // bits of file
                             // offset
        dwFileMapStart,      // low-order 32
                             // bits of file
                             // offset
        dwMapViewSize);      // number of bytes
                             // to map

    if (lpMapAddress == NULL)
    {
        _tprintf(TEXT("lpMapAddress is NULL: last error: %d\n"), GetLastError());
        return ;
    }

    data = (uint8_t*)lpMapAddress;

    n = GetFileSize(hFile, NULL);

    #elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
    fd = open(file_name.data(), O_RDONLY);
    if (fd == -1) {
        perror("Cannot open file "); cout << file_name << " "; exit(1);
    }
    fstat(fd, &sb);
    data = (uint8_t*)(mmap((caddr_t)0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    n = sb.st_size;

    #endif //__WXMSW__ else defined( __WXGTK__ ) || defined( __WXOSX__ ))


    line = data;
    long int tn = 0;


    uint32_t count = 0;
    for (long int i = 0; i < THREAD_COUNT; i++) { writing_track[i] = 0; }
    long int tracker = 0;
    long int np = 0, tmpn = np % THREAD_COUNT;
    thread rthread(enc_writing, oufile_name);

    set_thread_arg(np % THREAD_COUNT, np, tracker, n, 0, line, count, this);
    threads[np % THREAD_COUNT] = std::thread(multi_enc_pthrd, tmpn);
    np++;
    cout << "Staring file size " << n << endl;
    for (unsigned long int k = 0; k < ((unsigned long int)(n / 64) + 1); k++) {// If leak, try add -1

        if (n >= 64) {
            tracker += 64;
            if (tn % (BLOCK_SIZE) == 0 && (k != 0)) {
                if (threads[np % THREAD_COUNT].joinable()) {
                    threads[np % THREAD_COUNT].join();
                }
                set_thread_arg(np % THREAD_COUNT, np, tracker, n, tn, line + tn, count + 1, this);
                threads[np % THREAD_COUNT] = std::thread(multi_enc_pthrd, np % THREAD_COUNT);
                tracker = 0;
                np++;
            }
        }
        else {
            cout << "Uneven data reached, offset " << n << endl;
            if (threads[np % THREAD_COUNT].joinable()) {
                threads[np % THREAD_COUNT].join();
            }
            set_thread_arg(np % THREAD_COUNT, np, tracker, n, tn, line + tn, count + 1, this);
            threads[np % THREAD_COUNT] = std::thread(multi_enc_pthrd, np % THREAD_COUNT);
        }
        count += 1;
        n -= 64;
        tn += 64;
    }
    cout << "Finished dispatching joining" << endl;
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (threads[i].joinable())
            threads[i].join();
    }
    rthread.join();
    cout << "finished" << endl;
}


/*
    Sets arguments in arg_track for threads.

*/

void set_thread_arg(int thrd, long int np, long int tracker, long int n, long int tn, uint8_t* line, uint32_t count, Cc20* ptr) {
    arg_track[thrd][0] = thrd;
    arg_track[thrd][1] = np;        // NOT USED
    arg_track[thrd][2] = tracker;   // NOT USED
    arg_track[thrd][3] = n;
    arg_line[thrd] = line;
    arg_count[thrd] = count;
    arg_ptr[thrd] = ptr;
}

void multi_enc_pthrd(int thrd) {
    long int np = arg_track[thrd][1];
    long int tracker = 0;
    long int n = arg_track[thrd][3];
    uint8_t* line = arg_line[thrd];
    uint32_t count = arg_count[thrd];
    Cc20* ptr = arg_ptr[thrd];

    for (unsigned long int k = 0; k < BLOCK_SIZE / 64; k++) {
        ptr->one_block((int)thrd, (int)count);
        if (n >= 64) {
            if (writing_track[thrd] != 0) {

                cout << "pos1 waiting" << endl;
                while (true) {
                    if (writing_track[thrd] == 0) break;
                }

                cout << "pos1 " << endl;
            }
            // mtx.lock();
            for (long int i = 0; i < 64; i++)
            {
                thread_track[thrd][tracker + i] = (char)(line[i + tracker] ^ ptr->nex[thrd][i]);
            }

            tracker += 64;
            if (tracker >= (BLOCK_SIZE)) {
                writing_track[thrd] = tracker;
                tracker = 0;
                np++;
            }
            // mtx.unlock();
        }
        else {
            if (writing_track[thrd] != 0) {
                while (true) {
                    if (writing_track[thrd] == 0) break;
                }
            }
            // mtx.lock();
            for (int i = 0; i < n; i++)
            {
                thread_track[thrd][tracker + i] = (char)(line[i + tracker] ^ ptr->nex[thrd][i]);
            }
            tracker += n;
            writing_track[thrd] = tracker;
            // mtx.unlock();
            break;
        }
        count += 1;
        n -= 64;
    }
}

void enc_writing(string oufile_name) {
    FILE* oufile;
    #ifdef __WXMSW__
    errno_t err;

    err = fopen_s(&oufile,oufile_name.data(), "w");
    if (err != 0) {
        printf("The output file was not opened\n");
    }
    err = fclose(oufile);
    if (err != 0) {
        printf("The output file was not opened\n");
    }
    err = fopen_s(&oufile,oufile_name.data(), "ab");
    if (err != 0) {
        printf("The output file was not opened\n");
    }

    #elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))

    oufile = fopen(oufile_name.data(), "wb");
    fclose(oufile);
    oufile = fopen(oufile_name.data(), "ab");
    #endif

    long int local_np = 0;
    while (final_line_written != 1) {
        if (writing_track[local_np % THREAD_COUNT] != 0) {

            if (writing_track[local_np % THREAD_COUNT] >= BLOCK_SIZE) {
                // mtx.lock();
                cout << local_np << " Writing started " << writing_track[local_np % THREAD_COUNT] << endl;
                fwrite(thread_track[local_np % THREAD_COUNT], sizeof(char), writing_track[local_np % THREAD_COUNT], oufile);
                writing_track[local_np % THREAD_COUNT] = 0;
                // mtx.unlock();
                local_np++;
            }
            else {
                // mtx.lock();
                cout << local_np << " Writing ended " << writing_track[local_np % THREAD_COUNT] << endl;

                fwrite(thread_track[local_np % THREAD_COUNT], sizeof(char), writing_track[local_np % THREAD_COUNT], oufile);
                final_line_written = 1;
                fflush(oufile);
                // mtx.unlock();
            }
        }
    }
    fclose(oufile);
    cout << "Writing thread closed" << endl;
}

void Cc20::set_vals(uint8_t* nonce, uint8_t* key) {
    this->nonce = nonce;
    this->count = 0;
    for (unsigned int i = 0; i < THREAD_COUNT; i++)
    {
        this->cy[i][0] = 0x617178e5; this->cy[i][1] = 0xb72c676e; this->cy[i][2] = 0x79e2ad32; this->cy[i][3] = 0x6b246574;

        expan(this->cy[i], 13, this->nonce, 3);
        expan(this->cy[i], 4, key, 8);
    }
}


void Cc20::endicha(uint8_t* a, uint32_t* b) {
    for (unsigned int i = 0; i < 16; i++) {
        U32T8_S(a + 4 * i, b[i]);

    }
}


int encrypt_cc20(string infile_name) {
    //Encryption
    Cc20 test;
    std::string  oufile_name, text_key;
    Bytes key;
    Bytes nonce;

    oufile_name = "encrypted.pdm";
    //#ifdef HARD_CODE_DEBUG

     oufile_name = infile_name+".pdm";

     text_key = "1234";
     string tmp="11";
     if(tmp.size()!=0) {
         tmp = pad_to_key((string)tmp, 13);
         nonce = stob(tmp);
     }
     //init_byte_rand(nonce, 13);
     //#endif



    #ifdef LINUX
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cout << "输入密码： " << endl;
    std::getline(std::cin, text_key);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    #endif

    /*#ifdef WINDOWS
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    cout << "输入密码： " << endl;
    std::getline(std::cin, text_key);
    #endif*/

    /*string tmp;
    cout << "输入独立数字：" << endl;
    getline(cin, tmp);*/

    /*if (tmp.size() != 0) {
        tmp = pad_to_key((string)tmp, 13);
        nonce = stob(tmp);
    }*/
    text_key = pad_to_key(text_key, 32);
    key = stob(text_key);

    // #ifdef HARD_CODE_DEBUG

    auto start = std::chrono::high_resolution_clock::now();

    test.set_vals(nonce.data(), key.data());
    cout << "Read/Write encryption from file: " << infile_name << endl;

    #ifdef DE
    test.rd_file_encr(infile_name + ".pdm", infile_name);
    cout << "已解密: " << "dec-" + infile_name << endl;
    #else
    test.rd_file_encr(infile_name, infile_name + ".pdm");
    cout << _T("完成加密: ") << infile_name + _T(".pdm") << endl;
    #endif

    auto end = std::chrono::high_resolution_clock::now();

    auto dur = end - start;
    auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
    std::cout << f_secs.count() << '\n';
    start = std::chrono::high_resolution_clock::now();

    // #endif


    return 0;

}

int new_way(string usr_input) {

    //Encryption
    Cc20 test;
    std::string infile_name, oufile_name, text_key;
    Bytes key;
    Bytes nonce;

    oufile_name = "encrypted.pdm";
    // #ifdef HARD_CODE_DEBUG
    // infile_name = "test1.mp4";
    // if (usr_input.size() != 0){
    //     infile_name = usr_input;
    // }
    // oufile_name = "encrypted.pdm";

    // text_key = "1234";
    // string tmp="1";
    // if(tmp.size()!=0) {
    //     tmp = pad_to_key((string)tmp, 13);
    //     nonce = stob(tmp);
    // }
    // #endif

    #ifdef DE
    cout << "输入加密文件名： " << endl;
    #else
    cout << _T("输入文件名进行加密： ") << endl;
    #endif
    std::getline(std::cin, infile_name);

    #ifdef LINUX
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cout << "输入密码： " << endl;
    std::getline(std::cin, text_key);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    #endif

    #ifdef WINDOWS
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    cout << "输入密码： " << endl;
    std::getline(std::cin, text_key);
    #endif

    string tmp;
    cout << _T("输入独立数字：") << endl;
    getline(cin, tmp);

    if (tmp.size() != 0) {
        tmp = pad_to_key((string)tmp, 13);
        nonce = stob(tmp);
    }
    text_key = pad_to_key(text_key, 32);
    key = stob(text_key);

    // #ifdef HARD_CODE_DEBUG

    auto start = std::chrono::high_resolution_clock::now();

    test.set_vals(nonce.data(), key.data());
    cout << "Read/Write encryption from file: " << infile_name << endl;

    #ifdef DE
    test.rd_file_encr(infile_name + ".pdm", "dec-" + infile_name);
    cout << "已解密: " << "dec-" + infile_name << endl;
    #else
    test.rd_file_encr(infile_name, infile_name + ".pdm");
    cout << _T("完成加密: " )<< infile_name + ".pdm" << endl;
    #endif

    auto end = std::chrono::high_resolution_clock::now();

    auto dur = end - start;
    auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
    std::cout << f_secs.count() << '\n';
    start = std::chrono::high_resolution_clock::now();

    // #endif


    return 0;
}

//int main(int argc, char** argv) {
//    if (argc != 2) {
//        string usr_input = "";
//        cout << "new method: \n" << new_way(usr_input) << "\n\n";
//    }
//    else {
//        string usr_input(argv[1], (int)sizeof(argv[1]) / sizeof(char));
//        cout << "new method: \n" << new_way(usr_input) << "\n\n";
//    }
//    return 0;
//}
