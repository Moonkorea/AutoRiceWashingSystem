#include "i2c.h"
#include <string.h>
#include <unistd.h> // read(), write() 사용
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h> // ioctl() 사용
#include <fcntl.h>  // 파일 열기, 잠금 및 다른 작업을 할 수 있음. 여기서는 open()사용

// i2c address
#define ADDRESS 0x08

// i2c bus
static const char *deviceName = "/dev/i2c-1";


i2c::i2c()
{

}

i2c::~i2c()
{

}

void i2c::i2c_network(int mans)
{
    int file;

    // int open (const char* FILENAME, int FLAGS[, mode_t MODE])
    // FILENAME: 대상 파일 이름
    // FLAGS: 파일에 대한 열기 옵션. 옵션 사용에 의해 파일이 생성될 때 파일 접근 권한 지정
    // 반환: 파일 열기에 성공하면 파일 디스크립터의 양의 정수 값 반환
    // fcntl.h
    if ((file = open(deviceName, O_RDWR)) < 0) {
        fprintf(stderr, "I2C: Failed to access %s\n", deviceName);
        exit(1);
    }

    printf("I2C: Connected\n");

    printf("I2C: acquiring bus to 0x%x\n", ADDRESS);

    // int ioctl(int fd, int request, ...)
    // fd: open한 디바이스 드라이버의 fd값
    // request: 디바이스에게 전달할 명령. 이 명령에 따라서 디바이스를 컨트롤 할 수 있다
    // sys/ioctl.h
    if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
        fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);
        exit(1);
    }

    unsigned char cmd[16];

    printf("Sending %d\n", mans);

    cmd[0] = mans;

    // ssize_t write(int fd, const void *buf, size_t n)
    // fd: 파일 디스크립터
    // buf: 파일에 쓰기를 할 내용을 담은 버퍼
    // n: 쓰기할 바이트 개수
    // 반환: 쓰기에 성공했다면 쓰기한 바이트 개수를, 실패했다면 -1을 반환
    if (write(file, cmd, 1) == 1) {

        usleep(10000); // 마이크로 초 동안 대기 함수 -> 1000,000 = 1초

        char buf[1024] = "";
        char buf2[1024];

        // int read(int fd, void *buf, size_t nbytes)
        // fd: 파일 디스크립터
        // buf: fd파일로부터 읽어 들일 버퍼
        // nbytes: 버퍼의 크기
        // 반환: 읽기 성공하면 읽어들인 바이트 수. 실패하면 -1
        while (buf[0] != 'd') {
            read(file, buf, 1024);
        }

        int i;
        for (i = 0; i < 1024; ++i) {
            buf2[i] = buf[i];

            if (buf[i] == '\n') {
                buf2[i]='\0';
                break;
            }
        }

        printf("[%s]\n", buf2);
    }

    usleep(10000);

    close(file);
}
