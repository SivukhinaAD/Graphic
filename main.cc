/*
 * les.cc
 *
 *  Created on: 14 но€б. 2020 г.
 *      Author: Anastasiya
 */
#define _USE_MATH_DEFINES//дл€ доступа к некотомы функци€м,например, M_PI
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdint>
//рисование функции в файл
double sinc(double x){//вычисление sinc(x)
	if(x==0) return 1.;
	return sin(x)/x;
}
double ff(double x,double y){//вычисление z
	return sinc(sqrt(x*x+y*y));
}

constexpr int W = 1280, H = 1024;
constexpr double RX=9.,RY=18.,RZ=140.,XMAX=20.,XMIN=-20.,YMAX=20.,YMIN=-20.,STEP1=0.4,STEP2=0.01;

#pragma pack(push,1)//описание глобальной упакованной структуры
struct TGA{
	uint8_t id_len;
	uint8_t pal_type;
	uint8_t img_type;
	uint8_t pal_desc[5];
	uint16_t xpos,ypos;
	uint16_t w,h;//ширина и высота
	uint8_t bpp;//разр€дность цвета
	uint8_t img_desc;
};
#pragma pack(pop)

void plot(std::vector<uint32_t>& pixels, int x, int y, uint32_t c,std::vector<int>& h)//метод на проверку выхода за картинку
{
	if (x < 0 || x >= W) return;
	if ( y > h[x]) return;
	h[x]=y;
	if(y<0)return;
	pixels[y*W + x] = c;

}

int main(){
	int sx=0,sy=0;
	double z;
	std::fstream f;
	TGA h {};//зануление всей структуры
	std::vector<int> hor;
	hor.resize(W);
	for(auto && a:hor)a=H;
	std::vector <uint32_t> p;
	p.resize(W*H);
	for(auto && c:p)//&-ссылка на элемент, &&-конкретно тот самый элемент, если без амперсанда вообще, то будет копи€ данного элемента
		c=0x1F45001F;
	h.img_type=2;
	h.w=W;
	h.h=H;
	h.bpp=32;
	h.img_desc=0x38;
	for(double i=XMAX;i>XMIN;i=i-STEP1){
			for(double j=YMAX;j>YMIN;j=j-STEP2){
				z=ff(i,j);
				sx=int(W/2-RX*i*cos(M_PI*3/4)+RY*j*cos(0));
				sy=int(H/2+RX*i*sin(M_PI*3/4)+RY*j*sin(0)-RZ*z);

				plot(p, sx, sy, 0xff000000,hor);
			}
		}
	for(auto && a:hor)a=H;
	for(double j=YMAX;j>YMIN;j=j-STEP1){
				for(double i=XMAX;i>XMIN;i=i-STEP2){
					z=ff(i,j);
					sx=int(W/2-RX*i*cos(M_PI*3/4)+RY*j*cos(0));
					sy=int(H/2+RX*i*sin(M_PI*3/4)+RY*j*sin(0)-RZ*z);

					plot(p, sx, sy, 0xff000000,hor);
				}
			}
	f.open("1.tga",std::ios::out|std::ios::binary);
	f.write(reinterpret_cast<char*>(&h),sizeof(TGA));
	f.write(reinterpret_cast<char*>(&p[0]),W*H*4);//ширина*высота*глубина
	f.close();
	return 0;

}
