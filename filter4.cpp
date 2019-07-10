#include <iostream> //cout, cin islemleri icin gerekli kutuphane
#include <fstream> //Dosya islemleri icin gerekli kutuphane
#include <string> //substr, append kullanimi icin gerekli kutuphane
#include <math.h> //Medyan hesabi gibi matematiksel islemler yapmada gerekli olan kutuphane
#include <sys/time.h> //Zaman hesaplamasÄ± icin gerekli olan kutuphane
#include <omp.h> //#pragma icin 

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {

	//Kullanicidan aldigimiz argumanlari kontrol edecegiz.

	if(argc!=2) { 
		cout<<"Lutfen bir adet arguman giriniz! \n";
		cout<<"Kullanim bu sekildedir: ./a.out GirisDosyasininAdi.txt \n";
		exit(1);
	}
		//Degiskenleri burada  tanimlayacagiz.

		int satir, sutun, siralananDizi[9];
		int medyan;
		int basamak = 0;
		int nThreads;

	//Okuma amaci ile bir dosya acalim ve satir-sutun degerlerini alalim.

	ifstream okunacakDosya(argv[1]);

	string cikisDosyasininAdi(argv[1]);

	if (cikisDosyasininAdi.length()==19)
		cikisDosyasininAdi = cikisDosyasininAdi.insert(15, "_filtered");
	else
		cikisDosyasininAdi = cikisDosyasininAdi.insert(16, "_filtered");
		//Dosya mevcut ise silinecek. Ayrica str C++'a ait bir ozellik oldugu icin bunu C stiline .c_str kullnarak cevirdik.
		remove(cikisDosyasininAdi.c_str());
		ofstream cikisDosyasi(cikisDosyasininAdi.c_str(), ios::app); //Cikis  dosyasinin adiyla okuma amacli bir dosya actik.

		okunacakDosya>>satir>>sutun;

	if(okunacakDosya.fail()){
		cout<<"Dosya Bulunamadi!\n";
		exit(1);
	}


	//Ä°ki adet dinamik dizi olusturalim.

	int **girisMatrisiPtr = new int*[satir]; //Ilk boyutu olusturduk.

	for(int i=0; i<satir; i++)  //Ikinci buyutu olusturduk.
		girisMatrisiPtr[i] = new int[sutun]; 

	int **sonucMatrisiPtr = new int*[satir]; //Ilk boyutu olusturduk.

	for(int i=0; i<satir; i++)  //Ikinci buyutu olusturduk.
		sonucMatrisiPtr[i] = new int[sutun]; 



	//Dosyadan degerleri okuyup giris dosyasina aktaralim.

	for(int i=0; i<satir; i++){
		for(int j=0; j<sutun; j++){
			okunacakDosya>>girisMatrisiPtr[i][j];
		}
	}

	/*Dizimizin icinde dolasacagiz.Bunun icin iki adet for dongusu kullanacagiz.Birinci satir ve sutun
	sonuncu satir ve sutunu haric tutarak her elemanin tek tek komsuluklarini kontrol edip 3x3'luk bir diziye atacagiz
	(Iki boyutlu diziyi tek boyutlu olarak temsil edecegiz. 2. slayt, 22. sayfadaki gibi.).*/

	//Filtreleme islemleri burada yapiliyor.

	struct timeval currentTime;
	double startTime, endTime, elapsedTime;

	gettimeofday(&currentTime, NULL);
	startTime = currentTime.tv_sec+(currentTime.tv_usec/1000000.0);
	#pragma omp parallel private(medyan,siralananDizi)
	{
	// Burada paralel bir blok aciyoruz, medyan ve siralananDizi'yi private olarak tutmasak program her calistiginda farkli 
	// sonuclar verecek ve yanlis calisacak. Private olmayan degiskenler varsayýlan olarak shared tanimlidir.
	
	nThreads = omp_get_num_threads();
	
	#pragma omp for collapse(2) //pragma for varsayýlan olarak ilk donguyu 
	// otomatik dagitiyor. openmp 3 te tanimlayan bir collapse metodu var onunla ic ice donguler icin kullaniyoruz ve onlar da 
	// otomatik olarak esit bir sekilde baylastirilir.
	
		for(int i=0; i<satir; i++){
			for(int j=0; j<sutun; j++) {
				if(i==0 || j==0 || i==satir-1 || j==sutun-1) {
					sonucMatrisiPtr[i][j] = girisMatrisiPtr[i][j];
				}
				//Sadece filtreleme islemleri icin sure olcecegiz.
				else {

						siralananDizi[0] = girisMatrisiPtr[i-1][j-1];
						siralananDizi[1] = girisMatrisiPtr[i-1][j];
						siralananDizi[2] = girisMatrisiPtr[i-1][j+1];
						siralananDizi[3] = girisMatrisiPtr[i][j-1];
						siralananDizi[4] = girisMatrisiPtr[i][j];
						siralananDizi[5] = girisMatrisiPtr[i][j+1];
						siralananDizi[6] = girisMatrisiPtr[i+1][j-1];
						siralananDizi[7] = girisMatrisiPtr[i+1][j];
						siralananDizi[8] = girisMatrisiPtr[i+1][j+1];

						//Eklemeli siralama algoritmasi ile siralayacagiz.

						for(int x=0; x<9; x++) {
							for(int y=0; y<8; y++) {
								if(siralananDizi[y]>siralananDizi[y+1]) {
									int geciciDegisken = siralananDizi[y+1];
									siralananDizi[y+1] = siralananDizi[y];
									siralananDizi[y] = geciciDegisken;
								}
							}
						}

					//Medyan hesaplama islemi burada yapiliyor.

					medyan = siralananDizi[9/2];
					sonucMatrisiPtr[i][j] = medyan;

					}// else satir 90

		} // icteki for
		
	} // distaki for
} //#pragma
	for(int i=0; i<satir; i++)
	{
		for(int j=0; j<sutun; j++) 
		{			
			basamak=((sonucMatrisiPtr[i][j]<=1)? 1 : log10(sonucMatrisiPtr[i][j])+1);
			/*Sonuc dosyasinin gorsel dizyani icin kosul yaziyoruz.Her deger kac basamakli ise ogrenilip ona gore
			bosluklari ayarliyoruz.*/
			if(basamak==1) {
				cikisDosyasi<<sonucMatrisiPtr[i][j]<<"   ";
			}
			else if(basamak==2) {
				cikisDosyasi<<sonucMatrisiPtr[i][j]<<"  ";
			}
			else
				cikisDosyasi<<sonucMatrisiPtr[i][j]<<" ";
		}
		cikisDosyasi<<endl;
	}

	gettimeofday(&currentTime, NULL);
	endTime = currentTime.tv_sec+(currentTime.tv_usec/1000000.0);

	elapsedTime = endTime-startTime;
	printf("%d tane thread kullanarak olculen toplam zaman: %lf ms\n", nThreads, elapsedTime*1000);

	//Olusturdugumuz dinamik dizileri yok edelim.

	for(int i=0; i<satir; i++) //Giris matrisi uzerinde yapilacak islem.
		delete [] girisMatrisiPtr[i]; //Sutunlari yok ediyoruz.

	delete [] girisMatrisiPtr; //Satirlari yok ediyoruz.

	for(int i=0; i<satir; i++) //Sonuc matrisi uzerinde yapilacak islem.
		delete [] sonucMatrisiPtr[i]; //Sutunlari yok edelim.

	delete [] sonucMatrisiPtr; //Satirlari yok edelim.

	//Cikis islemlerini gerceklestiriyoruz.

	cikisDosyasi.close();
	okunacakDosya.close();

	return 0;
}
