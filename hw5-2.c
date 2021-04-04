//소프트웨어공학 실습을 위한 영상처리코드2
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <string.h>

#define HEIGHT 512
#define WIDTH 512

unsigned char* Laplacian_ex(unsigned char** Pad, int nHeight, int nWidth);
unsigned char* sobelX(unsigned char** Pad, int nHeight, int nWidth);
unsigned char** Padding(unsigned char In[][512], int nHeight, int nWidth, int nFilterSize);

void main() {

	unsigned char Img_Lap2[512][512];
	unsigned char Img_sobelx[512][512];

	
	FILE* infile1;                                       // ���� ����ü ����
	fopen_s(&infile1, "Lena_512.raw", "rb");              // �б� ���� ������ open��
	if (infile1 == NULL)
	{
		printf("Open error!!");
		return;
	}
	fread(Img_Lap2, sizeof(char), 512 * 512, infile1);        // char�� ũ��� 256*256�� �ݺ��Ͽ� �о� Img�� ����(infile�� ���� ���� ������)
	fclose(infile1);
	unsigned char** pad;
	unsigned char* Filter3_R;

	pad = Padding(Img_Lap2, HEIGHT, WIDTH, 3);
	Filter3_R = Laplacian_ex(pad, HEIGHT, WIDTH);

	FILE* outfile1;
	fopen_s(&outfile1, "LaplacienFilterex_Result.raw", "wb");             // ���� ���� ������ open��
	fwrite(Filter3_R, sizeof(char), 512 * 512, outfile1);
	fclose(outfile1);

	FILE* infile2;                                       // ���� ����ü ����
	fopen_s(&infile2, "Lena_512.raw", "rb");              // �б� ���� ������ open��
	if (infile1 == NULL)
	{
		printf("Open error!!");
		return;
	}
	fread(Img_sobelx, sizeof(char), 512 * 512, infile2);        // char�� ũ��� 256*256�� �ݺ��Ͽ� �о� Img�� ����(infile�� ���� ���� ������)
	fclose(infile2);
	unsigned char* Filter_sobelx;

	pad = Padding(Img_sobelx, HEIGHT, WIDTH, 3);
	Filter_sobelx = sobelX(pad, HEIGHT, WIDTH);

	FILE* outfile2;
	fopen_s(&outfile2, "sobelx_Result.raw", "wb");             // ���� ���� ������ open��
	fwrite(Filter_sobelx, sizeof(char), 512 * 512, outfile2);
	fclose(outfile2);


}


unsigned char* Laplacian_ex(unsigned char** Pad, int nHeight, int nWidth) {
	unsigned char* Result;	// ����� ������ �迭�� ���� ������
	Result = (unsigned char*)malloc(sizeof(unsigned char) * nHeight * nWidth); // �����Ҵ��� ���� Ȯ��� ũ���� �迭�� �����

	int Pad_size = (3 / 2);
	for (int i = Pad_size; i < nHeight + Pad_size; i++) {
		for (int j = Pad_size; j < nWidth + Pad_size; j++) {
			double ftemp = 0;

			for (int mask_x = -Pad_size; mask_x <= Pad_size; mask_x++) {
				for (int mask_y = -Pad_size; mask_y <= Pad_size; mask_y++) {

					if (mask_x == 0 && mask_y == 0) {
						ftemp += (Pad[i + mask_x][j + mask_y] * 9.0);
					}
					else if (mask_x == 0 || mask_y == 0) {
						ftemp += (Pad[i + mask_x][j + mask_y] * -1.0);
					}
					else {
						ftemp += (Pad[i + mask_x][j + mask_y] * -1.0);
					}
				}
			}
			if (ftemp > 255) {
				ftemp = 255;
			}
			else if (ftemp < 0) {
				ftemp = 0;
			}
			Result[(i - Pad_size) * 512 + (j - Pad_size)] = (unsigned char)ftemp;
		}
	}
	return Result;
}

unsigned char* sobelX(unsigned char** Pad, int nHeight, int nWidth) {
	unsigned char* Result;	// ����� ������ �迭�� ���� ������
	Result = (unsigned char*)malloc(sizeof(unsigned char) * nHeight * nWidth); // �����Ҵ��� ���� Ȯ��� ũ���� �迭�� �����

	int Pad_size = (3 / 2);
	for (int i = Pad_size; i < nHeight + Pad_size; i++) {
		for (int j = Pad_size; j < nWidth + Pad_size; j++) {
			double ftemp = 0;

			for (int mask_x = -Pad_size; mask_x <= Pad_size; mask_x++) {
				for (int mask_y = -Pad_size; mask_y <= Pad_size; mask_y++) {

					if (mask_y == 0) {
						ftemp += (Pad[i + mask_x][j + mask_y] * 0.0);
					}
					else if (mask_y == -1) {
						if(mask_x ==0)
							ftemp += (Pad[i + mask_x][j + mask_y] * -2.0);
						else
							ftemp += (Pad[i + mask_x][j + mask_y] * -1.0);
					}
					else if(mask_y == 1)
						if (mask_x == 0)
							ftemp += (Pad[i + mask_x][j + mask_y] * 2.0);
						else
							ftemp += (Pad[i + mask_x][j + mask_y] * 1.0);
					}
				}
			if (ftemp > 255) {
				ftemp = 255;
			}
			else if (ftemp < 0) {
				ftemp = 0;
			}
			Result[(i - Pad_size) * 512 + (j - Pad_size)] = (unsigned char)ftemp;
		}
	}
	return Result;
}


unsigned char** Padding(unsigned char In[][512], int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);

	unsigned char** Pad = (unsigned char**)malloc(sizeof(unsigned char*) * nHeight + 2 * nPadSize);
	for (int n = 0; n < nHeight + 2 * nPadSize; n++) {
		Pad[n] = (unsigned char*)malloc(sizeof(unsigned char) * (nWidth + 2 * nPadSize));
		memset(Pad[n], 0, sizeof(unsigned char) * nWidth);
	}

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}
