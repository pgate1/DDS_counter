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
	const int base = 
	//	50000000; // DE0
		27000000; // TangNano9K

	const double target = 
	//	1789772.5; // NES �������g��
	//	1662607; // NES PAL �������g��
	//	5369318; // NES PPU �������g��
	//	10000000; // test �������g��
	//	1024000; // SNES APU �������g��1
	//	2048000; // SNES APU �������g��2
	//	10738635; // SNES CPU base
	//	33868800; // PSX �������g��
	//	6144000; // test https://twitter.com/twi_kingyo/status/1112040773992640513
	//	0.2; // test
	//	0.01; // test ���Ԃ�������
	//	499006; // test ���Ԃ�������
		124875000; // TangNano9K HDMI
	//	224875200; // test

	int add, max;

	int best_add, best_max;
	double best_gosa = base;

	const double gosa_siki = 0.00000000001;

	if(target>=1){
		int gcd = euclid(base, (int)(target));
		printf("�ő����(GCD) %d\n", gcd);
	}

	int f_mode = 0; // ����
	if(target > base) f_mode = 1; // ���{

//	printf(" add   ���Z�l �������l   �덷\n");
	int bit_max = 32;
	if(f_mode) bit_max = 8; // ���{

int add_bit;
for(add_bit=1; add_bit<=bit_max; add_bit++){

//	for(add=target/gcd; add<1<<bits; add++){ // gcd==1�̎��͖��ɗ����Ȃ�
	for(add=1<<(add_bit-1); add<1<<add_bit; add++){ // �����Ō��Ă݂�
//	for(add=(1<<bits)-1; add>=1<<(bits-1); add--){ // �����Ō��Ă݂�

//		max = (int)(base / target * add);
		max = (int)((double)add * base / target + 0.5); // 0.5�͎l�̌ܓ��������������P�[�X������

		double hz = base * ((double)add / max);
		double gosa = hz - target;
		if(fabs(gosa) < fabs(best_gosa)){
			best_add = add;
			best_max = max;
			best_gosa = gosa;
			
			// �덷(PPM:Parts Per Million)
			double ppm = best_gosa / target * 1000000;

			// ����
			if(f_mode==0){
			//	printf("�X�V �덷 %f  %f Hz\n", gosa, hz);
			//	printf("%2dbit %7d %8d %17.10lf\n", bits, add, max, gosa);
				printf("���Z�l %2dbit %7d �������l %8d  �덷 %17.10lf %14.8lfPPM\n", add_bit, best_add, best_max, best_gosa, ppm);
			}
			// ���{
			else{
				printf("���g�� %f  Mul %8d  Div %7d  �덷 %17.10lf\n", hz, best_add, best_max, best_gosa);
			}
			
			//	if(best_gosa==0.0) goto END; // NG
			if(fabs(best_gosa) < gosa_siki) goto END;
		}
	}
}
END:
//	printf("���Z�l %d:0x%X  �������l %d:0x%X  �덷 %f\n", best_add, best_add, best_max, best_max, best_gosa);
//	printf("%dbit ���Z�l %d:0x%X  �������l %d:0x%X  �덷 %0.10lf Hz\n", add_bits, best_add, best_add, best_max, best_max, best_gosa);

	return 0;
}
