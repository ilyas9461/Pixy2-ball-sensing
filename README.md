<h1 align="center">PIXY2 Top Renk Algılama </h1>
<p  align="center">
    <img src="./img/pixy2_3_result.jpg" width="40%" height="40%" border: 5px solid #555 >
</p>

## İçerik

- [Giriş](#Giriş)
- [Çalışma Şekli](#Çalışma-Şekli)
- [Kullanılan Teknolojiler](#Kullanılan-Teknolojiler)
- [İletişim](#İletişim)

## Giriş

Bu çalışmada, Pixy2 kameradan faydalanarak kırmızı, mavi, yeşil, beyaz, sarı, turuncu toplamda 6 renkten oluşan küçük topların renkleri kamera önünden fırlatıldığında bunların tanınması ve renklerinin sonsuz aynaya yansıtılarak sesli bir biçimde söylenmesi gerçekleştirilmiştir.
Böylece küçük yaştaki çocukların (1-5 yaş) renkleri bir oyun senaryosu üzerinden öğrenmelerini kolaylaştırmak amaçlanmıştır.

<p  align="center">
    <img src="./img/Oyun-Havuzu-Topu.jpg" width="30%" height="30%" border: 5px solid #555 >
</p>


## Çalışma Şekli

Sistemi Arduino UNO kontrol etmektedir. Pixy2 kameraya renkli toplar PC yazılımı üzerinden tanıtılmaktadır. Farklı çalışma modları ve bunlara uygun oyun senaryoları geliştirilmiştir. Tanınan topların renkleri sesli olarak ve sonsuz aynada rgb ledler ile gösterilmektedir. 

<p  align="center">
    <img src="./img/kisiye-ozel-ledli-ayna-500x500.jpg" width="30%" height="40%" border: 5px solid #555 >
</p>
Pixy kamera oyun alanı olarak belirlenen mekanik yapının üst tarafına(yukarıdan aşağıya bakacak şekilde) monte edilmiştir. Topun atıldığı yerin karşısında ise sonsuz ayna bulunmaktadır. Top atış yönü toplar kameranın önünden geçecek sekilde olmalıdır. Kameranın güneş ışığından etkilenmemesi için yan duvarların kapalı yada örtülü olması önerilir. 

Örnek Senaryo :

Oyuncu alanın önünden geçereken mesafe sensörlerinden gelen bilgiye göre uygun mesafeye geldiğinde oyuncunun dikkatini çekecek random olarak bir ses verilir. Oyuncu yeterince yaklaştığında sistem bir top atmasını söyler. Sistem tarafından top rengi söylenir ve sonsuz aynada gösterilir. Böylece oyuna başlanmış olur. Önce atılan toplatın renkleri söylenirken sonrasında farklı renkteki topların atılması istenir ve oyuncuya her bildiği renk için puan verilir.

Gösterilecek renk sayısı farklı donanım kullanılarak arttırılabilir. Burada RGB şerit ledler ile net olarak gösterilebilecek olan renkler tercih edilmiştir. Bu renklerin de ana renklerden olması ayrıca tercih sebebidir.

<p  align="center">
    <img src="./schematic-pcb/pcb_3d.png" width="70%" height="70%" border: 5px solid #555 >
</p

 Oyun süresi,mod seçimi vb. ayarlar bir buton yardımı ile sesli uyarı yapılarak gerçekleştirilir.


<br>
<p  align="center">
    <img src="./schematic-pcb/pcb.png" width="70%" height="70%" border: 5px solid #555 >

</p>

## Kullanılan Teknolojiler

```bash
- ARDUINO UNO.
- C++.
- PIXY2 renk algılama.
- RGB şerit led sürme.
- PCB devre şeması çizme ve oluşturma.

```

## İletişim

- GitHub [@your-ilyas9461](https://github.com/ilyas9461)
- Linkedin [@your-linkedin](https://www.linkedin.com/in/ilyas-yağcioğlu-6a6b17217)

### *** Not : 
Devre endüstriyel olarak üretimde olduğu için PCB devre şemaları paylaşılmamıştır.
