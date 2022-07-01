#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
primes(int p[]){
    // 从上一个进程读到的第一个数一定是质数，输出这个数
    int first;  
    if(read(p[0], &first, 4) != 4){
        exit(0);
    }
    printf("prime %d\n", first);

    int fd[2];
    pipe(fd);

    if(fork() == 0){
        //及时关闭不用的管道
        close(fd[1]);
        primes(fd);
        close(fd[0]);
    }
    else{
        close(fd[0]);
        int second;
        while(read(p[0], &second, 4) == 4){
            if(second % first != 0){
                write(fd[1], &second, 4);
            }
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}

int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    if(fork() == 0){
        close(p[1]);
        primes(p);
        close(p[0]);
    }
    else{
        close(p[0]);    // 父进程不读
        for(int i = 2; i <= 35; ++i){
            write(p[1], &i, 4); // 起始进程向管道写入 2 ~ 35 的数字
        }
        close(p[1]);    // 写完及时关闭
        wait(0);
    }
    exit(0);

    return 0;
}