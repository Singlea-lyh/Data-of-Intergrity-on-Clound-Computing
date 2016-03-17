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

#define FPD    44               //ָ�Ƴ���
#define FPR    22               //ָ������


struct  Fchunk                                    //�����ļ���
{
	char fname[FILE_NAME_SIZE];                   //���ļ������ڵ��ļ���
	unsigned int fnumber;                         //�ļ�������
	unsigned char fcsize[EXP_CHUNK_MAX_SIZE];              //�ļ����С
	unsigned char fchunksum[SIGNAL_SIZE];                  //��ǩ
	struct Fchunk *next;
};


FILE *file_open(char *, char *);
struct Fchunk *file_chunk_cdc(FILE *, char *);
unsigned int hash(char *);
void file_finish(struct Fchunk *);


int main()
{
	char *filename = NULL;                                //�ļ���
	char *mode = NULL;                                          //�ļ���ģʽ
	FILE *pfile = NULL;
	char c = 0;

#if 1
	mode = "rb";
	filename = "file.bin";
#endif

	struct Fchunk *chunk_after = NULL; //�ֿ���ļ���ʼָ��

	pfile = (FILE *)file_open(filename, mode);                         //�����ļ�����ģʽ���ļ�

	chunk_after = file_chunk_cdc(pfile, filename);     //�ļ��ֿ�

	file_finish(chunk_after);                          //�����������

	getchar();
	return 0;

}

/*�ļ��򿪺��������ڽ��ļ���ָ����ʽ��·����*/

FILE *file_open(char *file_name, char *mode)
{
	FILE *fp = NULL;

	if (file_name == NULL)
	{
		printf_s("Please input file  of name.\n");
		exit(1);
	}

	if (fopen_s(&fp, file_name, mode))                                    //���ݸ���ģʽ�ļ������ļ�
	{
		printf_s("Error open %s for waiting. Program terminated.\n", file_name);
		exit(1);
	}

	setvbuf(fp, NULL, _IOFBF, BUFSIZ);                                   //���ô��ļ�����

	return fp;                                                          //�����ļ�ָ��
}

/*�ļ���ֿ麯�������ڽ��ļ���cdc�����з�*/

struct Fchunk *file_chunk_cdc(FILE *pfile, char *filename) {

	unsigned int num = 0;                      //�ļ��Ŀ����
	char win_small[WIN_CHUNK_SIZE] = { 0 };    //����ȡ���ֽ������������Ĵ��ڴ�Сʱ,�����е��ֽ�
	char block_small[EXP_CHUNK_MAX_SIZE] = { 0 };   //С��ʱblock������
	unsigned int block_size = 0;               //С��ʱ��block_buf�Ĵ�С
	unsigned int small_rwsize = 0;             //С��������Сʱ����ȡ���ֽ���

	struct Fchunk *pfirst = NULL;          //�ֿ��ʼָ��
	struct Fchunk *pcurrent = NULL;        //��ǰָ��

	while (!feof(pfile)) {                     //�ж������ļ��Ƿ��Ѷ�ȡ���

		char buf[BUF_MAX_SIZE] = { 0 };  //���������ֵ
		char block_buf[EXP_CHUNK_MAX_SIZE] = { 0 };  //�ֿ����ֵ
		char win_buf[WIN_CHUNK_SIZE] = { 0 };   //��Ĵ��ڴ�С
												
		unsigned int hkey = 0;             //��ϣ����ֵ
		unsigned int rwsize = 0;          //һ�ζ�ȡ���ֽ���
		unsigned int old_bsize = 0;       //��ȡ�ľ����ֽ���
		unsigned int new_bsize = 0;       //���ϸն�ȡ���ֽڵ��ֽ���

		while (rwsize = fread_s(win_buf, WIN_CHUNK_SIZE, sizeof(win_buf[0]), WIN_CHUNK_SIZE, pfile))
		{

			struct Fchunk *ptemp = NULL;           //�ֿ�����ʱָ��

			if (rwsize < WIN_CHUNK_SIZE)                 //��ȡ�ļ�С�������Ĵ��ڴ�Сʱ˵���ļ���������������ʱ��block_buf����,����ѭ������ֱ�����ɿ�
			{
				memcpy_s(win_small, WIN_CHUNK_SIZE, win_buf, rwsize);
				memcpy_s(block_small, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
				block_size = new_bsize;
				small_rwsize = rwsize;
				break;
			}

			new_bsize += rwsize;                               //���ֽ���

			if (new_bsize < EXP_CHUNK_MIN_SIZE)
			{                //���СС����������С��ʱ������ִ�ж�ȡ���ݿ飬���ù�ϣ���

				memcpy_s(block_buf + old_bsize, EXP_CHUNK_MAX_SIZE, win_buf, rwsize);          //���¶�ȡ�Ŀ����ֿ���
				old_bsize = new_bsize;
				continue;
			}

			else if (new_bsize >= EXP_CHUNK_MIN_SIZE  && new_bsize < EXP_CHUNK_MAX_SIZE)
			{
				//���С������������С��ʱ����ϣ��飬ȷ���߽�
				memcpy_s(block_buf + old_bsize, EXP_CHUNK_MAX_SIZE, win_buf, rwsize);
				hkey = hash(win_buf, new_bsize);
				if (hkey % FPD == FPR)                       //�����ϣֵ�����Ƿ�����ֿ�߽�,�����õ������ķֿ鲢���˿����ӵ��ѷֺõ�β��
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
			else if (new_bsize == EXP_CHUNK_MAX_SIZE)                       //������ֿ�߽磬��������
			{
				ptemp = (struct Fchunk *) malloc(sizeof(struct Fchunk));
				strcpy_s(ptemp->fname, 128, filename);
				ptemp->fnumber = num;
				memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
				MD5(block_buf, ptemp->fchunksum);
				ptemp->next = NULL;
				num++;                                         //��ż�1

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

unsigned int hash(char *str, unsigned int len)                   //��ϣ�����㷨
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


void file_finish(struct Fchunk *pfirst)            //��ÿ���ļ������ɵ�MD5��ǩ���
{
	struct Fchunk *pcurrent = pfirst;

    printf_s("�ļ���Ϊ %s �����ɵ�MD5��ǩ�ֱ�Ϊ��\n", pcurrent->fname);

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
