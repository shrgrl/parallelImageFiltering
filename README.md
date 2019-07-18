# parallelImageFiltering
Paralel Programlama basit şekliyle, bir problemin çözümünde birden çok hesaplama kaynağının aynı anda kullanılmasıdır.
<ul>
  <li>Problemin çözümünde birden çok işlemci kullanılır.</li>
  <li>Problem eşzamanlı çözülmek için farklı parçalara bölünür.</li>
  <li>Her parça farklı komut serilerine bölünür.</li>
  <li>Komutlar aynı anda farklı işlemciler üzerinde işlenir.</li>
</ul>
Bu projede, her bir uygulama farklı paralellism teknikleri kullanılarak filtreleme işlemleri yapmaktadır. Uygulamaları çalıştırmak için Ubuntu işletim sistemi edinmeniz gerekmekte. Bilgisayarınıza kurmak istemiyorsanız <strong>VirtualMachine</strong> sanal ortamına kurarak çalıştırabilirsiniz.

## filter1 Uygulaması
Ubuntu\'da bir terminal açıp <i>filter1.cpp</i> uygulamasını çalıştırmak için ilk adım olarak

g++ filter1.cpp

yazıyoruz. sonra;

./a.out GoruntuMatrisi1.txt

yazarak bir <i>a.out</i> dosyası oluşturuyoruz.

Şimdi bizim görüntü matrisimizi işleyerek filtrelenmiş bir görüntü matrisi elde etmemiz gerekli. Bunun için

./a.out GoruntuMatrisi1.txt

yazıyoruz ve filtrelenmiş görüntü matrisi olan GoruntuMatrisi1_filtered.txt dosyasını elde ediyoruz. Filtrelenmiş görüntüyü elde etmek için bu matrisi kullanacağız. 

Şimdi script dosyasını çalıştırarak filtrelenmiş görüntüyü elde edeceğiz. Bu işlemi yapmadan önce karşımıza çıkacak muhtemel paket yüklemelerini yapmamız gerekli. Bunun için <i>octave</i> paketine ihtiyacımız var. İndirmek için aşağıdaki adımları izleyebilirsiniz.

sudo add-apt-repository ppa:octave/stable
sudo apt-get update
sudo apt-get install octave

Paket yüklemelerini bitirdikten sonra projemizin son adımı tamamlayabiliriz.

./Proje1Script.sh GoruntuMatrisi1_filtered.txt

Çalıştırdıktan sonra orjinal Goruntu1.bmp görüntüsü ile filtrelenmiş görüntü aşağıdaki gibi olacaktır.

![Orjinal Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img1.jpg)
![Filtrelenmiş Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img2.jpg)


## filter2 Uygulaması


## filter3 Uygulaması


## filter4 Uygulaması
