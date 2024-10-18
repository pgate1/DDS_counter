/*
 DDSで使用する加算値としきい値を求めるプログラム

	clk_in : 入力クロック
	max : しきい値
	add : 加算値
	count : カウンタ
	sa : カウンタとしきい値の差
	enable_out : 出力クロック

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
	//	1789772.5; // NES 生成周波数
	//	1662607; // NES PAL 生成周波数
	//	5369318; // NES PPU 生成周波数
	//	10000000; // test 生成周波数
	//	1024000; // SNES APU 生成周波数1
	//	2048000; // SNES APU 生成周波数2
	//	10738635; // SNES CPU base
	//	33868800; // PSX 生成周波数
	//	6144000; // test https://twitter.com/twi_kingyo/status/1112040773992640513
	//	0.2; // test
	//	0.01; // test 時間がかかる
	//	499006; // test 時間がかかる
		124875000; // TangNano9K HDMI
	//	224875200; // test

	int add, max;

	int best_add, best_max;
	double best_gosa = base;

	const double gosa_siki = 0.00000000001;

	if(target>=1){
		int gcd = euclid(base, (int)(target));
		printf("最大公約数(GCD) %d\n", gcd);
	}

	int f_mode = 0; // 分周
	if(target > base) f_mode = 1; // 逓倍

//	printf(" add   加算値 しきい値   誤差\n");
	int bit_max = 32;
	if(f_mode) bit_max = 8; // 逓倍

int add_bit;
for(add_bit=1; add_bit<=bit_max; add_bit++){

//	for(add=target/gcd; add<1<<bits; add++){ // gcd==1の時は役に立たない
	for(add=1<<(add_bit-1); add<1<<add_bit; add++){ // 増加で見てみる
//	for(add=(1<<bits)-1; add>=1<<(bits-1); add--){ // 減少で見てみる

//		max = (int)(base / target * add);
		max = (int)((double)add * base / target + 0.5); // 0.5は四捨五入した方が検査ケースが多い

		double hz = base * ((double)add / max);
		double gosa = hz - target;
		if(fabs(gosa) < fabs(best_gosa)){
			best_add = add;
			best_max = max;
			best_gosa = gosa;
			
			// 誤差(PPM:Parts Per Million)
			double ppm = best_gosa / target * 1000000;

			// 分周
			if(f_mode==0){
			//	printf("更新 誤差 %f  %f Hz\n", gosa, hz);
			//	printf("%2dbit %7d %8d %17.10lf\n", bits, add, max, gosa);
				printf("加算値 %2dbit %7d しきい値 %8d  誤差 %17.10lf %14.8lfPPM\n", add_bit, best_add, best_max, best_gosa, ppm);
			}
			// 逓倍
			else{
				printf("周波数 %f  Mul %8d  Div %7d  誤差 %17.10lf\n", hz, best_add, best_max, best_gosa);
			}
			
			//	if(best_gosa==0.0) goto END; // NG
			if(fabs(best_gosa) < gosa_siki) goto END;
		}
	}
}
END:
//	printf("加算値 %d:0x%X  しきい値 %d:0x%X  誤差 %f\n", best_add, best_add, best_max, best_max, best_gosa);
//	printf("%dbit 加算値 %d:0x%X  しきい値 %d:0x%X  誤差 %0.10lf Hz\n", add_bits, best_add, best_add, best_max, best_max, best_gosa);

	return 0;
}
