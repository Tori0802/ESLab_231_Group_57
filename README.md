*University of Science and Technology - Vietnam National University, Ho Chi Minh City*
# Embedded System (Lab Experiment) - Semester 231
<ul>
<li>Lớp lí thuyết: L01 - Giảng viên lí thuyết: Vũ Trọng Thiên</li>
<li>Lớp thí nghiệm: L01 - Giảng viên thí nghiệm: Vũ Trọng Thiên</li>
</ul>


## Nhóm: 57  
*Danh sách thành viên:* <br>
| STT | Name | ID |
| :----: | :---- | :----: |
| 1 | Nguyễn Ngọc Trí | 2012286 |
| 2 | Đỗ Thành Minh | 2011610 |


## Tải và cài đặt Espressif-IDE  
_Bước 1:_ Vào trang chủ: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html <br> 

_Bước 2:_ Ở mục Manual Installation, nhấn vào **Windows Installer** như hình dưới đây (nếu hệ điều hành sử dụng là Windows) <br> 
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/2e586f1d-7ba6-4c24-8bfe-86ee8496c1ee" alt="" width="800" title="Download Espressif-IDE 1"> </p> <br> 

_Bước 3:_ Nhấn vào **Windows Installer Download** để chọn phiên bản cài đặt <br> 
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/28c923ce-da74-4158-ac51-a2dfd8171090" alt="" width="800" title="Download Espressif-IDE 2"> </p> <br>

_Bước 4:_ Hình dưới đây cho ta thấy các phiên bản cài đặt của Espressif-IDE, ở đây nhóm em sẽ chọn **_Espressif-IDE 2.11.0 with ESP-IDF v5.0.1_** (ô có màu xanh dương) để tải về và cài đặt trên máy <br> 
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/74de19ee-72b2-4f0d-8e74-91def4deec1b" alt="" width="500" title="Download Espressif-IDE 3"> </p> <br>
Sau khi cài đặt hoàn tất, ta sẽ có giao diện của tool Espressif-IDE như sau: <br> <br>
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/8b9c6117-c18e-48fe-a583-24a615912114" alt="" width="900" title="Download Espressif-IDE 4"> </p> <br>


## Cách build source vào tool Espressif-IDE  
_Bước 1:_ Mở tool Espressif-IDE <br> 

_Bước 2:_ Tạo một project mới bằng cách chọn **File** -> **New** -> **Espressif IDF Project** <br>
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/8338646f-5093-4382-b102-da03c44a31bc" alt="" width="800" title="Build source Espressif-IDE 1"> </p> <br>

_Bước 3:_ Sau khi đặt tên project, xác định đường dẫn và hoàn thành các bước khởi tạo, một project sẽ được khởi tạo như ví dụ hình dưới đây, lúc này chúng ta sẽ chỉnh sửa source code ở file **main.c** hoặc add thêm các file cần thiết khác để hoàn thành bước hiện thực code <br> 
<p align="center"> <img src="https://github.com/Tori0802/ESLab_231_Group_57/assets/109940367/c7ef2065-30a1-4540-9092-bab7889b9403" alt="" width="800" title="Build source Espressif-IDE 2"> </p> <br>

_Bước 4:_ Hiện thực **_build_**, **_run_** hay **_stop mode_** bằng cách nhấn các nút tùy chọn ở bên trái thanh bar dưới thanh menu hoặc vào **Run** -> Các mode tùy chọn <br>

