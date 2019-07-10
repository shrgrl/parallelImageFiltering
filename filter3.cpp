#include <iostream> // cout-cin için
#include <fstream> //dosya iþlemleri için
#include <string> //substr, append için
#include <math.h>
#include <mpi.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int main(int argc, char** argv) {
	//Deðiþkenleri tanýmlayalým.
	int satir, sutun, siralananDizi[9]; 
	int medyan, diziIJ;
	int basamak = 0; 	
	int size, myRank, kismiSatir, kismiSutun, kismiBoyut;
	double startTime, endTime, elapsedTime;
	int boyut;
	
	//Dinamik dizileri tanimliyoruz.
	int *girisDizisiPtr;
	int *sonucMatrisiPtr;
	int *kismiGirisDizisiPtr;
	int *kismiSonucMatrisiPtr;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &size); //Tum bilgisayarlar kumenin size ini ogrensin.
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank); //Tum bilgisayarlar kendi ID sini ogrensin.
	
	if(myRank==0) //Master bilgisayar icin. 
	{
		//kullanýcýdan aldýðýmýz argumanlarý kontrol edeceðiz.
	
		if(argc!=2) 
		{
			cout<<"Lutfen bir adet arguman giriniz.\n";
			cout<<"Kullanim Bu Sekildedir : ./a.out GirisDosyasininAdi.txt\n";
			MPI_Abort(MPI_COMM_WORLD,99);	
		
		}
		
		//okuma amacý ile bir dosya açalým ve satýr-sutun deðerlerini alalým.
		ifstream okunacakDosya(argv[1]);
		if(okunacakDosya.fail())
		{
			cout<<"Dosya Bulunamadi!\n";
			MPI_Abort(MPI_COMM_WORLD,99);	
		}	
		
		okunacakDosya>>satir>>sutun;
			
		boyut=satir*sutun;	
		if(boyut%size!=0)
		{
			cout<<"Dizi esit paylastiralamiyor!\n";
			MPI_Abort(MPI_COMM_WORLD,99);	
		}	
		
		/*Bu algoritmada biz sutun sayilarini sabit tutmaya calisacagiz. 
		Çunku send fonksiyonu ile tek boyutlu bir dizi gonderiyoruz. 
		Ana dizinin duzeninin bozulmamasi icin sutunlara dokunmayip 
		sadece satirlara gore matrisi bolecegiz. Burada iki tane durum karsilasacagiz. 
		Eger satir size a tam bolunuyorsa bolunen kisimlar ID sirasina gore master tarafindan 
		diger bilgisayarlara gonderilecek, tam bolunmuyorsa kalan kisim master tarafindan isletilecek.*/
		
		kismiSutun=sutun;
		kismiSatir=satir/size;
		kismiBoyut=kismiSatir*kismiSutun;
		
		//Master alan rezerve etti.
		girisDizisiPtr = new int[boyut]; 
		sonucMatrisiPtr = new int[boyut];

		
		//Dosyadan deðergleri okuyup giriþ dosyasýna aktaralým.
	
		for(int i=0; i<satir; i++)
		{
			for(int j=0; j<sutun; j++)
			{
				diziIJ = i*sutun+j;
				okunacakDosya>>girisDizisiPtr[diziIJ];
			}
		}
		okunacakDosya.close();		
		startTime=MPI_Wtime();
					
	}// if satir 31 
	
	// global kisminda yapmamiz gerekiyor 
	MPI_Bcast(&kismiBoyut,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&kismiSatir,1,MPI_INT,0,MPI_COMM_WORLD);	
	MPI_Bcast(&kismiSutun,1,MPI_INT,0,MPI_COMM_WORLD);
	
	kismiGirisDizisiPtr = new int[kismiBoyut];
	kismiSonucMatrisiPtr = new int[kismiBoyut];	
	
	MPI_Scatter(girisDizisiPtr,kismiBoyut,MPI_INT,kismiGirisDizisiPtr,kismiBoyut,MPI_INT,0,MPI_COMM_WORLD);
	
	/*dizimizin içinde dolaþacaðýz. Bunun için iki adet for döngüsü kullanacaðýz.Birinci satýr ve sütun
	sonuncu satir ve sütunu hariç tutarak her elemanýn tek tek komþuluklarýný kontrol edip 3x3'lük bir diziye atacaðýz
	(iki boyutlu diziyi tek boyutlu olarak temsil edeceðiz. 2. slayt, 22. sayfadaki gibi.)*/
	//Filtreleme iþlemleri burada yapýlýyor.
	for(int i=0; i<kismiSatir; i++)
	{
		for(int j=0; j<kismiSutun; j++) 
		{
			diziIJ = i*kismiSutun+j;
			if(i==0 || j==0 || i==kismiSatir-1 || j==kismiSutun-1) 
			{
				kismiSonucMatrisiPtr[diziIJ] = kismiGirisDizisiPtr[diziIJ];
			}
			else 
			{ 
				siralananDizi[0] = kismiGirisDizisiPtr[(i-1)*kismiSutun+(j-1)];
				siralananDizi[1] = kismiGirisDizisiPtr[(i-1)*kismiSutun+(j)];
				siralananDizi[2] = kismiGirisDizisiPtr[(i-1)*kismiSutun+(j+1)];
				siralananDizi[3] = kismiGirisDizisiPtr[(i)*kismiSutun+(j-1)];
				siralananDizi[4] = kismiGirisDizisiPtr[(i)*kismiSutun+(j)];
				siralananDizi[5] = kismiGirisDizisiPtr[(i)*kismiSutun+(j+1)];
				siralananDizi[6] = kismiGirisDizisiPtr[(i+1)*kismiSutun+(j-1)];
				siralananDizi[7] = kismiGirisDizisiPtr[(i+1)*kismiSutun+(j)];
				siralananDizi[8] = kismiGirisDizisiPtr[(i+1)*kismiSutun+(j+1)];
						
				//eklemeli sýralama algoritmasý ile sýralayacaðýz.
						
				for(int x=0; x<9; x++) 
				{
					for(int y=0; y<8; y++) 
					{
						if(siralananDizi[y]>siralananDizi[y+1]) 
						{
							int geciciDegisken = siralananDizi[y+1];
							siralananDizi[y+1] = siralananDizi[y];
							siralananDizi[y] = geciciDegisken;
						}
					}
				}
						
				medyan = siralananDizi[9/2];
				kismiSonucMatrisiPtr[diziIJ] = medyan;
					
			}
		}
	}
	// kismisonuc diziler sonuc dizi uzerinde master bilgisayarda topliyacagiz 
	MPI_Gather(kismiSonucMatrisiPtr,kismiBoyut,MPI_INT,sonucMatrisiPtr,kismiBoyut,MPI_INT,0,MPI_COMM_WORLD);	
		
	if(myRank==0)
	{
		if(satir%size!=0) //Filtreleme islemleri yaptýktan sonra kalan satirlari master isletecek.
		{
			for(int i=satir-(satir%size); i<satir; i++)
			{
				for(int j=0; j<sutun; j++)
				{
					diziIJ = i*sutun+j;
					if(i==0 || j==0 || i==satir-1 || j==sutun-1) 
					{
						sonucMatrisiPtr[diziIJ] = girisDizisiPtr[diziIJ];
					}
				
			
					else 
					{ 

						siralananDizi[0] = girisDizisiPtr[(i-1)*sutun+(j-1)];
						siralananDizi[1] = girisDizisiPtr[(i-1)*sutun+(j)];
						siralananDizi[2] = girisDizisiPtr[(i-1)*sutun+(j+1)];
						siralananDizi[3] = girisDizisiPtr[(i)*sutun+(j-1)];
						siralananDizi[4] = girisDizisiPtr[(i)*sutun+(j)];
						siralananDizi[5] = girisDizisiPtr[(i)*sutun+(j+1)];
						siralananDizi[6] = girisDizisiPtr[(i+1)*sutun+(j-1)];
						siralananDizi[7] = girisDizisiPtr[(i+1)*sutun+(j)];
						siralananDizi[8] = girisDizisiPtr[(i+1)*sutun+(j+1)];
						
						//eklemeli sýralama algoritmasý ile sýralayacaðýz.
						
						for(int x=0; x<9; x++) 
						{
							for(int y=0; y<8; y++) 
							{
								if(siralananDizi[y]>siralananDizi[y+1]) 
								{
									int geciciDegisken = siralananDizi[y+1];
									siralananDizi[y+1] = siralananDizi[y];
									siralananDizi[y] = geciciDegisken;
								}
							}
						}
						
					medyan = siralananDizi[9/2];
					sonucMatrisiPtr[i*sutun+j] = medyan;
					
					}						
				}
			}
		}
		
		//Burada komsulari kontrol edecegiz.
		/*Kismi dizinin ilk satiri ve son satirinda filtreleme islemi yapacagiz. 
		(komsulari sikintili(kismiSatir-1 ve kismiSatir) olan satirlar masterda isletilecek.) */
		
		//Ilk olarak kismiSatir-1'i kontrol edelim.
		for(int i=kismiSatir-1; i<satir-1; i+=kismiSatir)
		{
			for(int j=1; j<sutun-1; j++) //Birinci satir ve sonuncu satir haric.
			{
				diziIJ = i*sutun+j;
				siralananDizi[0] = girisDizisiPtr[(i-1)*sutun+(j-1)];
				siralananDizi[1] = girisDizisiPtr[(i-1)*sutun+(j)];
				siralananDizi[2] = girisDizisiPtr[(i-1)*sutun+(j+1)];
				siralananDizi[3] = girisDizisiPtr[(i)*sutun+(j-1)];
				siralananDizi[4] = girisDizisiPtr[(i)*sutun+(j)];
				siralananDizi[5] = girisDizisiPtr[(i)*sutun+(j+1)];
				siralananDizi[6] = girisDizisiPtr[(i+1)*sutun+(j-1)];
				siralananDizi[7] = girisDizisiPtr[(i+1)*sutun+(j)];
				siralananDizi[8] = girisDizisiPtr[(i+1)*sutun+(j+1)];
						
				//eklemeli sýralama algoritmasý ile sýralayacaðýz.
						
				for(int x=0; x<9; x++) 
				{
					for(int y=0; y<8; y++) 
					{
						if(siralananDizi[y]>siralananDizi[y+1]) 
						{
							int geciciDegisken = siralananDizi[y+1];
							siralananDizi[y+1] = siralananDizi[y];
							siralananDizi[y] = geciciDegisken;
						}
					}
				}
						
				medyan = siralananDizi[9/2];
				sonucMatrisiPtr[i*sutun+j] = medyan;				
			}
			
		}
		//Simdi kismiSatir'i kontrol edelim.
		for(int i=kismiSatir; i<satir-1; i+=kismiSatir)
		{
			for(int j=1; j<sutun-1; j++) //Birinci satir ve sonuncu satir haric.
			{
				diziIJ = i*sutun+j;
				siralananDizi[0] = girisDizisiPtr[(i-1)*sutun+(j-1)];
				siralananDizi[1] = girisDizisiPtr[(i-1)*sutun+(j)];
				siralananDizi[2] = girisDizisiPtr[(i-1)*sutun+(j+1)];
				siralananDizi[3] = girisDizisiPtr[(i)*sutun+(j-1)];
				siralananDizi[4] = girisDizisiPtr[(i)*sutun+(j)];
				siralananDizi[5] = girisDizisiPtr[(i)*sutun+(j+1)];
				siralananDizi[6] = girisDizisiPtr[(i+1)*sutun+(j-1)];
				siralananDizi[7] = girisDizisiPtr[(i+1)*sutun+(j)];
				siralananDizi[8] = girisDizisiPtr[(i+1)*sutun+(j+1)];
						
				//eklemeli sýralama algoritmasý ile sýralayacaðýz.
						
				for(int x=0; x<9; x++) 
				{
					for(int y=0; y<8; y++) 
					{
						if(siralananDizi[y]>siralananDizi[y+1]) 
						{
							int geciciDegisken = siralananDizi[y+1];
							siralananDizi[y+1] = siralananDizi[y];
							siralananDizi[y] = geciciDegisken;
						}
					}
				}
						
				medyan = siralananDizi[9/2];
				sonucMatrisiPtr[i*sutun+j] = medyan;				
			}
			
		}
		endTime=MPI_Wtime();
		elapsedTime = endTime - startTime;
		printf("sure : %lf\n",elapsedTime*1000);
		string cikisDosyasininAdi(argv[1]);
	 
		if (cikisDosyasininAdi.length()==19)
			cikisDosyasininAdi = cikisDosyasininAdi.insert(15, "_filtered");
		else
			cikisDosyasininAdi = cikisDosyasininAdi.insert(16, "_filtered");
		remove(cikisDosyasininAdi.c_str()); //Dosya mevcut ise silinsin. str C++ özelliði olduðundan bunu C stiline dönüþtürmek için .c_str kullandýk.  
		ofstream cikisDosyasi(cikisDosyasininAdi.c_str(), ios::app); //çýkýþ dosyasýnýn adýyla okuma amaçlý bir dosya açtýk.
		for(int i=0; i<satir; i++)
		{
			for(int j=0; j<sutun; j++) 
			{		
				basamak=((sonucMatrisiPtr[i*sutun+j]<=1)? 1 : log10(sonucMatrisiPtr[i*sutun+j])+1);
				//sonuc dosyasýnýn görsel dizyaný için. her deðer kaç basamaklý ise öðrenilip ona göre boþluklarý ayarlýyoruz.
				if(basamak==1) 
				{
					cikisDosyasi<<sonucMatrisiPtr[i*sutun+j]<<"   ";
				
				}
				else if(basamak==2)
				{
					cikisDosyasi<<sonucMatrisiPtr[i*sutun+j]<<"  ";
			
				}
				else
					cikisDosyasi<<sonucMatrisiPtr[i*sutun+j]<<" ";				
			}
		cikisDosyasi<<endl;	
		}
		
		delete [] girisDizisiPtr;
		delete [] sonucMatrisiPtr;
		cikisDosyasi.close();		
	}//if satir 148	

		//Alan rezerve ettik.
	
		
		//Rezerve ettigimiz alani yok ettik.
		delete [] kismiGirisDizisiPtr;
		delete  [] kismiSonucMatrisiPtr;
		
		

	
	MPI_Finalize();
	
	return 0;
}
