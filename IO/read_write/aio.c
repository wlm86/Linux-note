/*
 * gcc -o aio -laio aio.c 
 * Liming Wu
 */
#include <stdlib.h>
#include <stdio.h>
#include <libaio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libaio.h>

int srcfd = -1;
int tarfd = -1;

#define AIO_BLKSIZE  1024
#define AIO_MAXIO 64

static void wr_done(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
    if (res2 != 0) {
      printf("aio write error\n");
    }
    if (res != iocb->u.c.nbytes) {
      printf("write missed bytes expect %d got %d \n", iocb->u.c.nbytes, res);
      exit(1);
    }

   free(iocb->u.c.buf);
   free(iocb);
}

static void rd_done(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
    
    int iosize = iocb->u.c.nbytes;
    char *buf = (char *)iocb->u.c.buf;
    off_t offset = iocb->u.c.offset;
    int ret;
    char *wrbuff = NULL;

    if (res2 != 0) {
      printf("aio read\n");
    }
    if (res != iosize) {
      printf("read missing bytes expect %d got %d", iocb->u.c.nbytes, res);
      exit(1);
    }

    
    ret = posix_memalign((void **)&wrbuff, getpagesize(), AIO_BLKSIZE);
    if (ret < 0) {
      printf("posix_memalign222 \n");
      exit(1);
    }

   snprintf(wrbuff, iosize + 1, "%s", buf);

   printf("wrbuff - len = %d:%s \n", strlen(wrbuff), wrbuff);
   printf("wrbuff_len = %d \n", strlen(wrbuff));
   free(buf);

   io_prep_pwrite(iocb, tarfd, wrbuff, iosize, offset);
   io_set_callback(iocb, wr_done);

   if (1 != (res = io_submit(ctx, 1, &iocb)))
      printf("io_submit write error \n");

   printf("submit %d write request \n", res);
}

void main(int args, void *argv[])
{
    int length = sizeof("wlm");
    char *content = (char *)malloc(length);
    io_context_t myctx;
    int rc;
    char *buff = NULL;
    int offset = 0;
    int num, i, ret;

	sleep(5);

    if (args < 3) {
      printf("the number of param is wrong \n");
      exit(1);
    }

    if ((srcfd = open(argv[1], O_RDWR|O_SYNC)) < 0) {
      printf("open srcfile error \n");
      exit(1);
    }

    printf("srcfd = %d \n", srcfd);

    lseek(srcfd, 0, SEEK_SET);
    write(srcfd, "wlm", length);

	// seek the start location of file, SEEK_SET is location 0
    lseek(srcfd, 0, SEEK_SET);
    read(srcfd, content, length);

    printf("write in the srcfile successful, content is %s \n", content);

    if ((tarfd = open(argv[2], O_RDWR|O_SYNC)) < 0) {
      close(srcfd);
      printf("open odsfile error \n");
      exit(1);
    }

    memset(&myctx, 0, sizeof(myctx));
    io_queue_init(AIO_MAXIO, &myctx);

    struct iocb *io = (struct iocb *)malloc(sizeof(struct iocb));
    int iosize = AIO_BLKSIZE;
    ret = posix_memalign((void **)&buff, getpagesize(), AIO_BLKSIZE);
    if (ret < 0) {
      printf("posix_memalign error \n");
      exit(1);
    }
    if (NULL == io) {
      printf("io out of memeory \n");
      exit(1);
    }

    io_prep_pread(io, srcfd, buff, iosize, offset);

    io_set_callback(io, rd_done);

    printf("START...\n \n");

    rc = io_submit(myctx, 1, &io);

    if (rc < 0)
       printf("io_submit read error \n");

    printf("\nsubmit %d read request \n", rc);

    //m_io_queue_run(myctx);

    struct io_event events[AIO_MAXIO];
    io_callback_t cb;

    num = io_getevents(myctx, 1, AIO_MAXIO, events, NULL);
    printf("\n % d io_request completed \n \n", num);

    for (i = 0; i < num; i++) {
      cb = (io_callback_t)events[i].data;
      struct iocb *io = events[i].obj;

      printf("events[%d].data = %x, res = %d, res2 = %d  n", i, cb, events[i].res, events[i].res2);
      cb(myctx, io, events[i].res, events[i].res2);
    }

}
