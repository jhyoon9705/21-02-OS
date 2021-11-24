#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

char buf[1024];
char *errMsg = "Usage: 'copy file1 file2' or 'copy dir1 dir2' or 'copy file dir'\n";

void fileCopy(int fd, int dest) {
  int n;
  while((n = read(fd, buf, sizeof(buf))) > 0) {
        write(dest, buf, n); 
  }
}

int directoryCopy(char *srcPath, char *dstPath) {
  DIR *pdir;
  struct dirent *dirt;
  struct stat statBuf;
  char newSrcPath[255], newDstPath[255];
  int n, in, copied;

  memset(&dirt, 0, sizeof(dirt));
  memset(&statBuf, 0, sizeof(statBuf));

  // open directory
  if((pdir = opendir(srcPath)) <= 0) {
    perror("opendir");
    return -1;
  }

  while((dirt = readdir(pdir)) != NULL) {
    strcpy(newSrcPath, srcPath);
    strcat(newSrcPath, "/");
    strcat(newSrcPath, dirt->d_name);
    lstat(newSrcPath, &statBuf);
      // directory -> regular file
      if(S_ISREG(statBuf.st_mode)) {
        if ((in = open(srcPath, O_RDONLY )) < 0) {
          perror(srcPath);
          return -1;
        }
        strcpy(newDstPath, dstPath);
        strcat(newDstPath, "/");
        strcat(newDstPath, dirt->d_name);
        
        if ((copied = open(newDstPath, O_WRONLY|O_CREAT|O_TRUNC , S_IRUSR|S_IWUSR)) < 0) {
          perror(dstPath);
          return -1;
        }
        fileCopy(in, copied);
        close(in);
        close(copied);
      }  
      // directory -> directory -> ...
      else if(S_ISDIR(statBuf.st_mode)){
        if(strcmp(dirt->d_name, ".") && strcmp(dirt->d_name, "..")) {
          strcpy(newDstPath, dstPath);
          strcat(newDstPath, "/");
          strcat(newDstPath, dirt->d_name);
          mkdir(newDstPath, 700); // owner's r/w/x mode
          directoryCopy(newSrcPath, newDstPath);
        }
      }
      // error case
      else {
        return -1;
      }
    }
    closedir(pdir);
}

int main (int argc, char **argv) {
  char *src = argv[1];
  char *dst = argv[2];
  int n, in, out;
  DIR *pdir;
  struct stat statBuf;
  char dstPath[255];
  int copied;
  
  memset(&statBuf, 0, sizeof(statBuf));
  
  // arg error
  if (argc < 3) {
    write(2, errMsg, strlen(errMsg));
    return -1;
  }

  // open the source file or directory
  if ((in = open(src, O_RDONLY )) < 0) {
    perror(src);
    return -1;
  }

  // open the destination file or directory
  if ((out = open(dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) < 0) {
    if((pdir = opendir(dst)) <= 0) {
      perror(dst);
      return -1;
    }
    closedir(pdir);
  }  

  // save the file information
  fstat(in, &statBuf);
  
  if(S_ISREG(statBuf.st_mode)) {
    stat(dst, &statBuf);
    
    // from regular file to regular file
    if(S_ISREG(statBuf.st_mode)) {
      fileCopy(in, out);
    }
    
    // from regular file to directory
    else if(S_ISDIR(statBuf.st_mode)){
      strcpy(dstPath, dst);
      strcat(dstPath, "/");
      strcat(dstPath, src); 

      if ((copied = open(dstPath, O_WRONLY|O_CREAT|O_TRUNC , S_IRUSR|S_IWUSR)) < 0) {
        perror(dstPath);
        return -1;
      }
      fileCopy(in, copied);
      close(in);
      close(copied);
    }
    // error case
    else {
      return -1;
    }
  }
  
  else if(S_ISDIR(statBuf.st_mode)){
    fstat(out, &statBuf);
    // from directory to regular file(error)
    if(S_ISREG(statBuf.st_mode)) {
      write(2, errMsg, strlen(errMsg));
      return -1;
    }
    // from directory to directory
    else if(S_ISDIR(statBuf.st_mode)){
      directoryCopy(src, dst);
    }
    // error case
    else {
      return -1;
    }    
  }
  
  //error case
  else {
    return -1;
  }
  close(in);
  close(out);
  return 0;
}
