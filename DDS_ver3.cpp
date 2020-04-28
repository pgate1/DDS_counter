/*
 DDS�Ŏg�p������Z�l�Ƃ������l�����߂�v���O����

	clk_in : ���̓N���b�N
	max : �������l
	add : ���Z�l
	count : �J�E���^
	sa : �J�E���^�Ƃ������l�̍�
	enable_out : �o�̓N���b�N

	count = 0;
	add = 384;
	max = 3125;

	if(clk_in){
		sa = count - max;
		if(sa < 0){
			count += add;
		}
		else{
			count = sa + add;
			enable_out();
		}
	}

	if(clk_in){
		if(count < max){
			count += add;
		}
		else{
			count += add - max;
			enable_out();
		}
	}

*/

#include<stdio.h>
#include<math.h>

int euclid(int a, int b)
{
	if(b==0) return a;
    return euclid(b, a % b);
}

int main(void)
{
	const int base = 50000000; // �x�[�X�N���b�N
	const double target = 
	//	1789772.5; // NES �������g��
	//	1662607; // NES PAL �������g��
	//	5369318; // NES PPU �������g��
	//	10000000; // test �������g��
	//	1024000; // SNES APU �������g��1
	//	2048000; // SNES APU �������g��2
	//	10738635; // SNES CPU base
		33868800; // PSX �������g��
	//	6144000; // test https://twitter.com/twi_kingyo/status/1112040773992640513
	//	0.2; // test
	//	0.01; // test ���Ԃ�������

	int add, max;

	int best_add, best_max;
	double best_gosa = base;

	const double gosa_siki = 0.000000001;
	double gosa_min;
	if(target>=1){
		int gcd = euclid(base, (int)(target));
		printf("�ő����(GCD) %d\n", gcd);
	}

//	printf(" add   ���Z�l �������l   �덷\n");

int bits;
for(bits=1; bits<=32; bits++){

//	for(add=target/gcd; add<1<<bits; add++){ // gcd==1�̎��͖��ɗ����Ȃ�
	for(add=1<<(bits-1); add<1<<bits; add++){ // �����Ō��Ă݂�
//	for(add=(1<<bits)-1; add>=1<<(bits-1); add--){ // �����Ō��Ă݂�

		max = (int)(base / target * add + 0.5); // 0.5�͎l�̌ܓ��������������P�[�X������
//		max = (int)(base / target * add);

		double hz = base * ((double)add / max);
		double gosa = hz - target;
		gosa_min = fabs(gosa);
		if(gosa_min < best_gosa){
			best_add = add;
			best_max = max;
			best_gosa = gosa_min;
			
			// �덷(PPM:Parts Per Million)
			double ppm = best_gosa / target * 1000000;

		//	printf("�X�V �덷 %f  %f Hz\n", gosa_min, hz);
			printf("���Z�l %2dbit %7d �������l %8d  �덷 %17.10lf %14.8lfPPM\n", bits, best_add, best_max, best_gosa, ppm);
		//	printf("%2dbit %7d %8d %17.10lf\n", bits, add, max, gosa_min);
		//	if(best_gosa==0.0) goto END; // NG
			if(best_gosa < gosa_siki) goto END;
		}
	}
}
END:
//	printf("���Z�l %d:0x%X  �������l %d:0x%X  �덷 %f\n", best_add, best_add, best_max, best_max, best_gosa);
	printf("%dbit ���Z�l %d:0x%X  �������l %d:0x%X  �덷 %0.10lf Hz\n", bits, best_add, best_add, best_max, best_max, best_gosa);

	return 0;
}
