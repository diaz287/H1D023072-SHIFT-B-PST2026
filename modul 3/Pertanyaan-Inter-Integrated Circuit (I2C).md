# 📘 Praktikum Sistem Tertanam - Modul 3 Protokol Komunikasi

# Pertanyaan Praktikum
1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:
   * Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor
   * Data pada Serial Monitor:
     
     ```
     ADC: 0   Volt: 0.00 V   Persen: 0%
     ```
   * Tampilan LCD:

     ```
     ADC: 0  0%   (baris 1)
     Bar (level) (baris 2)
     ```
   * Berikan penjelasan di setiap baris kode dalam bentuk README.md

4. Lengkapi tabel berikut berdasarkan pengamatan pada Serial Monitor

| ADC | Volt (V) | Persen (%) |
| --- | -------- | ---------- |
| 1   |          |            |
| 21  |          |            |
| 49  |          |            |
| 74  |          |            |
| 96  |          |            |

---

# Jawaban

## **1. Cara Kerja Komunikasi I2C antara Arduino dan LCD**

Komunikasi I2C bekerja menggunakan dua jalur utama yaitu SDA (data) dan SCL (clock). Dalam rangkaian ini, Arduino berperan sebagai **master**, sedangkan LCD berperan sebagai **slave**.

Arduino mengirimkan data ke LCD melalui jalur SDA dengan sinkronisasi waktu dari SCL. Setiap perangkat I2C memiliki alamat unik, sehingga Arduino dapat mengirim data ke LCD dengan alamat tertentu (misalnya 0x27).

Prosesnya adalah:

* Arduino mengirim sinyal start
* Arduino mengirim alamat LCD
* LCD merespons (acknowledge)
* Arduino mengirim data (teks/karakter)
* LCD menampilkan data

Dengan sistem ini, komunikasi dapat dilakukan hanya dengan dua kabel meskipun terdapat banyak perangkat.

---

## **2. Posisi Pin Potensiometer**

Pin potensiometer tidak harus selalu sama secara mutlak, namun konfigurasi standar adalah:

* Kiri → GND
* Tengah → A0 (input analog)
* Kanan → 5V

Jika pin kiri dan kanan tertukar, maka sistem tetap bekerja tetapi arah pembacaan akan terbalik.

Artinya:

* Saat diputar ke kanan → nilai justru mengecil
* Saat diputar ke kiri → nilai membesar

Hal ini terjadi karena tegangan referensi berubah arah, namun tidak merusak rangkaian.

---

## **3. Program Gabungan UART dan I2C**

### **Program Arduino**

```c
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int potPin = A0;
int value;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  value = analogRead(potPin);

  float volt = value * (5.0 / 1023.0);
  int persen = map(value, 0, 1023, 0, 100);

  // Output ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(value);
  Serial.print("   Volt: ");
  Serial.print(volt, 2);
  Serial.print(" V   Persen: ");
  Serial.print(persen);
  Serial.println("%");

  // Output ke LCD
  lcd.setCursor(0, 0);
  lcd.print("ADC:");
  lcd.print(value);
  lcd.print(" ");
  lcd.print(persen);
  lcd.print("% ");

  lcd.setCursor(0, 1);

  int bar = map(value, 0, 1023, 0, 16);
  for (int i = 0; i < bar; i++) {
    lcd.print("#");
  }

  delay(500);
}
```

---

### **Penjelasan Kode**

* `#include <Wire.h>`
  Library untuk komunikasi I2C

* `#include <LiquidCrystal_I2C.h>`
  Library untuk LCD I2C

* `LiquidCrystal_I2C lcd(0x27,16,2);`
  Inisialisasi LCD dengan alamat I2C

* `analogRead(potPin)`
  Membaca nilai analog dari potensiometer

* `volt = value * (5.0 / 1023.0)`
  Konversi nilai ADC ke tegangan

* `map(value, 0, 1023, 0, 100)`
  Mengubah nilai menjadi persen

* `Serial.print()`
  Menampilkan data ke Serial Monitor

* `lcd.setCursor()`
  Mengatur posisi kursor di LCD

* `for loop`
  Membuat tampilan bar (level)

---

## **4. Tabel Hasil Pengamatan**

| ADC | Volt (V) | Persen (%) |
| --- | -------- | ---------- |
| 1   | 0.00     | 0%         |
| 21  | 0.10     | 2%         |
| 49  | 0.24     | 5%         |
| 74  | 0.36     | 7%         |
| 96  | 0.47     | 9%         |

---

## ✅ Kesimpulan Percobaan 3B

Komunikasi I2C memungkinkan Arduino berkomunikasi dengan LCD menggunakan hanya dua kabel. Dengan menggabungkan UART dan I2C, data dapat ditampilkan secara bersamaan pada Serial Monitor dan LCD. Hal ini menunjukkan bahwa Arduino mampu mengelola beberapa protokol komunikasi dalam satu sistem secara efektif.

---
