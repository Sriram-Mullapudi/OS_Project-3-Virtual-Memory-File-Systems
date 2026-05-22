#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void find(const char *path, const char *target_name, int type_flag, int inum_flag, int comparator, int print_inum) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0) {
    printf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0) {
    printf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_FILE:
      if(strcmp(fmtname((char *)path), target_name) == 0) {
            if((type_flag == 0 || type_flag == 'f') && (inum_flag == 0 || (comparator == 0 && st.ino == inum_flag) ||
              (comparator > 0 && st.ino > inum_flag) || (comparator < 0 && st.ino < inum_flag))) 
                print_inum ? printf(1, "%d %s\n", st.ino, path) : printf(1, "%s\n", path);
      }
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf(1, "find: path too long\n");
        break;
      }

      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';

      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0) {
          printf(1, "find: cannot stat %s\n", buf);
          continue;
        }

        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        if(type_flag == 'd' && strcmp(de.name, target_name) == 0)print_inum ? printf(1, "%d %s\n", de.inum, de.name) : printf(1, "%s\n", de.name);
        find(buf, target_name, type_flag, inum_flag, comparator, print_inum);
      }
  }
  close(fd);
  return;
}

int main(int argc, char *argv[]) {

  char *target_name = 0;
  int type_flag = 0, inum_flag = 0, comparator = 0, print_inum = 0;

  if(argc < 3) {
    printf(2, "Usage: find <folder> -name <name> [options]\n");
    exit();
  }

  for (int i = 2; i < argc; i++) {

    int index_check = (i + 1 < argc);
    if (strcmp(argv[i], "-name") == 0 && index_check) {
      target_name = argv[++i];
    } else if (strcmp(argv[i], "-type") == 0 && index_check) {

      if (argv[i+1][0] == 'f' || argv[i+1][0] == 'd')
        type_flag = argv[i+1][0];
      i++;

    } else if (strcmp(argv[i], "-inum") == 0 && index_check) {
      
      char *inum_str = argv[++i];
      if(inum_str[0] == '+' || inum_str[0] == '-'){
        comparator = inum_str[0] == '+' ? 1 : -1;
        inum_flag = atoi(&inum_str[1]);
      } else{
        inum_flag = atoi(inum_str);
      }

    } else if (strcmp(argv[i], "-printi") == 0) {
      print_inum = 1;
    }
  }

  if (!target_name) {
    printf(2, "find: -name flag is required\n");
    exit();
  }

  find(argv[1], target_name, type_flag, inum_flag, comparator, print_inum);
  exit();
}
