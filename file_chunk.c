#define _STDC_WANT_LIB_EXT1_ 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MD5.h"

#define EXP_CHUNK_MAX_SIZE 1024 * 6
#define EXP_CHUNK_MIN_SIZE 1024 * 4
#define WIN_CHUNK_SIZE     512
#define FILE_NAME_SIZE     128
#define SIGNAL_SIZE        16

#define BUF_MAX_SIZE        1024

#define FPD   44           //ָ�Ƴ���
#define FPR   22              //ָ������

//unsigned char decrypt[16] = { 0 };


struct  Fchunk                                    //�����ļ���
{
	char fname[FILE_NAME_SIZE];                   //���ļ������ڵ��ļ���
	unsigned int fnumber;                         //�ļ�������
	unsigned char fcsize[EXP_CHUNK_MAX_SIZE];              //�ļ����С
	unsigned char fchunksum[SIGNAL_SIZE];                  //��ǩ
	struct Fchunk *next;
};


struct Lchunk                                      //�ֿ����ļ����ݿ�����ṹ
{
	struct Fchunk fchunk;
	struct Fchunk *next;
};


//struct Lchunk *chunk_after = NULL;


FILE *file_open(char *, char *mode);
struct Fchunk *file_chunk_cdc();
unsigned int hash();
void file_finish();

int main()
{
#if 0
	char buffer[BUF_SIZE];
	char input[BUF_SIZE];
	char output[BUF_SIZE];
	char test;
#endif

	char *filename = NULL;                                //�ļ���
	char *mode = NULL;                                          //�ļ���ģʽ
    FILE *pfile = NULL;
	char c=0;

#if 1
	mode = "rb";
	filename = "file.bin";
#endif

	struct Fchunk *chunk_after = NULL;                       //�ֿ���ļ���ʼָ��

	pfile = (FILE *)file_open(filename, mode);                         //�����ļ�����ģʽ���ļ�
	
    chunk_after = file_chunk_cdc(pfile, filename);     //�ļ��ֿ�

	file_finish(chunk_after);                          //�����������

#if 0
	if (fopen_s(&pfile, filename, "wb"))
	{
		printf_s("Error open %s for waiting .Program terminated.\n", filename);
		exit(1);
	}



	setvbuf(pfile, NULL, _IOFBF, WIN_CHUNK_SIZE);


	fgets(input, BUFSIZ, stdin);

	fwrite(input, sizeof(char), 20, pfile );


	fread(output, sizeof(char), 20, pfile);

	printf_s(output);

	scanf_s("\n%c\n", &test, sizeof(char));

	printf_s("%c", test);

#endif
	getchar(c);
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
		printf_s("Error open %s for waiting. Program terminated.\n", file_name );
		exit(1);
	}

	setvbuf(fp, NULL, _IOFBF, BUFSIZ);                                   //���ô��ļ�����

	return fp;                                                          //�����ļ�ָ��
}

/*�ļ���ֿ麯�������ڽ��ļ���cdc�����з�*/

struct Fchunk *file_chunk_cdc(FILE *pfile,  char *filename){
	
	unsigned int num = 0;                      //�ļ��Ŀ����
	char win_small[WIN_CHUNK_SIZE] = { 0 };    //����ȡ���ֽ������������Ĵ��ڴ�Сʱ,�����е��ֽ�
    char block_small[EXP_CHUNK_MAX_SIZE] = { 0 };   //С��ʱblock������
	unsigned int block_size = 0;               //С��ʱ��block_buf�Ĵ�С
	unsigned int small_rwsize = 0;             //С��������Сʱ����ȡ���ֽ���

	struct Fchunk *pfirst = NULL;          //�ֿ��ʼָ��
	struct Fchunk *pcurrent = NULL;        //��ǰָ��

	//unsigned char decrypt[16] = { 0 };

	while (!feof(pfile)) {                     //�ж������ļ��Ƿ��Ѷ�ȡ���

		char buf[BUF_MAX_SIZE] = { 0 };  //���������ֵ
	    char block_buf[EXP_CHUNK_MAX_SIZE] = { 0 };  //�ֿ����ֵ
		char win_buf[WIN_CHUNK_SIZE] = { 0 };   //��Ĵ��ڴ�С
		//unsigned char md5_check[SIGNAL_SIZE] = { 0 };   //���ɵ�md5��ǩ
		unsigned char *md5_check;
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
#if 0
				for (int i = 0; i <= rwsize; ++i)
					block_buf[old_bsize] = win_buf[i];

#endif
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

					//memcpy_s(ptemp->fname, FILE_NAME_SIZE, filename, sizeof(filename));
					strcpy_s(ptemp->fname, 128, filename);
					//*(ptemp->fname) = filename;
					ptemp->fnumber = num;
					memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
					MD5(block_buf, ptemp->fchunksum);
					//strcpy_s(ptemp->fchunksum, 16, md5_check );
					//memcpy_s(ptemp->fchunksum, SIGNAL_SIZE, md5_check, SIGNAL_SIZE);
					//*(ptemp->fchunksum) = MD5(block_buf);
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
			else if(new_bsize = EXP_CHUNK_MAX_SIZE)                      //������ֿ�߽磬��������
			{
				ptemp = (struct Fchunk *) malloc(sizeof(struct Fchunk));
				//memcpy_s(ptemp->fname, FILE_NAME_SIZE, filename, sizeof(filename));
				strcpy_s(ptemp->fname, 128, filename);
				//*(ptemp->fname) = filename;
				ptemp->fnumber = num;
				memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_buf, new_bsize);
				MD5(block_buf,ptemp->fchunksum);
				//strcpy_s(ptemp->fchunksum, 128, md5_check);
				//memcpy_s(ptemp->fchunksum, SIGNAL_SIZE, md5_check, SIGNAL_SIZE );
				//*(ptemp->fchunksum) = MD5(block_buf);
				ptemp->next = NULL;
				num++;                                         //��ż�1

				if (!pfirst )
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
			//memcpy_s(ptemp->fname, FILE_NAME_SIZE, filename, strnlen_s(filename, sizeof(filename)));
			strcpy_s(ptemp->fname, 128, filename);
			//*(ptemp->fname) = filename;
			ptemp->fnumber = num;
			memcpy_s(ptemp->fcsize, EXP_CHUNK_MAX_SIZE, block_small, block_size + small_rwsize);
			MD5(block_small, ptemp->fchunksum);
#if  0
			
			md5_check = ptemp->fchunksum;
			for (int i = 0; i < 16; i++)
			{
				printf_s("%02x", (*ptemp).fchunksum[i]);
			}
			printf_s("\n");
#endif
#if 0
			for (int i = 0; i < 16; i++) {
				sprintf_s(dec + i,16,"%02x",md5_check );
				printf_s("%02x",dec[i]);

		}
	
			//MD5Encode(ptemp->fchunksum,decrypt,16);
			//strcpy_s(ptemp->fchunksum, 17, decrypt);
			//memcpy_s(ptemp->fchunksum, SIGNAL_SIZE, decrypt, SIGNAL_SIZE);
			//ptemp->fchunksum = decrypt;
			//*(ptemp->fchunksum) = MD5(block_buf);
#endif
			ptemp->next = NULL;
			if (pcurrent == NULL && pfirst == NULL)
			{
				pfirst = ptemp;
				//chunk_after = pfirst;
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
		//printf_s("%d  :   %02x", pcurrent->fnumber, pcurrent->fchunksum);
		printf_s("%d  :  ", pcurrent->fnumber);
		for (int i = 0; i<16; i++)
		{
			printf_s("%02x", (*pcurrent).fchunksum[i]);
		}
		pcurrent = pcurrent->next;
		printf_s("\n");
	}


}
#if 0
void *MD5(char *str)
{
	

	//unsigned char decrypt[16] = { 0 };
	//unsigned char transform[16] = { 0 };
	//unsigned char tran[16][1] = { 0 };
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, str, strlen((char *)str));
	MD5Final(&md5, decrypt);
#if 1
	for (int i = 0; i<16; i++)
	{
		printf_s("%02x", decrypt[i]);

	}
	printf_s("\n");

#if 0
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < 16; i++)
	{
		highByte = decrypt[i] >> 4;
		lowByte = decrypt[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			transform[i * 2] = highByte + 0x07;
		else
			transform[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			transform[i * 2 + 1] = lowByte + 0x07;
		else
			transform[i * 2 + 1] = lowByte;
	}
#endif
#if 0
	for (int i = 0; i < 16; i++)
	{
		char temp[8] = { 0 };
		snprintf(temp, 8, "%02x", decrypt[i]);
		transform[i] = temp;
		//strcat_s(&transform[i],sizeof(transform[i]),temp);
		printf_s("%02x", transform[i]);
	}

#endif

#endif
#if 0
	sprintf_s(transform, sizeof(transform), "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", \
		decrypt[0], decrypt[1], decrypt[2], decrypt[3], decrypt[4], decrypt[5], decrypt[6], decrypt[7], decrypt[8], decrypt[9], decrypt[10], decrypt[11], \
		decrypt[12], decrypt[13], decrypt[14], decrypt[15]);
#endif 
	//strcpy_s(transform, 16, tran);
#if 0

	for (int i = 0; i < 16; i++)
	{
		transform[i] = tran[i][0];
	}
#endif
	printf_s("\n");
	return decrypt;
}
#endif
#if 0
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len)
{
	unsigned int i = 0, j = 0;
	while (j < len)
	{
		output[j] = input[i] & 0xFF;
		output[j + 1] = (input[i] >> 8) & 0xFF;
		output[j + 2] = (input[i] >> 16) & 0xFF;
		output[j + 3] = (input[i] >> 24) & 0xFF;
		i++;
		j += 4;
	}
}
#endif