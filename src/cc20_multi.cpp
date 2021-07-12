/*
cc20_multi.cpp

pdm/Personal Data Management system is a encrypted and multiplatform data searching, building, archiving tool.
This is the encryption core module for pdm.

author:     Yi Yang
            5/2021
*/
#ifndef CC20_MULTI_CPP_
#define CC20_MULTI_CPP_ 


#include "cc20_dev.cpp"
#include "cc20_multi.h"
#include "../lib/sha3.cpp"
#include <thread>
#include <numeric>
#include <unistd.h>


using namespace std;

void enc_writing(string oufile_name);
void enc_writing_nw();
void multi_enc_pthrd(int thrd);
void multi_enc_pthrd_nw(int thrd);
void set_thread_arg(int thrd, long int np,long int tracker,long int n, long int tn,uint8_t* line,uint32_t count, Cc20 * ptr);
       

int ENABLE_SHA3_OUTPUT = 1;

const int BLOCK_SIZE = 4608000;
/* Invariant: BLOCK_SIZE % 64 == 0
                                 115200, 256000, 576000, 1152000,2304000,4608000,6912000,9216000 ...
                                 Block size*/

const int THREAD_COUNT = 30; // Make sure to change the header file's too.

const int PER_THREAD_BACK_LOG = 0; // This is not enabled.

uint32_t folow[THREAD_COUNT][17]; // A copy of a state.

// Statically allocates, and uses BLOCK_SIZE*THREAD_COUNT of memory. 
char thread_track[THREAD_COUNT][BLOCK_SIZE] = {{0}};

long int writing_track[THREAD_COUNT]; // Tells the writer thread how much to read; should only be different on the last block.

char *linew; // Tracks all the input

int progress_bar[THREAD_COUNT];

int DISPLAY_PROG =1;

long int arg_track[THREAD_COUNT][6];
/* Passes arguments into threads.
                                       arg_track[THREAD_COUNT][0] ---> Thread number
                                       arg_track[THREAD_COUNT][1] ---> NOT USED
                                       arg_track[THREAD_COUNT][2] ---> NOT USED
                                       arg_track[THREAD_COUNT][3] ---> Remaining plain size
                                       arg_track[THREAD_COUNT][4] ---> NOT USED*/

SHA3 hashing; // A rolling hash of the input data.

uint8_t * arg_line[THREAD_COUNT]; // Addresses of memory mapped plain text from disk.

uint32_t arg_count[THREAD_COUNT]; // Count of each chacha 20 block

Cc20 * arg_ptr[THREAD_COUNT]; // Parent pointers for each thread.

// recursive_mutex locks[THREAD_COUNT]; // All locks for threads, each waits for the writing is done on file or memory.

thread threads[THREAD_COUNT]; // Threads

char ** outthreads;

int final_line_written = 0; // Whether or not the fianl line is written

// mutex mtx;

/*
    XOR's two objects begaining at s1's off for n.
    And beginging at s2's 0 for n.

*/

template < typename NU >
  void set_xor(NU * s1, NU * s2, std::ofstream s3, unsigned int n, unsigned int off) {
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
  #ifdef ROUNDCOUNTTWLV
  for (unsigned int i = 0; i < 6; i++) tworounds(folow[thrd]); // 8 rounds
  #else
  for (unsigned int i = 0; i < 10; i++) tworounds(folow[thrd]); // 20 rounds
  #endif
  set_conc(cy[thrd], folow[thrd], 16);
  endicha(this -> nex[thrd], folow[thrd]);
}

/*
    Reads from line writes to linew, encryptes the same as rd_file_encr().

*/

void Cc20::encr(uint8_t*line,uint8_t*linew,unsigned long int fsize) {
  
  unsigned long int n = fsize;

  long int tn = 0;
  uint32_t count = 0;
  for (long int i = 0; i < THREAD_COUNT; i++) {
    writing_track[i] = 0;
  }
  long int tracker = 0;
  long int np = 0, tmpn = np % THREAD_COUNT;
  set_thread_arg(np % THREAD_COUNT, (long int)linew, tracker, n, 0, line, count, this);
  threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, tmpn);
  np++;
  
  for (unsigned long int k = 0; k < ((unsigned long int)(fsize / 64) + 1); k++) { // If leak, try add -1

    if (n >= 64) {
      tracker += 64;
      if (tn % (BLOCK_SIZE) == 0 && (k != 0)) {
        if (threads[np % THREAD_COUNT].joinable()) {
          #ifdef VERBOSE
          cout << "[main] Possible join, waiting " <<np % THREAD_COUNT<< endl;
          #endif
          threads[np % THREAD_COUNT].join();
        }
        set_thread_arg(np % THREAD_COUNT, (long int)linew+tn, tracker, n, tn, line + tn, count + 1, this);
        threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, np % THREAD_COUNT);

        tracker = 0;
        np++;
      }
    } else {
      if (threads[np % THREAD_COUNT].joinable() && final_line_written != 1) {
          #ifdef VERBOSE
          cout << "[main] Last Possible join, waiting " <<np % THREAD_COUNT<< endl;
          #endif
        threads[np % THREAD_COUNT].join();
      }
      set_thread_arg(np % THREAD_COUNT, (long int)linew+tn, tracker, n, tn, line + tn, count + 1, this);
      threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, np % THREAD_COUNT);
    }
    count += 1;
    n -= 64;
    tn += 64;
  }
  #ifdef VERBOSE
  cout << "[main] Finished dispatching joining" << endl;
  #endif
  
  for (int i = 0; i < THREAD_COUNT; i++) {
    // cout<<"Trying"<<endl;
    if (threads[i].joinable()){

      // cout << "[main] thread joining "<< i << endl;
      threads[i].join();

    }
  }
  if(ENABLE_SHA3_OUTPUT){
    #ifndef DE
    hashing.add(line,fsize );
    #else 
    hashing.add(linew,fsize );
    #endif // DE
  }
}

/*
    Creates one thread for writing and THREAD_COUNT threads for calculating the 
    cypher text. It also handles disk mapping for reading, and opens oufile for 
    writing. After, it will dispatch threads when there are vacancy in threads[].
    Returns when all plain is read, and all threads are joined.

*/

void Cc20::rd_file_encr(const std::string file_name, string oufile_name) {
  std::vector < uint8_t > content;
  unsigned long int n = 0;

  struct stat sb;
  long int fd;
  uint8_t * data;
  uint8_t * line; 
  fd = open(file_name.data(), O_RDONLY); // Reading file
  if (fd == -1) {
    perror("Cannot open file ");
    cout << file_name << " ";
    exit(1);
  }


  fstat(fd, & sb);

  #ifdef VERBOSE
  cout << "Staring file size " << sb.st_size << endl;
  #endif
  linew = new char[sb.st_size];
  data = (uint8_t * )(mmap( 0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  line = data;
  long int tn = 0;
  n = sb.st_size;
  unsigned int ttn = sb.st_size;
  uint32_t count = 0;
  for (long int i = 0; i < THREAD_COUNT; i++) {
    writing_track[i] = 0;
  }
  long int tracker = 0;
  long int np = 0, tmpn = np % THREAD_COUNT;
  
  #ifdef DE
  ttn-=12;
  line=line+12;
  #endif
  thread progress;
  if (DISPLAY_PROG){
    for (unsigned int i=0; i<THREAD_COUNT;i++){
      progress_bar[i] = 0;
    }
    progress = thread(display_progress,ttn);
  }
  
  set_thread_arg(np % THREAD_COUNT, (long int)linew, tracker, n, 0, line, count, this);
  threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, tmpn);
  np++;
  
  for (unsigned long int k = 0; k < ((unsigned long int)(ttn / 64) + 1); k++) { // If leak, try add -1

    if (n >= 64) {
      tracker += 64;
      if (tn % (BLOCK_SIZE) == 0 && (k != 0)) {
        if (threads[np % THREAD_COUNT].joinable()) {
          #ifdef VERBOSE
          cout << "[main] Possible join, waiting " <<np % THREAD_COUNT<< endl;
          #endif
          threads[np % THREAD_COUNT].join();
        }
        set_thread_arg(np % THREAD_COUNT, (long int)linew+tn, tracker, n, tn, line + tn, count + 1, this);
        threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, np % THREAD_COUNT);

        tracker = 0;
        np++;
      }
    } else {
      if (threads[np % THREAD_COUNT].joinable() && final_line_written != 1) {
          #ifdef VERBOSE
          cout << "[main] Last Possible join, waiting " <<np % THREAD_COUNT<< endl;
          #endif
        threads[np % THREAD_COUNT].join();
      }
      set_thread_arg(np % THREAD_COUNT, (long int)linew+tn, tracker, n, tn, line + tn, count + 1, this);
      threads[np % THREAD_COUNT] = thread(multi_enc_pthrd, np % THREAD_COUNT);
    }
    count += 1;
    n -= 64;
    tn += 64;
  }
  #ifdef VERBOSE
  cout << "[main] Finished dispatching joining" << endl;
  #endif
  
  for (int i = 0; i < THREAD_COUNT; i++) {
    // cout<<"Trying"<<endl;
    if (threads[i].joinable()){

      // cout << "[main] thread joining "<< i << endl;
      threads[i].join();

    }
  }
  if (ENABLE_SHA3_OUTPUT){
    #ifndef DE
    hashing.add(line,ttn );
    #else 
    hashing.add(linew,ttn );
    #endif // DE
  }
  FILE * oufile;
  oufile = fopen(oufile_name.data(), "wb");
  fclose(oufile);
  oufile = fopen(oufile_name.data(), "ab");
  #ifndef DE
  // cout<<"nonce_orig: "<<this->nonce_orig <<endl;
  fwrite(this->nonce_orig, sizeof(char), 12, oufile);

  #else

  #endif
  fwrite(linew, sizeof(char), ttn, oufile);
  fclose(oufile);

  #ifdef VERBOSE
  cout << "[main] Writing thread joined" << endl;
  #endif

  if (oufile_name == "a") {
    for (unsigned int i = 0; i < ttn / BLOCK_SIZE + 1; i++) {
      delete[] outthreads[i];
    }
    delete[] outthreads;
  }
  delete[] linew;
  if (munmap(data,ttn)!=0)
    fprintf(stderr,"Cannot close");
  close(fd);
  if(DISPLAY_PROG){
    if (progress.joinable())
      progress.join();
  }
}
/**
 * Displays progress
 * 
 * */
void display_progress(unsigned int n) {
  unsigned int current=0;
  unsigned int acum=0;
  unsigned int res =50;
  cout<<endl;
  while(current<res){
    acum=0;
    if(((float)accumulate(progress_bar,progress_bar+THREAD_COUNT,acum)/n) *res>=current){
      current++;
      cout<<"-"<<flush;
    }
    usleep(10000);
  }
  cout<<"100%"<<endl;
}



/*
    Sets arguments in arg_track for threads.

*/

void set_thread_arg(int thrd, long int linew1, long int tracker, long int n, long int tn, uint8_t * line, uint32_t count, Cc20 * ptr) {
  arg_track[thrd][0] = thrd;
  arg_track[thrd][1] = linew1; 
  arg_track[thrd][2] = tracker; 
  arg_track[thrd][3] = n;

  arg_line[thrd] = line;
  arg_count[thrd] = count;
  arg_ptr[thrd] = ptr;
}

void multi_enc_pthrd(int thrd) {
  uint8_t * linew1 = (uint8_t*)arg_track[thrd][1]; // Set but not used
  long int tracker = 0; // Used
  long int n = arg_track[thrd][3]; // Used 
  uint8_t * line = arg_line[thrd]; // Used
  uint32_t count = arg_count[thrd]; // Used 
  Cc20 * ptr = arg_ptr[thrd];

  #ifdef VERBOSE
          cout<<"[calc] "<<thrd<<" locks, starting write " << endl;
  #endif
  for (unsigned long int k = 0; k < BLOCK_SIZE / 64; k++) {
    ptr -> one_block((int) thrd, (int) count);
    #ifdef VERBOSE
      cout<<"[calc] "<<thrd<<" 1 iteration, current size "<<n << endl;
    #endif
    if (n >= 64) {
      for (long int i = 0; i < 64; i++) {
        linew1[i + tracker] = (char)(line[i + tracker] ^ ptr -> nex[thrd][i]);
      }

      tracker += 64;
      if (tracker >= (BLOCK_SIZE)) { // Notifies the writing tread when data can be read
        if (msync(linew1, tracker, MS_SYNC) == -1)
        {
        }
        writing_track[thrd] = tracker;
        tracker = 0;
        #ifdef VERBOSE
          cout<<"[calc] "<<thrd<<" returning lock, calling write, size "<<writing_track[thrd] << endl;
        #endif
      }
    } else {
      for (int i = 0; i < n; i++) {
        linew1[i+tracker] = (char)(line[i + tracker] ^ ptr -> nex[thrd][i]);
      }
      tracker += n;
      writing_track[thrd] = tracker; // Notifies the writing tread when data can be read
      if (msync(linew1, tracker, MS_SYNC) == -1)
      {
      }
        #ifdef VERBOSE
        cout<<"[calc] "<<thrd<<" on last lock, size "<<writing_track[thrd]<< endl;
        #endif
      break;
    }
    count += 1;
    n -= 64;
    if(DISPLAY_PROG)progress_bar[thrd]+=64;
  }
  #ifdef VERBOSE
          cout<<"[calc] "<<thrd<<" unlocks " << endl;
  #endif
}


void Cc20::set_vals(uint8_t * nonce, uint8_t * key) {
  this -> nonce = nonce;
  copy(nonce,nonce+12,this -> nonce_orig );
  this -> count = 0;
  for (unsigned int i = 0; i < THREAD_COUNT; i++) {
    // this -> cy[i][0] = 0x617178e5;
    // this -> cy[i][1] = 0xb72c676e;
    // this -> cy[i][2] = 0x79e2ad32;
    // this -> cy[i][3] = 0x6b246574;

    this -> cy[i][0] = 0x61707865;
    this -> cy[i][1] = 0x3320646e;
    this -> cy[i][2] = 0x79622d32;
    this -> cy[i][3] = 0x6b206574;

    expan(this -> cy[i], 13, this -> nonce, 3);
    expan(this -> cy[i], 4, key, 8);
  }
}

void Cc20::endicha(uint8_t * a, uint32_t * b) {
  for (unsigned int i = 0; i < 16; i++) {
    U32T8_S(a + 4 * i, b[i]);

  }
}

/**
 * Init encryption.
 * 
 * This version of pdm-crypt interfaces within memory, which means 
 * entire file will be read before the encryption. 
 * Thus, this version is not recommended for large files (more than half 
 * of your computer's memory).
 * For a memory effecient version, please use a history version (that version 
 * uses at most ~320 mb for an arbitrary-size file).
 * 
 * @param infile_name input file name
 * @param oufile_name output file name
 * @param nonce the nonce of this encryption
 * 
 * */
void cmd_enc(string infile_name, string oufile_name, string text_nonce){
  // cout<<infile_name<<","<<oufile_name<<","<<text_nonce<<"\n"<<endl;
  Cc20 cry_obj;
  string text_key;
  Bytes key;
  Bytes nonce;

  // boost::algorithm::trim(infile_name);

  #ifdef LINUX
  termios oldt;
  tcgetattr(STDIN_FILENO, & oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, & newt);
  cout << "Password/密码： " << endl;
  std::getline(std::cin, text_key);
  tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
  cout << endl;
  #endif

  #ifdef WINDOWS
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hStdin, & mode);
  SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
  cout << "Password/密码： " << endl;
  std::getline(std::cin, text_key);
  #endif

  
  SHA3 key_hash;
  key_hash.add(stob(text_key).data(),text_key.size());
  key_hash.add(stob(text_key).data(),text_key.size());

  #ifdef DE
  uint8_t *line1[13]={0};
  string infile_name_copy = infile_name+".pdm";
  FILE * infile = fopen(infile_name_copy.data(), "rb");
  fread(line1,sizeof(char), 12,infile);
  if(line1!=NULL)
    text_nonce=(char*)line1;
  fclose(infile);

  #endif

  if (text_nonce.size() != 0) {
    text_nonce = pad_to_key((string) text_nonce, 12);
  }

  // Timer
  auto start = std::chrono::high_resolution_clock::now();
  // cout<<"before: "<<text_nonce.data()<<endl;
  cry_obj.set_vals((uint8_t*)text_nonce.data(), (uint8_t *)key_hash.getHash().data());


  #ifdef DE
  cry_obj.rd_file_encr(infile_name_copy,"dec-"+infile_name);
  if (ENABLE_SHA3_OUTPUT) cout <<"SHA3: \""<<hashing.getHash()<<"\""<<endl;

  #else
  cry_obj.rd_file_encr(infile_name, infile_name+".pdm");
  if (ENABLE_SHA3_OUTPUT) cout <<"SHA3: \""<<hashing.getHash()<<"\""<<endl;
  #endif //END DE
  auto end = std::chrono::high_resolution_clock::now();
  auto dur = end - start;
  auto i_millis = std::chrono::duration_cast < std::chrono::milliseconds > (dur);
  auto f_secs = std::chrono::duration_cast < std::chrono::duration < float >> (dur);
  std::cout << f_secs.count() << '\n';
}

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

void set_config(char*inp){
  string a = inp;
  for(unsigned int i=0;i<a.size();i++){
    if (a[i] == 's' ) ENABLE_SHA3_OUTPUT = 0;
    else if (a[i] == 'h' ) DISPLAY_PROG = 0;
  }
}

int rd_inp(unsigned int argc, char ** argv, string *infile){
  int arg_c=1;
  for (unsigned int i = 1; i< argc;i++){
    if (argv[i][0] == '-'){
      set_config(argv[i]);
    }
    else{
      if (infile->empty()){
        arg_c++;
        *infile = argv[i];
      }
      else
        return 0;
    }
  }
  if(!ENABLE_SHA3_OUTPUT)
    cout<<"sha3 output disabled"<<endl;
  return arg_c;
}


int _main_cc20(int argc, char ** argv) {
  string infile,oufile,nonce;
  if (rd_inp(argc,argv,&infile)!=2){
    cout<<argc<<" Wrong input; Should have 1 file input!\n"<<endl;
    return 0;
  }
  Bytes cur ;
  init_byte_rand_cc20(cur,12);
  nonce="1";
  cmd_enc(infile,"",btos(cur));
  return 0;
}

#endif