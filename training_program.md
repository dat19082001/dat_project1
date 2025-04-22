# Project 1

Viết chương trình quản lý học sinh, điểm các môn học của từng sinh viên và các chức năng thống kê cơ bản. Chương trình có các chức năng cụ thể sau:
- Nhập dữ liệu của 1 học sinh bao gồm: ID, họ tên, ngày tháng năm sinh, lớp, điểm toán, lý, hóa.
  - Ngày tháng năm sinh: định dạng dd/mm/yyyy. Nếu định dạng không đúng, yêu cầu nhập lại (hoặc tự xử lý).
  - Lớp: định dạng <tên khối>-<STT lớp>. Ví dụ: 10-1. Nếu định dạng không đúng, yêu cầu nhập lại.
  - ID của mỗi sinh viên là duy nhất, không được trùng nhau.
- In ra danh sách sinh viên đã nhập.
- Sửa thông tin của sinh viên dựa trên ID (không được sửa ID).
- Xóa sinh viên theo ID.
- Tìm sinh viên có điểm toán/lý/hóa cao hoặc thấp nhất.
- Thống kê điểm trung bình, độ lệch chuẩn của từng môn học.
- Sắp xếp danh sách sinh viên theo điểm toán/lý/hóa theo chiều tăng hoặc giảm dần và in ra.
- Lưu danh sách sinh viên đã nhập vào file. Sau khi tắt chương trình và chạy lại, danh sách sinh viên đã nhập vẫn còn được lưu.
- Chương trình có menu cho người dùng chọn.
  
Sử dụng C++ và struct. Tùy ý sử dụng các thư viện trong std của C++.