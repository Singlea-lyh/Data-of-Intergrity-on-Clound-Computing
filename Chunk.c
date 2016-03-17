#define _STDC_WANT_LIB_EXT1_ 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined MD5_h  
#include "MD5.h"     
#endif

#define EXP_CHUNK_MAX_SIZE 1024 * 6
#define EXP_CHUNK_MIN_SIZE 1024 * 4
#define WIN_CHUNK_SIZE     512
#define FILE_NAME_SIZE     128
#define SIGNAL_SIZE        16

#define BUF_MAX_SIZE        1024

#define FPD    44               //指纹除数
#define FPR    22               //指纹余数


struct  Fchunk                                    //定义文件块
{
	char fname[FILE_NAME_SIZE];                   //此文件块属于的文件名
	unsigned int fnumber;                         //文件块的序号
	unsigned char fcsize[EXP_CHUNK_MAX_SIZE];              //文件块大小
	unsigned char fchunksum[SIGNAL_SIZE];                  //标签
	struct Fchunk *next;
};


FILE *file_open(char *, char *);
struct Fchunk *file_chunk_cdc(FILE *, char *);
unsigned int hash(char *);
void file_finish(struct Fchunk *);


int main()
{
	char *filename = NULL;                                //文件名
	char *mode = NULL;                                          //文件打开模式
	FILE *pfile = NULL;
	char c = 0;

#if 1
	mode = "rb";
	filename = "file.bin";
#endif

	struct Fchunk *chunk_after = NULL; //分块后文件初始指针

	pfile = (FILE *)file_open(filename, mode);                         //依据文件名与模式打开文件

	chunk_after = file_chunk_cdc(pfile, filename);     //文件分块

	file_finish(chunk_after);                          //结束输出函数

	getchar();
	return 0;

}

/*文件打开函数，用于将文件按指定格式与路径打开*/

FILE *file_open(char *file_name, char *mode)
{
	FILE *fp = NULL;

	if (file_name == NULL)
	{
		printf_s("Please input file  of name.\n");
		exit(1);
	}

	if (fopen_s(&fp, file_name, mode))                                    //依据给定模式文件名打开文件
	{
		printf_s("Error open %s for waiting. Program terminated.\n", file_name);
		exit(1);
	}

	setvbuf(fp, NULL, _IOFBF, BUFSIZ);                                   //设置打开文件缓存

	return fp;                                                          //返回文件指针
}

/*文件大分块函数，用于将文件按cdc进行切分*/

struct Fchunk *file_chunk_cdc(FILE *pfile, char *filename) {

	unsigned int num = 0;                      //文件的块序号
	char win_small[WIN_CHUNK_SIZE] = { 0 };    //当读取的字节数少于期望的窗口大小时,窗口中的字节
	char block_small[EXP_CHUNK_MAX_SIZE] = { 0 };   //小于时block中内容
	unsigned int block_size = 0;               //小于时，block_buf的大小
	unsigned int small_rwsize = 0;             //小于期望大小时，读取的字节数

	struct Fchunk *pfirst = NULL;          //分块初始指针
	struct Fchunk *pcurrent = NULL;        //当前指针

	while (!feof(pfile)) {                     //判断整个文件是否已读取完毕

		char buf[BUF_MAX_SIZE] = { 0 };  //缓冲区最大值
		char block_buf[EXP_CHUNK_MAX_SIZE] = { 0 };  //分块最大值
		char win_buf[WIN_CHUNK_SIZE] = { 0 };   //块的窗口大小
												
		unsigned int hkey = 0;             //哈希生成值
		unsigned int rwsize = 0;          //一次读取的字节数
		unsigned int old_bsize = 0;       //读取的旧总字节数
		unsigned int new_bsize = 0;       //加上刚读取的字节的字节数

		while (rwsize = fread_s(win_buf, WIN_CHUNK_SIZE, sizeof(win_buf[0]), WIN_CHUNK_SIZE, pfile))
		{

			struct Fchunk *ptemp = NULL;           //分块后的临时指针

			if (rwsize < WIN_CHUNK_SIZE)                 //读取文件小于期望的窗口大小时说明文件即将结束，将此时的block_buf保存,跳出循环连接直接生成块
			{
				memcpy_s(win_small, WIN_CHUNK_SIZE, win_buf, rwsize);
				memcpy_s(block_small, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
				block_size = new_bsize;
				small_rwsize = rwsize;
				break;
			}

			new_bsize += rwsize;                               //总字节数

			if (new_bsize < EXP_CHUNK_MIN_SIZE)
			{                //块大小小于期望的最小块时，继续执行读取数据块，不用哈希检查

				memcpy_s(block_buf + old_bsize, EXP_CHUNK_MAX_SIZE, win_buf, rwsize);          //将新读取的块放入分块中
				old_bsize = new_bsize;
				continue;
			}

			else if (new_bsize >= EXP_CHUNK_MIN_SIZE  && new_bsize < EXP_CHUNK_MAX_SIZE)
			{
				//块大小大于期望的最小块时做哈希检查，确定边界
				memcpy_s(block_buf + old_bsize, EXP_CHUNK_MAX_SIZE, win_buf, rwsize);
				hkey = hash(win_buf, new_bsize);
				if (hkey % FPD == FPR)                       //计算哈希值，看是否满足分块边界,满足后得到正常的分块并将此块连接到已分好的尾部
				{
					ptemp = (struct Fchunk *) malloc(sizeof(struct Fchunk));
					strcpy_s(ptemp->fname, 128, filename);
					ptemp->fnumber = num;
					memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
					MD5(block_buf, ptemp->fchunksum);
					ptemp->next = NULL;
					num++;

					if (!pfirst)
					{
						pfirst = ptemp;
						pcurrent = ptemp;
					}
					else {
						pcurrent->next = ptemp;
						pcurrent = ptemp;
					}
					break;
				}
			}
			else if (new_bsize == EXP_CHUNK_MAX_SIZE)                       //不满足分块边界，生成满块
			{
				ptemp = (struct Fchunk *) malloc(sizeof(struct Fchunk));
				strcpy_s(ptemp->fname, 128, filename);
				ptemp->fnumber = num;
				memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
				MD5(block_buf, ptemp->fchunksum);
				ptemp->next = NULL;
				num++;                                         //序号加1

				if (!pfirst)
				{
					pfirst = ptemp;
					//chunk_after = pfirst;
					pcurrent = pfirst;
				}
				else {
					pcurrent->next = ptemp;
					pcurrent = ptemp;
				}
				break;
			}

        }

		if (small_rwsize > 0 && small_rwsize < WIN_CHUNK_SIZE)
		{
			struct Fchunk *ptemp = NULL;
			memcpy_s(block_small + block_size, EXP_CHUNK_MAX_SIZE, win_small, small_rwsize);
			ptemp = (struct Fchunk *) malloc(sizeof(struct Fchunk));
			strcpy_s(ptemp->fname, 128, filename);
			ptemp->fnumber = num;
			memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_small, block_size + small_rwsize);
			MD5(block_small, ptemp->fchunksum);
			ptemp->next = NULL;
			if (pcurrent == NULL && pfirst == NULL)
			{
				pfirst = ptemp;
				pcurrent = ptemp;
			}
			else {
				pcurrent = ptemp;
			}
		}
	}
	return pfirst;
}

unsigned int hash(char *str, unsigned int len)                   //哈希函数算法
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++)
	{
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}


void file_finish(struct Fchunk *pfirst)            //将每个文件块生成的MD5标签输出
{
	struct Fchunk *pcurrent = pfirst;

    printf_s("文件名为 %s 的生成的MD5标签分别为：\n", pcurrent->fname);

	while (pcurrent)
	{
		printf_s("%d  :  ", pcurrent->fnumber);
		for (int i = 0; i<16; i++)
		{
			printf_s("%02x", (*pcurrent).fchunksum[i]);
		}
		pcurrent = pcurrent->next;
		printf_s("\n");
	}
}
