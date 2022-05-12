#include "process.h"
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

/*
Thread 1: đếm số từ trong file 1.txt
 Thread 2: đếm số file có trong thư mục hiện tại
 Thread 3: đếm số thread của process ‘explorer.exe’
*/

static int winner = 1;
static int pos[3] = {0, 0, 0};

int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


void Thread1(){
   FILE *file = fopen("file1.txt", "r");
   char buffer[1000];
   memset(buffer, 0, sizeof(buffer));

   fread(buffer, 1, 1000, file);

   //cout << "\nthread1: Trong file file1.txt co " << strlen(buffer) << " ky tu";

   //cout << strlen(buffer);
   pos[0] += winner;
   winner += 1;
}

void Thread2(){
   vector<char *> files;
   DIR *dir; struct dirent *diread;

   int i = 0;
   
   char p[100];
   getcwd(p, 256);

   char p1[100]; memset(p1, 0, sizeof(p1));
   char p2[100]; memset(p2, 0, sizeof(p2));

   for (i = 0; i<strlen(p); i++){
      if((int)p[i] == 92)
         break;
   }

   int idx = 0;

   for (idx = 0; idx < i; idx++){
      p1[idx] = p[idx];
   }
   
   strcat(p1, "//");


   int bk = 0;
   for (idx = i + 1; idx < strlen(p); idx++){
      
      p2[bk] = p[idx];
      bk += 1;
   }

   strcat(p1, p2);
   char bkt[256];
   //p1: path se dung
   int counter = 0;
   if ((dir = opendir(p1)) != nullptr) {
      while ((diread = readdir(dir)) != nullptr) {
         memset(bkt, 0, sizeof(bkt));
         for(i = 0; i < strlen(p1); i++){
            bkt[i] = p1[i];
         }
         strcat(bkt, "//");

         
         if(is_regular_file(strcat(bkt, diread->d_name))){
            counter += 1;
         }
      
      }
      closedir (dir);
      //cout << "\nThread2: Trong folder " << p1 << " co tong so " << counter << " file";
   } 
   else{
      perror ("opendir");
    }

   pos[1] += winner;
   winner += 1;

   // cout << "trong thu muc hien tai co " << i << " folder";

}

void Thread3(const char* procName){
   DWORD procId = 0;
   HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   //std::cout << hSnap;
   DWORD threadCount = 0;
   if (hSnap != INVALID_HANDLE_VALUE) {
      //std::cout << "ok Here" << std::endl;;
      PROCESSENTRY32 procEntry;
      procEntry.dwSize = sizeof(procEntry);
      

      if (Process32First(hSnap, &procEntry)) {
         do {
            if (!strcmp(procEntry.szExeFile, procName)) {
               
               threadCount = procEntry.cntThreads;
               break;
            }

         } 
         while (Process32Next(hSnap, &procEntry));
      }
   }
   CloseHandle(hSnap);

   
   //cout << "\nThread3: Trong process " << procName << " dang co " << threadCount << " thread";

   pos[2] += winner;
   winner += 1;
}


int main(){
   
   thread t1(Thread1);
   thread t2(Thread2);
   thread t3(Thread3, "opera.exe");

   t1.join();
   t2.join();
   t3.join();
  
   for(int i = 0; i<3; i++)
      if(pos[i] == 1){
         cout << "Thread " << i+1 << " giai nhat"; 
      }

   for(int i = 0; i<3; i++)
      if(pos[i] == 2){
         cout << "\nThread " << i+1 << " giai nhi"; 
      }

   for(int i = 0; i<3; i++)
      if(pos[i] == 3){
         cout << "\nThread " << i+1 << " giai ba"; 
      }

   return 0;
}