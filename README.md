# parallelImageFiltering
Paralel Programlama basit şekliyle, bir problemin çözümünde birden çok hesaplama kaynağının aynı anda kullanılmasıdır.
<ul>
  <li>Problemin çözümünde birden çok işlemci kullanılır.</li>
  <li>Problem eşzamanlı çözülmek için farklı parçalara bölünür.</li>
  <li>Her parça farklı komut serilerine bölünür.</li>
  <li>Komutlar aynı anda farklı işlemciler üzerinde işlenir.</li>
</ul>
Bu projede, her bir uygulama farklı paralellism teknikleri kullanılarak filtreleme işlemleri yapmaktadır. Uygulamaları çalıştırmak için Ubuntu işletim sistemi edinmeniz gerekmekte. Bilgisayarınıza kurmak istemiyorsanız <strong>VirtualMachine</strong> sanal ortamına kurarak çalıştırabilirsiniz.

<i>Uygulamaların hepsinde aynı görüntü ve matrisleri kullanıldığı için tek bir görüntü, görüntü matrisi ve script dosyası üzerinden işlem yapıldı.</i>

## filter1 Uygulaması
Ubuntu\'da bir terminal açıp projenin bulunduğu dizine gidiyoruz. <i>filter1.cpp</i> uygulamasını çalıştırmak için ilk adım olarak

```
  g++ filter1.cpp
```

yazarak bir <i>a.out</i> dosyası oluşturuyoruz. Şimdi bizim görüntü matrisimizi işleyerek filtrelenmiş bir görüntü matrisi elde etmemiz gerekli. Bunun için;

```
  ./a.out GoruntuMatrisi1.txt
```

yazıyoruz ve filtrelenmiş görüntü matrisi olan GoruntuMatrisi1_filtered.txt dosyasını elde ediyoruz. Filtrelenmiş görüntüyü elde etmek için bu matrisi kullanacağız. 

Şimdi script dosyasını çalıştırarak filtrelenmiş görüntüyü elde edeceğiz. Bu işlemi yapmadan önce karşımıza çıkacak muhtemel paket yüklemelerini yapmamız gerekli. Bunun için <i>octave</i> paketine ihtiyacımız var. İndirmek için aşağıdaki adımları izleyebilirsiniz.

```
  sudo add-apt-repository ppa:octave/stable
  sudo apt-get update
  sudo apt-get install octave
```

Paket yüklemelerini bitirdikten sonra projemizin son adımı tamamlayabiliriz.

```
  ./Proje1Script.sh GoruntuMatrisi1_filtered.txt
```

Çalıştırdıktan sonra orjinal Goruntu1.bmp görüntüsü ile filtrelenmiş görüntü aşağıdaki gibi olacaktır.

![Orjinal Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img1.jpg)
![Filtrelenmiş Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img2.jpg)


## filter2 Uygulaması
Bu uygulamada farklı bir teknik olarak <strong><i>MPI</i></strong> kütüphanesi kullanıldı. MPI, paralel programlama amacıyla kullanılan bir platformdur. MPI temel olarak birden fazla bilgisayarın üzerinde aynı programın çalıştırılması ve bu çalıştırma sırasında programların birbiri ile iletişime geçerek birbirinden veri transfer etmesine dayanır. MPI ile işimizi bilgisayarlara dağıtabildiğimiz gibi bir bilgisayar üzerinde de birden fazla iş (process) çalıştırmamız mümkündür. Dolayısyla örneğin 10 bilgisayarın bulunduğu bir ortamda her bilgisayarda 3’er iş çalıştırılırsa bu durumda toplam 30 iş çalışmış olur. Yani programımız sanki 30 farklı bilgisayarda çalışıyormuş gibi davranmak zorundadır. MPI kullanımı hakkında detaylı bilgiyi [bu linkten](http://bilgisayarkavramlari.sadievrenseker.com/2009/04/29/mpi-message-passing-interface-mesaj-gecirme-arayuzu/) edinebilirsiniz.

Ubuntu\'da bir terminal açıp projenin bulunduğu dizine gidiyoruz. <i>filter1.cpp</i> uygulamasını çalıştırmak için ilk adım olarak gerekli <i>mpi</i> kütüphanesini indirmemiz gerekiyor. Sırayla aşağıdaki işlemleri yapabilirsiniz:

```
  sudo apt install libmpich-dev
  sudo apt install libopenmpi-dev
```

İndirme işlemlerini tamamladıktan sonra projemizi çalıştırabiliriz. İlk olarak;

```
  mpicxx filter2.cpp
```

ile bir a.out dosyası oluşturduk. Daha sonra;

```
  mpi run -np 4 ./a.out
```
Programın kaç kere çalışmasını istiyorsanız 4 yerine herhangi bir çift sayı yazabilirsiniz. Bu işlemin ardından filtrelenmiş görüntünün matrisini elde edeceğiz. Daha sonra;

```
  ./Proje1Script.sh GoruntuMatrisi1_filtered.txt
```

komutunu yazarak filtrelenmiş görüntüyü elde ediyoruz. Sonuçlar aşağıdaki gibi olacaktır.

![Orjinal Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img1.jpg)
![Filtrelenmiş Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img2.jpg)

## filter3 Uygulaması
Bu uygulama da <i>MPI</i> kütüphanesi kullanılarak yazıldı. Çalışması sırayla;

```
  mpicxx filter3.cpp
  mpi run -np 4 ./a.out
  ./Proje1Script.sh GoruntuMatrisi1_filtered.txt
```
Ve sonuç;

![Orjinal Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img1.jpg)
![Filtrelenmiş Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img2.jpg)

## filter4 Uygulaması
Bu uygulamada <i>OPENMP</i> kütüphanesi kullanıldı. OpenMP paralel programlama işini kolaylaştıran bir API’dir. Çalışma prensibi, paralel işletilmesi gereken kısımlarda dallanır, paralel çalışıp işlem bitince tekrar birleşmesi şeklinde olur. Uygulamamızda kullanabilmek için öncelikle gerekli indirmeleri yapmalıyız.

```
  sudo apt-get install libomp-dev
```

Sorunsuz şekilde indirmelerimizi tamamladıktan sonra uygulamayı çalıştırabiliriz.

```
  g++ filter4.cpp -fopenmp
  ./a.out GoruntuMatrisi1.txt
  ./Proje1Script.sh GoruntuMatrisi1_filtered.txt
```

Çalıştırdıktan sonra orjinal ve filtrelenmiş görüntü aşağıdaki gibi olacaktır:

![Orjinal Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img1.jpg)
![Filtrelenmiş Görüntü](https://github.com/shrgrl/parallelImageFiltering/blob/master/img2.jpg)

