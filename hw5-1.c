//소프트웨어공학 실습을 위한 영상처리 코드1
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <string.h>

#define HEIGHT 512
#define WIDTH 512

//�ؾߵǴ� �� 
unsigned char* meanFilter(unsigned char** Pad, int nHeight, int nWidth, int filterSize);
unsigned char* Laplacian(unsigned char** Pad, int nHeight, int nWidth);
unsigned char** Padding(unsigned char In[][512], int nHeight, int nWidth, int nFilterSize);
unsigned char* Median_Filter(unsigned char** Pad, int nHeight, int nWidth);
unsigned char MED(unsigned char p1, unsigned char p2, unsigned char p3);


void main() {
	unsigned char Img_Mean[512][512];
	unsigned char Img_Lap[512][512];
	unsigned char Img_Median[512][512];

	FILE* infile1;                                       // ���� ����ü ����
	fopen_s(&infile1, "Lena_512.raw", "rb");              // �б� ���� ������ open��
	if (infile1 == NULL)
	{
		printf("Open error!!");
		return;
	}                                                   // ���� open �������� ��, error���� ���
	fread(Img_Mean, sizeof(char), 512 * 512, infile1);        // char�� ũ��� 256*256�� �ݺ��Ͽ� �о� Img�� ����(infile�� ���� ���� ������)
	fclose(infile1);

	FILE* infile2;                                       // ���� ����ü ����
	fopen_s(&infile2, "Lena_512.raw", "rb");              // �б� ���� ������ open��
	if (infile2 == NULL)
	{
		printf("Open error!!");
		return;
	}
	fread(Img_Lap, sizeof(char), 512 * 512, infile2);        // char�� ũ��� 256*256�� �ݺ��Ͽ� �о� Img�� ����(infile�� ���� ���� ������)
	fclose(infile2);
	
	FILE* infile3;                                       // ���� ����ü ����
	fopen_s(&infile3, "Lena_512.raw", "rb");              // �б� ���� ������ open��
	if (infile3 == NULL)
	{
		printf("Open error!!");
		return;
	}
	fread(Img_Median, sizeof(char), 512 * 512, infile3);        // char�� ũ��� 256*256�� �ݺ��Ͽ� �о� Img�� ����(infile�� ���� ���� ������)

	fclose(infile3);

	unsigned char* Filter1_R, * Filter2_R, * Median_Result;

	unsigned char** pad;
	pad = Padding(Img_Mean, HEIGHT, WIDTH, 3);
	Filter1_R = meanFilter(pad, HEIGHT, WIDTH, 3);

	FILE* outfile1;
	fopen_s(&outfile1, "meanFilter_Result.raw", "wb");             // ���� ���� ������ open��
	fwrite(Filter1_R, sizeof(char), 512 * 512, outfile1);
	fclose(outfile1);

	pad = Padding(Img_Lap, HEIGHT, WIDTH, 3);
	Filter2_R = Laplacian(pad, HEIGHT, WIDTH);

	FILE* outfile2;
	fopen_s(&outfile2, "LaplacienFilter_Result.raw", "wb");             // ���� ���� ������ open��
	fwrite(Filter2_R, sizeof(char), 512 * 512, outfile2);
	fclose(outfile2);


	pad = Padding(Img_Median, HEIGHT, WIDTH, 3);
	Median_Result = Median_Filter(pad, HEIGHT, WIDTH);

	FILE* outfile3;
	fopen_s(&outfile3, "Median_Result.raw", "wb");             // ���� ���� ������ open��
	fwrite(Median_Result, sizeof(char), 512 * 512, outfile3);
	fclose(outfile3);

}


unsigned char* meanFilter(unsigned char** Pad, int nHeight, int nWidth, int filterSize) {
	unsigned char* Result;	// ����� ������ �迭�� ���� ������
	Result = (unsigned char*)malloc(sizeof(unsigned char) * nHeight * nWidth); // �����Ҵ��� ���� Ȯ��� ũ���� �迭�� �����

	int Pad_size = (filterSize / 2);

	double** AverageF = (double**)malloc(sizeof(double*) * filterSize);		// ���� �����ŭ�� ������ �迭�� �Ҵ�
	for (int i = 0; i < filterSize; i++) {
		AverageF[i] = (double*)malloc(sizeof(double) * filterSize);
		for (int j = 0; j < filterSize; j++)
			AverageF[i][j] = (1.0) / (filterSize * filterSize);		// ���� �����ŭ �� ����� ���ϰ����ش�.
	}
	for (int i = Pad_size; i < nHeight + Pad_size; i++) {
		for (int j = Pad_size; j < nWidth + Pad_size; j++) {
			double ftemp = 0;
			for (int mask_x = -Pad_size; mask_x <= Pad_size; mask_x++) {
				for (int mask_y = -Pad_size; mask_y <= Pad_size; mask_y++) {	// ������ AverageF �� Pad �� ��ǥ�� ���ϰ� ���ϴ� ����
					ftemp += (Pad[i + mask_x][j + mask_y] * AverageF[mask_x + Pad_size][mask_y + Pad_size]);
				}
			}
			if (ftemp > 255) {		// ��� ���� 255�� �Ѱų� 0���� �۾� ������ �ʴ� ���� ������ �ʵ��� ����
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

unsigned char* Laplacian(unsigned char** Pad, int nHeight, int nWidth) {
	unsigned char* Result;	// ����� ������ �迭�� ���� ������
	Result = (unsigned char*)malloc(sizeof(unsigned char) * nHeight * nWidth); // �����Ҵ��� ���� Ȯ��� ũ���� �迭�� �����

	int Pad_size = (3 / 2);
	for (int i = Pad_size; i < nHeight + Pad_size; i++) {
		for (int j = Pad_size; j < nWidth + Pad_size; j++) {
			double ftemp = 0;

			for (int mask_x = -Pad_size; mask_x <= Pad_size; mask_x++) {
				for (int mask_y = -Pad_size; mask_y <= Pad_size; mask_y++) {

					if (mask_x == 0 && mask_y == 0) {
						ftemp += (Pad[i + mask_x][j + mask_y] * 8.0);
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

unsigned char* Median_Filter(unsigned char** Pad, int nHeight, int nWidth) {
	unsigned char* Result;	// ����� ������ �迭�� ���� ������
	Result = (unsigned char*)malloc(sizeof(unsigned char) * nHeight * nWidth); // �����Ҵ��� ���� Ȯ��� ũ���� �迭�� �����

	int Pad_size = (3 / 2);

	for (int i = Pad_size; i < nHeight + Pad_size; i++) {
		for (int j = Pad_size; j < nWidth + Pad_size; j++) {
			double ftemp = 0;

			unsigned char P1 = 0, P2 = 0, P3 = 0, P = 0;  // MED�� �� 4�� , ���� 4���� ������ ���� ����

			// �ʺ� ���ؼ� �߰� ���� ������ �� ���� 3���� mid ���� �ٽ� MED�Լ��� ���� ���� �߰����� �����־����ϴ�.
			P1 = MED(Pad[i - Pad_size][j - Pad_size], Pad[i - Pad_size][j - Pad_size + 1], Pad[i - Pad_size][j - Pad_size + 2]);
			P2 = MED(Pad[i - Pad_size + 1][j - Pad_size], Pad[i - Pad_size + 1][j - Pad_size + 1], Pad[i - Pad_size + 1][j - Pad_size + 2]);
			P3 = MED(Pad[i - Pad_size + 2][j - Pad_size], Pad[i - Pad_size + 2][j - Pad_size + 1], Pad[i - Pad_size + 2][j - Pad_size + 2]);

			P = MED(P1, P2, P3);	// ���� �߰���
			ftemp = P;

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

unsigned char MED(unsigned char p1, unsigned char p2, unsigned char p3) {
	unsigned char max = p1;  //���� Max�� ������
	if (max < p2)
		max = p2;
	if (max < p3)
		max = p3;

	unsigned char min = p1;		// ���� ���� min �� ���Ѵ�
	if (min > p2)
		min = p2;
	if (min > p3)
		min = p3;

	unsigned char mid = p1;		// max �� min �� �ƴ� mid ���� ���� �� return
	if (p1 != max && p1 != min)
		mid = p1;
	if (p2 != max && p2 != min)
		mid = p2;
	if (p3 != max && p3 != min)
		mid = p3;

	return mid;
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
