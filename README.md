# SCADE Display Modbus Control (Türkçe bilgilendirme için aşağı kaydırın)

A robust C library for integrating Modbus TCP/IP communication capabilities into HMI applications generated from Ansys SCADE Display. This project enables seamless communication between SCADE Display-generated HMI interfaces and industrial automation systems using the Modbus protocol.

## Overview

SCADE Display is a widely-used industrial software for creating Human-Machine Interface (HMI) applications. This project provides a communication layer that allows SCADE Display generated applications to communicate with PLCs and other industrial control systems using the Modbus TCP/IP protocol.

## Features

- **Modbus TCP/IP Integration**
  - Automated client connection management
  - Configurable server connection parameters
  - Support for multiple Modbus registers
  - Batch and individual value updates

- **Dynamic Configuration**
  - INI file-based configuration
  - Runtime mapping between SCADE variables and Modbus registers
  - Configurable server IP, port, and slave ID

- **Robust Error Handling**
  - Connection failure recovery
  - Timeout management
  - Comprehensive error logging
  - Memory management safety checks

- **Logging System**
  - Detailed operation logging
  - Timestamp-based entries
  - Connection status tracking
  - Value change monitoring


## Prerequisites

- Ansys SCADE Display v232 or later
- libmodbus library (must be pre-installed)
- MinGW GCC compiler (included with SCADE)
- CMake (included with SCADE)
- Ninja build system

## Building the Project

1. Create a build directory in the project root:
```bash
mkdir build
cd build
```

2. Configure the project using CMake:
```bash
"C:\Program Files\ANSYS Inc\v232\SCADE\contrib\cmake\bin\cmake.exe" ^
-D"CMAKE_C_COMPILER:PATH=C:\Program Files\ANSYS Inc\v232\SCADE\contrib\Msys64\mingw64\bin\gcc.exe" ^
-D"CMAKE_MAKE_PROGRAM:PATH=C:\Program Files\ANSYS Inc\v232\SCADE\contrib\ninja\ninja.exe" ^
-B . ^
-G "Ninja Multi-Config" ^
-D "BUILD_TARGET=exe" ^
--log-level WARNING ^
..
```

3. Build the project:
```bash
"C:\Program Files\MATLAB\R2023b\toolbox\shared\coder\ninja\win64\ninja.exe"
```

## Installation

1. Clone the repository:
```bash
git clone https://github.com/isikmuhamm/scade-display-modbus-control.git
```

2. Install dependencies:
   - Install Ansys SCADE Display v232 or later
   - Install libmodbus library
   - Ensure all SCADE environment variables are properly set

3. Build the project following the steps in the Building section

4. Add the compiled library to your SCADE project:
   - Copy the generated library files from the build directory
   - Include the header files in your SCADE-generated code


## Configuration

### Modbus Settings
Configure your connection in 

config.ini

:
```ini
[ModbusConfig]
server_ip=127.0.0.1
port=502
slave_id=1

[InputMappings]
scade_variable_name=register_address

[OutputMappings]
scade_variable_name=register_address
```

### Variable Mapping
Map your SCADE variables to Modbus registers in 

modbus_comm.c

:
```c
void init_mappings(CONTEXT_STRUCT_NAME *context) {
    // Your mappings here
}
```

## Usage

In your SCADE-generated specification file, add one of these update functions:

```c
// For individual updates
update_modbus_values(pContext);

// For batch updates
update_modbus_values_all(pContext);
```

## Architecture

- **Configuration Layer**: Handles INI file parsing and mapping setup
- **Communication Layer**: Manages Modbus TCP/IP connections
- **Data Management Layer**: Handles data synchronization between SCADE and Modbus
- **Logging System**: Provides comprehensive operation tracking

## Logging

Operations are logged to 

trackcircuit.log

, including:
- Configuration loading events
- Connection status changes
- Data transfer operations
- Error messages

## Safety Considerations

⚠️ This software is designed for industrial automation systems. Always:
- Test thoroughly before deployment
- Implement appropriate safety measures
- Validate all communications
- Monitor system behavior

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This software is available under a dual license:

1. Commercial License
- Required for commercial use
- Includes support and updates
- Contact [your email] for pricing

2. Non-Commercial License
- Free for non-commercial, educational use only
- No warranty provided
- No redistribution allowed

All rights reserved. Copyright (c) 2024.

## Support

For issues and feature requests, please use the GitHub issue tracker.

This project is designed to bridge the gap between SCADE Display's powerful HMI capabilities and industrial automation systems using the Modbus protocol.

---

# SCADE Display Modbus Kontrolü (For English, scroll up)

Ansys SCADE Display'den üretilen HMI uygulamalarına Modbus TCP/IP iletişim yetenekleri entegre eden güçlü bir C kütüphanesi. Bu proje, SCADE Display tarafından oluşturulan HMI arayüzleri ile endüstriyel otomasyon sistemleri arasında Modbus protokolü kullanarak kesintisiz iletişim sağlar.

## Genel Bakış

SCADE Display, İnsan-Makine Arayüzü (HMI) uygulamaları oluşturmak için yaygın olarak kullanılan endüstriyel bir yazılımdır. Bu proje, SCADE Display tarafından oluşturulan uygulamaların Modbus TCP/IP protokolünü kullanarak PLC'ler ve diğer endüstriyel kontrol sistemleriyle iletişim kurmasını sağlayan bir iletişim katmanı sunar.

## Özellikler

- **Modbus TCP/IP Entegrasyonu**
  - Otomatik istemci bağlantı yönetimi
  - Yapılandırılabilir sunucu bağlantı parametreleri
  - Çoklu Modbus kayıt desteği
  - Toplu ve tekil değer güncellemeleri

- **Dinamik Yapılandırma**
  - INI dosyası tabanlı yapılandırma
  - SCADE değişkenleri ile Modbus kayıtları arasında çalışma zamanı eşlemesi
  - Yapılandırılabilir sunucu IP, port ve slave ID

- **Güçlü Hata Yönetimi**
  - Bağlantı hatası kurtarma
  - Zaman aşımı yönetimi
  - Kapsamlı hata kaydı
  - Bellek yönetimi güvenlik kontrolleri

- **Kayıt Sistemi**
  - Detaylı operasyon kaydı
  - Zaman damgalı girişler
  - Bağlantı durumu takibi
  - Değer değişikliği izleme

## Gereksinimler

- Ansys SCADE Display v232 veya üzeri
- libmodbus kütüphanesi (önceden yüklenmiş olmalı)
- MinGW GCC derleyicisi (SCADE ile birlikte gelir)
- CMake (SCADE ile birlikte gelir)
- Ninja derleme sistemi

## Projeyi Derleme

1. Proje kök dizininde bir build klasörü oluşturun:
```bash
mkdir build
cd build
```

2. CMake kullanarak projeyi yapılandırın:
```bash
"C:\Program Files\ANSYS Inc\v232\SCADE\contrib\cmake\bin\cmake.exe" ^
-D"CMAKE_C_COMPILER:PATH=C:\Program Files\ANSYS Inc\v232\SCADE\contrib\Msys64\mingw64\bin\gcc.exe" ^
-D"CMAKE_MAKE_PROGRAM:PATH=C:\Program Files\ANSYS Inc\v232\SCADE\contrib\ninja\ninja.exe" ^
-B . ^
-G "Ninja Multi-Config" ^
-D "BUILD_TARGET=exe" ^
--log-level WARNING ^
..
```

3. Projeyi derleyin:
```bash
"C:\Program Files\MATLAB\R2023b\toolbox\shared\coder\ninja\win64\ninja.exe"
```

## Kurulum

1. Depoyu klonlayın:
```bash
git clone https://github.com/isikmuhamm/scade-display-modbus-control.git
```

2. Bağımlılıkları yükleyin:
   - Ansys SCADE Display v232 veya üzerini yükleyin
   - libmodbus kütüphanesini yükleyin
   - Tüm SCADE ortam değişkenlerinin doğru ayarlandığından emin olun

3. Projeyi Derleme bölümündeki adımları takip ederek derleyin

4. Derlenmiş kütüphaneyi SCADE projenize ekleyin:
   - Oluşturulan kütüphane dosyalarını build dizininden kopyalayın
   - Başlık dosyalarını SCADE tarafından oluşturulan kodunuza dahil edin

## Yapılandırma

### Modbus Ayarları


config.ini

 dosyasında bağlantınızı yapılandırın:
```ini
[ModbusConfig]
server_ip=127.0.0.1
port=502
slave_id=1

[InputMappings]
scade_variable_name=register_address

[OutputMappings]
scade_variable_name=register_address
```

### Değişken Eşleme
SCADE değişkenlerinizi 

modbus_comm.c

 içindeki Modbus kayıtlarına eşleyin:
```c
void init_mappings(CONTEXT_STRUCT_NAME *context) {
    // Eşlemelerinizi buraya ekleyin
}
```

## Kullanım

SCADE tarafından oluşturulan spesifikasyon dosyanıza bu güncelleme fonksiyonlarından birini ekleyin:

```c
// Tekil güncellemeler için
update_modbus_values(pContext);

// Toplu güncellemeler için
update_modbus_values_all(pContext);
```

## Mimari

- **Yapılandırma Katmanı**: INI dosyası ayrıştırma ve eşleme kurulumunu yönetir
- **İletişim Katmanı**: Modbus TCP/IP bağlantılarını yönetir
- **Veri Yönetim Katmanı**: SCADE ve Modbus arasındaki veri senkronizasyonunu yönetir
- **Kayıt Sistemi**: Kapsamlı operasyon takibi sağlar

## Kayıt Tutma

İşlemler 

trackcircuit.log

 dosyasına kaydedilir:
- Yapılandırma yükleme olayları
- Bağlantı durumu değişiklikleri
- Veri transfer işlemleri
- Hata mesajları

## Güvenlik Önlemleri

⚠️ Bu yazılım endüstriyel otomasyon sistemleri için tasarlanmıştır. Her zaman:
- Kullanıma almadan önce kapsamlı test yapın
- Uygun güvenlik önlemlerini uygulayın
- Tüm iletişimleri doğrulayın
- Sistem davranışını izleyin

## Katkıda Bulunma

1. Depoyu fork edin
2. Özellik dalınızı oluşturun
3. Değişikliklerinizi commit edin
4. Dalınıza push yapın
5. Pull Request oluşturun

## Lisans

Bu yazılım çift lisans altında sunulmaktadır:

1. Ticari Lisans
- Ticari kullanım için gereklidir
- Destek ve güncellemeleri içerir
- Fiyatlandırma için [e-posta adresiniz] ile iletişime geçin

2. Ticari Olmayan Lisans
- Sadece ticari olmayan, eğitim amaçlı kullanım için ücretsizdir
- Garanti sağlanmaz
- Yeniden dağıtıma izin verilmez

Tüm hakları saklıdır. Telif Hakkı (c) 2024.

## Destek

Sorunlar ve özellik istekleri için lütfen GitHub issue tracker'ı kullanın.

Bu proje, SCADE Display'in güçlü HMI yetenekleri ile endüstriyel otomasyon sistemleri arasındaki boşluğu Modbus protokolü kullanarak doldurmak için tasarlanmıştır.