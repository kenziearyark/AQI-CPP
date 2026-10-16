# AQI Look Up

Aplikasi terminal sederhana yang dibuat dengan C++ untuk mengambil data dari sensor PurpleAir dan memperkirakan nilai AQI PM2.5 berdasarkan skala US EPA.

## Latar belakang

Saya mendapat ide membuat aplikasi ini karena kampung halaman saya sedang mengalami masalah asap. Saat kondisinya memburuk, AQI di sana mencapai sekitar **230**. Saya ingin punya cara sederhana untuk melihat pembacaan sensor udara dan memahami seberapa buruk kualitas udaranya.

## Fitur

- Meminta API read key PurpleAir dan indeks sensor saat pertama kali dijalankan.
- Menyimpan keduanya dalam berkas lokal `settings.txt` agar tidak perlu diketik lagi pada penggunaan berikutnya.
- Mengambil pembacaan `pm2.5_atm` terbaru dari sensor menggunakan libcurl.
- Mengubah pembacaan tersebut menjadi perkiraan AQI dengan tabel batas PM2.5 US EPA.

Aplikasi mengambil **satu pembacaan saat dijalankan**. Menu yang ada saat ini belum memperbarui data secara otomatis. Angka yang ditampilkan adalah perkiraan dari pembacaan mentah sensor terbaru, bukan AQI harian resmi atau AQI NowCast.

## Kebutuhan

- Kompiler C++17, misalnya `g++`
- Header pengembangan dan pustaka libcurl
- [API read key PurpleAir](https://develop.purpleair.com/) dan indeks sensor

Proyek ini sudah menyertakan `json.hpp` untuk membaca respons API, jadi tidak perlu memasang pustaka JSON secara terpisah.

## Kompilasi dan penggunaan

Jalankan perintah berikut dari direktori proyek:

```sh
g++ -std=c++17 -O2 AQI.cpp -o AQI -lcurl
./AQI
```

Saat pertama kali dijalankan, masukkan API read key dan indeks sensor ketika diminta. Aplikasi akan menyimpannya ke `settings.txt`: baris pertama berisi API key dan baris kedua berisi indeks sensor. Pada penggunaan berikutnya, aplikasi akan membaca kedua nilai itu secara otomatis. Jalankan aplikasi dari direktori proyek agar berkas pengaturan dapat ditemukan.

## Cara menghitung perkiraan AQI

Aplikasi mengambil konsentrasi `pm2.5_atm` terbaru dari sensor luar ruangan, mencari rentangnya pada [tabel batas AQI PM2.5 US EPA](https://document.airnow.gov/technical-assistance-document-for-the-reporting-of-daily-air-quailty.pdf), lalu menghitung posisi konsentrasi tersebut dalam rentang AQI yang sesuai:

```text
AQI = I_low + (C - C_low) / (C_high - C_low) * (I_high - I_low)
```

Di sini, `C` adalah konsentrasi PM2.5. Empat nilai batas bawah dan atas diambil dari baris tabel yang sesuai. Versi saat ini memakai pembacaan mentah tanpa koreksi PurpleAir dan belum menghitung EPA NowCast.

## Sumber

- [Panduan API PurpleAir](https://community.purpleair.com/t/making-api-calls-with-the-purpleair-api/180)
- [Panduan perhitungan AQI US EPA](https://document.airnow.gov/technical-assistance-document-for-the-reporting-of-daily-air-quailty.pdf)
- [Dokumentasi libcurl](https://curl.se/libcurl/c/)
