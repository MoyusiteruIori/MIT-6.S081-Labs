#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int 
main(int argc, char *argv[]){
    int parent[2],child[2];
    // 创建管道。单工管道不支持双向读写，创建两个。
    pipe(parent);
    pipe(child);
    char buf1, buf2;

    int pid = fork();
    if(pid > 0){ 
        // 父进程
        write(parent[1], &buf1, 1);
        read(child[0], &buf2, 1);
        printf("%d: received pong\n", getpid());
    }
    else if(pid == 0){
        // 子进程
        read(parent[0], &buf2, 1);
        printf("%d: received ping\n", getpid());
        write(child[1], &buf1, 1);
    }
    exit(0);

    return 0;
}